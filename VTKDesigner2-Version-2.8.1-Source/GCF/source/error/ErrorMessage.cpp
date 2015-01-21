#include "ErrorMessage.h"
#include "ErrorDump.h"

namespace GCF
{

struct ErrorMessageData
{
    ErrorMessageData() : parent(0), errorLevel(GCF::ErrorMessage::Info) { }

    int errorLevel;
    QString context;
    QByteArray errorCode;
    QString message;
    QString details;
    QList<ErrorMessage*> children;
    ErrorMessage* parent;
};

}

GCF::ErrorMessage::ErrorMessage(int level,
                                const QString& context,
                                const QByteArray& errorCode,
                                const QString& msg,
                                const QString& details,
                                ErrorMessage* parent)
{
    d = new GCF::ErrorMessageData;
    d->errorLevel = level;
    d->context = context;
    d->errorCode = errorCode;
    d->message = msg;
    d->details = details;
    d->parent = parent;
    if(d->parent)
        d->parent->d->children.append(this);

    GCF::ErrorDump::instance().messageCreated(this);
}

GCF::ErrorMessage::ErrorMessage()
{
    d = new GCF::ErrorMessageData;
}

GCF::ErrorMessage::~ErrorMessage()
{
    GCF::ErrorDump::instance().messageDestroyed(this);

    if(d->parent)
        d->parent->d->children.removeAll(this);
    d->parent = 0;

    this->clear();

    delete d;
}

GCF::ErrorMessage* GCF::ErrorMessage::parent() const
{
    return d->parent;
}

QList<GCF::ErrorMessage*> GCF::ErrorMessage::children() const
{
    return d->children;
}

int GCF::ErrorMessage::errorLevel() const
{
    return d->errorLevel;
}

QString GCF::ErrorMessage::context() const
{
    return d->context;
}

QByteArray GCF::ErrorMessage::errorCode() const
{
    return d->errorCode;
}

QString GCF::ErrorMessage::message() const
{
    return d->message;
}

QString GCF::ErrorMessage::details() const
{
    return d->details;
}

void GCF::ErrorMessage::clear()
{
    QList<GCF::ErrorMessage*> children(d->children);
    d->children.clear();
    qDeleteAll(children);
}

void GCF::ErrorMessage::setMessage(const QString& msg)
{
    d->message = msg;
    GCF::ErrorDump::instance().messageUpdated(this);
}

///////////////////////////////////////////////////////////////////////////////

GCF::ErrorMessageBranch::ErrorMessageBranch(const QString& context)
{
    m_branchMessage = GCF::ErrorDump::instance().pushBranch(context);
}

GCF::ErrorMessageBranch::~ErrorMessageBranch()
{
    GCF::ErrorMessage* branch = GCF::ErrorDump::instance().popBranch();
    if(branch == m_branchMessage)
    {
        if(m_branchMessage->children().count() == 0)
            delete m_branchMessage;
    }

    m_branchMessage = 0;
}

