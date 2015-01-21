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

#ifndef I_DATA_FILE_LOADER_REGISTRY_H
#define I_DATA_FILE_LOADER_REGISTRY_H

#include <GCF/IContainer>

class IDataFileLoader;
class IDataFileLoaderRegistry : virtual public IContainer
{
public:
    virtual void registerDataFileLoader(IDataFileLoader* loader) = 0;
    virtual void unregisterDataFileLoader(IDataFileLoader* loader) = 0;
    virtual int dataFileLoaderCount() = 0;
    virtual IDataFileLoader* dataFileLoader(int index) = 0;
    virtual IDataFileLoader* dataFileLoader(QString fileName) = 0;
};

Q_DECLARE_INTERFACE(IDataFileLoaderRegistry, "com.vcreatelogic.IDataFileLoaderRegistry/1.0")

#endif

