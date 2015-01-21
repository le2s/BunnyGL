#include "RemoteApplication.h"
#include "RemoteApplicationCall.h"

#include <QCoreApplication>
#include <QStringList>

namespace GCF
{
namespace Connect
{

struct RemoteApplicationData
{
    QString applicationName;
    QHostAddress applicationAddress;
    quint16 applicationPort;
};

}
}

static QList<GCF::Connect::RemoteApplication*> RemoteApplicationList;

GCF::Connect::RemoteApplication* GCF::Connect::RemoteApplication::remoteApplication(const QHostAddress& address, quint16 port)
{
    // Check if the application object already exists.
    Q_FOREACH(GCF::Connect::RemoteApplication* app, RemoteApplicationList)
    {
        if(app->applicationAddress() == address && app->applicationPort() == port)
            return app;
    }

    // Create a new one otherwise
    return new GCF::Connect::RemoteApplication(address, port, qApp);
}

QList<GCF::Connect::RemoteApplication*> GCF::Connect::RemoteApplication::remoteApplications()
{
    return RemoteApplicationList;
}

GCF::Connect::RemoteApplication::RemoteApplication(const QHostAddress& address, quint16 port, QObject *parent) :
    QObject(parent)
{
    d = new GCF::Connect::RemoteApplicationData;
    d->applicationName = "Unknown";
    d->applicationAddress = address;
    d->applicationPort = port;

    if(!parent)
        this->setParent(qApp);

    // Find out the application name
    this->call(this, SLOT(onApplicationNameFetched(QVariant,bool)),
               "AppAccessPointComponent.appAccessPoint", "accessPointName");

    ::RemoteApplicationList.append(this);
}

GCF::Connect::RemoteApplication::~RemoteApplication()
{
    ::RemoteApplicationList.removeAll(this);
    delete d;
}

QString GCF::Connect::RemoteApplication::identifier() const
{
    return QString("%1 (%2:%3)").arg(d->applicationName).arg(d->applicationAddress.toString()).arg(d->applicationPort);
}

QString GCF::Connect::RemoteApplication::applicationName() const
{
    return d->applicationName;
}

QHostAddress GCF::Connect::RemoteApplication::applicationAddress() const
{
    return d->applicationAddress;
}

qint16 GCF::Connect::RemoteApplication::applicationPort() const
{
    return d->applicationPort;
}

bool GCF::Connect::RemoteApplication::call(QObject* receiver, const char* member, const QString& objectName, const QString& methodName, const QVariantList& args)
{
    if(!receiver || !member || qstrlen(member) == 0)
        return false;

    // Ensure that member does indeed accept two parameters
    // first is QVariant, second is bool
    QByteArray normalizedMethod = QMetaObject::normalizedSignature(member);
    if(normalizedMethod.indexOf('(') < 0)
        return false;

    QStringList parameters = QString(normalizedMethod).section('(', 1, 1).section(')', 0, 0).split(',');
    if(parameters.count() != 2)
        return false;

    if(parameters.first() != "QVariant" || parameters.last() != "bool")
        return false;

    // We are ok with the receiver and member. Now lets check other things.
    if(objectName.isEmpty() || methodName.isEmpty())
        return false;

    // All checks are through. Lets make the call
    GCF::Connect::RemoteApplicationCall* call = new GCF::Connect::RemoteApplicationCall(this);
    return call->call(receiver, normalizedMethod.constData(), objectName, methodName, args);
}

void GCF::Connect::RemoteApplication::onApplicationNameFetched(const QVariant& result, bool success)
{
    if(success)
        d->applicationName = result.toString();
}

