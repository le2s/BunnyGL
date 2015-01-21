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

#include "CDICOMFileLoader.h"
#include "IVisNetwork.h"
#include "IVisNetworkCanvas.h"
#include "IVisSystemNodeFactoryRegistry.h"
#include "IVisSystemNode.h"
#include "IVtkObjectWrapper.h"
#include "IScriptEngineManager.h"

#include <GCF/AbstractComponent>
#include <GCF/ComponentFactory>

#include <QFileInfo>
#include <QDialog>
#include <QGraphicsView>
#include <QTextStream>

#include "vtkDICOMImageReader.h"
#include "vtkContourFilter.h"
#include "vtkGlyph3D.h"
#include "vtkTubeFilter.h"
#include "vtkSphereSource.h"
#include "vtkDataSetMapper.h"
#include "vtkRenderer.h"
#include "ui_CDICOMFileLoadOptionsDialog.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkImageData.h"
#include "UserDefinedDataTypes.h"

enum LoadMethod
{
    LoadNone,
    LoadFile, // Just a single file
    LoadASC, // Axial, Sagittal, Coronal View
    LoadSurface, // Using vtkContourFilter
    LoadVolume, // Using Volume Rendering methods
};

enum View
{
    AxialView,
    SagittalView,
    CoronalView,
    PlanesView
};

static QStringList ViewNames = QStringList() << "axial" << "sagittal" << "coronal" << "planes";

struct CDICOMFileLoaderData
{
    CDICOMFileLoaderData() : visNetwork(0), 
        visNetworkCanvas(0), nodeFactoryReg(0),
        scriptEngine(0), scriptEditorComponent(0) { }

    IVisNetwork* visNetwork;
    IVisNetworkCanvas* visNetworkCanvas;
    IVisSystemNodeFactoryRegistry* nodeFactoryReg;
    IScriptEngineManager* scriptEngine;
    GCF::AbstractComponent* scriptEditorComponent;
    int loadType;
};

CDICOMFileLoader::CDICOMFileLoader(QObject* parent)
:QObject(parent)
{
    d = new CDICOMFileLoaderData;
    d->loadType = LoadNone;
}

CDICOMFileLoader::~CDICOMFileLoader()
{
    delete d;
}

QObject* CDICOMFileLoader::containerObject()
{
    return this;
}

void CDICOMFileLoader::setVisNetwork(IVisNetwork* visNetwork)
{
    d->visNetwork = visNetwork;
}

void CDICOMFileLoader::setVisNetworkCanvas(IVisNetworkCanvas* canvas)
{
    d->visNetworkCanvas = canvas;
}

void CDICOMFileLoader::setNodeFactoryRegistry(IVisSystemNodeFactoryRegistry* reg)
{
    d->nodeFactoryReg = reg;
}

QStringList CDICOMFileLoader::handledExtensions()
{
    static QStringList ret = QStringList() << "dcm";
    return ret;
}

bool CDICOMFileLoader::canLoad(QString fileName)
{
    QFileInfo fi(fileName);
    bool success = handledExtensions().contains( fi.suffix() );
    if(!success)
        return false;

    QDialog dialog( d->visNetworkCanvas ? d->visNetworkCanvas->graphicsView() : 0 );
    Ui::CDICOMFileLoadOptionsDialog ui;
    ui.setupUi(&dialog);
    dialog.exec();

    if(ui.rdLoadSelected->isChecked())
        d->loadType = LoadFile;
    else if(ui.rbASC->isChecked())
        d->loadType = LoadASC;
    else if(ui.rbSurface->isChecked())
        d->loadType = LoadSurface;
    else if(ui.rbVolume->isChecked())
        d->loadType = LoadVolume;
    else
        d->loadType = LoadNone;

    return d->loadType != LoadNone;
}

bool CDICOMFileLoader::load(QString fileName)
{
    if(d->loadType == LoadNone)
        return false;

    if(d->loadType == LoadFile)
        return loadFile(fileName);

    QFileInfo fi(fileName);
    QString dirName = fi.absolutePath();

    return loadDirectory(dirName, d->loadType);
}

bool CDICOMFileLoader::loadFile(const QString& fileName)
{
    // Create Nodes.
    IVisSystemNode* readerNode = d->nodeFactoryReg->createNode("vtkDICOMImageReader");
    if(!readerNode)
        return false;
    d->visNetwork->addNode(readerNode);

    IVisSystemNode* colorMapNode = d->nodeFactoryReg->createNode("vtkImageMapToWindowLevelColors");
    if(!colorMapNode)
        return false;
    d->visNetwork->addNode(colorMapNode);

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

    // Configure nodes.
    readerNode->containerObject()->setProperty("FileName", fileName);
    rendererNode->containerObject()->setProperty("BackgroundColor", Qt::black);

    // Connect nodes.
    d->visNetwork->connectNode(readerNode, colorMapNode);
    d->visNetwork->connectNode(colorMapNode, mapperNode);
    d->visNetwork->connectNode(mapperNode, actorNode);
    d->visNetwork->connectNode(actorNode, rendererNode);
    d->visNetwork->connectNode(rendererNode, rWinNode);

    // Layout nodes
    d->visNetworkCanvas->layoutNodes();

    // Setup the view.
    IVtkObjectWrapper* rWinWrapper = qobject_cast<IVtkObjectWrapper*>(rWinNode->containerObject());
    vtkRenderWindow* rWin = rWinWrapper ? vtkRenderWindow::SafeDownCast( rWinWrapper->getVtkObject() ) : 0;
    if(rWin)
        rWin->Render();

    IVtkObjectWrapper* rendererWrapper = qobject_cast<IVtkObjectWrapper*>(rendererNode->containerObject());
    vtkRenderer* renderer = rendererWrapper ? vtkRenderer::SafeDownCast( rendererWrapper->getVtkObject() ) : 0;
    if(renderer)
        renderer->ResetCamera();

    return true;
}

bool CDICOMFileLoader::loadDirectory(const QString& dirName, int loadMethod)
{
    switch(loadMethod)
    {
    case LoadASC:
        return loadDirectoryASC(dirName);
    case LoadSurface:
        return loadDirectorySurface(dirName);
    case LoadVolume:
        return loadDirectoryVolume(dirName);
    }

    return false;
}

bool CDICOMFileLoader::loadDirectoryASC(const QString& dirName)
{
    // Create the render window
    IVisSystemNode* renWinNode = d->nodeFactoryReg->createNode("vtkRenderWindow");
    if(!renWinNode)
        return false;
    renWinNode->setNodeName("renderWindow");
    d->visNetwork->addNode(renWinNode);

    // Create renderer(s) for each of the views
    IVisSystemNode* rendererNode[4];
    for(int i=0; i<4; i++)
    {
        rendererNode[i] = d->nodeFactoryReg->createNode("vtkRenderer");
        if(!rendererNode[i])
            return false;
        rendererNode[i]->containerObject()->setProperty("BackgroundColor", Qt::black);
        rendererNode[i]->setNodeName(ViewNames.at(i) + "Renderer");
        d->visNetwork->addNode(rendererNode[i]);

        d->visNetwork->connectNode(rendererNode[i], renWinNode);
    }

    // Create actors for the first three views
    IVisSystemNode* actorNodes[3];
    IVisSystemNode* mapperNodes[3];
    for(int i=0; i<3; i++)
    {
        actorNodes[i] = d->nodeFactoryReg->createNode("vtkActor");
        if(!actorNodes[i])
            return false;
        d->visNetwork->addNode(actorNodes[i]);
        actorNodes[i]->setNodeName(ViewNames.at(i) + "Actor");

        mapperNodes[i] = d->nodeFactoryReg->createNode("vtkDataSetMapper");
        if(!mapperNodes[i])
            return false;
        d->visNetwork->addNode(mapperNodes[i]);
        mapperNodes[i]->setNodeName(ViewNames.at(i) + "Mapper");

        d->visNetwork->connectNode(mapperNodes[i], actorNodes[i]);
        d->visNetwork->connectNode(actorNodes[i], rendererNode[i]);
    }

    // Have the DICOM reader load the image from the directory
    QString result;
    IVisSystemNode* readerNode = d->nodeFactoryReg->createNode("vtkDICOMImageReader");
    if(!readerNode)
        return false;
    d->visNetwork->addNode(readerNode);
    readerNode->setNodeName("dicomImageReader");
    readerNode->containerObject()->setProperty("DirectoryName", dirName);
    readerNode->executeCommand("Update", result);

    // Catch hold of the DICOM image and compute bounds and center
    IVtkObjectWrapper* dicomWrapper = qobject_cast<IVtkObjectWrapper*>(readerNode->containerObject());
    vtkDICOMImageReader* dicomReader = vtkDICOMImageReader::SafeDownCast(dicomWrapper->getVtkObject());
    vtkImageData* dicomImage = dicomReader->GetOutput();

    double bounds[6];
    dicomImage->GetBounds(bounds);

    double center[] = { (bounds[0]+bounds[1])/2,
                        (bounds[2]+bounds[3])/2,
                        (bounds[4]+bounds[5])/2 };

    // Lets now generate the script for execution later
    QString script;
    QTextStream scriptTS(&script);
    scriptTS << "// Bounds of the DICOM image = "
             << bounds[0] << ", " << bounds[1] << ", " << bounds[2] << ", "
             << bounds[3] << ", " << bounds[4] << ", " << bounds[5] << "\n";
    scriptTS << "// Center of the DICOM image = "
             << center[0] << ", " << center[1] << ", " << center[2] << "\n";

    // Create pipeline for slicing
    IVisSystemNode* sliceFilterNodes[3];
    for(int i=0; i<3; i++)
    {
        sliceFilterNodes[i] = d->nodeFactoryReg->createNode("vtkImageReslice");
        if(!sliceFilterNodes[i])
            return false;
        d->visNetwork->addNode(sliceFilterNodes[i]);

        sliceFilterNodes[i]->containerObject()->setProperty("OutputDimensionality", 2);
        sliceFilterNodes[i]->containerObject()->setProperty("Border", true);
        sliceFilterNodes[i]->setNodeName(ViewNames.at(i) + "SliceFilter");

        d->visNetwork->connectNode(readerNode, sliceFilterNodes[i]);
    }

    scriptTS << "\n// Configure the axialSliceFilter\n";
    scriptTS << "axialSliceFilter.resetTransform();\n";
    scriptTS << "axialSliceFilter.translate(" 
             << center[0] << ", " << center[1] << ", " << center[2] << ");\n";

    scriptTS << "\n// Configure the sagittalSliceFilter\n";
    scriptTS << "sagittalSliceFilter.resetTransform();\n";
    scriptTS << "sagittalSliceFilter.translate(" 
             << center[0] << ", " << center[1] << ", " << center[2] << ");\n";
    scriptTS << "sagittalSliceFilter.rotateY(90);\n";

    scriptTS << "\n// Configure the coronalSliceFilter\n";
    scriptTS << "coronalSliceFilter.resetTransform();\n";
    scriptTS << "coronalSliceFilter.translate(" 
             << center[0] << ", " << center[1] << ", " << center[2] << ");\n";
    scriptTS << "coronalSliceFilter.rotateX(90);\n";

    // Install color maps now
    IVisSystemNode* colorMapNodes[3];
    for(int i=0; i<3; i++)
    {
        colorMapNodes[i] = d->nodeFactoryReg->createNode("vtkImageMapToWindowLevelColors");
        if(!colorMapNodes[i])
            return false;

        colorMapNodes[i]->setNodeName(ViewNames.at(i) + "ColorMap");

        d->visNetwork->addNode(colorMapNodes[i]);
        d->visNetwork->connectNode(sliceFilterNodes[i], colorMapNodes[i]);
        d->visNetwork->connectNode(colorMapNodes[i], mapperNodes[i]);
    }

    // Install mappers to the planes now
    IVisSystemNode* planeMapperNodes[3];
    IVisSystemNode* planeActorNodes[3];
    for(int i=0; i<3; i++)
    {
        planeMapperNodes[i] = d->nodeFactoryReg->createNode("vtkDataSetMapper");
        if(!planeMapperNodes[i])
            return false;
        d->visNetwork->addNode(planeMapperNodes[i]);
        planeMapperNodes[i]->setNodeName(ViewNames.at(i) + "PlaneMapper");

        planeActorNodes[i] = d->nodeFactoryReg->createNode("vtkActor");
        if(!planeActorNodes[i])
            return false;
        d->visNetwork->addNode(planeActorNodes[i]);
        planeActorNodes[i]->setNodeName(ViewNames.at(i) + "PlaneActor");

        d->visNetwork->connectNode(colorMapNodes[i], planeMapperNodes[i]);
        d->visNetwork->connectNode(planeMapperNodes[i], planeActorNodes[i]);
        d->visNetwork->connectNode(planeActorNodes[i], rendererNode[PlanesView]);
    }

    // Update the script to move the actor to the same place
    scriptTS << "\n// Configure the axialPlaneActor\n";
    scriptTS << "axialPlaneActor.makeIdentity();\n";
    scriptTS << "axialPlaneActor.translate(" 
             << center[0] << ", " << center[1] << ", " << center[2] << ");\n";

    scriptTS << "\n// Configure the sagittalPlaneActor\n";
    scriptTS << "sagittalPlaneActor.makeIdentity();\n";
    scriptTS << "sagittalPlaneActor.translate(" 
             << center[0] << ", " << center[1] << ", " << center[2] << ");\n";
    scriptTS << "sagittalPlaneActor.rotateY(90);\n";

    scriptTS << "\n// Configure the coronalPlaneActor\n";
    scriptTS << "coronalPlaneActor.makeIdentity();\n";
    scriptTS << "coronalPlaneActor.translate(" 
             << center[0] << ", " << center[1] << ", " << center[2] << ");\n";
    scriptTS << "coronalPlaneActor.rotateX(90);\n";

    // Have an outline drawn around the 3D image in the planes view
    IVisSystemNode* outlineFilter = d->nodeFactoryReg->createNode("vtkOutlineFilter");
    if(!outlineFilter)
        return false;
    d->visNetwork->addNode(outlineFilter);
    d->visNetwork->connectNode(readerNode, outlineFilter);
    outlineFilter->setNodeName("outlineFilter");

    IVisSystemNode* outlineMapper = d->nodeFactoryReg->createNode("vtkDataSetMapper");
    if(!outlineMapper)
        return false;
    d->visNetwork->addNode(outlineMapper);
    d->visNetwork->connectNode(outlineFilter, outlineMapper);
    outlineMapper->setNodeName("outlineMapper");

    IVisSystemNode* outlineActor = d->nodeFactoryReg->createNode("vtkActor");
    if(!outlineActor)
        return false;
    d->visNetwork->addNode(outlineActor);
    d->visNetwork->connectNode(outlineMapper, outlineActor);
    outlineActor->setNodeName("outlineActor");

    d->visNetwork->connectNode(outlineActor, rendererNode[PlanesView]);

    // Now configure the custom form that users can use to manipulate the 
    // location of planes along X, Y and Z.
    int* dim = dicomImage->GetDimensions();
    double* dicomPixelSpacing = dicomReader->GetPixelSpacing();
    int nrAxialPlanes = int((bounds[5]-bounds[4])/dicomPixelSpacing[2]);
    int nrSagittalPlanes = int((bounds[1]-bounds[0])/dicomPixelSpacing[0]);
    int nrCoronalPlanes = int((bounds[3]-bounds[2])/dicomPixelSpacing[1]);

    scriptTS << "\n\n";
    scriptTS << "// Axial Plane (front to back)\n";
    scriptTS << "//     Length  = " << bounds[5]-bounds[4] << "\n";
    scriptTS << "//     Spacing = " << dicomPixelSpacing[2] << "\n";
    scriptTS << "//     Slices  = " << nrAxialPlanes << "\n";
    scriptTS << "// Sagittal Plane (left to right)\n";
    scriptTS << "//     Length  = " << bounds[1]-bounds[0] << "\n";
    scriptTS << "//     Spacing = " << dicomPixelSpacing[0] << "\n";
    scriptTS << "//     Slices  = " << nrSagittalPlanes << "\n";
    scriptTS << "// Coronal Plane (top to bottom)\n";
    scriptTS << "//     Length  = " << bounds[3]-bounds[2] << "\n";
    scriptTS << "//     Spacing = " << dicomPixelSpacing[1] << "\n";
    scriptTS << "//     Slices  = " << nrCoronalPlanes << "\n";

    // Load the custom form
    QWidget* formWidget = 0;
    if(!d->scriptEditorComponent)
        d->scriptEditorComponent = GCF::ComponentFactory::instance().findComponent("ScriptEditorComponent");
    if(d->scriptEditorComponent)
        QMetaObject::invokeMethod(d->scriptEditorComponent, "loadForm",
                                  Q_RETURN_ARG(QWidget*,formWidget),
                                  Q_ARG(QString,":/Forms/DICOMASCConfigPage.ui"),
                                  Q_ARG(QString,"MainWindowComponent.rightDockArea"));

    if(formWidget)
    {
        scriptTS << "\n// Show DICOM image properties in the config page\n";
        scriptTS << "DICOMASCConfigPage.gbPatientData.txtPatientName.text = \"" 
                 << dicomReader->GetPatientName() << "\"\n";
        scriptTS << "DICOMASCConfigPage.gbPatientData.txtPatientID.text = \"" 
                 << dicomReader->GetStudyID() << "\"\n";
        scriptTS << "DICOMASCConfigPage.gbPatientData.txtPatiendUID.text = \"" 
                 << dicomReader->GetStudyUID() << "\"\n";

        scriptTS << "DICOMASCConfigPage.gbImageData.txtDimensions.text = \""
                 << dim[0] << " x " << dim[1] << " x " << dim[1] << "\"\n";
        scriptTS << "DICOMASCConfigPage.gbImageData.txtAxialSliceCount.text = \""
                 << nrAxialPlanes << "\"\n";
        scriptTS << "DICOMASCConfigPage.gbImageData.txtSagittalSliceCount.text = \""
                 << nrSagittalPlanes << "\"\n";
        scriptTS << "DICOMASCConfigPage.gbImageData.txtCoronalSliceCount.text = \""
                 << nrCoronalPlanes << "\"\n";

        scriptTS << "DICOMASCConfigPage.gbPlaneControls.axialSlider.maximum = "
                 << nrAxialPlanes << "\n";
        scriptTS << "DICOMASCConfigPage.gbPlaneControls.sagittalSlider.maximum = "
                 << nrSagittalPlanes << "\n";
        scriptTS << "DICOMASCConfigPage.gbPlaneControls.coronalSlider.maximum = "
                 << nrCoronalPlanes << "\n";

        scriptTS << "DICOMASCConfigPage.gbPlaneControls.sbAxial.maximum = "
                 << nrAxialPlanes << "\n";
        scriptTS << "DICOMASCConfigPage.gbPlaneControls.sbSagittal.maximum = "
                 << nrSagittalPlanes << "\n";
        scriptTS << "DICOMASCConfigPage.gbPlaneControls.sbCoronal.maximum = "
                 << nrCoronalPlanes << "\n";

        scriptTS << "DICOMASCConfigPage.gbPlaneControls.sbAxial.value = " << nrAxialPlanes/2 << "\n";
        scriptTS << "DICOMASCConfigPage.gbPlaneControls.sbSagittal.value = " << nrSagittalPlanes/2 << "\n";
        scriptTS << "DICOMASCConfigPage.gbPlaneControls.sbCoronal.value = " << nrCoronalPlanes/2 << "\n";

        scriptTS << "\n" << 
            "// Event handler for ValueChanged event of DICOMASCConfigPage.gbPlaneControls.axialSlider\n" <<  
            "function DICOMASCConfigPage_gbPlaneControls_axialSlider_ValueChanged()\n" <<  
            "{\n" <<  
            "    var value = DICOMASCConfigPage.gbPlaneControls.axialSlider.value;\n" <<  
            "    axialPlaneActor.makeIdentity();\n" <<  
            "    axialPlaneActor.translate(" <<  center[0] << ", " <<  center[1] << ", " <<  dicomPixelSpacing[2] << "*value);\n" <<  
            "\n" <<  
            "    renderWindow.render();\n" <<  
            "}\n" <<  
            "DICOMASCConfigPage.gbPlaneControls.axialSlider.valueChanged.connect(DICOMASCConfigPage_gbPlaneControls_axialSlider_ValueChanged);\n\n";

        scriptTS << 
            "// Event handler for ValueChanged event of DICOMASCConfigPage.gbPlaneControls.sagittalSlider\n" <<  
            "function DICOMASCConfigPage_gbPlaneControls_sagittalSlider_ValueChanged()\n" <<  
            "{\n" <<  
            "    var value = DICOMASCConfigPage.gbPlaneControls.sagittalSlider.value;\n" <<  
            "    sagittalPlaneActor.makeIdentity();\n" <<  
            "    sagittalPlaneActor.translate(" <<  dicomPixelSpacing[0] << "*value, " <<  center[1] << ", " <<  center[2] << ");\n" <<  
            "    sagittalPlaneActor.rotateY(90);\n" <<  
            "\n" <<  
            "    renderWindow.render();\n" <<  
            "}\n" <<  
            "DICOMASCConfigPage.gbPlaneControls.sagittalSlider.valueChanged.connect(DICOMASCConfigPage_gbPlaneControls_sagittalSlider_ValueChanged);\n\n";

        scriptTS << 
            "// Event handler for ValueChanged event of DICOMASCConfigPage.gbPlaneControls.coronalSlider\n" << 
            "function DICOMASCConfigPage_gbPlaneControls_coronalSlider_ValueChanged()\n" << 
            "{\n" << 
            "    var value = DICOMASCConfigPage.gbPlaneControls.coronalSlider.value;\n" << 
            "    coronalPlaneActor.makeIdentity();\n" << 
            "    coronalPlaneActor.translate(" << center[0] << ", " << dicomPixelSpacing[1] << "*value, " << center[2] << ");\n" << 
            "    coronalPlaneActor.rotateX(90);\n" << 
            "\n" << 
            "    renderWindow.render();\n" << 
            "}\n" << 
            "DICOMASCConfigPage.gbPlaneControls.coronalSlider.valueChanged.connect(DICOMASCConfigPage_gbPlaneControls_coronalSlider_ValueChanged);\n\n";

        scriptTS << 
            "// Event handler for Clicked event of DICOMASCConfigPage.gbPlaneControls.cmdRefresh\n" << 
            "function DICOMASCConfigPage_gbPlaneControls_cmdRefresh_Clicked()\n" << 
            "{\n" << 
            "    var axialValue = DICOMASCConfigPage.gbPlaneControls.axialSlider.value;\n" << 
            "    var sagittalValue = DICOMASCConfigPage.gbPlaneControls.sagittalSlider.value;\n" << 
            "    var coronalValue = DICOMASCConfigPage.gbPlaneControls.coronalSlider.value;\n" << 
            "\n" << 
            "    // Configure the axialSliceFilter\n" << 
            "    axialSliceFilter.resetTransform();\n" << 
            "    axialSliceFilter.translate(" <<  center[0] << ", " <<  center[1] << ", " <<  dicomPixelSpacing[2] << "*axialValue);\n" <<  
            "\n" << 
            "    // Configure the sagittalSliceFilter\n" << 
            "    sagittalSliceFilter.resetTransform();\n" << 
            "    sagittalSliceFilter.translate(" <<  dicomPixelSpacing[0] << "*sagittalValue, " <<  center[1] << ", " <<  center[2] << ");\n" <<  
            "    sagittalSliceFilter.rotateY(90);\n" << 
            "\n" << 
            "    // Configure the coronalSliceFilter\n" << 
            "    coronalSliceFilter.resetTransform();\n" << 
            "    coronalSliceFilter.translate(" << center[0] << ", " << dicomPixelSpacing[1] << "*coronalValue, " << center[2] << ");\n" << 
            "    coronalSliceFilter.rotateX(90);\n" << 
            "\n" << 
            "    renderWindow.render();\n" << 
            "}\n" << 
            "DICOMASCConfigPage.gbPlaneControls.cmdRefresh.clicked.connect(DICOMASCConfigPage_gbPlaneControls_cmdRefresh_Clicked);\n\n";
    }

    // Execute the script and initialize the planes
    if(!d->scriptEngine)
        d->scriptEngine = GCF::findObject<IScriptEngineManager*>();
    if(d->scriptEngine)
    {
        d->scriptEngine->setScript(script);
        d->scriptEngine->executeScript();
    }

    // Have all the renderers reset their cameras
    for(int i=0; i<4; i++)
        rendererNode[i]->executeCommand("ResetCamera", result);

    // Layout all the nodes
    d->visNetworkCanvas->layoutNodes();

    return true;
}

bool CDICOMFileLoader::loadDirectorySurface(const QString& dirName)
{
    // Create Nodes.
    IVisSystemNode* readerNode = d->nodeFactoryReg->createNode("vtkDICOMImageReader");
    if(!readerNode)
        return false;
    d->visNetwork->addNode(readerNode);
    readerNode->setNodeName("dicomReader");

    IVisSystemNode* boneContourNode = d->nodeFactoryReg->createNode("vtkContourFilter");
    if(!boneContourNode)
        return false;
    d->visNetwork->addNode(boneContourNode);
    boneContourNode->setNodeName("boneExtractor");

    IVisSystemNode* boneMapperNode = d->nodeFactoryReg->createNode("vtkDataSetMapper");
    if(!boneMapperNode)
        return false;
    d->visNetwork->addNode(boneMapperNode);
    boneMapperNode->setNodeName("boneMapper");

    IVisSystemNode* boneActorNode = d->nodeFactoryReg->createNode("vtkActor");
    if(!boneActorNode)
        return false;
    d->visNetwork->addNode(boneActorNode);
    boneActorNode->setNodeName("boneActor");

    IVisSystemNode* skinContourNode = d->nodeFactoryReg->createNode("vtkContourFilter");
    if(!skinContourNode)
        return false;
    d->visNetwork->addNode(skinContourNode);
    skinContourNode->setNodeName("skinExtractor");

    IVisSystemNode* skinMapperNode = d->nodeFactoryReg->createNode("vtkDataSetMapper");
    if(!skinMapperNode)
        return false;
    d->visNetwork->addNode(skinMapperNode);
    skinMapperNode->setNodeName("skinMapper");

    IVisSystemNode* skinActorNode = d->nodeFactoryReg->createNode("vtkActor");
    if(!skinActorNode)
        return false;
    d->visNetwork->addNode(skinActorNode);
    skinActorNode->setNodeName("skinActor");

    IVisSystemNode* rendererNode = d->nodeFactoryReg->createNode("vtkRenderer");
    if(!rendererNode)
        return false;
    d->visNetwork->addNode(rendererNode);
    rendererNode->setNodeName("renderer");

    IVisSystemNode* rWinNode = d->nodeFactoryReg->createNode("vtkRenderWindow");
    if(!rWinNode)
        return false;
    d->visNetwork->addNode(rWinNode);
    rWinNode->setNodeName("renderWindow");

    // Configure nodes.
    QString result;
    readerNode->containerObject()->setProperty("DirectoryName", dirName);
    readerNode->executeCommand("Update", result);
    boneContourNode->containerObject()->setProperty("ComputeScalars", false);
    boneMapperNode->containerObject()->setProperty("ShowScalars", false);
    skinContourNode->containerObject()->setProperty("ComputeScalars", false);
    skinMapperNode->containerObject()->setProperty("ShowScalars", false);
    rendererNode->containerObject()->setProperty("BackgroundColor", Qt::black);
    boneActorNode->containerObject()->setProperty("Color", Qt::white);
    skinActorNode->containerObject()->setProperty("Color", Qt::darkYellow);

    // Connect nodes.
    d->visNetwork->connectNode(readerNode, boneContourNode);
    d->visNetwork->connectNode(boneContourNode, boneMapperNode);
    d->visNetwork->connectNode(boneMapperNode, boneActorNode);
    d->visNetwork->connectNode(boneActorNode, rendererNode);
    d->visNetwork->connectNode(readerNode, skinContourNode);
    d->visNetwork->connectNode(skinContourNode, skinMapperNode);
    d->visNetwork->connectNode(skinMapperNode, skinActorNode);
    d->visNetwork->connectNode(skinActorNode, rendererNode);
    d->visNetwork->connectNode(rendererNode, rWinNode);

    // Configure the contour nodes after connection is established, because
    // during connection contour nodes modify their contour values.
    boneContourNode->containerObject()->setProperty("NumberOfContours", 1);
    boneContourNode->containerObject()->setProperty("ValueRange", qVariantFromValue<ValueRange>(ValueRange(500,500)));

    skinContourNode->containerObject()->setProperty("NumberOfContours", 1);
    skinContourNode->containerObject()->setProperty("ValueRange", qVariantFromValue<ValueRange>(ValueRange(1150,1150)));

    // Layout nodes
    d->visNetworkCanvas->layoutNodes();

    // Setup the view.
    IVtkObjectWrapper* rWinWrapper = qobject_cast<IVtkObjectWrapper*>(rWinNode->containerObject());
    vtkRenderWindow* rWin = rWinWrapper ? vtkRenderWindow::SafeDownCast( rWinWrapper->getVtkObject() ) : 0;
    if(rWin)
        rWin->Render();

    IVtkObjectWrapper* rendererWrapper = qobject_cast<IVtkObjectWrapper*>(rendererNode->containerObject());
    vtkRenderer* renderer = rendererWrapper ? vtkRenderer::SafeDownCast( rendererWrapper->getVtkObject() ) : 0;
    if(renderer)
        renderer->ResetCamera();

    return true;
}

bool CDICOMFileLoader::loadDirectoryVolume(const QString& dirName)
{
    return false;
}

