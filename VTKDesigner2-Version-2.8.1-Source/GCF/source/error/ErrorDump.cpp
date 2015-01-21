#include "ErrorDump.h"
#include "ErrorMessage.h"
#include "ErrorMessageDialog.h"

#include <QtDebug>
#include <QUrl>
#include <QStack>
#include <QTextStream>
#include <GCF/ObjectList>
#include <QApplication>
#include <QClipboard>
#include <QDesktopServices>
#include <QDateTime>
#include <QBasicTimer>
#include <QSysInfo>
#include <QMessageBox>

namespace GCF
{
    struct ErrorDumpData
    {
        ErrorDumpData() : rootMessage(0), errorDialogVisible(false) { }

        GCF::ErrorMessage* rootMessage;
        QStack<GCF::ErrorMessage*> stack;
        GCF::IContainerList<IErrorMessageHandler> handler;
        QString supportEmail;
        QBasicTimer timer;
        bool errorDialogVisible;

        GCF::ErrorMessage* currentBranch() const {
            if(this->stack.count()) return this->stack.top();
            return this->rootMessage;
        }
    };
}

GCF::ErrorDump& GCF::ErrorDump::instance()
{
    static GCF::ErrorDump theInstance;
    return theInstance;
}

GCF::ErrorDump::ErrorDump()
{
    d = new GCF::ErrorDumpData;
    d->handler.add( (IErrorMessageHandler*)this );
    d->supportEmail = "support@vcreatelogic.com";
    d->rootMessage = new ErrorMessage;
}

GCF::ErrorDump::~ErrorDump()
{
    delete d;
}

void GCF::ErrorDump::setHandler(IErrorMessageHandler* handler)
{
    if(d->handler.count())
        d->handler.clear(false);
    if(handler)
        d->handler.add(handler);
    else
        d->handler.add( (IErrorMessageHandler*)this );
}

IErrorMessageHandler* GCF::ErrorDump::handler() const
{
    return d->handler.at(0);
}

void GCF::ErrorDump::setSupportEMail(const QString& email)
{
    d->supportEmail = email;
}

QString GCF::ErrorDump::supportEMail() const
{
    return d->supportEmail;
}

void GCF::ErrorDump::fatal(const QString& context, const QByteArray& errorCode,
           const QString& message, const QString& details)
{
    GCF::ErrorMessage* msg = new GCF::ErrorMessage(GCF::ErrorMessage::Fatal,
                                                   context, errorCode, message, details,
                                                   d->currentBranch());
    this->dumpErrorMessage(msg);
}

void GCF::ErrorDump::error(const QString& context, const QByteArray& errorCode,
           const QString& message, const QString& details)
{
    GCF::ErrorMessage* msg = new GCF::ErrorMessage(GCF::ErrorMessage::Error,
                                                   context, errorCode, message, details,
                                                   d->currentBranch());
    this->dumpErrorMessage(msg);
}

void GCF::ErrorDump::warning(const QString& context, const QByteArray& errorCode,
           const QString& message, const QString& details)
{
    GCF::ErrorMessage* msg = new GCF::ErrorMessage(GCF::ErrorMessage::Warning,
                                                   context, errorCode, message, details,
                                                   d->currentBranch());
    this->dumpErrorMessage(msg);
}

void GCF::ErrorDump::debug(const QString& context, const QByteArray& errorCode,
           const QString& message, const QString& details)
{
    GCF::ErrorMessage* msg = new GCF::ErrorMessage(GCF::ErrorMessage::Debug,
                                                   context, errorCode, message, details,
                                                   d->currentBranch());
    this->dumpErrorMessage(msg);
}

void GCF::ErrorDump::info(const QString& context, const QByteArray& errorCode,
           const QString& message, const QString& details)
{
    GCF::ErrorMessage* msg = new GCF::ErrorMessage(GCF::ErrorMessage::Info,
                                                   context, errorCode, message, details,
                                                   d->currentBranch());
    this->dumpErrorMessage(msg);
}

void GCF::ErrorDump::clear()
{
    d->rootMessage->clear();
    d->stack.clear();
}

QList<GCF::ErrorMessage*> GCF::ErrorDump::errorMessages() const
{
    return d->rootMessage->children();
}

QString GCF::ErrorDump::toString() const
{
    QString text;

    {
        QTextStream ts(&text, QIODevice::WriteOnly);
        ts << "Application: " << qApp->applicationName() << "("
           << qApp->applicationVersion() << ") from "<< qApp->organizationName() << "\n";
        ts << "ProcessID: " << qApp->applicationPid() << "\n";
        ts << "Date/Time: \n"
           << "    Local: " << QDateTime::currentDateTime().toString() << "\n"
           << "    UTC  : " << QDateTime::currentDateTimeUtc().toString() << "\n";
        ts << "Platform: ";
#ifdef Q_WS_WIN
        switch(QSysInfo::windowsVersion())
        {
        case QSysInfo::WV_XP: ts << "Windows XP"; break;
        case QSysInfo::WV_2003: ts << "Windows 2003"; break;
        case QSysInfo::WV_VISTA: ts << "Windows Vista"; break;
        case QSysInfo::WV_WINDOWS7: ts << "Windows 7"; break;
        default: ts << "Windows";
        }
#endif
#ifdef Q_OS_LINUX
        ts << "GNU/Linux";
#endif
#ifdef Q_OS_MAC
        ts << "Mac";
#endif
        ts << "\n\n";

        for(int i=0; i<d->rootMessage->children().count(); i++)
            d->handler.at(0)->print(d->rootMessage->children().at(i), ts);
    }

    return text;
}

void GCF::ErrorDump::copyToClipboard()
{
    qApp->clipboard()->setText(this->toString());
}

void GCF::ErrorDump::submitToSupport()
{
    d->handler.at(0)->submit(this->toString());
}

int GCF::ErrorDump::rowCount(const QModelIndex &parent) const
{
    if(parent.isValid())
    {
        GCF::ErrorMessage* msg = (GCF::ErrorMessage*)parent.internalPointer();
        return msg->children().count();
    }

    return d->rootMessage->children().count();
}

int GCF::ErrorDump::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 5;
}

QVariant GCF::ErrorDump::data(const QModelIndex &index, int role) const
{
    if(!index.isValid() || role != Qt::DisplayRole)
        return QVariant();

    GCF::ErrorMessage* msg = (GCF::ErrorMessage*)index.internalPointer();
    switch(index.column())
    {
    case 0: return msg->errorLevel();
    case 1: return msg->context();
    case 2: return msg->errorCode();
    case 3: return msg->message();
    case 4: return msg->details();
    default: break;
    }

    return QVariant();
}

QModelIndex GCF::ErrorDump::parent(const QModelIndex &child) const
{
    if(!child.isValid() || child.column() != 0)
        return QModelIndex();

    GCF::ErrorMessage* msg = (GCF::ErrorMessage*)child.internalPointer();
    if(msg->parent() == d->rootMessage)
        return QModelIndex();

    GCF::ErrorMessage* parentMsg = msg->parent();
    int parentRow = parentMsg->parent()->children().indexOf(parentMsg);
    return this->createIndex(parentRow, 0, parentMsg);
}

QModelIndex GCF::ErrorDump::index(int row, int column, const QModelIndex &parent) const
{
    GCF::ErrorMessage* parentMsg = 0;
    if(parent.isValid())
        parentMsg = (GCF::ErrorMessage*)parent.internalPointer();
    else
        parentMsg = d->rootMessage;

    if(row < 0 || row >= parentMsg->children().count())
        return QModelIndex();

    return this->createIndex(row, column, parentMsg->children().at(row));
}

QVariant GCF::ErrorDump::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        switch(section)
        {
        case 0: return tr("Error Level");
        case 1: return tr("Context");
        case 2: return tr("Code");
        case 3: return tr("Message");
        case 4: return tr("Details");
        break;
        }
    }

    return QVariant();
}

void GCF::ErrorDump::dumpErrorMessage(GCF::ErrorMessage* message)
{
    if(!message)
        return;

    IErrorMessageHandler* handler = d->handler.at(0);
    if(handler)
        handler->handleErrorMessage(message);
}

GCF::ErrorMessage* GCF::ErrorDump::pushBranch(const QString& context)
{
    GCF::ErrorMessage* msg = new GCF::ErrorMessage(GCF::ErrorMessage::Info, context,
                                                   QByteArray(), QString(), QString(),
                                                   d->currentBranch());
    d->stack.push(msg);
    return msg;
}

GCF::ErrorMessage* GCF::ErrorDump::popBranch()
{
    if(d->stack.isEmpty())
        return 0;

    return d->stack.pop();
}

void GCF::ErrorDump::messageUpdated(GCF::ErrorMessage*)
{
    // For the moment
    this->reset();
}

void GCF::ErrorDump::messageCreated(GCF::ErrorMessage*)
{
    // For the moment
    this->reset();
}

void GCF::ErrorDump::messageDestroyed(GCF::ErrorMessage* msg)
{
    int index = d->stack.indexOf(msg);
    if(index >= 0)
        d->stack.remove(index, 1);

    // For the moment
    this->reset();
}

QObject* GCF::ErrorDump::containerObject()
{
    return this;
}

void GCF::ErrorDump::handleErrorMessage(GCF::ErrorMessage *msg)
{
    if(!msg)
        return;

    d->timer.stop();

    switch(msg->errorLevel())
    {
    case GCF::ErrorMessage::Fatal: {
        GCF::ErrorMessageDialog dialog(qApp->activeWindow());
        dialog.exec();
        this->clear();
        } break;
    default:
        if(!d->errorDialogVisible)
            d->timer.start(100, this);
        break;
    }
}

void GCF::ErrorDump::print(GCF::ErrorMessage *msg, QTextStream &ts)
{
    int indent = -1;
    GCF::ErrorMessage* tmp = msg;
    while(tmp)
    {
        ++indent;
        tmp = tmp->parent();
    }

    ts << QString(indent*2, QChar(' ')) << msg->context() << " : ErrorLevel(" << msg->errorLevel() << ") "
       << msg->message() << msg->details() << "\n";

    for(int i=0; i<msg->children().count(); i++)
        d->handler.at(0)->print(msg->children().at(i), ts);
}

void GCF::ErrorDump::flush()
{
    this->showErrorDialog();
}

void GCF::ErrorDump::submit(const QString& msg)
{
    if(d->supportEmail.isEmpty())
        return;

    QString url = QString("mailto:%1?subject=Request for support in %2(%3)&body=%4")
            .arg(d->supportEmail).arg(qApp->applicationName()).arg(qApp->applicationVersion())
            .arg(msg);
    QUrl mailUrl(url);
    bool success = QDesktopServices::openUrl(mailUrl);
    if(!success)
    {
        this->error(GCF_DEFAULT_ERROR_CONTEXT, QByteArray(),
                    tr("Cannot launch email client for sending error report"),
                    QString());
        return;
    }
}

void GCF::ErrorDump::timerEvent(QTimerEvent *te)
{
    if(te->timerId() == d->timer.timerId())
    {
        d->timer.stop();
        this->showErrorDialog();
    }
}

void GCF::ErrorDump::showErrorDialog()
{
    if(d->errorDialogVisible)
        return;

    GCF::ErrorMessageDialog dialog(qApp->activeWindow());
    d->errorDialogVisible = true;
    dialog.exec();
    this->clear();
    d->errorDialogVisible = false;
}

QString GCF::ErrorDump::tidyFunctionContext(const QString &context) const
{
    QString fnName = context;
    int index = fnName.indexOf("__thiscall");
    if(index >= 0)
        index += 10;

    fnName.remove(0, index);
    fnName.truncate( fnName.indexOf('(') );
    fnName = fnName.simplified();

    return fnName;
}

