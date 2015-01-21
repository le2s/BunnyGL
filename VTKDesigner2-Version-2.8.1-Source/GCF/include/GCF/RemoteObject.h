/****************************************************************************
**
** Copyright (C) VCreate Logic Private Limited, Bangalore
**
** Use of this file is limited according to the terms specified by
** VCreate Logic Private Limited, Bangalore.  Details of those terms
** are listed in licence.txt included as part of the distribution package
** of this file. This file may not be distributed without including the
** licence.txt file.
**
** Contact info@vcreatelogic.com if any conditions of this licensing are
** not clear to you.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#ifndef REMOTE_OBJECT_H
#define REMOTE_OBJECT_H

#include "Common.h"
#include "DataStore.h"

#include <QObject>
#include <QVariant>
#include <QHostAddress>

#include "GenericSignalSpy.h"

namespace GCF
{
class RemoteAppAccessPoint;
class Message;

struct RemoteObjectData;
class GCF_EXPORT_CLASS RemoteObject : public QObject
{
    Q_OBJECT

public:
    static RemoteObject* currentSignalSender();
    ~RemoteObject();

    RemoteAppAccessPoint* remoteAppAccessPoint() const;
    bool isActive() const;
    qint32 id() const;
    DataStore customData() const;

    // Remoted method call
    Q_INVOKABLE qint32 call(const QString& method, const QVariant& arg1=QVariant(), const QVariant& arg2=QVariant(),
                            const QVariant& arg3=QVariant(), const QVariant& arg4=QVariant(),
                            const QVariant& arg5=QVariant(), const QVariant& arg6=QVariant(),
                            const QVariant& arg7=QVariant(), const QVariant& arg8=QVariant());
    qint32 call(const QString& method, const QVariantList& args);

    // Asynchronous call support
    static bool asyncCall(QObject* notifyObject, const char* member,
                          const QHostAddress& address, quint16 port,
                          const QString& objectName, const QString& method,
                          const QVariant& arg1=QVariant(), const QVariant& arg2=QVariant(),
                          const QVariant& arg3=QVariant(), const QVariant& arg4=QVariant(),
                          const QVariant& arg5=QVariant(), const QVariant& arg6=QVariant(),
                          const QVariant& arg7=QVariant(), const QVariant& arg8=QVariant());
    static bool asyncCall(QObject* notifyObject, const char* member,
                          const QHostAddress& address, quint16 port,
                          const QString& objectName, const QString& method,
                          const QVariantList& args);

    // Asynchronous call support (2)
    bool asyncCall(QObject* notifyObject, const char* member,
                   const QString& methodName,
                   const QVariant& arg1=QVariant(), const QVariant& arg2=QVariant(),
                   const QVariant& arg3=QVariant(), const QVariant& arg4=QVariant(),
                   const QVariant& arg5=QVariant(), const QVariant& arg6=QVariant(),
                   const QVariant& arg7=QVariant(), const QVariant& arg8=QVariant());
    bool asyncCall(QObject* notifyObject, const char* member,
                   const QString& methodName, const QVariantList& args);

    // Remote signal/slot connection
    qint32 makeConnection(const char* signal, QObject* receiver, const char* slot);
    bool makeConnection(QObject* sender, const char* signal, const char* member);
    bool breakConnection(const char* signal, QObject* receiver, const char* slot);
    bool breakConnection(const char* signal, QObject* receiver);
    bool breakConnection(QObject* sender, const char* signal, const char* member);
    bool breakConnection(QObject* sender);
    bool breakConnection(const char* signal);
    bool breakConnection2(QObject* receiver);

    // Set/Get properties
    Q_INVOKABLE qint32 setObjectProperty(const QString& propName, const QVariant& value);
    qint32 getObjectProperty(const QString& propName, QVariant& value) const;
    Q_INVOKABLE QVariant getObjectProperty(const QString& propName) const;

    // Cache properties
    Q_INVOKABLE void cacheProperties();
    Q_INVOKABLE QVariant getCachedObjectProperty(const QString& propName) const;

protected:
    RemoteObject(RemoteAppAccessPoint* remotePt);
    void sendMessage(Message* msg);
    virtual bool handleMessage(Message* received, Message* sent);

private:
    void setId(qint32 id);

signals:
    void activated();
    void deactivated();
    void couldNotActivate();
    void methodInvocationResult(qint32 callId, bool success, const QVariant& result, const QString& method);
    void setPropertyResult(qint32 callId, bool success, const QVariant& value, const QString& propName);
    void getPropertyResult(qint32 callId, bool success, const QVariant& value, const QString& propName);
    void propertiesCached();
    void makeConnectionResult(qint32 callId, bool success, const QString& memberName);

private slots:
    void sendQueuedMessages();
    void cacheProperties(const QStringList& propNames);

private:
    friend class RemoteAppAccessPoint;
    RemoteObjectData* d;
};

struct SignalSpy2Data;
class SignalSpy2 : public GenericSignalSpy
{
    Q_OBJECT

public:
    SignalSpy2(RemoteObject* remoteObj, QObject* sender, const char* signal, const char* method);
    ~SignalSpy2();

    QByteArray signal2() const;
    QByteArray method() const;

protected:
    void handleSignalEmission(const QVariantList& args);

private:
    SignalSpy2Data* d;
};

bool connect(GCF::RemoteObject* sender, const char* signal, QObject* receiver, const char* slot);
bool connect(QObject* sender, const char* signal, GCF::RemoteObject* receiver, const char* slot);
bool disconnect(GCF::RemoteObject* sender, const char* signal, QObject* receiver, const char* slot);
bool disconnect(QObject* sender, const char* signal, GCF::RemoteObject* receiver, const char* slot);
bool disconnect(QObject* sender, GCF::RemoteObject* receiver);
bool disconnect(GCF::RemoteObject* sender, QObject* receiver);
GCF::RemoteObject* sender();
}

inline bool GCF::connect(GCF::RemoteObject* sender, const char* signal, QObject* receiver, const char* slot)
{
    if(!sender)
        return false;

    return sender->makeConnection(signal, receiver, slot) >= 0;
}

inline bool GCF::connect(QObject* sender, const char* signal, GCF::RemoteObject* receiver, const char* slot)
{
    if(!receiver)
        return false;

    return receiver->makeConnection(sender, signal, slot);
}

inline bool GCF::disconnect(GCF::RemoteObject* sender, const char* signal, QObject* receiver, const char* slot)
{
    if(!signal && !slot)
        return disconnect(sender, receiver);

    if(!sender)
        return false;

    return sender->breakConnection(signal, receiver, slot);
}

inline bool GCF::disconnect(QObject* sender, const char* signal, GCF::RemoteObject* receiver, const char* slot)
{
    if(!receiver)
        return false;

    return receiver->breakConnection(sender, signal, slot);
}

inline bool GCF::disconnect(QObject* sender, GCF::RemoteObject* receiver)
{
    if(!receiver)
        return false;

    return receiver->breakConnection(sender);
}

inline bool GCF::disconnect(GCF::RemoteObject* sender, QObject* receiver)
{
    if(!sender)
        return false;

    return sender->breakConnection2(receiver);
}

inline GCF::RemoteObject* GCF::sender()
{
    return GCF::RemoteObject::currentSignalSender();
}

Q_DECLARE_METATYPE(GCF::RemoteObject*)

#endif
