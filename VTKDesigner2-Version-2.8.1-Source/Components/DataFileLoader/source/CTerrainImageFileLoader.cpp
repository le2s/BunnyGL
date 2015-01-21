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

#include "CTerrainImageFileLoader.h"
#include "IVisNetwork.h"
#include "IVisNetworkCanvas.h"
#include "IVisSystemNodeFactoryRegistry.h"
#include "IVisSystemNode.h"
#include "IVtkObjectWrapper.h"

#include <QFileInfo>
#include <QMessageBox>
#include <QGraphicsView>

#include "vtkDEMReader.h"
#include "vtkBMPReader.h"
#include "vtkJPEGReader.h"
#include "vtkPNGReader.h"
#include "vtkElevationFilter.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"


struct CTerrainImageFileLoaderData
{
    CTerrainImageFileLoaderData() : visNetwork(0), visNetworkCanvas(0), nodeFactoryReg(0) { }

    IVisNetwork* visNetwork;
    IVisNetworkCanvas* visNetworkCanvas;
    IVisSystemNodeFactoryRegistry* nodeFactoryReg;
};

CTerrainImageFileLoader::CTerrainImageFileLoader(QObject* parent)
:QObject(parent)
{
    d = new CTerrainImageFileLoaderData;
}

CTerrainImageFileLoader::~CTerrainImageFileLoader()
{
    delete d;
}

QObject* CTerrainImageFileLoader::containerObject()
{
    return this;
}

void CTerrainImageFileLoader::setVisNetwork(IVisNetwork* visNetwork)
{
    d->visNetwork = visNetwork;
}

void CTerrainImageFileLoader::setVisNetworkCanvas(IVisNetworkCanvas* canvas)
{
    d->visNetworkCanvas = canvas;
}

void CTerrainImageFileLoader::setNodeFactoryRegistry(IVisSystemNodeFactoryRegistry* reg)
{
    d->nodeFactoryReg = reg;
}

QStringList CTerrainImageFileLoader::handledExtensions()
{
    static QStringList ret = QStringList() << "dem" << "jpg" << "png" << "jpeg" << "bmp";
    return ret;
}

bool CTerrainImageFileLoader::canLoad(QString fileName)
{
    QFileInfo fi(fileName);

    QString ext = fi.suffix();
    if( ext == "dem" )
        return true;

    QMessageBox::StandardButton reply =
        QMessageBox::question( d->visNetworkCanvas->graphicsView(), "Image type confirmation",
                               QString("Should %1 be interpreted as a terrain file?").arg(fi.fileName()),
                               QMessageBox::Yes|QMessageBox::No, QMessageBox::Yes);

    return reply == QMessageBox::Yes;
}

bool CTerrainImageFileLoader::load(QString fileName)
{
    QFileInfo fi(fileName);
    QString ext = fi.suffix();

    vtkImageData* imageData = 0;
    IVisSystemNode* readerNode = 0;
    if(ext == "dem")
    {
        readerNode = d->nodeFactoryReg->createNode("vtkDEMReader");
        if(!readerNode)
            return false;

        IVtkObjectWrapper* readerWrapper = qobject_cast<IVtkObjectWrapper*>(readerNode->containerObject());
        vtkDEMReader* demReader = readerWrapper ? vtkDEMReader::SafeDownCast(readerWrapper->getVtkObject()) : 0;
        if(demReader)
        {
            demReader->SetFileName( qPrintable(fileName) );
            demReader->Update();
            imageData = demReader->GetOutput();
        }
    }
    else if(ext == "bmp")
    {
        readerNode = d->nodeFactoryReg->createNode("vtkBMPReader");
        if(!readerNode)
            return false;

        IVtkObjectWrapper* readerWrapper = qobject_cast<IVtkObjectWrapper*>(readerNode->containerObject());
        vtkBMPReader* bmpReader = readerWrapper ? vtkBMPReader::SafeDownCast(readerWrapper->getVtkObject()) : 0;
        if(bmpReader)
        {
            bmpReader->SetFileName( qPrintable(fileName) );
            bmpReader->Update();
            imageData = bmpReader->GetOutput();
        }
    }
    else if(ext == "jpg" || ext == "jpeg")
    {
        readerNode = d->nodeFactoryReg->createNode("vtkJPEGReader");
        if(!readerNode)
            return false;

        IVtkObjectWrapper* readerWrapper = qobject_cast<IVtkObjectWrapper*>(readerNode->containerObject());
        vtkJPEGReader* jpgReader = readerWrapper ? vtkJPEGReader::SafeDownCast(readerWrapper->getVtkObject()) : 0;
        if(jpgReader)
        {
            jpgReader->SetFileName( qPrintable(fileName) );
            jpgReader->Update();
            imageData = jpgReader->GetOutput();
        }
    }
    else if(ext == "png")
    {
        readerNode = d->nodeFactoryReg->createNode("vtkPNGReader");
        if(!readerNode)
            return false;

        IVtkObjectWrapper* readerWrapper = qobject_cast<IVtkObjectWrapper*>(readerNode->containerObject());
        vtkPNGReader* pngReader = readerWrapper ? vtkPNGReader::SafeDownCast(readerWrapper->getVtkObject()) : 0;
        if(pngReader)
        {
            pngReader->SetFileName( qPrintable(fileName) );
            pngReader->Update();
            imageData = pngReader->GetOutput();
        }
    }

    if(!readerNode)
        return false;
    d->visNetwork->addNode(readerNode);

    // Create the rest of the pipeline
    IVisSystemNode* textureNode = d->nodeFactoryReg->createNode("vtkTexture");
    if( !textureNode )
        return false;
    d->visNetwork->addNode(textureNode);

    IVisSystemNode* imageToTerrainNode = d->nodeFactoryReg->createNode("vtkImageToTerrain");
    if( !imageToTerrainNode )
        return false;
    else
    {
        QObject* i2tObj = imageToTerrainNode->containerObject();
        i2tObj->setProperty("GridCountX", 200);
        i2tObj->setProperty("GridCountY", 200);
    }
    d->visNetwork->addNode(imageToTerrainNode);

    IVisSystemNode* elevFilterNode = d->nodeFactoryReg->createNode("vtkElevationFilter");
    if(!elevFilterNode)
        return false;
    d->visNetwork->addNode(elevFilterNode);

    IVisSystemNode* normalsNode = d->nodeFactoryReg->createNode("vtkPolyDataNormals");
    if( !normalsNode )
        return false;
    else
        normalsNode->containerObject()->setProperty("FeatureAngle", 120);
    d->visNetwork->addNode(normalsNode);

    IVisSystemNode* lutNode = d->nodeFactoryReg->createNode("vtkLookupTable");
    if(!lutNode)
        return false;
    d->visNetwork->addNode(lutNode);

    // Now for mapper, actor, renderer, render window.
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

    d->visNetwork->connectNode(readerNode, imageToTerrainNode);
    d->visNetwork->connectNode(readerNode, textureNode);
    d->visNetwork->connectNode(imageToTerrainNode, elevFilterNode);
    d->visNetwork->connectNode(elevFilterNode, normalsNode);
    d->visNetwork->connectNode(normalsNode, mapperNode);
    d->visNetwork->connectNode(lutNode, mapperNode);
    d->visNetwork->connectNode(mapperNode, actorNode);
    d->visNetwork->connectNode(actorNode, rendererNode);
    d->visNetwork->connectNode(rendererNode, rWinNode);

    // Now place place the nodes appropriately.
    d->visNetworkCanvas->layoutNodes();

    // Forcefully update the pipeline
    QString result;
    readerNode->executeCommand("ForceUpdate", result);
    imageToTerrainNode->executeCommand("ForceUpdate", result);

    // Now ensure that elevation's low and high point are set appropriately
    IVtkObjectWrapper* wrapper2 = qobject_cast<IVtkObjectWrapper*>(elevFilterNode->containerObject());
    vtkElevationFilter* elevFilter = wrapper2 ? dynamic_cast<vtkElevationFilter*>(wrapper2->getVtkObject()) : 0;
    if(elevFilter)
    {
        double minHeight, maxHeight;
        QMetaObject::invokeMethod(imageToTerrainNode->containerObject(),
            "minHeight", Qt::DirectConnection, Q_RETURN_ARG(double,minHeight));
        QMetaObject::invokeMethod(imageToTerrainNode->containerObject(),
            "maxHeight", Qt::DirectConnection, Q_RETURN_ARG(double,maxHeight));

        elevFilter->SetLowPoint(0, 0, minHeight);
        elevFilter->SetHighPoint(0, 0, maxHeight);
    }

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


