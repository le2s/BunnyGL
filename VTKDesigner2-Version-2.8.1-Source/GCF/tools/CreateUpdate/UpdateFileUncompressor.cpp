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

#include "UpdateFileUncompressor.h"
#include "UpdateFileCompressorCommon.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QFSFileEngine>

struct UpdateFileUncompressorData
{
    QString ufFileName;
    QString destination;
};

UpdateFileUncompressor::UpdateFileUncompressor()
{
    d = new UpdateFileUncompressorData;
}

UpdateFileUncompressor::~UpdateFileUncompressor()
{
    delete d;
}

void UpdateFileUncompressor::setFileName(const QString& fileName)
{
    d->ufFileName = fileName;
}

QString UpdateFileUncompressor::fileName() const
{
    return d->ufFileName;
}

void UpdateFileUncompressor::setDestination(const QString& dest)
{
    d->destination = dest;
}

QString UpdateFileUncompressor::destination() const
{
    return d->destination;
}

QDir UpdateFileUncompressor::uncompress()
{
    QDir unpackedDirPath;

    // First open the uf file for reading
    QFile ufFile( d->ufFileName );
    if( !ufFile.open(QFile::ReadOnly) )
        return unpackedDirPath;

    QDataStream ufDS( &ufFile );

    // Now read the header.
    UpdateFileHeader header;
    ufDS >> header;

    // Some basic checks.
    if( header.Magic != UPDATE_FILE_HEADER_MAGIC )
        return unpackedDirPath;

    // Lets get to the destination directory
    QDir dir(d->destination);
    QFSFileEngine fileEngine;

    // Lets create the required directory structure
    for(int i=0; i<header.FileList.count(); i++)
    {
        QString fileName = header.FileList[i];
        // qDebug("ToUncompress %s", qPrintable(fileName));
        if( header.IsDirList[i] )
        {
            dir.mkpath( fileName );
            if( i == 0 )
                unpackedDirPath = QDir( QString("%1/%2").arg(d->destination).arg(fileName) );

            fileEngine.setFileName( QString("%1/%2").arg(d->destination).arg(fileName) );
            fileEngine.setPermissions( header.PermList[i] );
        }
    }

    // Lets now create files within these directories
    while( !ufDS.atEnd() )
    {
        UpdateFileEntry ufEntry;
        ufDS >> ufEntry;

        QString completeFileName = QString("%1/%2").arg(d->destination).arg(ufEntry.FileName);

        QByteArray ba = qUncompress( ufEntry.FileData );
        QFile ufeFile( completeFileName );
        ufeFile.open( QFile::WriteOnly );
        ufeFile.write(ba);
        ufeFile.close();

#ifdef Q_OS_LINUX
        if( ufEntry.FileName.endsWith(".so") )
            ufeFile.setPermissions(QFile::Permissions(65535));
#endif
        // ufeFile.setPermissions( QFile::Permissions(ufEntry.Permissions) );

        // qDebug("Uncompressed %s", qPrintable(completeFileName));
    }

    return unpackedDirPath;
}


