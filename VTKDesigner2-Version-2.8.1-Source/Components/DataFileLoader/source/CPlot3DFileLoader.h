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

#ifndef C_PLOT3D_FILE_LOADER_H
#define C_PLOT3D_FILE_LOADER_H

#include <QObject>
#include "IDataFileLoader.h"

struct CPlot3DFileLoaderData;
class CPlot3DFileLoader : public QObject, virtual public IDataFileLoader
{
    Q_OBJECT
    Q_INTERFACES(IDataFileLoader)

public:
    CPlot3DFileLoader(QObject* parent=0);
    ~CPlot3DFileLoader();

    // IContainer implementation
    QObject* containerObject();

    // IDataFileLoader implementation
    void setVisNetwork(IVisNetwork* visNetwork);
    void setVisNetworkCanvas(IVisNetworkCanvas* canvas);
    void setNodeFactoryRegistry(IVisSystemNodeFactoryRegistry* reg);
    QStringList handledExtensions();
    bool canLoad(QString fileName);
    bool load(QString fileName);

private:
    CPlot3DFileLoaderData* d;
};

#endif
