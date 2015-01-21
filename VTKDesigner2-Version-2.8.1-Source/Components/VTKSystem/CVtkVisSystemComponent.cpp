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

#include "CVtkVisSystemComponent.h"
#include <GCF/ComponentFactory>
#include <GCF/IComponentPlugin>
#include "CVtkObjectDesc.h"
#include "CVtkObject.h"
#include "IVisSystemNodeFactoryRegistry.h"
#include "IVisNetwork.h"
#include "IVisNetworkCanvas.h"
#include "IVtkObjectWrapper.h"
#include "IProjectPart.h"
#include "IProject.h"
#include "vtkRenderer.h"
// #include "vtkAVIWriter.h"
// #include "vtkMPEG2Writer.h"
#include "vtkGenericMovieWriter.h"
#include "vtkRendererSource.h"
#include "ui_CRecordSettingsDialog.h"
#include "CVtkMath.h"
#include "vtkOutputWindow.h"
#include "CPipelineProgressMonitor.h"
#include "IProjectExt.h"

#include "IVtkDataObjectIOData.h"
#include "IVisNetworkConnection.h"

#include <QWidget>
#include <QMessageBox>
#include <QMainWindow>
#include <QTabWidget>
#include <QMap>
#include <QDialog>
#include <QFileInfo>
#include <QAction>
#include <QWidgetAction>
#include <QLabel>
#include <QFileInfo>
#include <QFile>
#include <QImage>
#include <QClipboard>
#include <QMenu>

// For implementing IVtkXmlUtils
#include <QIODevice>
#include <QTextStream>
#include <QTemporaryFile>
#include <QDir>

#include "QVTKWidget.h"
#include "vtkPolyData.h"
#include "vtkPoints.h"
#include "vtkCellArray.h"
#include "vtkDataArray.h"
#include "vtkPointData.h"
#include "vtkCellData.h"
#include "vtkDoubleArray.h"
#include "vtkXMLPolyDataWriter.h"
#include "vtkXMLImageDataWriter.h"
#include "vtkXMLPolyDataReader.h"
#include "vtkXMLImageDataReader.h"

// #include "vtkDataObjectToTable.h"
// #include "vtkQtTableModelAdapter.h"
// #include "vtkQtTableView.h"
#include <QTableView>

#include "vtkQtGraphicsViewRenderWindow.h"
#include "CVtkRenderWindow.h"
#include <QGraphicsProxyWidget>
#include <QGraphicsScene>

Q_DECLARE_METATYPE(vtkDataObject*)

class CVtkVisSystemOutputWindow : public vtkOutputWindow
{
public:
    static CVtkVisSystemOutputWindow* New() { return new CVtkVisSystemOutputWindow; }
    CVtkVisSystemOutputWindow() { }
    ~CVtkVisSystemOutputWindow() { }

    void DisplayText (const char *str) { qDebug("%s", str); }
    void DisplayErrorText (const char *str) { qWarning("%s", str); }
    void DisplayWarningText (const char *str) { qWarning("%s", str); }
    void DisplayGenericWarningText (const char *str) { qWarning("%s", str); }
};

struct CVtkVisSystemComponentData
{
    CVtkVisSystemComponentData() : pipelineProgressMonitor(0), vtkOutputDisplay(0),
        nodeFactoryRegistry(0), nodeFactoryRegistryComp(0),
        recordStartAction(0), recordStopAction(0), recordedFramesLabel(0),
        movieWriter(0), recordTimerId(-1), currentRenderer(0),
        currentRendererNode(0), currentRendererSource(0), project(0), projectExt(0) { }

    CPipelineProgressMonitor* pipelineProgressMonitor;
    QTabWidget* vtkOutputDisplay;
    QMap<QString, CVtkObjectDesc*> nodeClassMap;
    QList<IVisSystemNode*> rendererNodeList;

    IVisSystemNodeFactoryRegistry* nodeFactoryRegistry;
    GCF::AbstractComponent* nodeFactoryRegistryComp;
    QTabWidget* workspaceTabWidget;

    // Recording stuff.
    QAction* recordStartAction;
    QAction* recordStopAction;
    QLabel* recordedFramesLabel;
    vtkGenericMovieWriter* movieWriter;
    int frameCount;
    int currentFrame;
    int recordTimerId;
    vtkRenderer* currentRenderer;
    IVisSystemNode* currentRendererNode;
    vtkRendererSource* currentRendererSource;
    IProject* project;
    IProjectExt* projectExt;
    QString dataPath;
    bool isVtkOutputDisplayActive;

    // Custom methods
    QImage toQImage(vtkImageData* imageData);
    QImage autoCrop(const QImage& input);
};

GCF_DEFINE_COMPONENT(CVtkVisSystemComponent)

CVtkVisSystemComponent& CVtkVisSystemComponent::instance()
{
    static CVtkVisSystemComponent* theInstance = GCF_CREATE_COMPONENT(CVtkVisSystemComponent);
    return *theInstance;
}

CVtkVisSystemComponent::CVtkVisSystemComponent()
{
    d = new CVtkVisSystemComponentData;
    d->vtkOutputDisplay = new QTabWidget;
    vtkOutputWindow::SetInstance(CVtkVisSystemOutputWindow::New());
    d->dataPath = QString("%1/Data").arg(qApp->applicationDirPath());
    d->isVtkOutputDisplayActive = false;
}

CVtkVisSystemComponent::~CVtkVisSystemComponent()
{
    delete d;
}

QTabWidget* CVtkVisSystemComponent::vtkOutputDisplay() const
{
    return d->vtkOutputDisplay;
}

QString CVtkVisSystemComponent::resolvePath(const QString& path)
{
    // If the path is absolute, then use the same
    if( QFile::exists(path) )
        return path;

    QString fileName;

    // Check to see if we can find the path in d->dataPath
    if(!d->dataPath.isEmpty())
    {
        fileName = QString("%1/%2").arg(d->dataPath).arg(path);
        if( QFile::exists(fileName) )
            return fileName;
    }

    // Check to see if we can find the path in the project's working directory
    if(d->projectExt)
    {
        QString projectPath = d->projectExt->currentProjectFilePath();
        fileName = QString("%1/%2").arg(projectPath).arg(path);
        if( QFile::exists(fileName) )
            return fileName;
    }

    // Check to see if we can find the path in the default project path
    if(d->projectExt)
    {
        QString projectPath = d->projectExt->defaultProjectPath();
        fileName = QString("%1/%2").arg(projectPath).arg(path);
        if( QFile::exists(fileName) )
            return fileName;
    }

    // Check to see if we can find the path in the application's working directory
    {
        QFileInfo fi(path);
        QString fileName = fi.absoluteFilePath();
        if( QFile::exists(fileName) )
            return fileName;
    }

    // All failed, so we return an modified string
    return path;
}

void CVtkVisSystemComponent::setDataPath(const QString& path)
{
    d->dataPath = path;
}

QString CVtkVisSystemComponent::dataPath() const
{
    return d->dataPath;
}

void CVtkVisSystemComponent::recordStart(QObject* renNode, QString fileName, QString format, int interval, int nrFrames)
{
/*
    if(d->recordTimerId >= 0)
        return;

    Q_UNUSED(format);

    IVtkObjectWrapper* wrapper = qobject_cast<IVtkObjectWrapper*>(renNode);
    if(!wrapper)
        return;

    vtkRenderer* ren = dynamic_cast<vtkRenderer*>(wrapper->getVtkObject());
    if(!ren)
        return;

    d->movieWriter = vtkMPEG2Writer::New();
    d->movieWriter->SetFileName(qPrintable(fileName));
    d->frameCount = nrFrames;
    d->currentFrame = 0;
    d->currentRenderer = ren;
    d->currentRendererNode = qobject_cast<IVisSystemNode*>(renNode);
    d->currentRendererSource = vtkRendererSource::New();
    d->currentRendererSource->SetInput(d->currentRenderer);
    d->movieWriter->SetInput(d->currentRendererSource->GetOutput());
    d->movieWriter->Start();
    d->recordTimerId = startTimer(interval);

    if(d->recordStartAction)
        d->recordStartAction->setEnabled(false);
    if(d->recordStopAction)
        d->recordStopAction->setEnabled(true);
    if(d->recordedFramesLabel)
        d->recordedFramesLabel->setText("Starting...");
*/
}

void CVtkVisSystemComponent::aboutVtk()
{
    QMessageBox::information(mainWindow(), "About VTK",
        "VTK - Visualization Toolkit\n"
        "Version 5.0.2 from Kitware\n");
}

void CVtkVisSystemComponent::recordStart()
{
/*
    if(d->recordTimerId >= 0 || !d->rendererNodeList.count())
        return;

    QDialog dialog;
    Ui::CRecordSettingsDialog ui;
    ui.setupUi(&dialog);

    // Load all renderers in the combo box.
    for(int i=0; i<d->rendererNodeList.count(); i++)
    {
        IVisSystemNode* node = d->rendererNodeList[i];
        ui.cmbRenderers->addItem(node->nodeName());
    }

    while(1)
    {
        if( dialog.exec() == QDialog::Rejected )
            return;

        if(ui.txtFileName->text().isEmpty())
        {
            QMessageBox::information(GCF::Components::mainWindow(), "Record Settings Error",
                "You have not ented a file name to save the movie into.");
        }
        else
            break;
    }

    IVisSystemNode* renNode = d->rendererNodeList[ui.cmbRenderers->currentIndex()];

    QString fileName = ui.txtFileName->text();
    int interval = ui.txtInterval->value();
    int frames = ui.txtTotalFrames->value();

    QFileInfo fi(fileName);
    fileName = fi.absoluteFilePath();
    if(fi.suffix() != "mpg")
        fileName += ".mpg";

    recordStart(renNode->containerObject(), fileName, "mpeg", interval, frames);
*/
}

void CVtkVisSystemComponent::recordEnd()
{
/*
    if(d->recordTimerId < 0 || !d->movieWriter)
        return;

    QString fileName = QString("%1").arg(d->movieWriter->GetFileName());

    d->movieWriter->End();
    d->movieWriter->Delete();
    d->movieWriter = 0;
    killTimer(d->recordTimerId);
    d->recordTimerId = -1;

    QMessageBox::information(GCF::Components::mainWindow(), "Movie Saved",
        QString("Movie was saved in '%1'").arg(fileName));

    if(d->recordStartAction)
        d->recordStartAction->setEnabled(true);
    if(d->recordStopAction)
        d->recordStopAction->setEnabled(false);
    if(d->recordedFramesLabel)
        d->recordedFramesLabel->setText("Not Recording");
*/
}

void CVtkVisSystemComponent::finalizeComponent()
{
    if(d->nodeFactoryRegistry)
        d->nodeFactoryRegistry->unregisterNodeFactory(qobject_cast<IVisSystemNodeFactory*>(this));
}

void CVtkVisSystemComponent::justActivated()
{
    // Do nothing
}

void CVtkVisSystemComponent::aboutToDeactivate()
{
    // Do nothing
}

QAction* CVtkVisSystemComponent::fetchAction(const QString& completeName) const
{
   QStringList comps = completeName.split('.');

   CVtkVisSystemComponent* that = const_cast<CVtkVisSystemComponent*>(this);

   if(comps.last() == "startRecord")
    {
        d->recordStartAction = fetchAction(completeName);
        d->recordStartAction->setEnabled(false);
        return d->recordStartAction;
    }

    if(comps.last() == "stopRecord")
    {
        d->recordStopAction = fetchAction(completeName);
        d->recordStopAction->setEnabled(false);
        return d->recordStopAction;
    }

    if(comps.last() == "recordedFrames")
    {
        d->recordedFramesLabel = new QLabel;
        d->recordedFramesLabel->setMinimumWidth(150);
        d->recordedFramesLabel->setText("Not Recording");
        d->recordedFramesLabel->setAlignment(Qt::AlignCenter);
        d->recordedFramesLabel->setFrameShape(QFrame::Box);
        QWidgetAction* wAction = new QWidgetAction(that);
        wAction->setDefaultWidget(d->recordedFramesLabel);
        return wAction;
    }

    return fetchAction(completeName);
}

QObject* CVtkVisSystemComponent::fetchObject(const QString& completeName) const
{
    QStringList comps = completeName.split('.');

    CVtkVisSystemComponent* that = const_cast<CVtkVisSystemComponent*>(this);

    if(comps.last() == "controller")
        return that;

    if(comps.last() == "vtkMath")
        return &CVtkMath::instance();

    if(comps.last() == "vtkTransformationMatrix")
        return CVtkMath::instance().newTransformationMatrix();

    return 0;
}

void CVtkVisSystemComponent::addChildWidget(QWidget* parent, QWidget* child, const QString& hint)
{
    vtkQtGraphicsViewRenderWindow* gView = qobject_cast<vtkQtGraphicsViewRenderWindow*>(parent);
    if(!gView)
    {
        GCF::AbstractComponent::addChildWidget(parent, child, hint);
        return;
    }

    child->setWindowOpacity(0.9);

    QGraphicsProxyWidget* widgetItem 
        = new QGraphicsProxyWidget(0, Qt::Dialog|Qt::CustomizeWindowHint|Qt::WindowTitleHint);
    widgetItem->setWidget(child);
    widgetItem->setPos( -widgetItem->boundingRect().topLeft() );
    widgetItem->setFlag(QGraphicsItem::ItemIsMovable);
    widgetItem->setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    widgetItem->setWindowTitle(child->windowTitle());

    QGraphicsScene* scene = gView->scene();
    scene->addItem(widgetItem);
}

void CVtkVisSystemComponent::removeChildWidget(QWidget* parent, QWidget* child, const QString& hint)
{
    vtkQtGraphicsViewRenderWindow* gView = qobject_cast<vtkQtGraphicsViewRenderWindow*>(parent);
    if(!gView)
    {
        GCF::AbstractComponent::addChildWidget(parent, child, hint);
        return;
    }

    QGraphicsScene* scene = gView->scene();
    QList<QGraphicsItem*> items = scene->items();
    Q_FOREACH(QGraphicsItem* item, items)
    {
        if( item->type() != QGraphicsProxyWidget::Type )
            continue;

        QGraphicsProxyWidget* widgetItem = (QGraphicsProxyWidget*)item;
        if(widgetItem->widget() != child)
            continue;

        child->hide();
        widgetItem->setWidget(0);
        scene->removeItem(widgetItem);
        delete widgetItem;
        break;
    }
}

QWidget* CVtkVisSystemComponent::fetchWidget(const QString& completeName) const
{
    QStringList comps = completeName.split('.');

    if(comps.last() == "vtkOutputDisplay")
        return d->vtkOutputDisplay;

    if(comps.last() == "pipelineProgressMonitor")
    {
        if(!d->pipelineProgressMonitor)
            d->pipelineProgressMonitor = new CPipelineProgressMonitor;
        return d->pipelineProgressMonitor;
    }

    QList<CVtkRenderWindow*> renderWindows = CVtkRenderWindow::RenderWindowList();
    Q_FOREACH(CVtkRenderWindow* rWin, renderWindows)
    {
        if(rWin->nodeName() == comps.last())
            return rWin->vtkWidget();
    }

    return 0;
}

void CVtkVisSystemComponent::creationAnnounced()
{
    GCF::AbstractComponent* compPtr = 0;
    QString completeName;
    QObject* objectPtr = findObject("IVisSystemNodeFactoryRegistry", completeName, &d->nodeFactoryRegistryComp);
    if(objectPtr)
    {
        d->nodeFactoryRegistry = qobject_cast<IVisSystemNodeFactoryRegistry*>(objectPtr);
        d->nodeFactoryRegistry->registerNodeFactory(qobject_cast<IVisSystemNodeFactory*>(this));
       addDependency(this, d->nodeFactoryRegistryComp);
    }

    objectPtr = findObject("IVisNetwork", completeName, &compPtr);
    if(objectPtr)
    {
        if(d->pipelineProgressMonitor)
            d->pipelineProgressMonitor->setVisNetwork( qobject_cast<IVisNetwork*>(objectPtr) );
        // This should be done to ensure that this component is finalized
        // after the network is destroyed.
        addDependency(this, compPtr);
    }

    objectPtr = findObject("IVisNetworkCanvas", completeName, &compPtr);
    if(objectPtr)
    {
        if(d->pipelineProgressMonitor)
            d->pipelineProgressMonitor->setVisNetworkCanvas( qobject_cast<IVisNetworkCanvas*>(objectPtr) );
        connect(objectPtr, SIGNAL(connectionContextMenuEvent(IVisNetworkConnection*, QMenu*, Qt::KeyboardModifiers)),
                this, SLOT(slotConnectionContextMenuEvent(IVisNetworkConnection*, QMenu*, Qt::KeyboardModifiers)));
    }

    objectPtr = findObject("IProject", completeName, &compPtr);
    if(objectPtr)
    {
        d->project = qobject_cast<IProject*>(objectPtr);
        addDependency(compPtr, this);
    }

    objectPtr = findObject("IProjectExt", completeName, &compPtr);
    if(objectPtr)
    {
        d->projectExt = qobject_cast<IProjectExt*>(objectPtr);
        addDependency(compPtr, this);
    }

    if(d->recordStartAction)
        d->recordStartAction->setEnabled(false);
    if(d->recordStopAction)
        d->recordStopAction->setEnabled(false);

    QWidget* wsTabWidget = findWidget("MainWindowComponent.workspace");
    d->workspaceTabWidget = qobject_cast<QTabWidget*>(wsTabWidget);
    connect(mainWindow(), SIGNAL(workspaceWidgetActivated(QWidget*)),
            this, SLOT(slotWorkspaceWidgetActivated(QWidget*)));
}

void CVtkVisSystemComponent::componentDestroyed(GCF::AbstractComponent* comp)
{
    if(comp == d->nodeFactoryRegistryComp)
        d->nodeFactoryRegistry = 0;
}

QObject* CVtkVisSystemComponent::containerObject()
{
    return this;
}

QStringList CVtkVisSystemComponent::nodeClassNameList()
{
    return d->nodeClassMap.keys();
}

IVisSystemNodeDesc* CVtkVisSystemComponent::nodeDesc(QString nodeClassName)
{
    if(d->nodeClassMap.contains(nodeClassName))
        return d->nodeClassMap[nodeClassName];
    return 0;
}

bool CVtkVisSystemComponent::canCreate(QString nodeClassName)
{
    CVtkObjectDesc* desc = 0;
    if(d->nodeClassMap.contains(nodeClassName))
        desc = d->nodeClassMap[nodeClassName];
    if(!desc)
        return false;
    return desc->canCreate();
}

IVisSystemNode* CVtkVisSystemComponent::createNode(QString nodeClassName)
{
    CVtkObjectDesc* desc = 0;
    if(d->nodeClassMap.contains(nodeClassName))
        desc = d->nodeClassMap[nodeClassName];
    if(!desc)
        return 0;
    IVisSystemNode* node = desc->createNode();
    if(!node)
        return 0;

    CVtkObject* vtkNode = qobject_cast<CVtkObject*>(node->containerObject());
    vtkNode->setNodeFactory( qobject_cast<IVisSystemNodeFactory*>(this) );
    vtkNode->initializeNode();

    if(d->project)
    {
        IProjectPart* projectPart = qobject_cast<IProjectPart*>(node->containerObject());
        if( projectPart )
            d->project->addProjectPart(projectPart);
    }

    if(dynamic_cast<vtkRenderer*>(vtkNode->getVtkObject()))
        d->rendererNodeList.append( vtkNode );
    if(d->recordStartAction)
        d->recordStartAction->setEnabled( d->rendererNodeList.count() );

    return node;
}

void CVtkVisSystemComponent::deleteNode(IVisSystemNode* node)
{
    if(!node)
        return;

    if(d->project)
    {
        IProjectPart* projectPart = qobject_cast<IProjectPart*>(node->containerObject());
        if( projectPart )
            d->project->removeProjectPart(projectPart);
    }

    if(d->currentRendererNode == node)
        recordEnd();
    d->rendererNodeList.removeAll(node);
    if(d->recordStartAction)
        d->recordStartAction->setEnabled( d->rendererNodeList.count() );
    node->finalizeNode();
}

void CVtkVisSystemComponent::timerEvent(QTimerEvent* te)
{
    if(!d->movieWriter || !d->currentRendererSource)
    {
        killTimer(d->recordTimerId);
        return;
    }

    d->currentRendererSource->Update();
    d->movieWriter->Write();
    ++d->currentFrame;

    if(d->recordedFramesLabel)
        d->recordedFramesLabel->setText( QString("%1 Frames").arg(d->currentFrame) );

    if(d->frameCount && d->currentFrame > d->frameCount)
        recordEnd();
}


//////////////////////////////////////////////////////////////////////////////////////////////////
/// CVtkVisSystemComponent::initializeComponent() implementation
//////////////////////////////////////////////////////////////////////////////////////////////////

#include "CVtkAlgorithm.h"
#include "CVtkThreadedImageAlgorithm.h"
#include "CVtkPolyDataMapper.h"
#include "CVtkDataSetMapper.h"
#include "CVtkActor.h"
#include "CVtkAssembly.h"
#include "CVtkRenderer.h"
// #include "CVtkRenderWindow.h"
#include "CVtkCamera.h"
#include "CVtkFlythroughCamera.h"
#include "CVtkTimerObject.h"
#include "CVtkPolyDataAlgorithm.h"
#include "CVtkSphereSource.h"
#include "CVtkArrowSource.h"
#include "CVtkCubeSource.h"
#include "CVtkConeSource.h"
#include "CVtkCylinderSource.h"
#include "CVtkLineSource.h"
#include "CVtkPlaneSource.h"
#include "CVtkPointSource.h"
#include "CVtkTextSource.h"
#include "CVtkDiskSource.h"
#include "CVtkVectorText.h"
#include "CVtkAxes.h"
#include "CVtkCursor3D.h"
#include "CVtkImageAlgorithm.h"
#include "CVtkImageReader2.h"
#include "CVtkJPEGReader.h"
#include "CVtkTexture.h"
#include "CVtkPNGReader.h"
#include "CVtkSLCReader.h"
#include "CVtkTIFFReader.h"
#include "CVtkImageReader.h"
#include "CVtkBMPReader.h"
#include "CVtkPNMReader.h"
#include "CVtkLinearExtrusionFilter.h"
#include "CVtkPolyDataNormals.h"
#include "CVtkTriangleFilter.h"
#include "CVtkContourFilter.h"
#include "CVtkBandedPolyDataContourFilter.h"
#include "CVtkRegularPolygonSource.h"
#include "CVtkSimplePointsReader.h"
#include "CVtkSuperquadricSource.h"
#include "CVtkTexturedSphereSource.h"
#include "CVtkEarthSource.h"
#include "CVtkTubeFilter.h"
#include "CVtkDataSetSurfaceFilter.h"
#include "CVtkExtractEdges.h"
#include "CVtkExtractPolyDataGeometry.h"
#include "CVtkTransformPolyDataFilter.h"
#include "CVtkShrinkPolyData.h"
#include "CVtkOutlineCornerFilter.h"
#include "CVtkOutlineFilter.h"
#include "CVtkGenericOutlineFilter.h"
#include "CVtkContourGrid.h"
#include "CVtkGenericContourFilter.h"
#include "CVtkRibbonFilter.h"
#include "CVtkGeometryFilter.h"
#include "CVtkFeatureEdges.h"
#include "CVtkMarchingContourFilter.h"
#include "CVtkMarchingSquares.h"
#include "CVtkCustomPolyDataSource.h"
#include "CVtkCustomUnstructuredGridSource.h"
#include "CVtkCustomPolyDataFilter.h"
#include "CVtkCustomGridPolyDataSource.h"
#include "CVtkCustomFunction.h"
#include "CVtkCustomImageSource.h"
#include "CVtkGraphLayoutFilter.h"
#include "CVtkEdgePoints.h"
#include "CVtkHedgeHog.h"
#include "CVtkMaskPolyData.h"
#include "CVtkPolyDataStreamer.h"
#include "CVtkRuledSurfaceFilter.h"
#include "CVtkHyperStreamline.h"
#include "CVtkImageToPolyDataFilter.h"
#include "CVtkPolyDataConnectivityFilter.h"
#include "CVtkRotationalExtrusionFilter.h"
#include "CVtkHierarchicalDataSetGeometryFilter.h"
#include "CVtkGreedyTerrainDecimation.h"
#include "CVtkGridSynchronizedTemplates3D.h"
#include "CVtkDecimatePro.h"
#include "CVtkProperty.h"
#include "CVtkElevationFilter.h"
#include "CVtkCurvatures.h"
#include "CVtkPLYReader.h"
#include "CVtkPlatonicSolidSource.h"
#include "CVtkFacetReader.h"
#include "CVtkParticleReader.h"
#include "CVtkCutter.h"
#include "CVtkGenericCutter.h"
#include "CVtkSmoothPolyDataFilter.h"
#include "CVtkDepthSortPolyData.h"
#include "CVtkProjectedTerrainPath.h"
#include "CVtkGlyph3D.h"
#include "CVtkSampleFunction.h"
#include "CVtkImplicitFunction.h"
#include "CVtkBox.h"
#include "CVtkCone.h"
#include "CVtkCylinder.h"
#include "CVtkPlane.h"
#include "CVtkQuadric.h"
#include "CVtkSphere.h"
#include "CVtkSuperquadric.h"
#include "CVtkImplicitBoolean.h"
#include "CVtkCenterlineFilter.h"
#include "CVtkBrownianPoints.h"
#include "CVtkProjectedTexture.h"
#include "CVtkSimpleElevationFilter.h"
#include "CVtkTextureMapToCylinder.h"
#include "CVtkTextureMapToPlane.h"
#include "CVtkTextureMapToSphere.h"
#include "CVtkTransformFilter.h"
#include "CVtkWarpLens.h"
#include "CVtkWarpScalar.h"
#include "CVtkWarpTo.h"
#include "CVtkWarpVector.h"
#include "CVtkWeightedTransformFilter.h"
#include "CVtkLocator.h"
#include "CVtkPointLocator.h"
#include "CVtkCellLocator.h"
#include "CVtkAbstractPropPicker.h"
#include "CVtkPicker.h"
#include "CVtkCellPicker.h"
#include "CVtkPointPicker.h"
#include "CVtkPropPicker.h"
#include "CVtkVolume16Reader.h"
#include "CVtkDICOMImageReader.h"
#include "CVtkPLOT3DReader.h"
#include "CVtkTransform.h"
#include "CVtkClipDataSet.h"
#include "CVtkClipPolyData.h"
#include "CVtkDEMReader.h"
#include "CVtkGaussianCubeReader.h"
#include "CVtkPDBReader.h"
#include "CVtkXYZMolReader.h"
#include "CVtkDataObjectReader.h"
#include "CVtkDataSetReader.h"
#include "CVtkPolyDataReader.h"
#include "CVtkRectilinearGridReader.h"
#include "CVtkStructuredGridReader.h"
#include "CVtkUnstructuredGridReader.h"
#include "CVtkStructuredPointsReader.h"
#include "CVtk3DSImporter.h"
#include "CVtkVRMLImporter.h"
#include "CVtkSplineFilter.h"
#include "CVtkStreamer.h"
#include "CVtkStreamLine.h"
#include "CVtkDashedStreamLine.h"
#include "CVtkStreamPoints.h"
#include "CVtkStripper.h"
#include "CVtkAppendPolyData.h"
#include "CVtkCellCenters.h"
#include "CVtkCleanPolyData.h"
// #include "CVtkDuplicatePolyData.h"
#include "CVtkGenericGeometryFilter.h"
#include "CVtkGenericGlyph3DFilter.h"
#include "CVtkGenericStreamTracer.h"
#include "CVtkStreamTracer.h"
#include "CVtkBYUReader.h"
#include "CVtkColorTransferFunction.h"
#include "CVtkLookupTable.h"
#include "CVtkPiecewiseFunction.h"
#include "CVtkLoopSubdivisionFilter.h"
#include "CVtkInterpolatingSubdivisionFilter.h"
#include "CVtkButterflySubdivisionFilter.h"
#include "CVtkLinearSubdivisionFilter.h"
#include "CVtkActor2D.h"
#include "CVtkMapper2D.h"
#include "CVtkImageMapper.h"
#include "CVtkAxisActor2D.h"
#include "CVtkInteractorObserver.h"
#include "CVtk3DWidget.h"
#include "CVtkBoxWidget.h"
// #include "CVtkLineWidget.h"
#include "CVtkSphereWidget.h"
#include "CVtkSplineWidget.h"
#include "CVtkPointWidget.h"
#include "CVtkImageToTerrain.h"
#include "CVtkStructuredGridGeometryFilter.h"
#include "CVtkStructuredGridOutlineFilter.h"
#include "CVtkGaussianSplatter.h"
#include "CVtkImageLuminance.h"
#include "CVtkImageDataGeometryFilter.h"
#include "CVtkMergeFilter.h"
#include "CVtkProbeFilter.h"
#include "CVtkBezierSurfaceSource.h"
#include "CVtkBezierSurfaceWidget.h"
#include "CVtkWedgeSource.h"
#include "CVtkImageReslice.h"
#include "CVtkImageMapToColors.h"
#include "CVtkImageMapToWindowLevelColors.h"
#include "CVtkSTLReader.h"

#define ADD_WRAPPER(ClassName) \
ClassName::InitializeObjectDesc(); \
d->nodeClassMap[ClassName::ObjectDesc.nodeClassName()] = (&ClassName::ObjectDesc);

void CVtkVisSystemComponent::initializeComponent()
{
    // In this function we will register object descriptions
    // for all VTK Wrappers. Object Descriptions coming in
    // from plugins will have a different way of registering
    // object descriptions.
    ADD_WRAPPER(CVtkAlgorithm);
    ADD_WRAPPER(CVtkThreadedImageAlgorithm);
    ADD_WRAPPER(CVtkConeSource);
    ADD_WRAPPER(CVtkPolyDataMapper);
    ADD_WRAPPER(CVtkDataSetMapper);
    ADD_WRAPPER(CVtkActor);
    ADD_WRAPPER(CVtkAssembly);
    ADD_WRAPPER(CVtkRenderer);
    ADD_WRAPPER(CVtkRenderWindow);
    ADD_WRAPPER(CVtkCamera);
    ADD_WRAPPER(CVtkFlythroughCamera);
    ADD_WRAPPER(CVtkTimerObject);
    ADD_WRAPPER(CVtkPolyDataAlgorithm);
    ADD_WRAPPER(CVtkSphereSource);
    ADD_WRAPPER(CVtkArrowSource);
    ADD_WRAPPER(CVtkCubeSource);
    ADD_WRAPPER(CVtkCylinderSource);
    ADD_WRAPPER(CVtkLineSource);
    ADD_WRAPPER(CVtkPlaneSource);
    ADD_WRAPPER(CVtkPointSource);
    ADD_WRAPPER(CVtkTextSource);
    ADD_WRAPPER(CVtkDiskSource);
    ADD_WRAPPER(CVtkVectorText);
    ADD_WRAPPER(CVtkAxes);
    ADD_WRAPPER(CVtkCursor3D);
    ADD_WRAPPER(CVtkImageAlgorithm);
    ADD_WRAPPER(CVtkImageReader2);
    ADD_WRAPPER(CVtkJPEGReader);
    ADD_WRAPPER(CVtkTexture);
    ADD_WRAPPER(CVtkPNGReader);
    ADD_WRAPPER(CVtkSLCReader);
    ADD_WRAPPER(CVtkTIFFReader);
    ADD_WRAPPER(CVtkImageReader);
    ADD_WRAPPER(CVtkBMPReader);
    ADD_WRAPPER(CVtkPNMReader);
    ADD_WRAPPER(CVtkLinearExtrusionFilter);
    ADD_WRAPPER(CVtkPolyDataNormals);
    ADD_WRAPPER(CVtkTriangleFilter);
    ADD_WRAPPER(CVtkContourFilter);
    ADD_WRAPPER(CVtkBandedPolyDataContourFilter);
    ADD_WRAPPER(CVtkRegularPolygonSource);
    ADD_WRAPPER(CVtkSimplePointsReader);
    ADD_WRAPPER(CVtkSuperquadricSource);
    ADD_WRAPPER(CVtkTexturedSphereSource);
    ADD_WRAPPER(CVtkEarthSource);
    ADD_WRAPPER(CVtkTubeFilter);
    ADD_WRAPPER(CVtkDataSetSurfaceFilter);
    ADD_WRAPPER(CVtkExtractEdges);
    ADD_WRAPPER(CVtkExtractPolyDataGeometry);
    ADD_WRAPPER(CVtkTransformPolyDataFilter);
    ADD_WRAPPER(CVtkShrinkPolyData);
    ADD_WRAPPER(CVtkOutlineCornerFilter);
    ADD_WRAPPER(CVtkOutlineFilter);
    ADD_WRAPPER(CVtkGenericOutlineFilter);
    ADD_WRAPPER(CVtkContourGrid);
    ADD_WRAPPER(CVtkGenericContourFilter);
    ADD_WRAPPER(CVtkRibbonFilter);
    ADD_WRAPPER(CVtkGeometryFilter);
    ADD_WRAPPER(CVtkFeatureEdges);
    ADD_WRAPPER(CVtkMarchingContourFilter);
    ADD_WRAPPER(CVtkMarchingSquares);
    ADD_WRAPPER(CVtkCustomPolyDataSource);
    ADD_WRAPPER(CVtkCustomPolyDataFilter);
    ADD_WRAPPER(CVtkCustomFunction);
    ADD_WRAPPER(CVtkCustomImageSource);
    ADD_WRAPPER(CVtkGraphLayoutFilter);
    ADD_WRAPPER(CVtkEdgePoints);
    ADD_WRAPPER(CVtkHedgeHog);
    ADD_WRAPPER(CVtkMaskPolyData);
    ADD_WRAPPER(CVtkPolyDataStreamer);
    ADD_WRAPPER(CVtkRuledSurfaceFilter);
    ADD_WRAPPER(CVtkHyperStreamline);
    ADD_WRAPPER(CVtkImageToPolyDataFilter);
    ADD_WRAPPER(CVtkPolyDataConnectivityFilter);
    ADD_WRAPPER(CVtkRotationalExtrusionFilter);
    ADD_WRAPPER(CVtkHierarchicalDataSetGeometryFilter);
    ADD_WRAPPER(CVtkGreedyTerrainDecimation);
    ADD_WRAPPER(CVtkGridSynchronizedTemplates3D);
    ADD_WRAPPER(CVtkDecimatePro);
    ADD_WRAPPER(CVtkProperty);
    ADD_WRAPPER(CVtkCustomGridPolyDataSource);
    ADD_WRAPPER(CVtkCustomUnstructuredGridSource);
    ADD_WRAPPER(CVtkElevationFilter);
    ADD_WRAPPER(CVtkCurvatures);
    ADD_WRAPPER(CVtkPLYReader);
    ADD_WRAPPER(CVtkPlatonicSolidSource);
    ADD_WRAPPER(CVtkFacetReader);
    ADD_WRAPPER(CVtkParticleReader);
    ADD_WRAPPER(CVtkCutter);
    ADD_WRAPPER(CVtkGenericCutter);
    ADD_WRAPPER(CVtkSmoothPolyDataFilter);
    ADD_WRAPPER(CVtkDepthSortPolyData);
    ADD_WRAPPER(CVtkProjectedTerrainPath);
    ADD_WRAPPER(CVtkGlyph3D);
    ADD_WRAPPER(CVtkSampleFunction);
    ADD_WRAPPER(CVtkImplicitFunction);
    ADD_WRAPPER(CVtkBox);
    ADD_WRAPPER(CVtkCone);
    ADD_WRAPPER(CVtkCylinder);
    ADD_WRAPPER(CVtkPlane);
    ADD_WRAPPER(CVtkQuadric);
    ADD_WRAPPER(CVtkSphere);
    ADD_WRAPPER(CVtkSuperquadric);
    ADD_WRAPPER(CVtkImplicitBoolean);
    ADD_WRAPPER(CVtkCenterlineFilter);
    ADD_WRAPPER(CVtkBrownianPoints);
    ADD_WRAPPER(CVtkProjectedTexture);
    ADD_WRAPPER(CVtkSimpleElevationFilter);
    ADD_WRAPPER(CVtkTextureMapToCylinder);
    ADD_WRAPPER(CVtkTextureMapToPlane);
    ADD_WRAPPER(CVtkTextureMapToSphere);
    ADD_WRAPPER(CVtkTransformFilter);
    ADD_WRAPPER(CVtkWarpLens);
    ADD_WRAPPER(CVtkWarpScalar);
    ADD_WRAPPER(CVtkWarpTo);
    ADD_WRAPPER(CVtkWarpVector);
    ADD_WRAPPER(CVtkWeightedTransformFilter);
    ADD_WRAPPER(CVtkLocator)
    ADD_WRAPPER(CVtkPointLocator);
    ADD_WRAPPER(CVtkCellLocator);
    ADD_WRAPPER(CVtkAbstractPropPicker);
    ADD_WRAPPER(CVtkPicker);
    ADD_WRAPPER(CVtkCellPicker);
    ADD_WRAPPER(CVtkPointPicker);
    ADD_WRAPPER(CVtkPropPicker);
    ADD_WRAPPER(CVtkVolume16Reader);
    ADD_WRAPPER(CVtkDICOMImageReader);
    ADD_WRAPPER(CVtkPLOT3DReader);
    ADD_WRAPPER(CVtkTransform);
    ADD_WRAPPER(CVtkClipDataSet);
    ADD_WRAPPER(CVtkClipPolyData);
    ADD_WRAPPER(CVtkDEMReader);
    ADD_WRAPPER(CVtkGaussianCubeReader);
    ADD_WRAPPER(CVtkPDBReader);
    ADD_WRAPPER(CVtkXYZMolReader);
    ADD_WRAPPER(CVtkDataObjectReader);
    ADD_WRAPPER(CVtkDataSetReader);
    ADD_WRAPPER(CVtkPolyDataReader);
    ADD_WRAPPER(CVtkRectilinearGridReader);
    ADD_WRAPPER(CVtkStructuredGridReader);
    ADD_WRAPPER(CVtkUnstructuredGridReader);
    ADD_WRAPPER(CVtkStructuredPointsReader);
    ADD_WRAPPER(CVtk3DSImporter)
    ADD_WRAPPER(CVtkVRMLImporter)
    ADD_WRAPPER(CVtkSplineFilter)
    ADD_WRAPPER(CVtkStreamer)
    ADD_WRAPPER(CVtkStreamLine)
    ADD_WRAPPER(CVtkDashedStreamLine)
    ADD_WRAPPER(CVtkStreamPoints)
    ADD_WRAPPER(CVtkStripper)
    ADD_WRAPPER(CVtkAppendPolyData)
    ADD_WRAPPER(CVtkCellCenters)
    ADD_WRAPPER(CVtkCleanPolyData)
    // ADD_WRAPPER(CVtkDuplicatePolyData)
    ADD_WRAPPER(CVtkGenericGeometryFilter)
    ADD_WRAPPER(CVtkGenericGlyph3DFilter)
    ADD_WRAPPER(CVtkGenericStreamTracer)
    // ADD_WRAPPER(CVtkStreamTracer)
    ADD_WRAPPER(CVtkBYUReader)
    ADD_WRAPPER(CVtkColorTransferFunction)
    ADD_WRAPPER(CVtkLookupTable)
    ADD_WRAPPER(CVtkPiecewiseFunction)
    ADD_WRAPPER(CVtkLoopSubdivisionFilter)
    ADD_WRAPPER(CVtkInterpolatingSubdivisionFilter)
    ADD_WRAPPER(CVtkButterflySubdivisionFilter)
    ADD_WRAPPER(CVtkLinearSubdivisionFilter)
    ADD_WRAPPER(CVtkActor2D)
    ADD_WRAPPER(CVtkMapper2D)
    ADD_WRAPPER(CVtkImageMapper)
    ADD_WRAPPER(CVtkAxisActor2D)
    ADD_WRAPPER(CVtkInteractorObserver)
    ADD_WRAPPER(CVtk3DWidget)
    ADD_WRAPPER(CVtkBoxWidget)
    // ADD_WRAPPER(CVtkLineWidget)
    ADD_WRAPPER(CVtkSphereWidget)
    ADD_WRAPPER(CVtkSplineWidget)
    ADD_WRAPPER(CVtkPointWidget)
    ADD_WRAPPER(CVtkImageToTerrain)
    ADD_WRAPPER(CVtkStructuredGridGeometryFilter)
    ADD_WRAPPER(CVtkStructuredGridOutlineFilter)
    ADD_WRAPPER(CVtkGaussianSplatter)
    ADD_WRAPPER(CVtkImageLuminance)
    ADD_WRAPPER(CVtkImageDataGeometryFilter)
    ADD_WRAPPER(CVtkMergeFilter)
    ADD_WRAPPER(CVtkProbeFilter)
    ADD_WRAPPER(CVtkBezierSurfaceSource)
    ADD_WRAPPER(CVtkBezierSurfaceWidget)
    ADD_WRAPPER(CVtkWedgeSource);
    ADD_WRAPPER(CVtkImageReslice)
    ADD_WRAPPER(CVtkImageMapToColors)
    ADD_WRAPPER(CVtkImageMapToWindowLevelColors)
    ADD_WRAPPER(CVtkSTLReader)
}


QString CVtkVisSystemComponent::productName() const
{
    return "VTK System Component";
}

QString CVtkVisSystemComponent::organization() const
{
    return "VCreate Logic (P) Ltd (Classes from VTK www.vtk.org)";
}

QImage CVtkVisSystemComponent::programLogo() const
{
    return QImage();
}

QString CVtkVisSystemComponent::version() const
{
    return "2.0";
}

QString CVtkVisSystemComponent::shortDescription() const
{
    return "Node factory for commonly used VTK Classes";
}

QString CVtkVisSystemComponent::homepage() const
{
    return "http://www.vcreatelogic.com/oss/vtkdesigner";
}

QString CVtkVisSystemComponent::bugAddress() const
{
    return "http://www.vcreatelogic.com/oss/vtkdesigner/feedback/bugs";
}

const QList<GCF::AboutPerson> CVtkVisSystemComponent::authors() const
{
    static QList<GCF::AboutPerson> retList;
    if(retList.count() == 0)
    {
        retList << GCF::AboutPerson("Prashanth N Udupa", "prashanth@vcreatelogic.com", "http://www.vcreatelogic.com", "Founder, Vision, Design, Development");
        retList << GCF::AboutPerson("Lohith D S", "NA", "http://www.vcreatelogic.com", "VTK System Development");
        retList << GCF::AboutPerson("Brian Gee Chacko", "brian.chacko@vcreatelogic.com", "http://www.vcreatelogic.com", "VTK System Development");
    }
    return retList;
}

const QList<GCF::AboutPerson> CVtkVisSystemComponent::credits() const
{
    static QList<GCF::AboutPerson> retList;
    if(retList.count() == 0)
    {
        retList << GCF::AboutPerson("Paul Bourke", "paul.bourke@gmail.com", "http://local.wasp.uwa.edu.au/~pbourke/geometry/bezier/index.html",
                                    "Thanks to Paul Bourke for explaining Bezier Surfaces algorithm so well in his page. The vtkBezierSurfaceSource "
                                    "algorithm is based on the code and materials given in his page");
    }
    return retList;
}

QString CVtkVisSystemComponent::license() const
{
    return "Dual License: GPL Version 2 for FOSS development, Commercial for others.";
}

QString CVtkVisSystemComponent::copyrightStatement() const
{
    return QString();
}

QString CVtkVisSystemComponent::projectPartID()
{
    return "VtkSystem";
}

bool CVtkVisSystemComponent::newProjectPart()
{
    return true;
}

bool CVtkVisSystemComponent::loadProjectPart(QDomDocument doc, QDomElement partE)
{
    Q_UNUSED(doc);
    Q_UNUSED(partE);
    return true;
}

bool CVtkVisSystemComponent::initializeProjectPart()
{
    if(d->workspaceTabWidget)
        d->workspaceTabWidget->setCurrentWidget( d->vtkOutputDisplay );
    return true;
}

bool CVtkVisSystemComponent::saveProjectPart(QDomDocument doc, QDomElement partParentE)
{
    Q_UNUSED(doc);
    Q_UNUSED(partParentE);
    return true;
}

bool CVtkVisSystemComponent::closeProjectPart()
{
    return true;
}

void CVtkVisSystemComponent::savePoints(vtkPoints* points, QDomDocument doc, QDomElement ele)
{
    if( !points )
        return;

    QString str;
    QTextStream ts(&str, QIODevice::WriteOnly);

    vtkIdType nrPoints = points->GetNumberOfPoints();
    ele.setAttribute("Count", nrPoints);

    for(vtkIdType i=0; i<nrPoints; i++)
    {
        double* pt = points->GetPoint(i);
        ts << pt[0] << " " << pt[1] << " " << pt[2] << " ";
    }

    ele.appendChild( doc.createTextNode(str) );
}

void CVtkVisSystemComponent::loadPoints(vtkPoints* points, QDomDocument doc, QDomElement ele)
{
    if( !points )
        return;

    QString str = ele.text();
    QTextStream ts(&str, QIODevice::ReadOnly);

    vtkIdType nrPoints = vtkIdType( ele.attribute("Count").toInt() );
    points->SetNumberOfPoints(nrPoints);

    for(vtkIdType i=0; i<nrPoints; i++)
    {
        double pt[3];
        ts >> pt[0] >> pt[1] >> pt[2];
        points->SetPoint(i, pt);
    }
}

void CVtkVisSystemComponent::saveCells(vtkCellArray* cells, QDomDocument doc, QDomElement ele)
{
    if( !cells )
        return;

    QString str;
    QTextStream ts(&str, QIODevice::WriteOnly);

    vtkIdType cellCount = cells->GetNumberOfCells();
    ele.setAttribute("Count", cellCount);

    for(vtkIdType i=0; i<cellCount; i++)
    {
        vtkIdType* pts;
        vtkIdType nrPts;
        cells->GetCell(i, nrPts, pts);

        ts << nrPts << " ";
        for(vtkIdType j=0; j<nrPts; j++)
            ts << pts[j] << " ";
    }

    ele.appendChild( doc.createTextNode(str) );
}

void CVtkVisSystemComponent::loadCells(vtkCellArray* cells, QDomDocument doc, QDomElement ele)
{
    if( !cells )
        return;

    QString str = ele.text();
    QTextStream ts(&str, QIODevice::ReadOnly);

    vtkIdType cellCount = vtkIdType(ele.attribute("Count").toInt());
    vtkIdType nrPts = 0, oldNrPts = 0;
    vtkIdType* pts = 0;

    for(vtkIdType i=0; i<cellCount; i++)
    {
        ts >> nrPts;
        if( nrPts != oldNrPts )
            pts = new vtkIdType[nrPts];
        oldNrPts = nrPts;

        for(vtkIdType j=0; j<nrPts; j++)
            ts >> pts[j];
        cells->InsertNextCell(nrPts, pts);
    }

    delete [] pts;
}

void CVtkVisSystemComponent::saveData(vtkDataArray* dataArray, QDomDocument doc, QDomElement ele)
{
    if( !dataArray )
        return;

    QString str;
    QTextStream ts(&str, QIODevice::WriteOnly);
    vtkIdType nrComps = dataArray->GetNumberOfComponents();
    int dataType = dataArray->GetDataType();
    vtkIdType nrTuples = dataArray->GetNumberOfTuples();

    ele.setAttribute("DataType", dataType);
    ele.setAttribute("NrComps", nrComps);
    ele.setAttribute("Count", nrTuples);
    for(vtkIdType i=0; i<nrTuples; i++)
    {
        double* tuple = dataArray->GetTuple(i);
        for(vtkIdType j=0; j<nrComps; j++)
            ts << tuple[j] << " ";
    }

    ele.appendChild( doc.createTextNode(str) );
}

void CVtkVisSystemComponent::loadData(vtkDataArray* dataArray, QDomDocument doc, QDomElement ele)
{
    if( !dataArray )
        return;

    QString str = ele.text();
    QTextStream ts(&str, QIODevice::ReadOnly);

    vtkIdType nrComps = vtkIdType(ele.attribute("NrComps").toInt());
    vtkIdType nrTuples = vtkIdType(ele.attribute("Count").toInt());
    if( !nrTuples )
        return;

    double* tuple = new double[nrComps];

    dataArray->SetNumberOfComponents(nrComps);
    dataArray->SetNumberOfTuples(nrTuples);

    for(vtkIdType i=0; i<nrTuples; i++)
    {
        for(vtkIdType j=0; j<nrComps; j++)
            ts >> tuple[j];

        dataArray->SetTuple(i, tuple);
    }

    delete [] tuple;
}

void CVtkVisSystemComponent::savePolyData(vtkPolyData* polyData, QDomDocument doc, QDomElement ele)
{
    if( !polyData )
        return;

    // Create a temporary file name
    QTemporaryFile tempFile;
    QString tempFileName;
    tempFile.open();
    tempFileName = tempFile.fileName();
    tempFile.close();

    // Write into the temporary file
    vtkXMLPolyDataWriter* writer = vtkXMLPolyDataWriter::New();
    writer->SetInput(polyData);
    writer->SetFileName( qPrintable(tempFileName) );
    writer->Write();
    writer->Delete();

    // Read the contents of the temporary file
    QFile file(tempFileName);
    file.open(QFile::ReadOnly);
    QString fileStr = file.readAll();
    fileStr = fileStr.simplified();
    file.close();

    // Delete the temporary file
    QFile::remove(tempFileName);

    // Write the fileStr into ele
    ele.appendChild( doc.createCDATASection(fileStr) );
}

void CVtkVisSystemComponent::loadPolyData(vtkPolyData* polyData, QDomDocument doc, QDomElement ele)
{
    if( !polyData )
        return;

    // Get the XML of vtkPolyData
    QString pdXml = ele.text();

    // Store it in a temporary file
    QTemporaryFile tempFile;
    QString tempFileName;
    tempFile.open();
    tempFile.write( pdXml.toAscii() );
    tempFile.setAutoRemove(false);
    tempFileName = tempFile.fileName();
    tempFile.close();

    // Now read from the temp file
    vtkXMLPolyDataReader* reader = vtkXMLPolyDataReader::New();
    reader->SetFileName( qPrintable(tempFileName) );
    reader->Update();
    polyData->ShallowCopy(reader->GetOutput());
    reader->Delete();

    // Delete the temporary file
    QFile::remove(tempFileName);
}

void CVtkVisSystemComponent::saveImageData(vtkImageData* imageData, QDomDocument doc, QDomElement ele)
{
    if( !imageData )
        return;

    // Create a temporary file name
    QTemporaryFile tempFile;
    QString tempFileName;
    tempFile.open();
    tempFileName = tempFile.fileName();
    tempFile.close();

    // Write into the temporary file
    vtkXMLImageDataWriter* writer = vtkXMLImageDataWriter::New();
    writer->SetInput(imageData);
    writer->SetFileName( qPrintable(tempFileName) );
    writer->Write();
    writer->Delete();

    // Read the contents of the temporary file
    QFile file(tempFileName);
    file.open(QFile::ReadOnly);
    QString fileStr = file.readAll();
    fileStr = fileStr.simplified();
    file.close();

    // Delete the temporary file
    QFile::remove(tempFileName);

    // Write the fileStr into ele
    ele.appendChild( doc.createCDATASection(fileStr) );
}

void CVtkVisSystemComponent::loadImageData(vtkImageData* imageData, QDomDocument doc, QDomElement ele)
{
    if( !imageData )
        return;

    // Get the XML of vtkImageData
    QString pdXml = ele.text();

    // Store it in a temporary file
    QTemporaryFile tempFile;
    QString tempFileName;
    tempFile.open();
    tempFile.write( pdXml.toAscii() );
    tempFile.setAutoRemove(false);
    tempFileName = tempFile.fileName();
    tempFile.close();

    // Now read from the temp file
    vtkXMLImageDataReader* reader = vtkXMLImageDataReader::New();
    reader->SetFileName( qPrintable(tempFileName) );
    reader->Update();
    imageData->ShallowCopy(reader->GetOutput());
    reader->Delete();

    // Delete the temporary file
    QFile::remove(tempFileName);
}

void CVtkVisSystemComponent::captureSnapshotToClipboard()
{
    if(!d->vtkOutputDisplay || !d->isVtkOutputDisplayActive)
        return;

    QWidget* currentTab = d->vtkOutputDisplay->currentWidget();
    if(!currentTab)
        return;

    QVTKWidget* vtkw = qobject_cast<QVTKWidget*>(currentTab);
    if(!vtkw)
        return;

    QApplication::setOverrideCursor(Qt::WaitCursor);
    vtkw->saveImageToCache();
    vtkImageData* image = vtkw->cachedImage();
    QImage qimage = d->toQImage(image);
    qimage = d->autoCrop(qimage);
    QClipboard* clipboard = QApplication::clipboard();
    clipboard->setImage(qimage);
    QApplication::restoreOverrideCursor();
}

void CVtkVisSystemComponent::slotWorkspaceWidgetActivated(QWidget* widget)
{
    d->isVtkOutputDisplayActive = (widget == d->vtkOutputDisplay);
}

void CVtkVisSystemComponent::slotConnectionContextMenuEvent(IVisNetworkConnection* node, QMenu* menu, Qt::KeyboardModifiers modifiers)
{
    IVisSystemNodeIOData* iodata = node->connectionData();
    if(!iodata)
        return;

    // Check if the iodata is a polydata
    IVtkDataObjectIOData* dataObjIOData = 0;
    bool success = iodata->queryInterface("IVtkDataObjectIOData", (void**)&dataObjIOData);
    if(!success)
        return;

    // Catch hold of the vtkDataObject
    vtkDataObject* dataObj = dataObjIOData->getVtkDataObject();

    // Show menu option for exploring the contents of the data object
    QAction* exploreDataAction = menu->addAction("Explore Data");
    exploreDataAction->setProperty("vtkDataObject", qVariantFromValue<vtkDataObject*>(dataObj));
    connect(exploreDataAction, SIGNAL(triggered(bool)), this, SLOT(slotExploreData()));
}

void CVtkVisSystemComponent::slotExploreData()
{
#if 0
    QAction* action = qobject_cast<QAction*>(sender());
    if(!action->dynamicPropertyNames().contains("vtkDataObject"))
        return;

    vtkDataObject* dataObj = action->property("vtkDataObject").value<vtkDataObject*>();
    if(!dataObj)
        return;

    // Create a dialog box.
    QDialog dialog( GCF::AbstractComponent::mainWindow() );
    dialog.setWindowTitle( QString("Exploring %1").arg(dataObj->GetClassName()) );
    dialog.resize(640, 480);

    QVBoxLayout* dialogLayout = new QVBoxLayout(&dialog);

    QTabWidget* tabWidget = new QTabWidget(&dialog);
    dialogLayout->addWidget(tabWidget);

    // vtkDataObjectToTable algorithm helps in converting
    // data contained in a vtkDataObject into a vtkTable, which
    // can then be interfaced via vtkQtTableModelAdapter to show
    // values in a QTableView.
    static QStringList tabNames = QStringList() << "Point Data"
        << "Cell Data" << "Field Data" << "Vertex Data"
        << "Edge Data";
    static QList<int> tabTypes = QList<int>() << vtkDataObjectToTable::POINT_DATA
        << vtkDataObjectToTable::CELL_DATA << vtkDataObjectToTable::FIELD_DATA
        << vtkDataObjectToTable::VERTEX_DATA << vtkDataObjectToTable::EDGE_DATA;

    for(int i=0; i<tabTypes.count(); i++)
    {
        vtkDataObjectToTable* tableConverter = vtkDataObjectToTable::New();
        tableConverter->SetFieldType(tabTypes.at(i));
        tableConverter->SetInput(dataObj);
        tableConverter->Update();

        vtkTable* table = tableConverter->GetOutput();

        vtkQtTableModelAdapter* tableAdaptor = new vtkQtTableModelAdapter(&dialog);
        tableAdaptor->setTable(table);

        tableConverter->Delete();

        QTableView* tableView = new QTableView;
        tableView->setModel(tableAdaptor);

        tabWidget->addTab(tableView, tabNames.at(i));
    }

    dialog.exec();
#endif
}

QImage CVtkVisSystemComponentData::toQImage(vtkImageData* imageData)
{
    if(!imageData)
        return QImage();

    int* dim = imageData->GetDimensions();
    int width = dim[0];
    int height = dim[1];
    QImage retImage(width, height, QImage::Format_RGB32);

    for(int i=0; i<width; i++)
    {
        for(int j=0; j<height; j++)
        {
            double r = imageData->GetScalarComponentAsDouble(i, j, 0, 0);
            double g = imageData->GetScalarComponentAsDouble(i, j, 0, 1);
            double b = imageData->GetScalarComponentAsDouble(i, j, 0, 2);

            QRgb value = qRgb((int)r, (int)g, (int)b);
            retImage.setPixel(i, (height-j-1), value);
        }
    }

    return retImage;
}

QImage CVtkVisSystemComponentData::autoCrop(const QImage& input)
{
    // The first pixel is treated as the background color.
    QRgb bgColor = input.pixel(0,0);
    int left=-1, top=-1, right=-1, bottom=-1, width=0, height=0;

    for(int i=0; i<input.width(); i++)
    {
        for(int j=0; j<input.height(); j++)
        {
            QRgb color = input.pixel(i, j);
            if(color == bgColor)
                continue;

            if(left >= 0)
            {
                if(i < left)
                    left = i;
            }
            else
                left = i;

            if(right >= 0)
            {
                if(i > right)
                    right = i;
            }
            else
                right = i;

            if(top >= 0)
            {
                if(j < top)
                    top = j;
            }
            else
                top = j;

            if(bottom >= 0)
            {
                if(j > bottom)
                    bottom = j;
            }
            else
                bottom = j;
        }
    }

    width = right-left;
    height = bottom-top;

    QImage retImage = input.copy(left, top, width, height);
    return retImage;
}

GCF_EXPORT_COMPONENT_PLUGIN(CVtkVisSystemComponent)
