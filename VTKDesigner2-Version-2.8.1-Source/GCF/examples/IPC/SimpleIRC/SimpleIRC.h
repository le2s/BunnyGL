#ifndef SIMPLEIRC_H
#define SIMPLEIRC_H

#include <GCF/AbstractComponent>

namespace GCF { class RemoteAppAccessPoint; }
class IRCWindow;

class SimpleIRC : public GCF::AbstractComponent
{
    Q_OBJECT
    GCF_DECLARE_COMPONENT(SimpleIRC)

    public:
        static SimpleIRC& instance();
    ~SimpleIRC();

protected:
    SimpleIRC();
    void initializeComponent();
    void finalizeComponent();
    void creationAnnounced();
    QObject* fetchObject(const QString &completeName) const;
    QWidget* fetchWidget(const QString &completeName) const;

private slots:
    // This slot is called when GCF::RemoteAppDiscovery emits
    // the discoveredRemoteApp() signal. In this slot we try to
    // figure out the name of user on the other side.
    void slotFoundFriend(GCF::RemoteAppAccessPoint* rap);

    void slotFriendNameFound(qint32 callId, bool success, const QVariant& result, const QString& method);

private:
    IRCWindow* m_ircWindow;
};

#endif // SIMPLEIRC_H
