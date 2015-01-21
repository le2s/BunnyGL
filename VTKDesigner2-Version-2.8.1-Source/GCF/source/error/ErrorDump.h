#ifndef ERRORDUMP_H
#define ERRORDUMP_H

#include <QAbstractItemModel>
#include <GCF/Common>
#include "IErrorMessageHandler.h"
#include "ErrorMessage.h"

namespace GCF
{

class ErrorMessage;
class ErrorMessageBranch;

struct ErrorDumpData;
class GCF_EXPORT_CLASS ErrorDump : public QAbstractItemModel,
                        virtual public IErrorMessageHandler
{
    Q_OBJECT
    Q_INTERFACES(IErrorMessageHandler)

public:
    static ErrorDump& instance();
    ~ErrorDump();

    void setHandler(IErrorMessageHandler* handler);
    IErrorMessageHandler* handler() const;

    void setSupportEMail(const QString& email);
    QString supportEMail() const;

    void fatal(const QString& context, const QByteArray& errorCode,
               const QString& message, const QString& details);
    void error(const QString& context, const QByteArray& errorCode,
               const QString& message, const QString& details);
    void warning(const QString& context, const QByteArray& errorCode,
               const QString& message, const QString& details);
    void debug(const QString& context, const QByteArray& errorCode,
               const QString& message, const QString& details);
    void info(const QString& context, const QByteArray& errorCode,
               const QString& message, const QString& details);
    void clear();

    QList<GCF::ErrorMessage*> errorMessages() const;
    QString toString() const;
    void copyToClipboard();
    void submitToSupport();

    // QAbstractItemModel interface
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QModelIndex parent(const QModelIndex &child) const;
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

protected:
    ErrorDump();
    void dumpErrorMessage(GCF::ErrorMessage* message);

    // These methods will only be used via GCF::ErrorMessageBranch
    GCF::ErrorMessage* pushBranch(const QString& context);
    GCF::ErrorMessage* popBranch();
    void messageUpdated(GCF::ErrorMessage* msg);
    void messageCreated(GCF::ErrorMessage* msg);
    void messageDestroyed(GCF::ErrorMessage* msg);

public:
    // IContainer implementation
    QObject* containerObject();

    // IErrorMessageHandler implementation
    void handleErrorMessage(GCF::ErrorMessage *msg);
    void print(GCF::ErrorMessage *msg, QTextStream &ts);
    void submit(const QString& msg);
    void flush();

protected:
    void timerEvent(QTimerEvent *);
    void showErrorDialog();

public:
    QString tidyFunctionContext(const QString& context) const;

private:
    friend class ErrorMessageBranch;
    friend class ErrorMessage;
    ErrorDumpData* d;
};

}

#define GCF_DEFAULT_ERROR_CONTEXT GCF::ErrorDump::instance().tidyFunctionContext(Q_FUNC_INFO)

#endif // ERRORDUMP_H
