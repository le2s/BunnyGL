#ifndef IRCWINDOW_H
#define IRCWINDOW_H

#include <QWidget>

class ContactList;
namespace GCF { class RemoteObject; }
namespace Ui {
class IRCWindow;
}

class IRCWindow : public QWidget
{
    Q_OBJECT

public:
    explicit IRCWindow(QWidget *parent = 0);
    ~IRCWindow();

    // This function returns the userName of the user who has launched
    // the current instance of the program. Essentially we return,
    // value of the "USERNAME" environment variable.
    Q_INVOKABLE QString userName() const;

    void addContact(const QString& name, GCF::RemoteObject* remoteIRC);
    ContactList* contactList() const;

public slots:
    // This slot is called to show message from another user on this chat window.
    // The slot is connected to the broadcastMessage() signal sent by a remote
    // IRCWindow instance.
    void showMessage(const QString& name, const QString& message);

signals:
    // This signal is emitted when the user on this computer has typed a message
    // on the IRCWindow and clicked "Send". First parameter would be name of this
    // user (which is equal to this->userName()). Second parameter would be the
    // message that this user has typed.
    void broadcastMessage(const QString& name, const QString& message);

private slots:
    void on_contactList_contactAdded(const QString& name, GCF::RemoteObject* remoteIRC);
    void on_contactList_contactRemoved(const QString& name);
    void on_cmdSend_clicked();
    void on_txtMyMessage_textEdited(const QString& text);

private:
    Ui::IRCWindow *ui;
};

#endif // IRCWINDOW_H
