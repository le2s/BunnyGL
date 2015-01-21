#include "RemoteApplicationCall.h"

#include <QTimer>
#include <QTcpSocket>
#include <QByteArray>
#include <QScriptEngine>
#include <QScriptValue>
#include <QVariant>
#include <QVariantMap>
#include <QVariantList>

namespace GCF
{
namespace Connect
{
struct RemoteApplicationCallData
{
    RemoteApplicationCallData() : receiver(0) { }

    QTimer timeoutTimer;
    QTcpSocket socket;
    QObject* receiver;
    QByteArray member;

    QByteArray request;
    QByteArray response;
};

class JSON
{
public:
    static JSON& instance();
    ~JSON() { }

    QVariantMap parse(const QString& string, bool* ok=0);
    QString serialize(const QVariant& value);

protected:
    JSON();

private:
    QScriptEngine m_engine;
    QScriptValue m_parseFn;
    QScriptValue m_streamFn;

    QScriptValue createValue(const QVariant& value);
};
}
}

GCF::Connect::RemoteApplicationCall::RemoteApplicationCall(GCF::Connect::RemoteApplication *parent) :
    QObject(parent)
{
    d = new GCF::Connect::RemoteApplicationCallData;
}

GCF::Connect::RemoteApplicationCall::~RemoteApplicationCall()
{
    delete d;
}

bool GCF::Connect::RemoteApplicationCall::call(QObject* receiver, const char* member,
          const QString& objectName, const QString& methodName, const QVariantList& args)
{
    if(d->receiver)
        return false; // a call was already made!

    if(!receiver || !member)
    {
        this->deleteLater();
        return false;
    }

    GCF::Connect::RemoteApplication* remoteApp = qobject_cast<GCF::Connect::RemoteApplication*>(this->parent());
    if(!remoteApp)
    {
        this->deleteLater();
        return false;
    }

    // Store member name
    d->member = QByteArray(member);
    if( QChar(d->member.at(0)).isDigit() )
        d->member.remove(0, 1);

    int bracketIndex = d->member.indexOf('(');
    if(bracketIndex >= 0)
        d->member = d->member.left(bracketIndex);

    if(d->member.isEmpty())
    {
        this->deleteLater();
        return false;
    }

    // Store receiver and connect to it
    d->receiver = receiver;
    connect(receiver, SIGNAL(destroyed()), this, SLOT(onReceiverDestroyed()));

    // Prepare call data
    QVariantMap map;
    map["ObjectName"] = objectName;
    map["MethodName"] = methodName;
    map["Arguments"] = args;

    d->request = GCF::Connect::JSON::instance().serialize(map).toLatin1();
    if(d->request.isEmpty())
    {
        d->receiver = 0;
        this->deleteLater();
        return false;
    }

    // connect to the remote application
    connect(&d->socket, SIGNAL(connected()), this, SLOT(onConnected()));
    connect(&d->socket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
    connect(&d->socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    d->socket.connectToHost(remoteApp->applicationAddress(), remoteApp->applicationPort());

    // Lets wait for 10 seconds before firing a timeout
    connect(&d->timeoutTimer, SIGNAL(timeout()), this, SLOT(onTimeout()));
    d->timeoutTimer.start(10000);
    return true;
}

void GCF::Connect::RemoteApplicationCall::onConnected()
{
    // So, we dont have to timeout
    d->timeoutTimer.stop();
    disconnect(&d->timeoutTimer, SIGNAL(timeout()), this, SLOT(onTimeout()));

    // Lets send the request packet
    d->socket.write(d->request);
    d->socket.flush();
}

void GCF::Connect::RemoteApplicationCall::onDisconnected()
{
    if(d->response.isEmpty())
        this->notifyResponse("No response was received from the remote application", false);
    else
    {
        bool ok = true;
        QVariantMap result = GCF::Connect::JSON::instance().parse(d->response, &ok);
        if(!ok)
            this->notifyResponse("Could not parse the response received from the remote application", false);
        else
        {
            QVariant callResult;
            bool success = result.value("Success", false).toBool();
            if(success)
                callResult = result.value("Result", QVariant());
            else
                callResult = result.value("Error", QVariant());

            this->notifyResponse(callResult, success);
        }
    }

    d->receiver = 0;
    this->deleteLater();
}

void GCF::Connect::RemoteApplicationCall::onReadyRead()
{
    d->response += d->socket.readAll();
}

void GCF::Connect::RemoteApplicationCall::onReceiverDestroyed()
{
    d->receiver = 0;

    if(d->timeoutTimer.isActive())
    {
        d->timeoutTimer.stop();
        disconnect(&d->timeoutTimer, SIGNAL(timeout()), this, SLOT(onTimeout()));
        d->socket.abort();
        this->deleteLater();
    }
}

void GCF::Connect::RemoteApplicationCall::onTimeout()
{
    d->timeoutTimer.stop();
    disconnect(&d->timeoutTimer, SIGNAL(timeout()), this, SLOT(onTimeout()));
    this->notifyResponse("Connection timeout", false);
    d->receiver = 0;
    this->deleteLater();
}

void GCF::Connect::RemoteApplicationCall::notifyResponse(const QVariant& result, bool success)
{
    if(!d->receiver)
        return;

    QMetaObject::invokeMethod(d->receiver, d->member.constData(), Qt::DirectConnection,
                              Q_ARG(QVariant,result), Q_ARG(bool,success));
}

///////////////////////////////////////////////////////////////////////////////

GCF::Connect::JSON& GCF::Connect::JSON::instance()
{
    static GCF::Connect::JSON theInstance;
    return theInstance;
}


GCF::Connect::JSON::JSON()
{
    const QString script = "function parse_json(string) { return JSON.parse(string); }\n"
                           "function stream_json(object) { return JSON.stringify(object); }";
    m_engine.evaluate(script);

    m_parseFn = m_engine.globalObject().property("parse_json");
    m_streamFn = m_engine.globalObject().property("stream_json");
}

QVariantMap GCF::Connect::JSON::parse(const QString& string, bool *ok)
{
    QScriptValue result = m_parseFn.call(QScriptValue(), QScriptValueList() << QScriptValue(string));
    if(result.isError())
    {
        if(ok)
            *ok = false;
        return QVariantMap();
    }

    QVariantMap resultMap = result.toVariant().toMap();
    if(ok)
        *ok = true;
    return resultMap;
}

QString GCF::Connect::JSON::serialize(const QVariant& value)
{
    QScriptValue arg = this->createValue(value);
    QScriptValue result = m_streamFn.call(QScriptValue(), QScriptValueList() << arg);
    QString resultString = result.toString();
    return resultString;
}

QScriptValue GCF::Connect::JSON::createValue(const QVariant& value)
{
    if(value.type() == QVariant::Map)
    {
        QScriptValue obj = m_engine.newObject();

        QVariantMap map = value.toMap();
        QVariantMap::const_iterator it = map.begin();
        QVariantMap::const_iterator end = map.end();
        while(it != end)
        {
            obj.setProperty( it.key(), this->createValue(it.value()) );
            ++it;
        }

        return obj;
    }

    if(value.type() == QVariant::List)
    {
        QVariantList list = value.toList();
        QScriptValue array = m_engine.newArray(list.length());
        for(int i=0; i<list.count(); i++)
            array.setProperty(i, this->createValue(list.at(i)));

        return array;
    }

    switch(value.type())
    {
    case QVariant::String:
        return QScriptValue(value.toString());
    case QVariant::Int:
        return QScriptValue(value.toInt());
    case QVariant::UInt:
        return QScriptValue(value.toUInt());
    case QVariant::Bool:
        return QScriptValue(value.toBool());
    case QVariant::ByteArray:
        return QScriptValue(QLatin1String(value.toByteArray()));
    case QVariant::Double:
        return QScriptValue((qsreal)value.toDouble());
    default:
        break;
    }

    if(value.isNull())
        return QScriptValue(QScriptValue::NullValue);

    return m_engine.newVariant(value);
}


