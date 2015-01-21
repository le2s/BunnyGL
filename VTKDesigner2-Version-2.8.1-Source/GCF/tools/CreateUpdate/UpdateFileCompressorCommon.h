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

#ifndef UPDATE_COMPRESSOR_COMMON_H
#define UPDATE_COMPRESSOR_COMMON_H

#include <QStringList>
#include <QDataStream>
#include <QByteArray>

#define UPDATE_FILE_HEADER_MAGIC "GCFVCLZ"

struct UpdateFileHeader
{
    QString Magic;
    QStringList FileList;
    QList<uint> PermList;
    QList<bool> IsDirList;

    friend QDataStream& operator << (QDataStream& stream, const UpdateFileHeader& hdr) {
        stream << hdr.Magic;
        stream << hdr.FileList;
        stream << hdr.PermList;
        stream << hdr.IsDirList;
        return stream;
    }

    friend QDataStream& operator >> (QDataStream& stream, UpdateFileHeader& hdr) {
        stream >> hdr.Magic;
        stream >> hdr.FileList;
        stream >> hdr.PermList;
        stream >> hdr.IsDirList;
        return stream;
    }
};

struct UpdateFileEntry
{
    QString FileName;
    uint Permissions;
    QByteArray FileData;

    friend QDataStream& operator << (QDataStream& stream, const UpdateFileEntry& entry) {
        stream << entry.FileName;
        stream << entry.FileData;
        return stream;
    }

    friend QDataStream& operator >> (QDataStream& stream, UpdateFileEntry& entry) {
        stream >> entry.FileName;
        stream >> entry.FileData;
        return stream;
    }
};

#endif
