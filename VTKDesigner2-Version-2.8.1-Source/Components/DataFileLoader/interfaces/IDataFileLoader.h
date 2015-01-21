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

#ifndef I_DATA_FILE_LOADER_H
#define I_DATA_FILE_LOADER_H

#include <GCF/IContainer>
#include <QStringList>

class IVisNetwork;
class IVisNetworkCanvas;
class IVisSystemNodeFactoryRegistry;
class IDataFileLoader : virtual public IContainer
{
public:
    virtual void setVisNetwork(IVisNetwork* visNetwork) = 0;
    virtual void setVisNetworkCanvas(IVisNetworkCanvas* canvas) = 0;
    virtual void setNodeFactoryRegistry(IVisSystemNodeFactoryRegistry* reg) = 0;

    virtual QStringList handledExtensions() = 0;
    virtual bool canLoad(QString fileName) = 0;
    virtual bool load(QString fileName) = 0;
};

Q_DECLARE_INTERFACE(IDataFileLoader, "com.vcreatelogic.IDataFileLoader/1.0")

#endif

