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

#include "CPlot3DFileLoader.h"
#include "IVisNetwork.h"
#include "IVisNetworkCanvas.h"
#include "IVisSystemNodeFactoryRegistry.h"
#include "IVisSystemNode.h"
#include "IVtkObjectWrapper.h"

#include <QFileInfo>

#include "vtkPLOT3DReader.h"
#include "vtkGlyph3D.h"
#include "vtkTubeFilter.h"
#include "vtkSphereSource.h"
#include "vtkDataSetMapper.h"
#include "vtkRenderer.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"

struct CPlot3DFileLoaderData
{
    CPlot3DFileLoaderData() : visNetwork(0), visNetworkCanvas(0), nodeFactoryReg(0) { }

    IVisNetwork* visNetwork;
    IVisNetworkCanvas* visNetworkCanvas;
    IVisSystemNodeFactoryRegistry* nodeFactoryReg;
};

CPlot3DFileLoader::CPlot3DFileLoader(QObject* parent)
:QObject(parent)
{
    d = new CPlot3DFileLoaderData;
}

CPlot3DFileLoader::~CPlot3DFileLoader()
{
    delete d;
}

QObject* CPlot3DFileLoader::containerObject()
{
    return this;
}

void CPlot3DFileLoader::setVisNetwork(IVisNetwork* visNetwork)
{
    d->visNetwork = visNetwork;
}

void CPlot3DFileLoader::setVisNetworkCanvas(IVisNetworkCanvas* canvas)
{
    d->visNetworkCanvas = canvas;
}

void CPlot3DFileLoader::setNodeFactoryRegistry(IVisSystemNodeFactoryRegistry* reg)
{
    d->nodeFactoryReg = reg;
}

QStringList CPlot3DFileLoader::handledExtensions()
{
    static QStringList ret = QStringList() << "bin";
    return ret;
}

bool CPlot3DFileLoader::canLoad(QString fileName)
{
    QFileInfo fi(fileName);
    return handledExtensions().contains( fi.suffix() );
}

bool CPlot3DFileLoader::load(QString fileName)
{
    QFileInfo fi(fileName);
    QString ext = fi.suffix();

    // Create Nodes.
    IVisSystemNode* readerNode = d->nodeFactoryReg->createNode("vtkPLOT3DReader");
    if(!readerNode)
        return false;
    d->visNetwork->addNode(readerNode);

    IVisSystemNode* mapperNode = d->nodeFactoryReg->createNode("vtkDataSetMapper");
    if(!mapperNode)
        return false;
    d->visNetwork->addNode(mapperNode);

    IVisSystemNode* actorNode = d->nodeFactoryReg->createNode("vtkActor");
    if(!actorNode)
        return false;
    d->visNetwork->addNode(actorNode);

    IVisSystemNode* rendererNode = d->nodeFactoryReg->createNode("vtkRenderer");
    if(!rendererNode)
        return false;
    d->visNetwork->addNode(rendererNode);

    IVisSystemNode* rWinNode = d->nodeFactoryReg->createNode("vtkRenderWindow");
    if(!rWinNode)
        return false;
    d->visNetwork->addNode(rWinNode);

    // Connect nodes.
    d->visNetwork->connectNode(readerNode, mapperNode);
    d->visNetwork->connectNode(mapperNode, actorNode);
    d->visNetwork->connectNode(actorNode, rendererNode);
    d->visNetwork->connectNode(rendererNode, rWinNode);

    // Configure nodes.
    IVtkObjectWrapper* readerWrapper = qobject_cast<IVtkObjectWrapper*>(readerNode->containerObject());
    vtkPLOT3DReader* reader = readerWrapper ? vtkPLOT3DReader::SafeDownCast( readerWrapper->getVtkObject() ) : 0;
    double scalarRange[2];
    if(reader)
    {
        // Lets figure out if the directory has a q filename.
        QString path = fi.absolutePath();
        QString baseName = fi.baseName();
        QString xyzFileName, qFileName;

        if(baseName.endsWith("q", Qt::CaseInsensitive))
        {
            qFileName = fileName;
            baseName = baseName.replace(baseName.length()-1, 1, "xyz");
            xyzFileName = QString("%1/%2.bin").arg(path).arg(baseName);
        }
        else if(baseName.endsWith("xyz", Qt::CaseInsensitive))
        {
            xyzFileName = fileName;
            baseName = baseName.replace(baseName.length()-3, 3, "q");
            qFileName = QString("%1/%2.bin").arg(path).arg(baseName);
        }

        if(xyzFileName.isEmpty())
            xyzFileName = fileName;

        if(!qFileName.isEmpty())
        {
            // Verify that the file exists.
            fi = QFileInfo(qFileName);
            if(!fi.exists())
                qFileName.clear();
        }

        // Filename is same as XYZ File name.
        reader->SetFileName( qPrintable(xyzFileName) );
        reader->SetQFileName( qPrintable(qFileName) );
    }

    // Layout nodes
    d->visNetworkCanvas->layoutNodes();

    // Setup the view.
    IVtkObjectWrapper* rendererWrapper = qobject_cast<IVtkObjectWrapper*>(rendererNode->containerObject());
    vtkRenderer* renderer = rendererWrapper ? vtkRenderer::SafeDownCast( rendererWrapper->getVtkObject() ) : 0;
    if(renderer)
        renderer->ResetCamera();

    IVtkObjectWrapper* rWinWrapper = qobject_cast<IVtkObjectWrapper*>(rWinNode->containerObject());
    vtkRenderWindow* rWin = rWinWrapper ? vtkRenderWindow::SafeDownCast( rWinWrapper->getVtkObject() ) : 0;
    if(rWin)
        rWin->Render();

    return true;
}


