#ifndef REMOTEAPPLICATIONCALL_H
#define REMOTEAPPLICATIONCALL_H

#include "RemoteApplication.h"
#include <QVariantList>

namespace GCF
{

namespace Connect
{

class RemoteApplication;

struct RemoteApplicationCallData;
class RemoteApplicationCall : public QObject
{
    Q_OBJECT

public:
    ~RemoteApplicationCall();

private:
    RemoteApplicationCall(GCF::Connect::RemoteApplication *parent = 0);
    
    bool call(QObject* receiver, const char* member, const QString& objectName,
              const QString& methodName, const QVariantList& args);

private slots:
    void onConnected();
    void onDisconnected();
    void onReadyRead();
    void onReceiverDestroyed();
    void onTimeout();

private:
    void notifyResponse(const QVariant& result, bool success);

private:
    friend class RemoteApplication;
    RemoteApplicationCallData* d;
};

}

}

#endif // REMOTEAPPLICATIONCALL_H
