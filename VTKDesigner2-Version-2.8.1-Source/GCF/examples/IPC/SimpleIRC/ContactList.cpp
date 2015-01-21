#include "ContactList.h"
#include <GCF/RemoteObject>
#include <QContextMenuEvent>

ContactList::ContactList(QWidget *parent) :
    QListWidget(parent)
{

}

ContactList::~ContactList()
{

}

void ContactList::addContact(const QString& name, GCF::RemoteObject* remoteIRC)
{
    QListWidgetItem* item = new QListWidgetItem(this);
    item->setText(name);
    item->setData(Qt::UserRole, QVariant::fromValue( (QObject*)remoteIRC ) );
    remoteIRC->setProperty("__ContactName__", name);

    emit contactAdded(name, remoteIRC);

    /* We would like to become aware when a contact goes offline */
    connect(remoteIRC, SIGNAL(deactivated()), this, SLOT(contactObjectDeactivated()));
}

void ContactList::removeContact(const QString& name)
{
    QList<QListWidgetItem*> items = this->findItems(name, Qt::MatchExactly);
    while(items.count())
    {
        QListWidgetItem* item = items.first();

        QString name = item->text();
        GCF::RemoteObject* object =
                    qobject_cast<GCF::RemoteObject*>(
                                item->data(Qt::UserRole).value<QObject*>()
                            );

        emit contactRemoved(name, object);
        delete item;
        disconnect(object, 0, this, 0);
    }
}

int ContactList::contactCount() const
{
    return this->count();
}

QString ContactList::contactAt(int index) const
{
    QListWidgetItem* item = this->item(index);
    if(item)
        return item->text();

    return QString();
}

GCF::RemoteObject * ContactList::contactObjectAt(int index) const
{
    QListWidgetItem* item = this->item(index);
    if(item)
        return qobject_cast<GCF::RemoteObject*>(
                        item->data(Qt::UserRole).value<QObject*>()
                    );

    return 0;
}

void ContactList::contactObjectDeactivated()
{
    GCF::RemoteObject *remoteIRC = qobject_cast<GCF::RemoteObject*>(sender());
    for(int i=0; i<this->count(); i++)
    {
        QListWidgetItem* item = this->item(i);
        if(item->data(Qt::UserRole).value<QObject*>() == remoteIRC)
        {
            emit contactRemoved(item->text(), remoteIRC);
            delete item;
            disconnect(remoteIRC, 0, this, 0);
        }
    }
}
