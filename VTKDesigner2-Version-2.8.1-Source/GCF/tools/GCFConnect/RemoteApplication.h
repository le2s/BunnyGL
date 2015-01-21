#ifndef REMOTEAPPLICATION_H
#define REMOTEAPPLICATION_H

#include <QObject>
#include <QVariantList>
#include <QHostAddress>

namespace GCF
{

namespace Connect
{

struct RemoteApplicationData;
class RemoteApplication : public QObject
{
    Q_OBJECT

public:
    static RemoteApplication* remoteApplication(const QHostAddress& address=QHostAddress(QHostAddress::LocalHost), quint16 port=4235);
    static QList<RemoteApplication*> remoteApplications();

    ~RemoteApplication();

    // returns {applicationName} ({applicationAddress}|{applicationPort})
    QString identifier() const;
    QString applicationName() const;
    QHostAddress applicationAddress() const;
    qint16 applicationPort() const;
    
    // member must always be SLOT(function(QVariant,bool))
    bool call(QObject* receiver, const char* member,
              const QString& objectName, const QString& methodName,
              const QVariantList& args);

    bool call(QObject* receiver, const char* member,
              const QString& objectName, const QString& methodName,
              const QVariant& arg1=QVariant(), const QVariant& arg2=QVariant(),
              const QVariant& arg3=QVariant(), const QVariant& arg4=QVariant(),
              const QVariant& arg5=QVariant(), const QVariant& arg6=QVariant(),
              const QVariant& arg7=QVariant(), const QVariant& arg8=QVariant()) {
        QVariantList args = QVariantList() << arg1 << arg2
                << arg3 << arg4 << arg5 << arg6 << arg7 << arg8;
        while(args.count() && !args.last().isValid())
            args.takeLast();
        return this->call(receiver, member, objectName, methodName, args);
    }

private slots:
    void onApplicationNameFetched(const QVariant& result, bool success);

protected:
    RemoteApplication(const QHostAddress& address, quint16 port, QObject* parent = 0);

private:
    RemoteApplicationData* d;
};
}

}

#endif // REMOTEAPPLICATION_H
