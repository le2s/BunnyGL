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

#ifndef FILE_OBJECT_H
#define FILE_OBJECT_H

#include <QObject>
#include <QFile>

struct FileObjectData;
class FileObject : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int Position READ position WRITE setPosition)

public:
    FileObject(const QString& name);
    ~FileObject();

    Q_INVOKABLE QString GetFileName() const;
    Q_INVOKABLE bool OpenForRead();
    Q_INVOKABLE bool OpenForWrite();
    Q_INVOKABLE bool OpenForReadWrite();
    Q_INVOKABLE bool Close();
    Q_INVOKABLE bool IsOpen();
    Q_INVOKABLE QString ReadAll();
    Q_INVOKABLE QString ReadLine();
    Q_INVOKABLE QString ReadField(const QString& delim);
    Q_INVOKABLE bool Write(const QString& str);
    Q_INVOKABLE bool IsAtEnd() const;
    Q_INVOKABLE bool IsAtBeginning() const;

    void setPosition(int pos);
    int position() const;

    const QFile* qfile() const;

private:
    QFile m_file;
};

#endif
