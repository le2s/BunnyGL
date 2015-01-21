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

#include "CVtkOpenHapticsCursorActor.h"
#include "COpenHapticsVTKComponent.h"
#include "vtkOpenHapticsCursorActor.h"
#include "IVtkMapperIOData.h"
#include "IVtkTextureIOData.h"
#include "IVtkPropertyIOData.h"
#include "CVtkActorData.h"
#include "vtkProperty.h"

struct CVtkOpenHapticsCursorActorData
{
    CVtkOpenHapticsCursorActorData() : hasProperty(false) { }

	CVtkActorData actorData;
    vtkOpenHapticsCursorActor* hapticsActor;
    bool hasProperty;
};

DEFINE_VIS_NODE(CVtkOpenHapticsCursorActor, CGenericVisNodeBase)
{
	pDesc->setNodeClassCategory("Open Haptics + VTK");
	pDesc->setNodeClassName("vtkOpenHapticsCursorActor");
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
        new CGenericVisNodeConnectionPath("OpenHapticsActor", IVisSystemNodeConnectionPath::OutputPath, "vtkOpenHapticsCursorActor", 0)
        );
}

CVtkOpenHapticsCursorActor::CVtkOpenHapticsCursorActor()
{
    CVtkOpenHapticsCursorActor::InitializeNodeDesc();
	d = new CVtkOpenHapticsCursorActorData;
    d->hapticsActor = vtkOpenHapticsCursorActor::New();
    nodeData()["vtkObject"] = qVariantFromValue<vtkObject*>(d->hapticsActor);
}

CVtkOpenHapticsCursorActor::~CVtkOpenHapticsCursorActor()
{
    if(d->hapticsActor)
        d->hapticsActor->Delete();
    delete d;
}

QColor CVtkOpenHapticsCursorActor::color() const
{
    QColor retColor;
    double* color = d->hapticsActor->GetProperty()->GetColor();
    retColor.setRgbF(color[0], color[1], color[2]);
    return retColor;
}

void CVtkOpenHapticsCursorActor::setColor(QColor c)
{
    d->hapticsActor->GetProperty()->SetColor(c.redF(), c.greenF(), c.blueF());
}

void CVtkOpenHapticsCursorActor::setOpacity(double val)
{
    d->hapticsActor->GetProperty()->SetOpacity(val);
}

double CVtkOpenHapticsCursorActor::opacity() const
{
    return d->hapticsActor->GetProperty()->GetOpacity();
}

bool CVtkOpenHapticsCursorActor::hasInput(IVisSystemNodeConnectionPath* path)
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

    return false;
}

bool CVtkOpenHapticsCursorActor::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
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

    return false;
}

bool CVtkOpenHapticsCursorActor::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
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

    return false;
}

bool CVtkOpenHapticsCursorActor::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    if(path->pathName() == "OpenHapticsActor")
    {
        d->actorData.setActor(d->hapticsActor);
        *outputData = &(d->actorData);
        return true;
    }

    return false;
}

bool CVtkOpenHapticsCursorActor::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
{
    Q_UNUSED(path);
    Q_UNUSED(outputData);
    return true;
}

void CVtkOpenHapticsCursorActor::initializeNode()
{
    CGenericVisNodeBase::initializeNode();
}

void CVtkOpenHapticsCursorActor::finalizeNode()
{
    d->hapticsActor->Delete();
    d->hapticsActor = 0;
}



