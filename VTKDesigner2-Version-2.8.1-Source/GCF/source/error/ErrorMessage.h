#ifndef ERRORMESSAGE_H
#define ERRORMESSAGE_H

#include <GCF/Common>

namespace GCF
{

class ErrorDump;
class ErrorMessageBranch;

struct ErrorMessageData;
class GCF_EXPORT_CLASS ErrorMessage
{
public:
    ~ErrorMessage();

    GCF::ErrorMessage* parent() const;
    QList<GCF::ErrorMessage*> children() const;

    enum ErrorLevel
    {
        Fatal,
        Error,
        Warning,
        Debug,
        Info,
        User
    };
    int errorLevel() const;
    QString context() const;
    QByteArray errorCode() const;
    QString message() const;
    QString details() const;

    void clear();

protected:
    ErrorMessage(int level,
                 const QString& context,
                 const QByteArray& errorCode,
                 const QString& msg,
                 const QString& details,
                 ErrorMessage* parent=0);
    void setMessage(const QString& msg);

private:
    // Provided for ErrorDump to create a dummy-root-message
    ErrorMessage();

private:
    friend class ErrorDump;
    friend class ErrorMessageBranch;
    ErrorMessageData* d;
};

class GCF_EXPORT_CLASS ErrorMessageBranch
{
public:
    ErrorMessageBranch(const QString& context);
    ~ErrorMessageBranch();

    GCF::ErrorMessage* branchMessage() const { return m_branchMessage; }
    void setMessage(const QString& msg) {
        m_branchMessage->setMessage(msg);
    }
    bool hasErrors() const {
        return m_branchMessage->children().count();
    }

private:
    friend class ErrorDump;
    GCF::ErrorMessage* m_branchMessage;
};


}

#endif // ERRORMESSAGE_H
