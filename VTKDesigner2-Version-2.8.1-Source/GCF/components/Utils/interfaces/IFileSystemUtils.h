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

#ifndef I_FILE_SYSTEM_UTILS_H
#define I_FILE_SYSTEM_UTILS_H

#include <GCF/IContainer>
#include <QString>

class IFileSystemUtils : virtual public IContainer
{
public:
    virtual QString findFileInPath(const QString& fileName) = 0;
    virtual QStringList findFilesInPath(const QString& fileName) = 0;
    virtual void deleteDirectory(const QString& dir, bool deleteDir=false) = 0;
};

Q_DECLARE_INTERFACE(IFileSystemUtils, "com.vcreatelogic.IFileSystemUtils/1.0")

#endif

