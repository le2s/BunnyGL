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

#include "CVTKDataFileLoader.h"
#include "IVisNetwork.h"
#include "IVisNetworkCanvas.h"
#include "IVisSystemNodeFactoryRegistry.h"
#include "IVisSystemNode.h"
#include "IVtkObjectWrapper.h"

#include "vtkDataSetReader.h"
#include "vtkDataSet.h"
#include "vtkDataSetMapper.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"

#include "vtkPolyDataReader.h"
#include "vtkStructuredPointsReader.h"
#include "vtkStructuredGridReader.h"
#include "vtkUnstructuredGridReader.h"
#include "vtkRectilinearGridReader.h"

#include "vtkPolyData.h"
#include "vtkStructuredPoints.h"
#include "vtkStructuredGrid.h"
#include "vtkUnstructuredGrid.h"
#include "vtkRectilinearGrid.h"
#include "vtkContourFilter.h"

struct CVtkDataFileLoaderData
{
    CVtkDataFileLoaderData() : visNetwork(0), visNetworkCanvas(0), nodeFactoryReg(0) { }

    IVisNetwork* visNetwork;
    IVisNetworkCanvas* visNetworkCanvas;
    IVisSystemNodeFactoryRegistry* nodeFactoryReg;
};

CVtkDataFileLoader::CVtkDataFileLoader(QObject* parent)
:QObject(parent)
{
    d = new CVtkDataFileLoaderData;
}

CVtkDataFileLoader::~CVtkDataFileLoader()
{
    delete d;
}

QObject* CVtkDataFileLoader::containerObject()
{
    return this;
}

void CVtkDataFileLoader::setVisNetwork(IVisNetwork* visNetwork)
{
    d->visNetwork = visNetwork;
}

void CVtkDataFileLoader::setVisNetworkCanvas(IVisNetworkCanvas* canvas)
{
    d->visNetworkCanvas = canvas;
}

void CVtkDataFileLoader::setNodeFactoryRegistry(IVisSystemNodeFactoryRegistry* reg)
{
    d->nodeFactoryReg = reg;
}

QStringList CVtkDataFileLoader::handledExtensions()
{
    static QStringList ret = QStringList() << "vtk";
    return ret;
}

bool CVtkDataFileLoader::canLoad(QString fileName)
{
    if(!d->visNetwork || !d->nodeFactoryReg || !d->visNetworkCanvas)
        return false;

    vtkDataSetReader* dsReader = vtkDataSetReader::New();
    dsReader->SetFileName( qPrintable(fileName) );
    bool success = dsReader->OpenVTKFile() != 0;
    dsReader->Delete();

    return success;
}

bool CVtkDataFileLoader::load(QString fileName)
{
    if(!d->visNetwork || !d->nodeFactoryReg || !d->visNetworkCanvas)
        return false;

    vtkDataSetReader* dsReader = vtkDataSetReader::New();
    dsReader->SetFileName( qPrintable(fileName) );
    dsReader->Update();
    vtkPolyData* polyData = dsReader->GetPolyDataOutput();
    vtkStructuredPoints* sPoints = dsReader->GetStructuredPointsOutput();
    vtkStructuredGrid* sGrid = dsReader->GetStructuredGridOutput();
    vtkUnstructuredGrid* usGrid = dsReader->GetUnstructuredGridOutput();
    vtkRectilinearGrid* rGrid = dsReader->GetRectilinearGridOutput();
    dsReader->Delete();

    QString readerName;
    if(polyData)
        readerName = "vtkPolyDataReader";
    else if(sPoints)
        readerName = "vtkStructuredPointsReader";
    else if(sGrid)
        readerName = "vtkStructuredGridReader";
    else if(usGrid)
        readerName = "vtkUnstructuredGridReader";
    else if(rGrid)
        readerName = "vtkRectilinearGridReader";

    if(readerName.isEmpty())
        return false;

    // Create the reader node first.
    IVisSystemNode* readerNode = d->nodeFactoryReg->createNode(readerName);
    if(!readerNode)
        return false;
    d->visNetwork->addNode(readerNode);

    IVisSystemNode* contourNode = 0;
    if(readerName == "vtkStructuredPointsReader")
    {
        contourNode = d->nodeFactoryReg->createNode("vtkContourFilter");
        d->visNetwork->addNode(contourNode);
    }

    IVisSystemNode* normalsNode = 0;
    if(polyData || contourNode)
        normalsNode = d->nodeFactoryReg->createNode("vtkPolyDataNormals");
    if(normalsNode)
        d->visNetwork->addNode(normalsNode);

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

    // Now configure the nodes.
    IVtkObjectWrapper* readerWrapper = qobject_cast<IVtkObjectWrapper*>(readerNode->containerObject());
    IVtkObjectWrapper* mapperWrapper = qobject_cast<IVtkObjectWrapper*>(mapperNode->containerObject());
    vtkDataReader* reader = vtkDataReader::SafeDownCast( readerWrapper->getVtkObject() );
    vtkDataSetMapper* dsMapper = vtkDataSetMapper::SafeDownCast( mapperWrapper->getVtkObject() );

    double scalarRange[2] = {0, 0};
    if(reader)
    {
        reader->SetFileName( qPrintable(fileName) );
        reader->Update();

        vtkDataObject* dataObject = reader->GetOutputDataObject(0);
        vtkDataSet* dataSet = vtkDataSet::SafeDownCast( dataObject );
        if(dataSet)
            dataSet->GetScalarRange(scalarRange);
    }

    if(dsMapper && (scalarRange[0] != scalarRange[1]))
    {
        dsMapper->SetColorModeToMapScalars();
        dsMapper->SetScalarRange(scalarRange);
    }

    // Now connect the nodes.
    IVisSystemNode* node = 0;
    if(contourNode)
    {
        d->visNetwork->connectNode(readerNode, contourNode);
        node = contourNode;
    }
    else
        node = readerNode;

    if(normalsNode)
    {
        d->visNetwork->connectNode(node, normalsNode);
        d->visNetwork->connectNode(normalsNode, mapperNode);
    }
    else
        d->visNetwork->connectNode(node, mapperNode);

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

