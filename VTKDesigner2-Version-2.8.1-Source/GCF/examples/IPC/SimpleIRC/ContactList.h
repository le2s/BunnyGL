#ifndef CONTACTLIST_H
#define CONTACTLIST_H

#include <QListWidget>

namespace GCF { class RemoteObject; }

class ContactList : public QListWidget
{
    Q_OBJECT

public:
    ContactList(QWidget *parent = 0);
    ~ContactList();

    // These functions help add users to the contact list
    // Added contacts automatically get removed the moment
    // the contact goes offline.
    void addContact(const QString& name, GCF::RemoteObject* remoteIRC);
    void removeContact(const QString& name);
    int contactCount() const;
    QString contactAt(int index) const;
    GCF::RemoteObject* contactObjectAt(int index) const;

signals:
    // These signals help figure out who joined and who left the contact-list
    void contactAdded(const QString& name, GCF::RemoteObject* remoteIRC);
    void contactRemoved(const QString& name, GCF::RemoteObject* remoteIRC);

private slots:
    void contactObjectDeactivated();
};

#endif // CONTACTLIST_H
