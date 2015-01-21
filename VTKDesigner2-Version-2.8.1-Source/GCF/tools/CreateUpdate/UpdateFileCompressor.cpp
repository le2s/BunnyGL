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

#include "UpdateFileCompressor.h"
#include "UpdateFileCompressorCommon.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>

struct UpdateFileCompressorData
{
    QString ufFileName;
    QString source;

    void updateUpdateFileHeader(QString stripFromPath, QDir dir, UpdateFileHeader& header);
    QString fileNameRelativeTo(QString fileName, QString relativeTo);
};

UpdateFileCompressor::UpdateFileCompressor()
{
    d = new UpdateFileCompressorData;
}

UpdateFileCompressor::~UpdateFileCompressor()
{
    delete d;
}

void UpdateFileCompressor::setFileName(const QString& fileName)
{
    d->ufFileName = fileName;
}

QString UpdateFileCompressor::fileName() const
{
    return d->ufFileName;
}

void UpdateFileCompressor::setSource(const QString& source)
{
    d->source = source;
}

QString UpdateFileCompressor::source() const
{
    return d->source;
}

void UpdateFileCompressor::compress()
{
    // First open the uf file for writing
    QFile ufFile( d->ufFileName );
    if( !ufFile.open(QFile::WriteOnly) )
        return;

    QDataStream ufDS( &ufFile );

    // Perform some basic checks.
    QFileInfo sourceInfo(d->source);
    if( !sourceInfo.isReadable() )
        return;

    QDir sourceDir( sourceInfo.absoluteFilePath() ); // = sourceInfo.dir();
    sourceDir.cdUp();
    QString sourcePath = sourceDir.absolutePath();

    // First create the ZIP header.
    UpdateFileHeader header;
    header.Magic = UPDATE_FILE_HEADER_MAGIC;
    header.FileList << d->fileNameRelativeTo(sourceInfo.absoluteFilePath(), sourcePath);
    header.PermList << (uint)(sourceInfo.permissions());
    header.IsDirList << sourceInfo.isDir();
    // qDebug("ToCompress: %s", qPrintable(header.FileList.first()));

    if(sourceInfo.isDir())
        d->updateUpdateFileHeader(sourcePath, QDir(d->source), header);

    // Insert the header into the UF file
    ufDS << header;

    // Now create ZIP entries and add them.
    for(int i=0; i<header.FileList.count(); i++)
    {
        if(header.IsDirList[i])
            continue;

        UpdateFileEntry ufEntry;
        ufEntry.FileName = header.FileList[i];

        QString completeFileName = QString("%1/%2").arg(sourcePath).arg(ufEntry.FileName);
        QFile zeFile( completeFileName );
        zeFile.open( QFile::ReadOnly );
        ufEntry.FileData = qCompress(zeFile.readAll());
        zeFile.close();
        ufEntry.Permissions = zeFile.permissions();

        // qDebug("Compressed %s as %s", qPrintable(completeFileName), qPrintable(ufEntry.FileName));

        ufDS << ufEntry;
    }

    // All done :)
    ufFile.close();
}

void UpdateFileCompressorData::updateUpdateFileHeader(QString stripFromPath, QDir dir, UpdateFileHeader& header)
{
    QFileInfoList children = dir.entryInfoList(QDir::AllEntries|QDir::NoDotAndDotDot|QDir::NoSymLinks);
    for(int i=0; i<children.count(); i++)
    {
        QFileInfo fi = children[i];

        // Skip files that are not readable.
        if( !fi.isReadable() )
            continue;

        // Format the file name relative to stripFromPath
        QString fileName = fileNameRelativeTo(fi.absoluteFilePath(), stripFromPath);
        header.FileList << fileName;
        header.PermList << (uint)(fi.permissions());
        header.IsDirList << fi.isDir();
        // qDebug("ToCompress: %s", qPrintable(fileName));

        if(fi.isDir())
        {
            QDir dir2 = dir;
            dir2.cd(fi.fileName());
            updateUpdateFileHeader(stripFromPath, dir2, header);
            dir.cdUp();
        }
    }
}

QString UpdateFileCompressorData::fileNameRelativeTo(QString fileName, QString relativeTo)
{
    QDir dir( relativeTo );
    QString ret = dir.relativeFilePath( fileName );
    return QDir::fromNativeSeparators(ret);
}



