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

#include "CSTLFileLoader.h"

#include <QFileInfo>

#include "IVisNetwork.h"
#include "IVisNetworkCanvas.h"
#include "IVisSystemNodeFactoryRegistry.h"
#include "IVisSystemNode.h"
#include "IVtkObjectWrapper.h"

#include <QFileInfo>

#include "vtkRenderer.h"
#include "vtkRenderWindow.h"

struct CSTLFileLoaderData
{
    CSTLFileLoaderData() : visNetwork(0), visNetworkCanvas(0), nodeFactoryReg(0) { }

    IVisNetwork* visNetwork;
    IVisNetworkCanvas* visNetworkCanvas;
    IVisSystemNodeFactoryRegistry* nodeFactoryReg;
};

CSTLFileLoader::CSTLFileLoader(QObject* parent)
    : QObject(parent)
{
    d = new CSTLFileLoaderData;
}

CSTLFileLoader::~CSTLFileLoader()
{
    delete d;
}

QObject* CSTLFileLoader::containerObject()
{
    return this;
}

void CSTLFileLoader::setVisNetwork(IVisNetwork* visNetwork)
{
    d->visNetwork = visNetwork;
}

void CSTLFileLoader::setVisNetworkCanvas(IVisNetworkCanvas* canvas)
{
    d->visNetworkCanvas = canvas;
}

void CSTLFileLoader::setNodeFactoryRegistry(IVisSystemNodeFactoryRegistry* reg)
{
    d->nodeFactoryReg = reg;
}

QStringList CSTLFileLoader::handledExtensions()
{
    static QStringList ret = QStringList() << "stl";
    return ret;
}

bool CSTLFileLoader::canLoad(QString fileName)
{
    QFileInfo fi(fileName);
    return handledExtensions().contains( fi.suffix().toLower() );
}

bool CSTLFileLoader::load(QString fileName)
{
    QFileInfo fi(fileName);
    if(!fi.isReadable() || !fi.exists())
        return false;

    IVisSystemNode* visNode1 = d->nodeFactoryReg->createNode("vtkSTLReader");
    if(!visNode1)
        return false;
    visNode1->containerObject()->setProperty("FileName", fileName);
    d->visNetwork->addNode(visNode1);

    IVisSystemNode* visNode2 = d->nodeFactoryReg->createNode("vtkPolyDataNormals");
    if(!visNode2)
        return false;
    d->visNetwork->addNode(visNode2);

    IVisSystemNode* visNode3 = d->nodeFactoryReg->createNode("vtkPolyDataMapper");
    if(!visNode3)
        return false;
    d->visNetwork->addNode(visNode3);

    IVisSystemNode* visNode4 = d->nodeFactoryReg->createNode("vtkActor");
    if(!visNode4)
        return false;
    d->visNetwork->addNode(visNode4);

    IVisSystemNode* visNode5 = d->nodeFactoryReg->createNode("vtkRenderer");
    if(!visNode5)
        return false;
    d->visNetwork->addNode(visNode5);

    IVisSystemNode* visNode6 = d->nodeFactoryReg->createNode("vtkRenderWindow");
    if(!visNode6)
        return false;
    d->visNetwork->addNode(visNode6);

    d->visNetwork->connectNode(visNode1, visNode2);
    d->visNetwork->connectNode(visNode2, visNode3);
    d->visNetwork->connectNode(visNode3, visNode4);
    d->visNetwork->connectNode(visNode4, visNode5);
    d->visNetwork->connectNode(visNode5, visNode6);

    d->visNetworkCanvas->layoutNodes();

    // Setup the view.
    IVtkObjectWrapper* rendererWrapper = qobject_cast<IVtkObjectWrapper*>(visNode5->containerObject());
    vtkRenderer* renderer = rendererWrapper ? vtkRenderer::SafeDownCast( rendererWrapper->getVtkObject() ) : 0;
    if(renderer)
        renderer->ResetCamera();

    IVtkObjectWrapper* rWinWrapper = qobject_cast<IVtkObjectWrapper*>(visNode6->containerObject());
    vtkRenderWindow* rWin = rWinWrapper ? vtkRenderWindow::SafeDownCast( rWinWrapper->getVtkObject() ) : 0;
    if(rWin)
        rWin->Render();

    return true;
}
