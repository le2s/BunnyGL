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

#include "CImageFileLoader.h"
#include "IVisNetwork.h"
#include "IVisNetworkCanvas.h"
#include "IVisSystemNodeFactoryRegistry.h"
#include "IVisSystemNode.h"
#include "IVtkObjectWrapper.h"

#include <QFileInfo>

#include "vtkBMPReader.h"
#include "vtkJPEGReader.h"
#include "vtkPNGReader.h"
#include "vtkPNMReader.h"
#include "vtkSLCReader.h"
#include "vtkTIFFReader.h"
#include "vtkImageData.h"
#include "vtkContourFilter.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"

struct CImageFileLoaderData
{
    CImageFileLoaderData() : visNetwork(0), visNetworkCanvas(0), nodeFactoryReg(0) { }

    IVisNetwork* visNetwork;
    IVisNetworkCanvas* visNetworkCanvas;
    IVisSystemNodeFactoryRegistry* nodeFactoryReg;
};

CImageFileLoader::CImageFileLoader(QObject* parent)
:QObject(parent)
{
    d = new CImageFileLoaderData;
}

CImageFileLoader::~CImageFileLoader()
{
    delete d;
}

QObject* CImageFileLoader::containerObject()
{
    return this;
}

void CImageFileLoader::setVisNetwork(IVisNetwork* visNetwork)
{
    d->visNetwork = visNetwork;
}

void CImageFileLoader::setVisNetworkCanvas(IVisNetworkCanvas* canvas)
{
    d->visNetworkCanvas = canvas;
}

void CImageFileLoader::setNodeFactoryRegistry(IVisSystemNodeFactoryRegistry* reg)
{
    d->nodeFactoryReg = reg;
}

QStringList CImageFileLoader::handledExtensions()
{
    static QStringList ret = QStringList() << "bmp" << "jpg" << "png" << "jpeg" << "pnm" << "slc" << "tiff" << "tif";
    return ret;
}

bool CImageFileLoader::canLoad(QString fileName)
{
    QFileInfo fi(fileName);
    return handledExtensions().contains( fi.suffix() );
}

bool CImageFileLoader::load(QString fileName)
{
    QFileInfo fi(fileName);
    QString ext = fi.suffix();

    vtkImageData* imageData = 0;
    IVisSystemNode* readerNode = 0;
    if(ext == "bmp")
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
    else if(ext == "pnm")
    {
        readerNode = d->nodeFactoryReg->createNode("vtkPNMReader");
        if(!readerNode)
            return false;

        IVtkObjectWrapper* readerWrapper = qobject_cast<IVtkObjectWrapper*>(readerNode->containerObject());
        vtkPNMReader* pnmReader = readerWrapper ? vtkPNMReader::SafeDownCast(readerWrapper->getVtkObject()) : 0;
        if(pnmReader)
        {
            pnmReader->SetFileName( qPrintable(fileName) );
            pnmReader->Update();
            imageData = pnmReader->GetOutput();
        }
    }
    else if(ext == "slc")
    {
        readerNode = d->nodeFactoryReg->createNode("vtkSLCReader");
        if(!readerNode)
            return false;

        IVtkObjectWrapper* readerWrapper = qobject_cast<IVtkObjectWrapper*>(readerNode->containerObject());
        vtkSLCReader* slcReader = readerWrapper ? vtkSLCReader::SafeDownCast(readerWrapper->getVtkObject()) : 0;
        if(slcReader)
        {
            slcReader->SetFileName( qPrintable(fileName) );
            slcReader->Update();
            imageData = slcReader->GetOutput();
        }
    }
    else if(ext == "tiff" || ext == "tif")
    {
        readerNode = d->nodeFactoryReg->createNode("vtkTIFFReader");
        if(!readerNode)
            return false;

        IVtkObjectWrapper* readerWrapper = qobject_cast<IVtkObjectWrapper*>(readerNode->containerObject());
        vtkTIFFReader* tifReader = readerWrapper ? vtkTIFFReader::SafeDownCast(readerWrapper->getVtkObject()) : 0;
        if(tifReader)
        {
            tifReader->SetFileName( qPrintable(fileName) );
            tifReader->Update();
            imageData = tifReader->GetOutput();
        }
    }

    if(!readerNode)
        return false;
    d->visNetwork->addNode(readerNode);

    // Study the image output.
    int* dim = imageData ? imageData->GetDimensions() : 0;
    bool extractContour = dim ? dim[2] > 1 : false;

    IVisSystemNode* contourNode = 0, *normalsNode = 0;
    if(extractContour)
    {
        double scalarRange[2];
        imageData->GetScalarRange(scalarRange);

        contourNode = d->nodeFactoryReg->createNode("vtkContourFilter");
        if(contourNode)
        {
            IVtkObjectWrapper* contourWrapper = qobject_cast<IVtkObjectWrapper*>(contourNode->containerObject());
            vtkContourFilter* contourFilter = contourWrapper ? vtkContourFilter::SafeDownCast(contourWrapper->getVtkObject()) : 0;
            if(contourFilter)
                contourFilter->GenerateValues(2, scalarRange);
            d->visNetwork->addNode(contourNode);
        }

        normalsNode = d->nodeFactoryReg->createNode("vtkPolyDataNormals");
        if(normalsNode)
            d->visNetwork->addNode(normalsNode);
    }

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

    // Now connect the nodes.
    if(contourNode)
    {
        d->visNetwork->connectNode(readerNode, contourNode);
        if(normalsNode)
        {
            d->visNetwork->connectNode(contourNode, normalsNode);
            d->visNetwork->connectNode(normalsNode, mapperNode);
        }
        else
            d->visNetwork->connectNode(contourNode, mapperNode);
    }
    else
        d->visNetwork->connectNode(readerNode, mapperNode);
    d->visNetwork->connectNode(mapperNode, actorNode);
    d->visNetwork->connectNode(actorNode, rendererNode);
    d->visNetwork->connectNode(rendererNode, rWinNode);

    // Now place place the nodes appropriately.
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


