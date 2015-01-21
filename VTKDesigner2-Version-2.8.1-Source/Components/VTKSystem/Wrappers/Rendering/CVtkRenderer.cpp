/****************************************************************************
**
** Copyright (C) VCreate Logic Private Limited, Bangalore
**
** Use of this file is limited according to the terms specified by
** VCreate Logic Private Limited, Bangalore. Details of those terms
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

#include "CVtkRenderer.h"
#include "vtkActorCollection.h"
#include "vtkActor2DCollection.h"
#include "IVtkProp3DIOData.h"
#include "IVtkCameraIOData.h"
#include "IVtkActorIOData.h"
#include "IVtkVolumeIOData.h"
#include "IVtkImporterIOData.h"
#include "IVtkActor2DIOData.h"

#include "vtkActorCollection.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkActor2D.h"
#include "vtkProp3D.h"
#include "vtkCamera.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper.h"
#include "vtkActor.h"
#include "vtkPoints.h"
#include "vtkCellArray.h"
#include "vtkCommand.h"
#include "vtkProperty.h"
#include "vtkLight.h"
#include "vtkLightCollection.h"
#include "vtkLightKit.h"
#include "vtkImporter.h"

#include <QMessageBox>
#include <QMouseEvent>
#include <QKeyEvent>

class CVtkRendererGridScene : public vtkCommand
{
public:
    CVtkRendererGridScene();
    ~CVtkRendererGridScene();
    void ConstructScene();
    void UpdatePolyData();
    void Execute (vtkObject *caller, unsigned long eventId, void *callData);

    vtkPolyData* mPolyData;
    vtkPolyDataMapper* mMapper;
    vtkActor* mActor;
    double mBBox[6];
};

DEFINE_VTK_OBJECT(CVtkRenderer, CVtkObject, vtkRenderer)
{
    pDesc->setNodeClassCategory("Rendering");
    pDesc->setNodeClassName("vtkRenderer");

    pDesc->setPropertyInfo("BackgroundColor", "SetBackground" , "GetBackground", QStringList () << "color_double" << "color_double" << "color_double", QStringList() , "void" ,"color_double*" );
    pDesc->setPropertyInfo("LightsFollowCamera", "SetLightFollowCamera" , "GetLightFollowCamera", QStringList () << "bool" , QStringList() , "void" ,"bool" );
    pDesc->setPropertyInfo("TwoSidedLighting", "SetTwoSidedLighting" , "GetTwoSidedLighting", QStringList () << "bool" , QStringList() , "void" ,"bool" );
    pDesc->setPropertyInfo("Viewport", "SetViewport" , "GetViewport", QStringList () << "double" << "double" << "double" , QStringList() , "void" ,"double*" );
    
    // Prop3D Input
    pDesc->addConnectionPath(
        new CVtkObjectConnectionPath("Prop3D", IVisSystemNodeConnectionPath::InputPath, "vtkProp3D", 0, true)
        );

    // Camera Input
    pDesc->addConnectionPath(
        new CVtkObjectConnectionPath("Camera", IVisSystemNodeConnectionPath::InputPath, "vtkCamera", 0)
        );

    // Renderer Output
    pDesc->addConnectionPath(
        new CVtkObjectConnectionPath("Renderer", IVisSystemNodeConnectionPath::OutputPath, "vtkRenderer", 0)
        );

    // Importer Input
    pDesc->addConnectionPath(
        new CVtkObjectConnectionPath("Importer", IVisSystemNodeConnectionPath::InputPath, "vtkImporter", 0)
        );

    pDesc->setPropertyInfo("BackgroundColor", "SetBackground", "GetBackground", QStringList() << "color_double" << "color_double" << "color_double", QStringList(), "void", "double*" );
}

CVtkRenderer::CVtkRenderer() : m_vtkRenderer(0), m_hasUserCamera(false), m_RendererSceneGrid(0)
{
    CVtkRenderer::InitializeObjectDesc();
    setVtkObject(vtkRenderer::New());
    // setBackgroundColor(QColor(0, 85, 127));
    setBackgroundColor(Qt::white);
    setShowGrid(true);
}

CVtkRenderer::~CVtkRenderer()
{

}

void CVtkRenderer::setBackgroundColor(QColor color)
{
    m_vtkRenderer->SetBackground(color.redF(), color.greenF(), color.blueF());
}

QColor CVtkRenderer::backgroundColor() const
{
    QColor color;
    double* bgColor = m_vtkRenderer->GetBackground();
    color.setRgbF(bgColor[0], bgColor[1], bgColor[2]);
    return color;
}

void CVtkRenderer::setShowGrid(bool val)
{
    /*
    if(val && m_RendererSceneGrid)
        return;

    if(!val && !m_RendererSceneGrid)
        return;

    if(val)
    {
        m_RendererSceneGrid = new CVtkRendererGridScene;
        m_vtkRenderer->ComputeVisiblePropBounds(m_RendererSceneGrid->mBBox);
        m_RendererSceneGrid->ConstructScene();
        m_vtkRenderer->AddActor(m_RendererSceneGrid->mActor);
    }
    else
    {
        m_vtkRenderer->RemoveActor(m_RendererSceneGrid->mActor);
        delete m_RendererSceneGrid;
        m_RendererSceneGrid = 0;
    }

    if(m_vtkRenderer->GetRenderWindow())
        m_vtkRenderer->Render();
    */
}


bool CVtkRenderer::isShowGrid() const
{
    return m_RendererSceneGrid != 0;
}

void CVtkRenderer::setLightsFollowCamera(bool val)
{
    m_vtkRenderer->SetLightFollowCamera(val);
}

bool CVtkRenderer::isLightsFollowCamera() const
{
    return m_vtkRenderer->GetLightFollowCamera();
}

void CVtkRenderer::setTwoSidedLighting(bool val)
{
    m_vtkRenderer->SetTwoSidedLighting(val);
}

bool CVtkRenderer::isTwoSidedLighting() const
{
    return m_vtkRenderer->GetTwoSidedLighting();
}

void CVtkRenderer::setViewport(QRectF vRect)
{
    m_vtkRenderer->SetViewport(vRect.left(), vRect.bottom(), vRect.right(), vRect.top());
}

QRectF CVtkRenderer::viewport() const
{
    double* v = m_vtkRenderer->GetViewport();
    return QRectF(v[0], v[1], v[2]-v[0], v[3]-v[1]);
}

void CVtkRenderer::createAutomaticLights()
{
    vtkLightCollection* lCol = m_vtkRenderer->GetLights();
    while(lCol->GetNumberOfItems())
    {
        vtkLight* light = dynamic_cast<vtkLight*>(lCol->GetItemAsObject(0));
        if(light)
            m_vtkRenderer->RemoveLight(light);
    }

    vtkLightKit* lightKit = vtkLightKit::New();
    lightKit->AddLightsToRenderer(m_vtkRenderer);
    lightKit->Delete();

#define MACRO(x) x[0], x[1], x[2]
    lCol = m_vtkRenderer->GetLights();
    for(int i=0; i<lCol->GetNumberOfItems(); i++)
    {
        vtkLight* light = vtkLight::SafeDownCast(lCol->GetItemAsObject(i));
        qWarning("Color[%d]: Ambient(%.2f, %.2f, %.2f), Diffuse(%.2f, %.2f, %.2f), Specular(%.2f, %.2f, %.2f)",
            i, MACRO(light->GetAmbientColor()), MACRO(light->GetDiffuseColor()), MACRO(light->GetSpecularColor()));
    }
#undef MACRO
}

Point3D CVtkRenderer::toScreenCoordinates(Point3D p)
{
    m_vtkRenderer->SetWorldPoint(p.x, p.y, p.y, 1);
    m_vtkRenderer->WorldToDisplay();
    return m_vtkRenderer->GetDisplayPoint();
}

Point3D CVtkRenderer::toSceneCoordinates(Point3D p)
{
    int* size = m_vtkRenderer->GetRenderWindow()->GetSize();
    m_vtkRenderer->SetDisplayPoint(p.x, size[1]-p.y, 1.0);
    m_vtkRenderer->DisplayToWorld();
    double* world = m_vtkRenderer->GetWorldPoint();
    if(world[3] == 0.0)
        return Point3D();
    for(int i=0; i < 3; i++) 
        world[i] = world[i] / world[3];
    return Point3D(world);
}

void CVtkRenderer::flyTo(Point3D point, int nrFrames)
{
    vtkRenderWindow* renderWindow = m_vtkRenderer->GetRenderWindow();
    if(!renderWindow)
        return;

    vtkRenderWindowInteractor* rWinInt = renderWindow->GetInteractor();
    if(!rWinInt)
        return;

    rWinInt->SetNumberOfFlyFrames(nrFrames);
    rWinInt->FlyTo(m_vtkRenderer, point.x, point.y, point.z);
}

void CVtkRenderer::resetCamera()
{
    if(m_hasUserCamera)
        return;

    command_ResetCamera();
}

void CVtkRenderer::render()
{
    command_Render();
}

void CVtkRenderer::command_CreateLights()
{
    this->createAutomaticLights();
}

void CVtkRenderer::command_ResetCamera()
{
    if(m_hasUserCamera)
    {
        QMessageBox::information(0, "Reset Camera Denied",
                "Cannot reset the camera, because it is a user defined camera");
    }
    else
        m_vtkRenderer->ResetCamera();
}

void CVtkRenderer::command_Render()
{
    if(m_vtkRenderer->GetRenderWindow())
        m_vtkRenderer->Render();
}

bool CVtkRenderer::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    if(path->pathName() == "Prop3D")
    {
        return m_vtkRenderer->GetActors()->GetNumberOfItems() != 0 ||
               m_vtkRenderer->GetVolumes()->GetNumberOfItems() != 0 ||
               m_vtkRenderer->GetActors2D()->GetNumberOfItems() != 0;
    }

    if(path->pathName() == "Camera")
    {
        return m_hasUserCamera;
    }

    return CVtkObject::hasInput(path);
}

bool CVtkRenderer::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    if(path->pathName() == "Prop3D")
    {
        // Find out if the Prop3D is vtkVolume or vtkActor.
        IVtkVolumeIOData* volData = 0;
        IVtkActorIOData* actorData = 0;
        IVtkProp3DIOData* prop3DData = 0;
        IVtkActor2DIOData* actor2DData = 0;
        vtkVolume* volume = 0;
        vtkActor* actor = 0;
        vtkProp3D* prop3D = 0;
        vtkActor2D* actor2D = 0;
        inputData->queryInterface("IVtkVolumeIOData", (void**)&volData);
        inputData->queryInterface("IVtkActorIOData", (void**)&actorData);
        inputData->queryInterface("IVtkProp3DIOData", (void**)&prop3DData);
        inputData->queryInterface("IVtkActor2DIOData", (void**)&actor2DData);
        if(volData)
            volume = volData->getVtkVolume();
        else if(actorData)
            actor = actorData->getVtkActor();
        else if(prop3DData)
            prop3D = prop3DData->getVtkProp3D();
        else if(actor2DData)
            actor2D = actor2DData->getVtkActor2D();

        if(actor)
            m_vtkRenderer->AddActor(actor);
        else if(volume)
            m_vtkRenderer->AddVolume(volume);
        else if(prop3D)
            m_vtkRenderer->AddActor(prop3D);
        else if(actor2D)
            m_vtkRenderer->AddActor2D(actor2D);
        else
            return false;

        if(m_RendererSceneGrid)
        {
            m_RendererSceneGrid->mActor->SetVisibility(false);
            m_vtkRenderer->ComputeVisiblePropBounds(m_RendererSceneGrid->mBBox);
            m_RendererSceneGrid->mActor->SetVisibility(true);
            m_RendererSceneGrid->UpdatePolyData();
        }

        return true;
    }

    if(path->pathName() == "Camera")
    {
        IVtkCameraIOData* cameraData = 0;
        bool success = inputData->queryInterface("IVtkCameraIOData", (void**)&cameraData);
        if(success && cameraData)
        {
            m_vtkRenderer->SetActiveCamera(cameraData->getVtkCamera());
            m_hasUserCamera = true;
            if(m_vtkRenderer->GetRenderWindow())
                m_vtkRenderer->GetRenderWindow()->Render();
            return true;
        }

        return false;
    }

    if(path->pathName() == "Importer")
    {
        if(!m_vtkRenderer->GetRenderWindow())
        {
            qWarning("Cannot connect to importer because there is no renderwindow associated with this renderer.");
            return false;
        }

        IVtkImporterIOData* importerData = 0;
        bool success = inputData->queryInterface("IVtkImporterIOData", (void**)&importerData);
        if(success && importerData)
        {
            vtkImporter* importer = importerData->getVtkImporter();
            importer->SetRenderWindow( m_vtkRenderer->GetRenderWindow() );
            importer->Update();
            return true;
        }

        return false;
    }

    return CVtkObject::setInput(path, inputData);
}

bool CVtkRenderer::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    if(path->pathName() == "Prop3D")
    {
        // Find out if the Prop3D is vtkVolume or vtkActor.
        IVtkVolumeIOData* volData = 0;
        IVtkActorIOData* actorData = 0;
        IVtkProp3DIOData* prop3DData = 0;
        IVtkActor2DIOData* actor2DData = 0;
        vtkVolume* volume = 0;
        vtkActor* actor = 0;
        vtkProp3D* prop3D = 0;
        vtkActor2D* actor2D = 0;
        inputData->queryInterface("IVtkVolumeIOData", (void**)&volData);
        inputData->queryInterface("IVtkActorIOData", (void**)&actorData);
        inputData->queryInterface("IVtkProp3DIOData", (void**)&prop3DData);
        inputData->queryInterface("IVtkActor2DIOData", (void**)&actor2DData);
        if(volData)
            volume = volData->getVtkVolume();
        else if(actorData)
            actor = actorData->getVtkActor();
        else if(prop3DData)
            prop3D = prop3DData->getVtkProp3D();
        else if(actor2DData)
            actor2D = actor2DData->getVtkActor2D();

        if(actor)
            m_vtkRenderer->RemoveActor(actor);
        else if(volume)
            m_vtkRenderer->RemoveVolume(volume);
        else if(prop3D)
            m_vtkRenderer->RemoveActor(prop3D);
        else if(actor2D)
            m_vtkRenderer->RemoveActor2D(actor2D);
        else
            return false;

        if(m_RendererSceneGrid)
        {
            m_RendererSceneGrid->mActor->SetVisibility(false);
            m_vtkRenderer->ComputeVisiblePropBounds(m_RendererSceneGrid->mBBox);
            m_RendererSceneGrid->mActor->SetVisibility(true);
            m_RendererSceneGrid->UpdatePolyData();
        }

        return true;
    }

    if(path->pathName() == "Camera")
    {
        IVtkCameraIOData* cameraData = 0;
        bool success = inputData->queryInterface("IVtkCameraIOData", (void**)&cameraData);
        if(success && cameraData && cameraData->getVtkCamera() == m_vtkRenderer->GetActiveCamera())
        {
            m_vtkRenderer->SetActiveCamera(0);
            m_hasUserCamera = false;
            if(m_vtkRenderer->GetRenderWindow())
                m_vtkRenderer->GetRenderWindow()->Render();
            return true;
        }

        return false;
    }

    if(path->pathName() == "Importer")
    {
        // None of the actors, lights, meshes previously imported by importer
        // are removed from this renderer
        return true; 
    }

    return CVtkObject::removeInput(path, inputData);
}

bool CVtkRenderer::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    if(path->pathName() == "Renderer")
    {
        if(m_vtkRendererOutput.getVtkRenderer())
            return false;

        m_vtkRendererOutput.setRenderer(m_vtkRenderer);
        *outputData = &m_vtkRendererOutput;
        return true;
    }

    return CVtkObject::fetchOutput(path, outputData);
}

bool CVtkRenderer::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
{
    Q_UNUSED(path);
    Q_UNUSED(outputData);
    m_vtkRenderer->SetRenderWindow(0);
    m_vtkRendererOutput.setRenderer(0);
    return true;
}

#define QtEvent(x) (QEvent::Type)(int(x) - int(vtkCommand::UserEvent))

void CVtkRenderer::handleVtkEvent(unsigned long event, void* callData)
{
    QEvent::Type qtEvent = QtEvent(event);
    switch(qtEvent)
    {
    case QEvent::MouseButtonPress: {
        QMouseEvent* me = static_cast<QMouseEvent*>(callData);
        Point3D pt(me->x(), me->y(), 0);
        pt = toSceneCoordinates(pt);
        emit OnMousePress(pt);
        }
        break;
    case QEvent::MouseMove: {
        QMouseEvent* me = static_cast<QMouseEvent*>(callData);
        Point3D pt(me->x(), me->y(), 0);
        pt = toSceneCoordinates(pt);
        emit OnMouseMove(pt);
        }
        break;
    case QEvent::MouseButtonRelease: {
        QMouseEvent* me = static_cast<QMouseEvent*>(callData);
        Point3D pt(me->x(), me->y(), 0);
        pt = toSceneCoordinates(pt);
        emit OnMouseRelease(pt);
        }
        break;
    case QEvent::MouseButtonDblClick: {
        QMouseEvent* me = static_cast<QMouseEvent*>(callData);
        Point3D pt(me->x(), me->y(), 0);
        pt = toSceneCoordinates(pt);
        emit OnMouseDoubleClick(pt);
        }
        break;
    case QEvent::KeyPress: {
        QKeyEvent* ke = static_cast<QKeyEvent*>(callData);
        emit OnKeyPress(ke->text());
        }
        break;
    case QEvent::KeyRelease: {
        QKeyEvent* ke = static_cast<QKeyEvent*>(callData);
        emit OnKeyRelease(ke->text());
        }
        break;
    default:
        CVtkObject::handleVtkEvent(event, callData);
    }
}

CVtkRendererGridScene::CVtkRendererGridScene() : mPolyData(0), mMapper(0), mActor(0) 
{ 
    mBBox[0] = -1.0f;
    mBBox[1] = 1.0f;
    mBBox[2] = -1.0f;
    mBBox[3] = 1.0f;
    mBBox[4] = -1.0f;
    mBBox[5] = 1.0f;
}

CVtkRendererGridScene::~CVtkRendererGridScene()
{
    mPolyData->Delete();
    mMapper->Delete();
    mActor->Delete();
}

void CVtkRendererGridScene::ConstructScene()
{
    mPolyData = vtkPolyData::New();
    mMapper = vtkPolyDataMapper::New();
    mActor = vtkActor::New();

    this->UpdatePolyData();

    mMapper->SetInput(mPolyData);
    mActor->SetMapper(mMapper);
    mActor->GetProperty()->SetRepresentationToWireframe();
}

void CVtkRendererGridScene::UpdatePolyData()
{    
    // Least grid size
    int grid_x = 50;
    int grid_y = 50;

    // Now create the grid
    static double bbox[] = { -1.0, 1.0, -1.0, 1.0, -1.0, 1.0 };

    double dx = (bbox[1]-bbox[0]) / double(grid_x);
    double dy = (bbox[3]-bbox[2]) / double(grid_y);
    double xMin = bbox[0];
    double yMin = bbox[2];
    double zVal = (bbox[4]+bbox[5])/2.0f;
    double sx = (mBBox[1]-mBBox[0])/(bbox[1]-bbox[0]);
    double sy = (mBBox[3]-mBBox[2])/(bbox[3]-bbox[2]);

    vtkPoints* points = vtkPoints::New();
    points->SetNumberOfPoints(grid_x*grid_y);
    for(int i=0; i<grid_x; i++)
    {
        for(int j=0; j<grid_y; j++)
        {
            double x = xMin + double(i)/double(grid_x) * 0.1;
            double y = yMin + double(j)/double(grid_y) * 0.1;
            double z = zVal;
            points->SetPoint(i*grid_x+j, x, y, z);
        }
    }
    mPolyData->SetPoints(points);
    points->Delete();

    vtkCellArray* cells = vtkCellArray::New();
    for(int i=0; i<grid_x-1; i++)
    {
        for(int j=0; j<grid_y-1; j++)
        {
            int base = i*grid_x + j; 
            int a = base;
            int b = base+1;
            int c = base+grid_x+1;
            int d = base+grid_x;

            vtkIdType tgl[4];
            tgl[0] = d; tgl[1] = c; tgl[2] = b; tgl[3] = a;
            cells->InsertNextCell(4, tgl);
        }
    }
    mPolyData->SetPolys(cells);
    cells->Delete();

    if(sx < 0)
        sx *= 1.0f;
    if(sy < 0)
        sy *= -1.0f;
    if(sx < 1.0f)
        sx = 1.0f;
    if(sy <= 1.0f)
        sy = 1.0;

    mActor->SetScale(sx, sy, 1.0f);
    printf("Scale = %.2f, %.2f\n", sx, sy);
}

void CVtkRendererGridScene::Execute(vtkObject *caller, unsigned long eventId, void *callData)
{
    Q_UNUSED(caller);
    Q_UNUSED(eventId);
    Q_UNUSED(callData);
}

