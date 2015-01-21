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

#include "FileSystemObject.h"
#include "FileObject.h"
#include "UpdateInstaller.h"

#include <QDir>
#include <QFile>
#include <QPointer>
#include <QFileInfo>
#include <QByteArray>
#include <QCoreApplication>

FileSystemObject& FileSystemObject::instance()
{
    static QPointer<FileSystemObject> ptr = new FileSystemObject(qApp);
    return *(ptr.data());
}

FileSystemObject::FileSystemObject(QObject* parent)
:QObject(parent)
{

}

FileSystemObject::~FileSystemObject()
{

}

bool FileSystemObject::CreateFile(const QString& file)
{
    // Resolve the file name by replacing ${APPDIR}, ${TEMP} etc
    QString file2 = UpdateInstaller::instance().resolveFileName(file);

    // If the file already exists, then there is nothing to create
    if( QFile::exists(file2) )
    {
        GCF_UINST_LOG( QString("  File \"%1\" exists").arg(file) );
        return true;
    }

    // Create the file by opening it in writeonly mode
    QFile fileObj(file2);
    if( !fileObj.open(QFile::WriteOnly) )
        return false;
    fileObj.write( QByteArray(" ") );
    fileObj.close();

    GCF_UINST_LOG( QString("  Created file \"%1\"").arg(file) );
    return true;
}

bool FileSystemObject::CreateDirectory(const QString& file)
{
    // Resolve the file name by replacing ${APPDIR}, ${TEMP} etc
    QString file2 = UpdateInstaller::instance().resolveFileName(file);

    // If the directory already exists, then no need to create one.
    QDir dir(file2);
    if( dir.exists() )
    {
        GCF_UINST_LOG( QString("  Directory \"%1\" exists").arg(file) );
        return true;
    }

    // Create the directory
    bool success = dir.mkpath(file2);
    if(success)
        GCF_UINST_LOG( QString("  Created directory \"%1\"").arg(file) );
    else
        GCF_UINST_ERROR( QString("  Could not create directory \"%1\"").arg(file) );
    return success;
}

bool FileSystemObject::RemoveFile(const QString& file)
{
    // Resolve the file name by replacing ${APPDIR}, ${TEMP} etc
    QString file2 = UpdateInstaller::instance().resolveFileName(file);
    if( !QFileInfo(file2).exists() )
        return true;

    // Ensure that file2 is a file name and not a directory name.
    if( !QFileInfo(file2).isFile() )
    {
        GCF_UINST_ERROR( QString("  Cannot remove file \"%1\"").arg(file2) );
        return false;
    }

    // Remove the file
    bool success = QFile::remove(file2);
    if(success)
        GCF_UINST_LOG( QString("  Removed file \"%1\"").arg(file2) );
    else
        GCF_UINST_LOG( QString("  Could not reomve file \"%1\"").arg(file2) );
    return success;
}

bool FileSystemObject::RemoveDirectory(const QString& file)
{
    // Resolve the file name by replacing ${APPDIR}, ${TEMP} etc
    QString file2 = UpdateInstaller::instance().resolveFileName(file);

    // Check to see if the directory exists
    QDir dir(file2);
    if( !dir.exists() || dir.isRoot() )
    {
        GCF_UINST_LOG( QString("  Cannot remove directory \"%1\"").arg(file2) );
        return false;
    }

    // Try to remove the directory
    QString dirName = dir.dirName();
    dir.cdUp();
    bool success = dir.rmdir(dirName);
    if(success)
        GCF_UINST_LOG( QString("  Removed directory \"%1\"").arg(file2) );
    else
        GCF_UINST_ERROR( QString("  Cannot remove directory \"%1\"").arg(file2) );
    return success;
}

bool FileSystemObject::RemoveDirectoryRecursive(const QString& file)
{
    // Resolve the file name by replacing ${APPDIR}, ${TEMP} etc
    QString file2 = UpdateInstaller::instance().resolveFileName(file);

    // Check to see if the directory exists
    QDir dir(file2);
    if( !dir.exists() || dir.isRoot() )
    {
        GCF_UINST_LOG( QString("  Cannot remove (recursive) directory \"%1\"").arg(file) );
        return false;
    }

    this->deleteDirectory(dir);
    GCF_UINST_LOG( QString("  Removed (recursive) directory \"%1\"").arg(file) );
    return true;
}

bool FileSystemObject::Copy(const QString& source, const QString& dest)
{
    QString source2 = UpdateInstaller::instance().resolveFileName(source);
    QString dest2 = UpdateInstaller::instance().resolveFileName(dest);
    if( QFileInfo(source2) == QFileInfo(dest2) )
    {
        GCF_UINST_ERROR( QString("  Cannot copy %1 on to itself").arg(source) );
        return false;
    }

    // Ensure that the destination doesnt exist
    bool isDestPath = QFileInfo(dest2).isDir();
    bool success = isDestPath ? true : RemoveFile(dest2);
    if(!success)
    {
        GCF_UINST_ERROR( QString("  Pre-existing destination file \"%1\" could not be deleted").arg(dest2) );
        return false;
    }

    // Ensure that source exists and that it is a file (and not directory)
    if( !QFile::exists(source2) || !QFileInfo(source2).isFile() )
    {
        GCF_UINST_ERROR( QString("  Source file %1 doesnt exist, or is a directory").arg(source2) );
        return false;
    }

    // If the destination is a file name then good, otherwise append the source filename
    // to dest2 (which is basically a path)
    if( isDestPath )
        dest2 += QString("%1%2").arg( QDir::separator() ).arg( QFileInfo(source2).fileName() );

    // Now perform the copy
    success = QFile::copy( source2, dest2 );
    if(success)
        GCF_UINST_LOG( QString("  Copied %1 into %2").arg(source2).arg(dest2) );
    else
        GCF_UINST_ERROR( QString("  Could not copy %1 into %2").arg(source2).arg(dest2) );
    return success;
}

bool FileSystemObject::Move(const QString& source, const QString& dest)
{
    // This is basically copy+delete
    bool success = Copy(source, dest);
    if( !success )
        return success;

    success = RemoveFile(source);
    return success;
}

bool FileSystemObject::CopyDirectory(const QString& source, const QString& dest)
{
    // This function should recursively copy from source directory to destination directory
    QString source2 = UpdateInstaller::instance().resolveFileName(source);
    QString dest2 = UpdateInstaller::instance().resolveFileName(dest);

    // Make sure that both source and destination are directories
    if( !QFileInfo(source2).isDir() || !QFileInfo(dest2).isDir() )
        return false;

    bool success = copyDirectory(QDir(source2), QDir(dest2));
    return success;
}

bool FileSystemObject::MoveDirectory(const QString& source, const QString& dest)
{
    // This is basically copy+delete
    bool success = CopyDirectory(source, dest);
    if( !success )
        return success;

    success = RemoveDirectory(source);
    return success;
}

bool FileSystemObject::Rename(const QString& source, const QString& dest)
{
    QString source2 = UpdateInstaller::instance().resolveFileName(source);
    QString dest2 = QFileInfo(dest).fileName();

    bool success = false;
    QFileInfo fi(source2);
    if( !fi.exists() )
    {
        GCF_UINST_ERROR( QString("  Cannot rename non-existant file %1").arg(source2) );
        return false;
    }

    if( fi.isDir() )
    {
        QDir dir(source2);
        QString dName = dir.dirName();
        dir.cdUp();
        success = dir.rename(dName, dest2);
    }
    else
    {
        QDir dir = fi.dir();
        success = dir.rename(fi.fileName(), dest2);
    }

    if(success)
        GCF_UINST_LOG( QString("  Renamed %1 as %2").arg(source2).arg(dest2) );
    else
        GCF_UINST_ERROR( QString("  Could not rename %1 as %2").arg(source2).arg(dest2) );
    return success;
}

bool FileSystemObject::IsExists(const QString& file)
{
    QString file2 = UpdateInstaller::instance().resolveFileName(file);

    return QFile::exists(file2);
}

bool FileSystemObject::IsReadable(const QString& file)
{
    QString file2 = UpdateInstaller::instance().resolveFileName(file);

    return QFileInfo(file2).isReadable();
}

bool FileSystemObject::IsWritable(const QString& file)
{
    QString file2 = UpdateInstaller::instance().resolveFileName(file);

    return QFileInfo(file2).isWritable();
}

// Internal methods
void FileSystemObject::deleteDirectory(const QDir& d, bool deleteDir)
{
    QFileInfoList list = d.entryInfoList(QDir::Files|QDir::Dirs|QDir::NoDotAndDotDot);
    for(int i=0; i<list.count(); i++)
    {
        QFileInfo fi = list[i];
        if( fi.isFile() )
            QFile::remove( fi.absoluteFilePath() );
        else
        {
            QDir d2 = d;
            d2.cd( fi.fileName() );
            deleteDirectory( d2 );
        }
    }

    if(deleteDir)
    {
        QDir d2(d);
        d2.cdUp();
        d2.rmdir( d.dirName() );
    }
}

bool FileSystemObject::copyDirectory(const QDir& source, const QDir& dest)
{
    QFileInfoList list = source.entryInfoList(QDir::Files|QDir::Dirs|QDir::NoDotAndDotDot);
    for(int i=0; i<list.count(); i++)
    {
        QFileInfo fi = list[i];
        if( fi.isFile() )
        {
            bool success = QFile::copy( fi.absoluteFilePath(), dest.absoluteFilePath(fi.fileName()) );
            if(!success)
                return false;
        }
        else
        {
            QDir s2 = source;
            QDir d2 = dest;

            bool success = d2.mkdir( fi.fileName() );
            if(!success)
                return false;

            success = s2.cd( fi.fileName() );
            if(!success)
                return false;

            success = copyDirectory( s2, d2 );
            if(!success)
                return false;
        }
    }

    return true;
}

QObject* FileSystemObject::NewFile(const QString& file)
{
    FileObject* fileObj = new FileObject(file);
    return fileObj;
}

void FileSystemObject::releaseAllFileObjects()
{
    QList<FileObject*> fileObjects = findChildren<FileObject*>();
    qDeleteAll(fileObjects);
}


