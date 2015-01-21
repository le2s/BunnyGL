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

#include "CVRMLFileLoader.h"
#include "IVisNetwork.h"
#include "IVisNetworkCanvas.h"
#include "IVisSystemNodeFactoryRegistry.h"
#include "IVisSystemNode.h"
#include "IVtkObjectWrapper.h"
#include "IVtkObjectWrapperModifier.h"

#include "vtkVRMLImporter.h"
#include "vtkPolyDataMapper.h"
#include "vtkActor.h"
#include "vtkCamera.h"
#include "vtkProperty.h"
#include "vtkPolyDataAlgorithm.h"
#include "vtkPolyData.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkActorCollection.h"

#include <QFileInfo>

struct CVRMLFileLoaderData
{
    CVRMLFileLoaderData() : visNetwork(0), visNetworkCanvas(0), nodeFactoryReg(0) { }

    IVisNetwork* visNetwork;
    IVisNetworkCanvas* visNetworkCanvas;
    IVisSystemNodeFactoryRegistry* nodeFactoryReg;

    IVisSystemNode* loadActor(vtkActor* actor);
    bool setVtkObjectInNode(IVisSystemNode* node, vtkObject* object);
};

CVRMLFileLoader::CVRMLFileLoader(QObject* parent)
:QObject(parent)
{
    d = new CVRMLFileLoaderData;
}

CVRMLFileLoader::~CVRMLFileLoader()
{
    delete d;
}

QObject* CVRMLFileLoader::containerObject()
{
    return this;
}

void CVRMLFileLoader::setVisNetwork(IVisNetwork* visNetwork)
{
    d->visNetwork = visNetwork;
}

void CVRMLFileLoader::setVisNetworkCanvas(IVisNetworkCanvas* canvas)
{
    d->visNetworkCanvas = canvas;
}

void CVRMLFileLoader::setNodeFactoryRegistry(IVisSystemNodeFactoryRegistry* reg)
{
    d->nodeFactoryReg = reg;
}

QStringList CVRMLFileLoader::handledExtensions()
{
    static QStringList ret = QStringList() << "vrml" << "wrl";
    return ret;
}

bool CVRMLFileLoader::canLoad(QString fileName)
{
    QFileInfo fi(fileName);
    return (fi.suffix() == "vrml") || (fi.suffix() == "wrl");
}

bool CVRMLFileLoader::load(QString fileName)
{
    // Use vtkVRMLImporter to read the file
    vtkVRMLImporter* importer = vtkVRMLImporter::New();
    importer->SetFileName( qPrintable(fileName) );
    importer->Read();

    // Begin to parse the generated pipeline and create them in VTK Designer

    // Create the render window first
    vtkRenderWindow* rWin = importer->GetRenderWindow();
    IVisSystemNode* rWinNode = rWin ? d->nodeFactoryReg->createNode("vtkRenderWindow") : 0;
    if( !rWinNode )
    {
        importer->Delete();
        return false;
    }
    else
        d->visNetwork->addNode(rWinNode);

    // Now create renderer
    vtkRenderer* renderer = importer->GetRenderer();
    IVisSystemNode* rendererNode = renderer ? d->nodeFactoryReg->createNode("vtkRenderer") : 0;
    if( !rendererNode || !d->setVtkObjectInNode(rendererNode, renderer) )
    {
        importer->Delete();
        return false;
    }
    else
        d->visNetwork->addNode(rendererNode);

    // Now create camera
    /*
    vtkCamera* camera = renderer->GetActiveCamera();
    IVisSystemNode* cameraNode = renderer ? d->nodeFactoryReg->createNode("vtkCamera") : 0;
    if( camera && !d->setVtkObjectInNode(cameraNode, camera) )
    {
        importer->Delete();
        return false;
    }
    else
        d->visNetwork->addNode(cameraNode);*/

    // Now ensure that the connections are alive
    renderer->SetRenderWindow(0);
    renderer->SetActiveCamera(0);
    // d->visNetwork->connectNode(cameraNode, rendererNode);
    d->visNetwork->connectNode(rendererNode, rWinNode);

    // The original render window is now no longer needed
    rWin->Delete();

    // Now create actors on the renderer
    vtkActorCollection* actors = renderer->GetActors();
    actors->InitTraversal();

    QMap<vtkActor*, IVisSystemNode*> actorNodeMap;
    while(1)
    {
        vtkActor* actor = actors->GetNextActor();
        if( !actor )
            break;
        IVisSystemNode* actorNode = d->loadActor(actor);

        actorNodeMap[actor] = actorNode;
        actor->Register(0);
    }

    // Now connect actors in the network
    QList<vtkActor*> actorList = actorNodeMap.keys();
    for(int i=0; i<actorList.count(); i++)
    {
        vtkActor* actor = actorList[i];
        IVisSystemNode* actorNode = actorNodeMap[actor];
        if( !actorNode )
            continue;

        renderer->RemoveActor( actor );
        d->visNetwork->connectNode(actorNode, rendererNode);
        actor->Delete();
    }

    d->visNetworkCanvas->layoutNodes();

    // Setup the view.
    rWin->Render();

    return true;
}

IVisSystemNode* CVRMLFileLoaderData::loadActor(vtkActor* actor)
{
    if( !actor )
        return 0;

    // First create a node for the actor itself
    IVisSystemNode* actorNode = nodeFactoryReg->createNode("vtkActor");
    if( !actorNode || !setVtkObjectInNode(actorNode, actor) )
        return 0;
    visNetwork->addNode(actorNode);

    // Create mapper node
    vtkMapper* mapper = actor->GetMapper();
    vtkPolyDataMapper* polyDataMapper = vtkPolyDataMapper::SafeDownCast(mapper);
    IVisSystemNode* polyDataMapperNode = nodeFactoryReg->createNode("vtkPolyDataMapper");
    if( !polyDataMapperNode || !setVtkObjectInNode(polyDataMapperNode, polyDataMapper) )
        return 0;
    visNetwork->addNode(polyDataMapperNode);

    // Create source node
    IVisSystemNode* sourceNode = nodeFactoryReg->createNode("vtkCustomPolyDataSource");
    if( !sourceNode )
        return 0;
    visNetwork->addNode(sourceNode);

    // The source node will need to use the the mapper's input.
    vtkPolyData* polyData = polyDataMapper->GetInput();
    IVtkObjectWrapper* vtkWrapper = qobject_cast<IVtkObjectWrapper*>(sourceNode->containerObject());
    vtkPolyDataAlgorithm* algorithm = vtkPolyDataAlgorithm::SafeDownCast(vtkWrapper->getVtkObject());
    algorithm->SetOutput(polyData);
    algorithm->Modified();
    sourceNode->containerObject()->setProperty("StoreData", true);

    // Now register connections
    actor->Register(0);
    polyDataMapper->Register(0);
    polyData->Register(0);
    actor->SetMapper(0);
    polyDataMapper->SetInput(0);
    visNetwork->connectNode(polyDataMapperNode, actorNode);
    visNetwork->connectNode(sourceNode, polyDataMapperNode);
    actor->Delete();
    polyDataMapper->Delete();
    polyData->Delete();

    return actorNode;
}

bool CVRMLFileLoaderData::setVtkObjectInNode(IVisSystemNode* node, vtkObject* object)
{
    if( !node || !object )
        return false;

    QObject* obj = node->containerObject();
    IVtkObjectWrapperModifier* modifier = qobject_cast<IVtkObjectWrapperModifier*>(obj);
    if(!modifier)
        return false;

    modifier->setVtkObject(object);
    return true;
}

