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

#ifndef LOGGER_H
#define LOGGER_H

#include <QString>
#include <GCF/Common>

/*
Provides a simple logger for use with GCF applications. The
design is based on log4cxx, but it is very light weight and
Qt-ised.
*/

namespace GCF
{
class Logger;
class Log;
class AbstractLogBackend;
class LogFileBackend;
class LogConsoleBackend;
class LogQtMsgBackend;
class AbstractLogLayout;
class LogSimpleLayout;
class LogDateLayout;
class LogHtmlLayout;
class LogXmlLayout;

struct LoggerData;
class GCF_EXPORT_CLASS Logger
{
public:
    static Logger& instance();
    ~Logger();

    AbstractLogBackend* fileBackend(const QString& fileName);
    AbstractLogBackend* consoleBackend();
    AbstractLogBackend* qtMsgBackend();

    AbstractLogLayout* simpleLayout();
    AbstractLogLayout* dateLayout();
    AbstractLogLayout* htmlLayout();
    AbstractLogLayout* xmlLayout();

    Log* log(const QString& context, AbstractLogBackend* backend=0, AbstractLogLayout *layout=0);

protected:
    Logger();
    void registerBackend(AbstractLogBackend* backend);
    void unregisterBackend(AbstractLogBackend* backend);
    void registerLayout(AbstractLogLayout* layout);
    void unregisterLayout(AbstractLogLayout* layout);

private:
    friend class AbstractLogBackend;
    friend class AbstractLogLayout;
    LoggerData* d;
};

struct LogData;
class GCF_EXPORT_CLASS Log
{
public:
    ~Log();

    QString context() const;
    AbstractLogBackend* primaryBackend() const;
    AbstractLogLayout* layout() const;

    void addBackend(AbstractLogBackend* backend, AbstractLogLayout* layout=0);
    void removeBackend(AbstractLogBackend* backend);
    int backendCount() const;
    AbstractLogBackend* backend(int index) const;

    void debugLog(const QString& msg);
    void errorLog(const QString& msg);
    void warningLog(const QString& msg);
    void fatalLog(const QString& msg);
    void log(const QString& msg);

protected:
    void writeHeader();
    void writeFooter();
    Log(const QString& context, AbstractLogBackend* backend, AbstractLogLayout* layout);

private:
    friend class Logger;
    LogData* d;
};

struct AbstractLogBackendData;
class GCF_EXPORT_CLASS AbstractLogBackend
{
public:
    virtual ~AbstractLogBackend();

    virtual void debugLog(const QString& msg) = 0;
    virtual void errorLog(const QString& msg) = 0;
    virtual void warningLog(const QString& msg) = 0;
    virtual void fatalLog(const QString& msg) = 0;
    virtual void log(const QString& msg) = 0;
    virtual void write(const QString& text);

protected:
    AbstractLogBackend();

    // Used for concurrency management
    bool hold();
    bool release();

private:
    AbstractLogBackendData* d;
    friend class Log;
};

struct LogFileBackendData;
class GCF_EXPORT_CLASS LogFileBackend : public AbstractLogBackend
{
public:
    ~LogFileBackend();

    QString fileName() const;
    bool isValid() const;

    void setAppendWrite(bool val);
    bool isAppendWrite() const;

    void debugLog(const QString& msg);
    void errorLog(const QString& msg);
    void warningLog(const QString& msg);
    void fatalLog(const QString& msg);
    void log(const QString& msg);

protected:
    LogFileBackend(const QString& fileName);

private:
    friend class Logger;
    LogFileBackendData* d;
};

class GCF_EXPORT_CLASS LogConsoleBackend : public AbstractLogBackend
{
public:
    ~LogConsoleBackend();

    void debugLog(const QString& msg);
    void errorLog(const QString& msg);
    void warningLog(const QString& msg);
    void fatalLog(const QString& msg);
    void log(const QString& msg);

protected:
    friend class Logger;
    LogConsoleBackend();
};

class GCF_EXPORT_CLASS LogQtMsgBackend : public AbstractLogBackend
{
public:
    ~LogQtMsgBackend();

    void debugLog(const QString& msg);
    void errorLog(const QString& msg);
    void warningLog(const QString& msg);
    void fatalLog(const QString& msg);
    void log(const QString& msg);

protected:
    friend class Logger;
    LogQtMsgBackend();
};

class GCF_EXPORT_CLASS AbstractLogLayout
{
public:
    virtual ~AbstractLogLayout();

    enum MessageType
    {
        DebugMessageType,
        ErrorMessageType,
        WarningMessageType,
        FatalMessageType,
        LogMessageType
    };

    virtual QString header(const QString& context) const = 0;
    virtual QString formatString(const QString& context, MessageType msgType, const QString& msg) const = 0;
    virtual QString footer(const QString& context) const = 0;

protected:
    AbstractLogLayout();
    QString messageTypeString(MessageType msgType) const;
};

class GCF_EXPORT_CLASS LogSimpleLayout : public AbstractLogLayout
{
    friend class Logger;
    LogSimpleLayout();

public:
    ~LogSimpleLayout();

    QString header(const QString& context) const;
    QString formatString(const QString& context, MessageType msgType, const QString& msg) const;
    QString footer(const QString& context) const;
};

class GCF_EXPORT_CLASS LogDateLayout : public AbstractLogLayout
{
    friend class Logger;
    LogDateLayout();

public:
    ~LogDateLayout();

    QString header(const QString& context) const;
    QString formatString(const QString& context, MessageType msgType, const QString& msg) const;
    QString footer(const QString& context) const;
};

class GCF_EXPORT_CLASS LogHtmlLayout : public AbstractLogLayout
{
    friend class Logger;
    LogHtmlLayout();

public:
    ~LogHtmlLayout();

    QString header(const QString& context) const;
    QString formatString(const QString& context, MessageType msgType, const QString& msg) const;
    QString footer(const QString& context) const;
};

class GCF_EXPORT_CLASS LogXmlLayout : public AbstractLogLayout
{
    friend class Logger;
    LogXmlLayout();

public:
    ~LogXmlLayout();

    QString header(const QString& context) const;
    QString formatString(const QString& context, MessageType msgType, const QString& msg) const;
    QString footer(const QString& context) const;
};
}

#define GCF_DEBUG(Log, Msg) (Log)->debugLog(Msg)
#define GCF_ERROR(Log, Msg) (Log)->errorLog(Msg)
#define GCF_WARNING(Log, Msg) (Log)->warningLog(Msg)
#define GCF_FATAL(Log, Msg) (Log)->fatalLog(Msg)
#define GCF_LOG(Log, Msg) (Log)->log(Msg)

#endif
