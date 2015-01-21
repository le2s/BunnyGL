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

#ifndef FILE_SYSTEM_OBJECT_H
#define FILE_SYSTEM_OBJECT_H

#include <QObject>

class QDir;
class FileSystemObject : public QObject
{
    Q_OBJECT

public:
    static FileSystemObject& instance();
    ~FileSystemObject();

    Q_INVOKABLE bool CreateFile(const QString& file);
    Q_INVOKABLE bool CreateDirectory(const QString& file);
    Q_INVOKABLE bool RemoveFile(const QString& file);
    Q_INVOKABLE bool RemoveDirectory(const QString& file);
    Q_INVOKABLE bool RemoveDirectoryRecursive(const QString& file);
    Q_INVOKABLE bool Copy(const QString& source, const QString& dest);
    Q_INVOKABLE bool Move(const QString& source, const QString& dest);
    Q_INVOKABLE bool CopyDirectory(const QString& source, const QString& dest);
    Q_INVOKABLE bool MoveDirectory(const QString& source, const QString& dest);
    Q_INVOKABLE bool Rename(const QString& source, const QString& dest);
    Q_INVOKABLE bool IsExists(const QString& file);
    Q_INVOKABLE bool IsReadable(const QString& file);
    Q_INVOKABLE bool IsWritable(const QString& file);
    Q_INVOKABLE QObject* NewFile(const QString& file);

    void releaseAllFileObjects();

private:
    FileSystemObject(QObject* parent=0);

    void deleteDirectory(const QDir& d, bool deleteDir=true);
    bool copyDirectory(const QDir& source, const QDir& dest);
};

#endif

