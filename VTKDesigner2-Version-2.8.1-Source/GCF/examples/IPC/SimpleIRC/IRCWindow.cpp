#include "IRCWindow.h"
#include "ui_IRCWindow.h"
#include <GCF/RemoteObject>

#include <QProcessEnvironment>

IRCWindow::IRCWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IRCWindow)
{
    ui->setupUi(this);
}

IRCWindow::~IRCWindow()
{
    delete ui;
}

QString IRCWindow::userName() const
{
    static QString myName;
    if(myName.isEmpty())
        myName = QProcessEnvironment::systemEnvironment().value("USERNAME");

    return myName;
}

void IRCWindow::addContact(const QString& name, GCF::RemoteObject* remoteIRC)
{
    ui->contactList->addContact(name, remoteIRC);
}

ContactList* IRCWindow::contactList() const
{
    return ui->contactList;
}

void IRCWindow::showMessage(const QString &name, const QString &message)
{
    QString msg = QString("<b>%1</b>: %2").arg(name).arg(message);
    ui->txtMessages->append(msg);
}

void IRCWindow::on_contactList_contactAdded(const QString &name, GCF::RemoteObject* remoteIRC)
{
    QString msg = QString("<i>User <b>%1</b> joined the chat</i>").arg(name);
    ui->txtMessages->append(msg);

    GCF::connect(remoteIRC, SIGNAL(broadcastMessage(QString,QString)),
                 this, SLOT(showMessage(QString,QString)));
}

void IRCWindow::on_contactList_contactRemoved(const QString& name)
{
    QString msg = QString("<i>User <b>%1</b> has left the chat</i>").arg(name);
    ui->txtMessages->append(msg);
}

void IRCWindow::on_cmdSend_clicked()
{
    QString text = ui->txtMyMessage->text();
    this->showMessage("Me", text);
    ui->txtMyMessage->clear();
    ui->cmdSend->setEnabled(false);

    emit broadcastMessage( userName(), text );
}

void IRCWindow::on_txtMyMessage_textEdited(const QString& text)
{
    ui->cmdSend->setEnabled(!text.isEmpty());
}

