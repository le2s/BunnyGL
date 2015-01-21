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

#include "CVtkOpenHapticsActor.h"
#include "COpenHapticsVTKComponent.h"
#include <QHash>

#include "vtkOpenHapticsActor.h"
#include "IVtkMapperIOData.h"
#include "IVtkTextureIOData.h"
#include "IVtkPropertyIOData.h"
#include "CVtkActorData.h"
#include "vtkProperty.h"
#include "vtkOpenHapticsProperty.h"
#include "IVtkOpenHapticsPropertyIOData.h"
#include "vtkCommand.h"
#include "vtkMatrix4x4.h"

#include "IVtkAbstractTransformIOData.h"
#include "vtkLinearTransform.h"

static QHash<unsigned, CVtkOpenHapticsActor*> CVtkOpenHapticsActorMap;
static CVtkOpenHapticsActor* CurrentHapticsActor = 0;

struct CVtkOpenHapticsActorData : public vtkCommand
{
    CVtkOpenHapticsActorData() : hasProperty(false) { }

    CVtkOpenHapticsActor* hapticsActorWrapper;
	CVtkActorData actorData;
    vtkOpenHapticsActor* hapticsActor;
    bool hasProperty;
    bool hasHapticProperty;

    void Execute(vtkObject *caller, unsigned long eventId, void *)
    {
        if(caller != hapticsActor)
            return;

        CurrentHapticsActor = hapticsActorWrapper;

        switch(eventId)
        {
        case vtkOpenHapticsActor::TouchEvent:
            emit hapticsActorWrapper->OnHapticTouch();
            break;
        case vtkOpenHapticsActor::UnTouchEvent:
            emit hapticsActorWrapper->OnHapticUnTouch();
            break;
        case vtkOpenHapticsActor::MotionEvent:
            emit hapticsActorWrapper->OnHapticMotion();
            break;
        case vtkOpenHapticsActor::Button1DownEvent:
            emit hapticsActorWrapper->OnHapticButton1Down();
            break;
        case vtkOpenHapticsActor::Button1UpEvent:
            emit hapticsActorWrapper->OnHapticButton1Up();
            break;
        case vtkOpenHapticsActor::Button2DownEvent:
            emit hapticsActorWrapper->OnHapticButton2Down();
            break;
        case vtkOpenHapticsActor::Button2UpEvent:
            emit hapticsActorWrapper->OnHapticButton2Up();
            break;
        }
    }
};

DEFINE_VIS_NODE(CVtkOpenHapticsActor, CGenericVisNodeBase)
{
	pDesc->setNodeClassCategory("Open Haptics + VTK");
	pDesc->setNodeClassName("vtkOpenHapticsActor");
	pDesc->setNodeClassDescription("Open Haptics Cursor Actor");
    pDesc->setNodeIcon( COpenHapticsVTKComponent::instance().nodeIcon() );

    // Mapper Input
    pDesc->addConnectionPath(
        new CGenericVisNodeConnectionPath("Mapper", IVisSystemNodeConnectionPath::InputPath, "vtkMapper", 0)
        );

    // Texture Input
    pDesc->addConnectionPath(
        new CGenericVisNodeConnectionPath("Texture", IVisSystemNodeConnectionPath::InputPath, "vtkTexture", 0)
        );

    // Property Input
    pDesc->addConnectionPath(
        new CGenericVisNodeConnectionPath("Property", IVisSystemNodeConnectionPath::InputPath, "vtkProperty", 0)
        );

    // Actor Output
    pDesc->addConnectionPath(
        new CGenericVisNodeConnectionPath("OpenHapticsActor", IVisSystemNodeConnectionPath::OutputPath, "vtkOpenHapticsActor", 0)
        );

    // Haptics Property
    pDesc->addConnectionPath(
        new CGenericVisNodeConnectionPath("HapticsProperty", IVisSystemNodeConnectionPath::InputPath, "vtkOpenHapticsProperty", 0)
        );

    // Transform Input
    pDesc->addConnectionPath(
        new CGenericVisNodeConnectionPath("LinearTransform", IVisSystemNodeConnectionPath::InputPath, "vtkLinearTransform", 0)
        );

    // Prop3D Output
    pDesc->addConnectionPath(
        new CGenericVisNodeConnectionPath("Prop3D", IVisSystemNodeConnectionPath::OutputPath, "vtkOpenHapticsActor", 0)
        );
}

CVtkOpenHapticsActor*& CVtkOpenHapticsActor::GetCurrentHapticsActor()
{
    return CurrentHapticsActor;
}

CVtkOpenHapticsActor* CVtkOpenHapticsActor::GetHapticsActorFor(unsigned id)
{
    if(CVtkOpenHapticsActorMap.contains(id))
        return CVtkOpenHapticsActorMap[id];
    return 0;
}

CVtkOpenHapticsActor::CVtkOpenHapticsActor()
{
    CVtkOpenHapticsActor::InitializeNodeDesc();
	d = new CVtkOpenHapticsActorData;
    d->hapticsActor = vtkOpenHapticsActor::New();
    d->hasHapticProperty = false;
    d->hasProperty = false;
    d->hapticsActorWrapper = this;
    d->hapticsActor->AddObserver(vtkCommand::AnyEvent, d);
    
    CVtkOpenHapticsActorMap[d->hapticsActor->GetOpenHapticsShapeID()] = this;
    nodeData()["vtkObject"] = qVariantFromValue<vtkObject*>(d->hapticsActor);
}

CVtkOpenHapticsActor::~CVtkOpenHapticsActor()
{
    if(d->hapticsActor)
    {
        CVtkOpenHapticsActorMap.remove(d->hapticsActor->GetOpenHapticsShapeID());
        d->hapticsActor->Delete();

        if(d->hapticsActorWrapper == CurrentHapticsActor)
            CurrentHapticsActor = 0;
    }
    delete d;
}

QColor CVtkOpenHapticsActor::color() const
{
    QColor retColor;
    double* color = d->hapticsActor->GetProperty()->GetColor();
    retColor.setRgbF(color[0], color[1], color[2]);
    return retColor;
}

void CVtkOpenHapticsActor::setColor(QColor c)
{
    d->hapticsActor->GetProperty()->SetColor(c.redF(), c.greenF(), c.blueF());
}

void CVtkOpenHapticsActor::setOpacity(double val)
{
    d->hapticsActor->GetProperty()->SetOpacity(val);
}

double CVtkOpenHapticsActor::opacity() const
{
    return d->hapticsActor->GetProperty()->GetOpacity();
}

Point3D CVtkOpenHapticsActor::position() const
{
    return Point3D(d->hapticsActor->GetPosition());
}

void CVtkOpenHapticsActor::setPosition(Point3D pos)
{
    d->hapticsActor->SetPosition(pos.x, pos.y, pos.z);
}

Point3D CVtkOpenHapticsActor::scale() const
{
    return Point3D(d->hapticsActor->GetScale());
}

void CVtkOpenHapticsActor::setScale(Point3D s)
{
    d->hapticsActor->SetScale(s.x, s.y, s.z);
}

Point3D CVtkOpenHapticsActor::orientation() const
{
    return Point3D(d->hapticsActor->GetOrientation());
}

void CVtkOpenHapticsActor::setOrientation(Point3D o)
{
    d->hapticsActor->SetOrientation(o.x, o.y, o.z);
}

void CVtkOpenHapticsActor::setEnableHapticRendering(bool val)
{
    d->hapticsActor->SetEnableHapticRendering(val);
}

bool CVtkOpenHapticsActor::isEnableHapticRendering() const
{
    return d->hapticsActor->GetEnableHapticRendering();
}

void CVtkOpenHapticsActor::setMoveWithStylus(bool val)
{
    d->hapticsActor->SetMoveWithStylus(val);
}

bool CVtkOpenHapticsActor::isMoveWithStylus() const
{
    return d->hapticsActor->GetMoveWithStylus();
}

void CVtkOpenHapticsActor::setRotateWithStylus(bool val)
{
    d->hapticsActor->SetRotateWithStylus(val);
}

bool CVtkOpenHapticsActor::isRotateWithStylus() const
{
    return d->hapticsActor->GetRotateWithStylus();
}

void CVtkOpenHapticsActor::setUseHapticTransform(bool val)
{
    d->hapticsActor->SetUseHapticTransform(val);
}

bool CVtkOpenHapticsActor::isUseHapticTransform() const
{
    return d->hapticsActor->GetUseHapticTransform();
}

void CVtkOpenHapticsActor::setDynamicSurfaceChange(bool val)
{
    d->hapticsActor->SetDynamicSurfaceChange(val);
}

bool CVtkOpenHapticsActor::isDynamicSurfaceChange() const
{
    return d->hapticsActor->GetDynamicSurfaceChange();
}

int CVtkOpenHapticsActor::openHapticsShapeID()
{
    return d->hapticsActor->GetOpenHapticsShapeID();
}

bool CVtkOpenHapticsActor::isTouched()
{
    return d->hapticsActor->GetTouched();
}

bool CVtkOpenHapticsActor::isOneButtonDown()
{
    return d->hapticsActor->GetButton1Down();
}

bool CVtkOpenHapticsActor::isTwoButtonDown()
{
    return d->hapticsActor->GetButton2Down();
}

void CVtkOpenHapticsActor::makeIdentity()
{
    d->hapticsActor->GetMatrix()->Identity();
}

void CVtkOpenHapticsActor::translate(Point3D dp)
{
    d->hapticsActor->AddPosition(dp.x, dp.y, dp.z);
}

void CVtkOpenHapticsActor::scale(double x, double y, double z)
{
    double scale[3];
    d->hapticsActor->GetScale(scale);
    d->hapticsActor->SetScale(scale[0]*x,scale[1]*y,scale[2]*z);
}

void CVtkOpenHapticsActor::rotateX(double x)
{
    d->hapticsActor->RotateX(x);
}

void CVtkOpenHapticsActor::rotateY(double y)
{
    d->hapticsActor->RotateY(y);
}

void CVtkOpenHapticsActor::rotateZ(double z)
{
    d->hapticsActor->RotateZ(z);
}

void CVtkOpenHapticsActor::rotate(QList<double> quat)
{
    if(quat.count() != 4)
        return;

    d->hapticsActor->RotateWXYZ(quat[0], quat[1], quat[2], quat[3]);
}

void CVtkOpenHapticsActor::rotate(double w, double x, double y, double z)
{
    d->hapticsActor->RotateWXYZ(w, x, y, z);
}

QList<double> CVtkOpenHapticsActor::rotation()
{
    QList<double> ret;
    for(int i=0; i<4; i++)
        ret << d->hapticsActor->GetOrientationWXYZ()[i];
    return ret;
}

bool CVtkOpenHapticsActor::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    if(path->pathName() == "Mapper")
    {
        return d->hapticsActor->GetMapper() != 0;
    }

    if(path->pathName() == "Texture")
    {
        return d->hapticsActor->GetTexture() != 0;
    }

    if(path->pathName() == "Property")
    {
        return d->hasProperty;
    }

    if(path->pathName() == "HapticsProperty")
    {
        return d->hasHapticProperty;
    }

    if(path->pathName() == "LinearTransform")
    {
        return d->hapticsActor->GetUserTransform();
    }


    return false;
}

bool CVtkOpenHapticsActor::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    if(path->pathName() == "Mapper")
    {
        IVtkMapperIOData* mapperData = 0;
        bool success = inputData->queryInterface("IVtkMapperIOData", (void**)&mapperData);
        if(success && mapperData)
        {
            d->hapticsActor->SetMapper(mapperData->getVtkMapper());
            return true;
        }

        return false;
    }

    if(path->pathName() == "Texture")
    {
        IVtkTextureIOData* texData = 0;
        bool success = inputData->queryInterface("IVtkTextureIOData", (void**)&texData);
        if(success && texData)
        {
            d->hapticsActor->SetTexture(texData->getVtkTexture());
            return true;
        }

        return false;
    }

    if(path->pathName() == "Property")
    {
        IVtkPropertyIOData* propData = 0;
        bool success = inputData->queryInterface("IVtkPropertyIOData", (void**)&propData);
        if(success && propData)
        {
            d->hapticsActor->SetProperty(propData->getVtkProperty());
            d->hasProperty = true;
            return true;
        }

        return false;
    }

    if(path->pathName() == "HapticsProperty")
    {
        IVtkOpenHapticsPropertyIOData* propData = 0;
        bool success = inputData->queryInterface("IVtkOpenHapticsPropertyIOData", (void**)&propData);
        if(success && propData)
        {
            d->hapticsActor->SetHapticsProperty(propData->getVtkOpenHapticsProperty());
            d->hasHapticProperty = true;
            return true;
        }

        return false;
    }

    if(path->pathName() == "LinearTransform")
    {
        IVtkAbstractTransformIOData* data = 0;
        bool success = inputData->queryInterface("IVtkAbstractTransformIOData", (void**)&data);
        if(success && data)
        {
            vtkLinearTransform* tx = vtkLinearTransform::SafeDownCast(data->getVtkAbstractTransform());
            if(tx)
            {
                d->hapticsActor->SetUserTransform(tx);
                return true;
            }
        }

        return false;
    }

    return false;
}

bool CVtkOpenHapticsActor::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    if(path->pathName() == "Mapper")
    {
        IVtkMapperIOData* mapperData = 0;
        bool success = inputData->queryInterface("IVtkMapperIOData", (void**)&mapperData);
        if(success && mapperData && mapperData->getVtkMapper() == d->hapticsActor->GetMapper())
        {
            d->hapticsActor->SetMapper(0);
            return true;
        }

        return false;
    }

    if(path->pathName() == "Texture")
    {
        IVtkTextureIOData* texData = 0;
        bool success = inputData->queryInterface("IVtkTextureIOData", (void**)&texData);
        if(success && texData && texData->getVtkTexture() == d->hapticsActor->GetTexture())
        {
            d->hapticsActor->SetTexture(0);
            return true;
        }

        return false;
    }

    if(path->pathName() == "Property")
    {
        IVtkPropertyIOData* propData = 0;
        bool success = inputData->queryInterface("IVtkPropertyIOData", (void**)&propData);
        if(success && propData && propData->getVtkProperty() == d->hapticsActor->GetProperty())
        {
            d->hapticsActor->SetProperty(d->hapticsActor->MakeProperty());
            d->hasProperty = false;
            return true;
        }

        return false;
    }

    if(path->pathName() == "HapticsProperty")
    {
        IVtkOpenHapticsPropertyIOData* propData = 0;
        bool success = inputData->queryInterface("IVtkOpenHapticsPropertyIOData", (void**)&propData);
        if(success && propData)
        {
            d->hapticsActor->SetHapticsProperty(0);
            d->hasHapticProperty = false;
            return true;
        }

        return false;
    }

    if(path->pathName() == "LinearTransform")
    {
        IVtkAbstractTransformIOData* data = 0;
        bool success = inputData->queryInterface("IVtkAbstractTransformIOData", (void**)&data);
        if(success && data)
        {
            vtkLinearTransform* tx = vtkLinearTransform::SafeDownCast(data->getVtkAbstractTransform());
            if(tx && d->hapticsActor->GetUserTransform() == tx)
            {
                d->hapticsActor->SetUserTransform(0);
                return true;
            }
        }

        return false;
    }

    return false;
}

bool CVtkOpenHapticsActor::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    if(path->pathName() == "OpenHapticsActor" || path->pathName() == "Prop3D")
    {
        d->actorData.setActor(d->hapticsActor);
        *outputData = &(d->actorData);
        return true;
    }

    return false;
}

bool CVtkOpenHapticsActor::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
{
    Q_UNUSED(path);
    Q_UNUSED(outputData);
    return true;
}

void CVtkOpenHapticsActor::initializeNode()
{
    CGenericVisNodeBase::initializeNode();
}

void CVtkOpenHapticsActor::finalizeNode()
{
    CVtkOpenHapticsActorMap.remove(d->hapticsActor->GetOpenHapticsShapeID());
    if(d->hapticsActorWrapper == CurrentHapticsActor)
        CurrentHapticsActor = 0;
    d->hapticsActor->Delete();
    d->hapticsActor = 0;
}



