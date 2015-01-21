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

#ifndef UPDATE_FILE_UNCOMPRESSOR_H
#define UPDATE_FILE_UNCOMPRESSOR_H

#include <QString>
#include <QDir>

struct UpdateFileUncompressorData;
class UpdateFileUncompressor
{
public:
    UpdateFileUncompressor();
    ~UpdateFileUncompressor();

    void setFileName(const QString& fileName);
    QString fileName() const;

    void setDestination(const QString& dest);
    QString destination() const;

    QDir uncompress();

private:
    UpdateFileUncompressorData* d;
};

#endif

