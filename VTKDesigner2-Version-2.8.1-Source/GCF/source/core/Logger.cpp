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

#include <GCF/Logger>
#include <QDir>
#include <QFile>
#include <QMutex>
#include <QtDebug>
#include <QFileInfo>
#include <QDateTime>
#include <stdio.h>

///////////////////////////////////////////////////////////////////////////////
// GCF::Logger implementation
///////////////////////////////////////////////////////////////////////////////

namespace GCF
{
    struct LoggerData
    {
        LoggerData() : inDestructor(false), consoleBackend(0),
                       qtMsgBackend(0), simpleLayout(0),
                       dateLayout(0), htmlLayout(0),
                       xmlLayout(0) { }

        bool inDestructor;
        QList<GCF::Log*> logList;
        QList<GCF::AbstractLogBackend*> backendList;
        QList<GCF::AbstractLogLayout*> layoutList;

        QMap<QString, GCF::LogFileBackend*> fileBackendMap;
        GCF::LogConsoleBackend* consoleBackend;
        GCF::LogQtMsgBackend* qtMsgBackend;

        GCF::AbstractLogLayout* simpleLayout;
        GCF::AbstractLogLayout* dateLayout;
        GCF::AbstractLogLayout* htmlLayout;
        GCF::AbstractLogLayout* xmlLayout;
    };
}

/**
\ingroup logger
\class GCF::Logger GCF/Logger
\brief Core logger class
*/

/**
Since GCF::Logger is a singleton class, the one and only instance of GCF::Logger
can be fetched using the Instance() static method.

As with all singleton classes, the constructor of GCF::Logger class is declared protected
and the destructor is declared public.
*/
GCF::Logger& GCF::Logger::instance()
{
    static Logger theLogger;
    return theLogger;
}

/**
Protected Constructor
*/
GCF::Logger::Logger()
{
    d = new LoggerData;
}

/**
Public Destructor
*/
GCF::Logger::~Logger()
{
    d->inDestructor = true;
    qDeleteAll( d->logList );
    qDeleteAll( d->backendList );
    qDeleteAll( d->layoutList );
    delete d;
}

/**
Creates the instance of GCF::LogFileBackend. If file-backend is present for specified filename already,
then it returns that instance.
It is used to dump log messages on to file.
*/
GCF::AbstractLogBackend* GCF::Logger::fileBackend(const QString& fileName)
{
    GCF::LogFileBackend* fileBackend = 0;
    if(d->fileBackendMap.contains(fileName))
        fileBackend = d->fileBackendMap[fileName];
    else
    {
        fileBackend = new GCF::LogFileBackend(fileName);
        d->fileBackendMap[fileName] = fileBackend;
    }

    return fileBackend;
}

/**
Creates the instance of GCF::LogConsoleBackend. If console-backend has already been created once,
it returns the exisiting instance.
It is used to dump log messages on to console.
*/
GCF::AbstractLogBackend* GCF::Logger::consoleBackend()
{
    if(!d->consoleBackend)
        d->consoleBackend = new GCF::LogConsoleBackend;

    return d->consoleBackend;
}

/**
Creates the instance of GCF::qtMsgBackend. If message-backend has already been created once,
it returns the exisiting instance.
It is used by Logger to dump log messages.
*/
GCF::AbstractLogBackend* GCF::Logger::qtMsgBackend()
{
    if(!d->qtMsgBackend)
        d->qtMsgBackend = new GCF::LogQtMsgBackend;

    return d->qtMsgBackend;
}

/**
Creates instance of GCF::LogSimpleLayout. If the instance has already been created once,
it returns the exisiting one.
Used for writing log messages in simple layout.
*/
GCF::AbstractLogLayout* GCF::Logger::simpleLayout()
{
    if(!d->simpleLayout)
        d->simpleLayout = new GCF::LogSimpleLayout;

    return d->simpleLayout;
}

/**
Creates instance of GCF::LogDateLayout. If the instance has already been created once,
it returns the exisiting one.
Used for writing log messages in date layout.
*/
GCF::AbstractLogLayout* GCF::Logger::dateLayout()
{
    if(!d->dateLayout)
        d->dateLayout = new GCF::LogDateLayout;

    return d->dateLayout;
}

/**
Creates instance of GCF::htmlLayout. If the instance has already been created once,
it returns the exisiting one.
Used for writing log messages in html layout.
*/
GCF::AbstractLogLayout* GCF::Logger::htmlLayout()
{
    if(!d->htmlLayout)
        d->htmlLayout = new GCF::LogHtmlLayout;

    return d->htmlLayout;
}

/**
Creates instance of GCF::xmlLayout. If the instance has already been created once,
it returns the exisiting one.
Used for writing log messages in xml layout.
*/
GCF::AbstractLogLayout* GCF::Logger::xmlLayout()
{
    if(!d->xmlLayout)
        d->xmlLayout = new GCF::LogXmlLayout;

    return d->xmlLayout;
}

/**
Logs messages into specified backend in layout style, and returns the pointer to GCF::Log.
If layout is invalid, LogSimpleLayout is considered.
If backend is invalid, LogQtMsgBackend is considered.
Log is written under the specified context.
*/
GCF::Log* GCF::Logger::log(const QString& context, GCF::AbstractLogBackend* backend, AbstractLogLayout *layout)
{
    for(int i=0; i<d->logList.count(); i++)
    {
        GCF::Log* log = d->logList[i];
        if( log->context() == context )
            return log;
    }

    GCF::AbstractLogBackend* backend2 = backend;
    if( !backend2 )
        backend2 = qtMsgBackend();

    GCF::AbstractLogLayout* layout2 = layout;
    if(!layout2)
        layout2 = simpleLayout();

    GCF::Log* log = new GCF::Log(context, backend2, layout2);
    d->logList.append(log);

    return log;
}

void GCF::Logger::registerBackend(GCF::AbstractLogBackend* backend)
{
    d->backendList.append(backend);
}

void GCF::Logger::unregisterBackend(GCF::AbstractLogBackend* backend)
{
    if(d->inDestructor)
        return;

    d->backendList.removeAll(backend);
}

void GCF::Logger::registerLayout(AbstractLogLayout* layout)
{
    d->layoutList.append(layout);
}

void GCF::Logger::unregisterLayout(AbstractLogLayout* layout)
{
    if(d->inDestructor)
        return;

    d->layoutList.removeAll(layout);
}

///////////////////////////////////////////////////////////////////////////////
// GCF::Log implementation
///////////////////////////////////////////////////////////////////////////////

/**
\ingroup logger
\class GCF::Log GCF/Logger
\brief Performs logging of messages.
*/

/**
Logging is done through instances of this class.
Provides methods to log debug, error, critical and fatal messages. It makes use of a
GCF::AbstractLogLayout instance to format messages before dumping the message into a
GCF::AbstractLogBackend instance.

Instance of GCF::Log can be created using GCF::Logger::log() method against a
{context, backend, layout} triplet. Multiple calls to log() with the same context name returns
the same log object.
*/

namespace GCF
{
    struct LogData
    {
        LogData() : primaryBackend(0), layout(0),
                    headerWritten(false),
                    footerWritten(false) { }

        QString context;
        GCF::AbstractLogBackend* primaryBackend;
        QList<GCF::AbstractLogBackend*> backendList;
        QMap<GCF::AbstractLogBackend*,GCF::AbstractLogLayout*> backendLayoutMap;
        GCF::AbstractLogLayout* layout;
        bool headerWritten;
        bool footerWritten;
    };
}

/**
Protected Constructor.
*/
GCF::Log::Log(const QString& context, AbstractLogBackend* backend, AbstractLogLayout* layout)
{
    d = new GCF::LogData;
    d->context = context;
    d->primaryBackend = backend;
    d->layout = layout;
}

/**
Public Desctructor.
*/
GCF::Log::~Log()
{
    writeFooter();

    delete d;
}

/**
Returns the contenxt of the Log. A single application might have several types of log messages.
"Context" identifies each one by a context name so that all context type logs can be written into
single log file.
*/
QString GCF::Log::context() const
{
    return d->context;
}

/**
Returns the primary backgroud.
When Logger::instance().log(context, backend, layout) is called for the first time, the backend becomes
primary backend. Then on, whenever backends are added to the instance of the log class using addBackend()
method, they become non-primary backends.
*/
GCF::AbstractLogBackend* GCF::Log::primaryBackend() const
{
    return d->primaryBackend;
}

/**
Returns the layout used by GCF::Log instance.
*/
GCF::AbstractLogLayout* GCF::Log::layout() const
{
    return d->layout;
}

/**
Adds additional backends. When log messages are written, they show up in all the backends added
at a time.
*/
void GCF::Log::addBackend(GCF::AbstractLogBackend* backend, AbstractLogLayout* layout)
{
    if(!backend || backend == d->primaryBackend)
        return;

    if(d->backendList.contains(backend))
        return;

    d->backendList.append(backend);
    if(layout)
        d->backendLayoutMap[backend] = layout;
}

/**
Removes the backend fro the list of backends. Note that one can not remove primaryBackend.
*/
void GCF::Log::removeBackend(GCF::AbstractLogBackend* backend)
{
    if(!backend || backend == d->primaryBackend)
        return;

    d->backendList.removeAll(backend);
    d->backendLayoutMap.remove(backend);
}

/**
Returns backend count.
*/
int GCF::Log::backendCount() const
{
    return d->backendList.count();
}

/**
Returns pointer backend given the index.
*/
GCF::AbstractLogBackend* GCF::Log::backend(int index) const
{
    if(index < 0 || index >= d->backendList.count())
        return 0;

    return d->backendList[index];
}

/**
Writes debug log message msg to the file.
*/
void GCF::Log::debugLog(const QString& msg)
{
    if(!d->layout)
        return;

    writeHeader(); // this one works only once

    QString msg2 = d->layout->formatString(d->context, GCF::AbstractLogLayout::DebugMessageType, msg);
    if(d->primaryBackend)
    {
        d->primaryBackend->hold();
        d->primaryBackend->debugLog(msg2);
        d->primaryBackend->release();
    }

    Q_FOREACH(GCF::AbstractLogBackend* backend, d->backendList)
    {
        QString msg3 = msg2;
        if(d->backendLayoutMap.contains(backend))
        {
            AbstractLogLayout* layout2 = d->backendLayoutMap[backend];
            msg3 = layout2->formatString(d->context, GCF::AbstractLogLayout::DebugMessageType, msg);
        }

        backend->hold();
        backend->debugLog(msg3);
        backend->release();
    }
}

/**
Writes error log message msg to the file.
*/
void GCF::Log::errorLog(const QString& msg)
{
    if(!d->layout)
        return;

    writeHeader(); // this one works only once

    QString msg2 = d->layout->formatString(d->context, GCF::AbstractLogLayout::ErrorMessageType, msg);
    if(d->primaryBackend)
    {
        d->primaryBackend->hold();
        d->primaryBackend->errorLog(msg2);
        d->primaryBackend->release();
    }

    Q_FOREACH(GCF::AbstractLogBackend* backend, d->backendList)
    {
        QString msg3 = msg2;
        if(d->backendLayoutMap.contains(backend))
        {
            AbstractLogLayout* layout2 = d->backendLayoutMap[backend];
            msg3 = layout2->formatString(d->context, GCF::AbstractLogLayout::ErrorMessageType, msg);
        }

        backend->hold();
        backend->errorLog(msg3);
        backend->release();
    }
}

/**
Writes warning log message msg to the file.
*/
void GCF::Log::warningLog(const QString& msg)
{
    if(!d->layout)
        return;

    writeHeader(); // this one works only once

    QString msg2 = d->layout->formatString(d->context, GCF::AbstractLogLayout::WarningMessageType, msg);
    if(d->primaryBackend)
    {
        d->primaryBackend->hold();
        d->primaryBackend->warningLog(msg2);
        d->primaryBackend->release();
    }

    Q_FOREACH(GCF::AbstractLogBackend* backend, d->backendList)
    {
        QString msg3 = msg2;
        if(d->backendLayoutMap.contains(backend))
        {
            AbstractLogLayout* layout2 = d->backendLayoutMap[backend];
            msg3 = layout2->formatString(d->context, GCF::AbstractLogLayout::WarningMessageType, msg);
        }

        backend->hold();
        backend->warningLog(msg3);
        backend->release();
    }
}

/**
Writes fatal log message msg to the file.
*/
void GCF::Log::fatalLog(const QString& msg)
{
    if(!d->layout)
        return;

    writeHeader(); // this one works only once

    QString msg2 = d->layout->formatString(d->context, GCF::AbstractLogLayout::FatalMessageType, msg);
    if(d->primaryBackend)
    {
        d->primaryBackend->hold();
        d->primaryBackend->fatalLog(msg2);
        d->primaryBackend->release();
    }

    Q_FOREACH(GCF::AbstractLogBackend* backend, d->backendList)
    {
        QString msg3 = msg2;
        if(d->backendLayoutMap.contains(backend))
        {
            AbstractLogLayout* layout2 = d->backendLayoutMap[backend];
            msg3 = layout2->formatString(d->context, GCF::AbstractLogLayout::FatalMessageType, msg);
        }

        backend->hold();
        backend->fatalLog(msg3);
        backend->release();
    }
}

/**
Writes log message msg to the file.
*/
void GCF::Log::log(const QString& msg)
{
    if(!d->layout)
        return;

    writeHeader(); // this one works only once

    QString msg2 = d->layout->formatString(d->context, GCF::AbstractLogLayout::LogMessageType, msg);
    if(d->primaryBackend)
    {
        d->primaryBackend->hold();
        d->primaryBackend->log(msg2);
        d->primaryBackend->release();
    }

    Q_FOREACH(GCF::AbstractLogBackend* backend, d->backendList)
    {
        QString msg3 = msg2;
        if(d->backendLayoutMap.contains(backend))
        {
            AbstractLogLayout* layout2 = d->backendLayoutMap[backend];
            msg3 = layout2->formatString(d->context, GCF::AbstractLogLayout::LogMessageType, msg);
        }

        backend->hold();
        backend->log(msg3);
        backend->release();
    }
}

/**
Writes the header. Header is written only once.
*/
void GCF::Log::writeHeader()
{
    if(d->headerWritten || !d->layout)
        return;

    QString hdr = d->layout->header(d->context);
    if(d->primaryBackend)
    {
        d->primaryBackend->hold();
        d->primaryBackend->write(hdr);
        d->primaryBackend->release();
    }

    Q_FOREACH(GCF::AbstractLogBackend* backend, d->backendList)
    {
        QString hdr2 = hdr;
        if(d->backendLayoutMap.contains(backend))
        {
            AbstractLogLayout* layout2 = d->backendLayoutMap[backend];
            hdr2 = layout2->header(d->context);
        }

        backend->hold();
        backend->write(hdr2);
        backend->release();
    }

    d->headerWritten = true;
}

/**
Writes the Footer. Header is written only once.
*/
void GCF::Log::writeFooter()
{
    if(d->footerWritten || !d->layout)
        return;

    QString ftr = d->layout->footer(d->context);
    if(d->primaryBackend)
    {
        d->primaryBackend->hold();
        d->primaryBackend->write(ftr);
        d->primaryBackend->release();
    }

    Q_FOREACH(GCF::AbstractLogBackend* backend, d->backendList)
    {
        QString ftr2 = ftr;
        if(d->backendLayoutMap.contains(backend))
        {
            AbstractLogLayout* layout2 = d->backendLayoutMap[backend];
            ftr2 = layout2->footer(d->context);
        }

        backend->hold();
        backend->write(ftr2);
        backend->release();
    }

    d->footerWritten = true;
}

///////////////////////////////////////////////////////////////////////////////
// GCF::AbstractLogBackend implementation
///////////////////////////////////////////////////////////////////////////////

/**
\ingroup logger
\class GCF::AbstractLogBackend GCF/Logger
\Base class for all the backend classes.
*/

namespace GCF
{
    struct AbstractLogBackendData
    {
        QMutex mutex;
    };
}

/**
Protected Constructor.
*/
GCF::AbstractLogBackend::AbstractLogBackend()
{
    d = new AbstractLogBackendData;
    GCF::Logger::instance().registerBackend(this);
}

/**
Public Destructor.
*/
GCF::AbstractLogBackend::~AbstractLogBackend()
{
    GCF::Logger::instance().unregisterBackend(this);
    delete d;
}

/**
Wrires tge test to the backend by calling log function.
*/
void GCF::AbstractLogBackend::write(const QString& text)
{
    this->log(text);
}

bool GCF::AbstractLogBackend::hold()
{
    d->mutex.lock();
    return true;
}

bool GCF::AbstractLogBackend::release()
{
    d->mutex.unlock();
    return true;
}

/**
\fn virtual void GCF::AbstractLogBackend::debugLog(const QString& msg) = 0;
Writes debug log message msg to the file. Pure virtual function. Subclasses must
re-implement this method.
*/

/**
\fn virtual void GCF::AbstractLogBackend::errorLog(const QString& msg) = 0;
Writes error log message msg to the file. Pure virtual function. Subclasses must
re-implement this method.
*/

/**
\fn virtual void GCF::AbstractLogBackend::warningLog(const QString& msg) = 0;
Writes warning log message msg to the file. Pure virtual function. Subclasses must
re-implement this method.
*/

/**
\fn virtual void GCF::AbstractLogBackend::fatalLog(const QString& msg) = 0;
Writes fatal log message msg to the file. Pure virtual function. Subclasses must
re-implement this method.
*/

/**
\fn virtual void GCF::AbstractLogBackend::log(const QString& msg) = 0;
Writes log message msg to the file. Pure virtual function. Subclasses must
re-implement this method.
*/

///////////////////////////////////////////////////////////////////////////////
// GCF::LogFileBackend implementation
///////////////////////////////////////////////////////////////////////////////

/**
\ingroup logger
\class GCF::LogFileBackend GCF/Logger
Backend for writing the log into the text file.
*/

/**
This is a subclass of AbstractLogBackend class.
*/

namespace GCF
{
    struct LogFileBackendData
    {
        LogFileBackendData() : file(0) { }

        QString fileName;
        QFile* file;
    };
}

/**
Protected Constructor. Pass the file name of the file into which the messages are to be logged.
If file does not exist, it creates one.
*/
GCF::LogFileBackend::LogFileBackend(const QString& fileName)
{
    d = new LogFileBackendData;

    // Ensure that the path to the filename exists! Otherwise,
    // logs dont get created :-(
    QFileInfo fi(fileName);
    QDir().mkpath( fi.absolutePath() );

    d->fileName = fileName;
    d->file = new QFile(fileName);
    if( !d->file->open(QFile::Append) )
    {
        delete d->file;
        d->file = 0;
    }
}

/**
Public Destructor.
*/
GCF::LogFileBackend::~LogFileBackend()
{
    if(d->file)
    {
        d->file->close();
        delete d->file;
    }

    delete d;
}

/**
Returns filename of the file into which log is written.
*/
QString GCF::LogFileBackend::fileName() const
{
    return d->fileName;
}

/**
Returns true if file pointer pointing to the backend file is valid.
*/
bool GCF::LogFileBackend::isValid() const
{
    return (d->file != 0);
}

/**
Sets the write mode of the file to Append mode if val is true. If false, sets the mode to
WriteOnly.
*/
void GCF::LogFileBackend::setAppendWrite(bool val)
{
    if(d->file)
    {
        if(val && d->file->openMode() == QFile::Append)
            return;

        if(!val && d->file->openMode() == QFile::WriteOnly)
            return;

        d->file->close();
        delete d->file;
        d->file = 0;
    }

    bool success = false;
    d->file = new QFile(d->fileName);
    if(val)
        success = d->file->open( QFile::Append );
    else
        success = d->file->open( QFile::WriteOnly );

    if(!success)
    {
        delete d->file;
        d->file = 0;
    }
}

/**
Returns true if the mode of the file being written is Append mode. Else, returns false.
*/
bool GCF::LogFileBackend::isAppendWrite() const
{
    return d->file && (d->file->openMode() == QFile::Append);
}

/**
Writes debug log message msg to the file.
*/
void GCF::LogFileBackend::debugLog(const QString& msg)
{
    if(d->file)
    {
        d->file->write(msg.toAscii());
        d->file->flush();
    }
}

/**
Writes error log message msg to the file.
*/
void GCF::LogFileBackend::errorLog(const QString& msg)
{
    if(d->file)
    {
        d->file->write(msg.toAscii());
        d->file->flush();
    }
}

/**
Writes warning log message msg to the file.
*/
void GCF::LogFileBackend::warningLog(const QString& msg)
{
    if(d->file)
    {
        d->file->write(msg.toAscii());
        d->file->flush();
    }
}

/**
Writes fatal log message msg to the file.
*/
void GCF::LogFileBackend::fatalLog(const QString& msg)
{
    if(d->file)
    {
        d->file->write(msg.toAscii());
        d->file->flush();
    }
}

/**
Writes log message msg to the file.
*/
void GCF::LogFileBackend::log(const QString& msg)
{
    if(d->file)
    {
        d->file->write(msg.toAscii());
        d->file->flush();
    }
}

///////////////////////////////////////////////////////////////////////////////
// GCF::LogConsoleBackend implementation
///////////////////////////////////////////////////////////////////////////////

/**
\ingroup logger
\class GCF::LogConsoleBackend GCF/Logger
Backend for writing the log to the console.
*/

/**
This is a subclass of AbstractLogBackend class.
*/

/**
Protected Constructor.
*/
GCF::LogConsoleBackend::LogConsoleBackend()
{

}

/**
Public Destructor.
*/
GCF::LogConsoleBackend::~LogConsoleBackend()
{

}

/**
Writes the debug message msg to the console.
*/
void GCF::LogConsoleBackend::debugLog(const QString& msg)
{
    fprintf(stdout, "%s", qPrintable(msg));
}

/**
Writes the error message msg to the console.
*/
void GCF::LogConsoleBackend::errorLog(const QString& msg)
{
    fprintf(stdout, "%s", qPrintable(msg));
}

/**
Writes the warning message msg to the console.
*/
void GCF::LogConsoleBackend::warningLog(const QString& msg)
{
    fprintf(stderr, "%s", qPrintable(msg));
}

/**
Writes the fatal message msg to the console.
*/
void GCF::LogConsoleBackend::fatalLog(const QString& msg)
{
    fprintf(stderr, "%s", qPrintable(msg));
}

/**
Writes the message msg to the console.
*/
void GCF::LogConsoleBackend::log(const QString& msg)
{
    fprintf(stdout, "%s", qPrintable(msg));
}

///////////////////////////////////////////////////////////////////////////////
// GCF::LogQtMsgBackend implementation
///////////////////////////////////////////////////////////////////////////////

/**
\ingroup logger
\class GCF::LogQtMsgBackend GCF/Logger
Backend for writing the messages as qt messages.
*/

/**
Subclasses from AbstractLogBackend.
Uses methods such as qDebug, qWarning etc to write the log messages.
*/

/**
Protected Constructor.
*/
GCF::LogQtMsgBackend::LogQtMsgBackend()
{

}

/**
Public Destructor.
*/
GCF::LogQtMsgBackend::~LogQtMsgBackend()
{

}

/**
Uses qDebug() to write the log message.
*/
void GCF::LogQtMsgBackend::debugLog(const QString& msg)
{
    qDebug( "%s", qPrintable(msg) );
}

/**
Uses qCritical() to write the log message.
*/
void GCF::LogQtMsgBackend::errorLog(const QString& msg)
{
    qCritical( "%s", qPrintable(msg) );
}

/**
Uses qWarning() to write the log message.
*/
void GCF::LogQtMsgBackend::warningLog(const QString& msg)
{
    qWarning( "%s", qPrintable(msg) );
}

/**
Uses qFatal() to write the log message.
*/
void GCF::LogQtMsgBackend::fatalLog(const QString& msg)
{
    qFatal( "%s", qPrintable(msg) );
}

/**
Uses qDebug() to write the log message. This method is provided for convenience.
*/
void GCF::LogQtMsgBackend::log(const QString& msg)
{
    qDebug( "%s", qPrintable(msg) );
}

///////////////////////////////////////////////////////////////////////////////
// GCF::AbstractLogLayout implementation
///////////////////////////////////////////////////////////////////////////////

/**
\ingroup logger
\class GCF::AbstractLogLayout GCF/Logger
Absract class for all layout classes.
*/

/**
Log messages are formatted using instances of these classes.Following calles inherit from
this class.
\li LogSimpleLayout
\li LogDateLayout
\li LogHtmlLayout
\li LogXmlLayout
*/

/**
Protected Constructor.
*/
GCF::AbstractLogLayout::AbstractLogLayout()
{
    GCF::Logger::instance().registerLayout(this);
}

/**
Public Destructor.
*/
GCF::AbstractLogLayout::~AbstractLogLayout()
{
    GCF::Logger::instance().unregisterLayout(this);
}

/**
Returns corresponding string for the message type MessageType.
For example, "DEBUG" string is returned for msgType DebugMessageType.
*/
QString GCF::AbstractLogLayout::messageTypeString(MessageType msgType) const
{
    switch(msgType)
    {
    case DebugMessageType:
        return "DEBUG";
    case ErrorMessageType:
        return "ERROR";
    case WarningMessageType:
        return "WARNING";
    case FatalMessageType:
        return "FATAL";
    case LogMessageType:
        return "LOG";
    }

    return "LOG";
}

///////////////////////////////////////////////////////////////////////////////
// GCF::LogSimpleLayout implementation
///////////////////////////////////////////////////////////////////////////////

/**
\ingroup logger
\class GCF::LogSimpleLayout GCF/Logger
Formats messages as "Simple Context: [DEBUG] Debug message" while loging into the backend.
*/

/**
Private Constructor.
*/
GCF::LogSimpleLayout::LogSimpleLayout()
{

}

/**
Public Destructor.
*/
GCF::LogSimpleLayout::~LogSimpleLayout()
{

}

/**
\todo document this method
*/
QString GCF::LogSimpleLayout::header(const QString& context) const
{
    Q_UNUSED(context);
    return QString();
}

/**
Returns the Formatted string using the context, MessageType and the Log message.
For Example, "Simple Context: [ERROR] Failed to login".
*/
QString GCF::LogSimpleLayout::formatString(const QString& context, MessageType msgType, const QString& msg) const
{
    return QString("%1: [%2] %3\n").arg(context).arg(messageTypeString(msgType)).arg(msg);
}

/**
\todo document this method
*/
QString GCF::LogSimpleLayout::footer(const QString& context) const
{
    Q_UNUSED(context);
    return QString();
}

///////////////////////////////////////////////////////////////////////////////
// GCF::LogDateLayout implementation
///////////////////////////////////////////////////////////////////////////////

/**
\ingroup logger
\class GCF::LogDateLayout GCF/Logger
Formats messages as "Wed Apr 8 10:16:51 2009 - Date Context: [DEBUG] Debug message"
*/

/**
Private Constructor.
*/
GCF::LogDateLayout::LogDateLayout()
{

}

/**
Public Destructor.
*/
GCF::LogDateLayout::~LogDateLayout()
{

}

/**
\todo document this method
*/
QString GCF::LogDateLayout::header(const QString& context) const
{
    Q_UNUSED(context);
    return QString();
}

/**
Returns the Formatted string using the context, MessageType and the Log message.
For Example, "Wed Apr 8 10:16:51 2009 - Date Context: [DEBUG] Debug message"
*/
QString GCF::LogDateLayout::formatString(const QString& context, MessageType msgType, const QString& msg) const
{
    QString theDateTimeStr = QDateTime::currentDateTime().toString();
    return QString("%1 - %2: [%3] %4\n").arg(theDateTimeStr).arg(context)
                                      .arg(messageTypeString(msgType)).arg(msg);
}

/**
\todo document this method
*/
QString GCF::LogDateLayout::footer(const QString& context) const
{
    Q_UNUSED(context);
    return QString();
}

///////////////////////////////////////////////////////////////////////////////
// GCF::LogHtmlLayout implementation
///////////////////////////////////////////////////////////////////////////////

/**
\ingroup logger
\class GCF::LogHtmlLayout GCF/Logger
Writes logs into a HTML file
*/

/**
Private Consturctor.
*/
GCF::LogHtmlLayout::LogHtmlLayout()
{

}

/**
Public Destructor.
*/
GCF::LogHtmlLayout::~LogHtmlLayout()
{

}

/**
Returns the title string given the context in html format.
Current date will be added to the title.
*/
QString GCF::LogHtmlLayout::header(const QString& context) const
{
    QString theDateTimeStr = QDateTime::currentDateTime().toString();
    return  QString("<html>"
            "<title>\"%1\" Log Messages</title>"
            "<body><h1>%1 - %2</h1>"
            "<table border=1 width=100%>"
            "<tr><td width=10%>Type</td><td>Message</td></tr>\n")
            .arg(context)
            .arg(theDateTimeStr);
}

/**
\todo document this method
*/
QString GCF::LogHtmlLayout::formatString(const QString& context, MessageType msgType, const QString& msg) const
{
    QString color = "#ffffff";
    switch(msgType)
    {
    case DebugMessageType:
        color = "#aaaaaa";
        break;
    case ErrorMessageType:
        color = "#ff7777";
        break;
    case WarningMessageType:
        color = "#f5e78d"; // based on my Wife's inputs..
        break;
    case FatalMessageType:
        color = "#ff0000";
        break;
    case LogMessageType:
        break;
    }

    return QString("<tr bgcolor=\"%1\">"
                   "<td width=10%>%2</td>"
                   "<td>%3</td></tr>\n")
                   .arg(color)
                   .arg(messageTypeString(msgType))
                   .arg(msg);

    Q_UNUSED(context);
}

/**
Returns the footer string given the context in the html format.
*/
QString GCF::LogHtmlLayout::footer(const QString& context) const
{
    Q_UNUSED(context);
    return "</table></body></html>\n";
}

///////////////////////////////////////////////////////////////////////////////
// GCF::LogXmlLayout implementation
///////////////////////////////////////////////////////////////////////////////

/**
\ingroup logger
\class GCF::LogXmlLayout GCF/Logger
Writes logs into a XML file
*/

/**
Private Constructor.
*/
GCF::LogXmlLayout::LogXmlLayout()
{

}

/**
Public Destructor.
*/
GCF::LogXmlLayout::~LogXmlLayout()
{

}

/**
Returns the title string given the context in XML format.
Current date will be added to the title.
For exmample : <Logs Context = Simple Context Date=<Current Date>"
*/
QString GCF::LogXmlLayout::header(const QString& context) const
{
    QString theDateTimeStr = QDateTime::currentDateTime().toString();
    return QString("<Logs Context=\"%1\" Date=\"%2\">\n").arg(context).arg(theDateTimeStr);
}

/**
Returns the formatted string given the context, MessageType ang log message in XML format.
*/
QString GCF::LogXmlLayout::formatString(const QString& context, MessageType msgType, const QString& msg) const
{
    return QString(
            "  <Log>\n"
            "    <Type>%1</Type>\n"
            "    <Message>%2</Message>\n"
            "  </Log>\n")
            .arg(messageTypeString(msgType))
            .arg(msg);

    Q_UNUSED(context);
}

/**
Returns the footer as "</Logs>\n"
*/
QString GCF::LogXmlLayout::footer(const QString& context) const
{
    Q_UNUSED(context);
    return "</Logs>\n";
}
