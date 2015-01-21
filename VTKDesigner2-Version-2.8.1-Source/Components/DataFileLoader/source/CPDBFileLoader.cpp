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

#include "CPDBFileLoader.h"
#include "IVisNetwork.h"
#include "IVisNetworkCanvas.h"
#include "IVisSystemNodeFactoryRegistry.h"
#include "IVisSystemNode.h"
#include "IVtkObjectWrapper.h"

#include <QFileInfo>

#include "vtkPDBReader.h"
#include "vtkGlyph3D.h"
#include "vtkTubeFilter.h"
#include "vtkSphereSource.h"
#include "vtkDataSetMapper.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"

struct CPDBFileLoaderData
{
    CPDBFileLoaderData() : visNetwork(0), visNetworkCanvas(0), nodeFactoryReg(0) { }

    IVisNetwork* visNetwork;
    IVisNetworkCanvas* visNetworkCanvas;
    IVisSystemNodeFactoryRegistry* nodeFactoryReg;
};

CPDBFileLoader::CPDBFileLoader(QObject* parent)
:QObject(parent)
{
    d = new CPDBFileLoaderData;
}

CPDBFileLoader::~CPDBFileLoader()
{
    delete d;
}

QObject* CPDBFileLoader::containerObject()
{
    return this;
}

void CPDBFileLoader::setVisNetwork(IVisNetwork* visNetwork)
{
    d->visNetwork = visNetwork;
}

void CPDBFileLoader::setVisNetworkCanvas(IVisNetworkCanvas* canvas)
{
    d->visNetworkCanvas = canvas;
}

void CPDBFileLoader::setNodeFactoryRegistry(IVisSystemNodeFactoryRegistry* reg)
{
    d->nodeFactoryReg = reg;
}

QStringList CPDBFileLoader::handledExtensions()
{
    static QStringList ret = QStringList() << "pdb";
    return ret;
}

bool CPDBFileLoader::canLoad(QString fileName)
{
    QFileInfo fi(fileName);
    return handledExtensions().contains( fi.suffix() );
}

bool CPDBFileLoader::load(QString fileName)
{
    QFileInfo fi(fileName);
    QString ext = fi.suffix();

    // Create Nodes.
    IVisSystemNode* readerNode = d->nodeFactoryReg->createNode("vtkPDBReader");
    if(!readerNode)
        return false;
    d->visNetwork->addNode(readerNode);

    IVisSystemNode* sphereNode = d->nodeFactoryReg->createNode("vtkSphereSource");
    if(!sphereNode)
        return false;
    d->visNetwork->addNode(sphereNode);

    IVisSystemNode* tubeFilterNode = d->nodeFactoryReg->createNode("vtkTubeFilter");
    if(!tubeFilterNode)
        return false;
    d->visNetwork->addNode(tubeFilterNode);

    IVisSystemNode* glyphNode = d->nodeFactoryReg->createNode("vtkGlyph3D");
    if(!glyphNode)
        return false;
    d->visNetwork->addNode(glyphNode);

    IVisSystemNode* mapperNode1 = d->nodeFactoryReg->createNode("vtkDataSetMapper");
    if(!mapperNode1)
        return false;
    d->visNetwork->addNode(mapperNode1);

    IVisSystemNode* mapperNode2 = d->nodeFactoryReg->createNode("vtkDataSetMapper");
    if(!mapperNode2)
        return false;
    d->visNetwork->addNode(mapperNode2);

    IVisSystemNode* actorNode1 = d->nodeFactoryReg->createNode("vtkActor");
    if(!actorNode1)
        return false;
    d->visNetwork->addNode(actorNode1);

    IVisSystemNode* actorNode2 = d->nodeFactoryReg->createNode("vtkActor");
    if(!actorNode2)
        return false;
    d->visNetwork->addNode(actorNode2);

    IVisSystemNode* rendererNode = d->nodeFactoryReg->createNode("vtkRenderer");
    if(!rendererNode)
        return false;
    d->visNetwork->addNode(rendererNode);

    IVisSystemNode* rWinNode = d->nodeFactoryReg->createNode("vtkRenderWindow");
    if(!rWinNode)
        return false;
    d->visNetwork->addNode(rWinNode);

    // Connect nodes.
    d->visNetwork->connectNode(readerNode, tubeFilterNode);
    d->visNetwork->connectNode(tubeFilterNode, mapperNode1);
    d->visNetwork->connectNode(mapperNode1, actorNode1);
    d->visNetwork->connectNode(actorNode1, rendererNode);
    d->visNetwork->connectNode(rendererNode, rWinNode);
    d->visNetwork->connectNode(readerNode, glyphNode);
    d->visNetwork->connectNode(sphereNode, glyphNode);
    d->visNetwork->connectNode(glyphNode, mapperNode2);
    d->visNetwork->connectNode(mapperNode2, actorNode2);
    d->visNetwork->connectNode(actorNode2, rendererNode);

    // Configure nodes.
    IVtkObjectWrapper* readerWrapper = qobject_cast<IVtkObjectWrapper*>(readerNode->containerObject());
    vtkPDBReader* reader = readerWrapper ? vtkPDBReader::SafeDownCast( readerWrapper->getVtkObject() ) : 0;
    double scalarRange[2];
    if(reader)
    {
        reader->SetFileName( qPrintable(fileName) );
        reader->Update();
        vtkPolyData* pd = reader->GetOutput();
        pd->GetScalarRange(scalarRange);
    }

    IVtkObjectWrapper* sphereWrapper = qobject_cast<IVtkObjectWrapper*>(sphereNode->containerObject());
    vtkSphereSource* sphere = sphereWrapper ? vtkSphereSource::SafeDownCast( sphereWrapper->getVtkObject() ) : 0;
    if(sphere)
    {
        sphere->SetRadius(0.01);
        sphere->SetThetaResolution(32);
        sphere->SetPhiResolution(32);
    }

    IVtkObjectWrapper* tubeFilterWrapper = qobject_cast<IVtkObjectWrapper*>(tubeFilterNode->containerObject());
    vtkTubeFilter* tubeFilter = tubeFilterWrapper ? vtkTubeFilter::SafeDownCast( tubeFilterWrapper->getVtkObject() ) : 0;
    if(tubeFilter)
    {
        tubeFilter->SetRadius(0.1);
        tubeFilter->SetNumberOfSides(10);
    }

    IVtkObjectWrapper* glyphWrapper = qobject_cast<IVtkObjectWrapper*>(glyphNode->containerObject());
    vtkGlyph3D* glyph = glyphWrapper ? vtkGlyph3D::SafeDownCast( glyphWrapper->getVtkObject() ) : 0;
    if(glyph)
    {
        glyph->SetScaleModeToScaleByScalar();
        glyph->SetRange(scalarRange);
        glyph->SetScaleFactor(0.1);
    }

    IVtkObjectWrapper* mapperWrapper = qobject_cast<IVtkObjectWrapper*>(mapperNode2->containerObject());
    vtkDataSetMapper* mapper = mapperWrapper ? vtkDataSetMapper::SafeDownCast( mapperWrapper->getVtkObject() ) : 0;
    if(mapper)
    {
        mapper->SetColorModeToMapScalars();
        mapper->SetScalarRange(scalarRange);
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


