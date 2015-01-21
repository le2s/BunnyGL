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

#include "CVtkOpenHapticsEffect.h"
#include "COpenHapticsVTKComponent.h"
#include "vtkOpenHapticsEffect.h"
#include "CVtkOpenHapticsEffectIOData.h"

#include "HL/hl.h"
#include "HD/hd.h"

DEFINE_VIS_NODE(CVtkOpenHapticsEffect, CGenericVisNodeBase)
{
	pDesc->setNodeClassCategory("Open Haptics + VTK");
	pDesc->setNodeClassName("vtkOpenHapticsEffect");
	pDesc->setNodeClassDescription("HLAPI Effect");
    pDesc->setNodeIcon( COpenHapticsVTKComponent::instance().nodeIcon() );

    // Effect Output
    pDesc->addConnectionPath(
        new CGenericVisNodeConnectionPath("Effect", IVisSystemNodeConnectionPath::OutputPath, "vtkOpenHapticsEffect", 0, false)
        );
}

struct CVtkOpenHapticsEffectData
{
    vtkOpenHapticsEffect* effect;
    CVtkOpenHapticsEffectIOData ioData;
};

CVtkOpenHapticsEffect::CVtkOpenHapticsEffect()
{
    d = new CVtkOpenHapticsEffectData;
    d->effect = vtkOpenHapticsEffect::New();
    d->ioData.setObject(d->effect);
}

CVtkOpenHapticsEffect::~CVtkOpenHapticsEffect()
{
    d->ioData.setObject(0);

    if(d->effect)
        d->effect->Delete();

    delete d;
}

void CVtkOpenHapticsEffect::setEffectType(CVtkOpenHapticsEffect::EffectType type)
{
    if(type == this->effectType())
        return;

    switch(type)
    {
    case ConstantForceEffect:
        d->effect->SetEffectType(HL_EFFECT_CONSTANT);
        break;
    case SpringEffect:
        d->effect->SetEffectType(HL_EFFECT_SPRING);
        break;
    case ViscousEffect:
        d->effect->SetEffectType(HL_EFFECT_VISCOUS);
        break;
    case FrictionEffect:
        d->effect->SetEffectType(HL_EFFECT_FRICTION);
        break;
    case CustomEffect:
        qDebug("Custom effects are not yet supported!");
        break;
    }
}

CVtkOpenHapticsEffect::EffectType CVtkOpenHapticsEffect::effectType() const
{
    HLenum type = d->effect->GetEffectType();

    if(type == HL_EFFECT_CONSTANT)
        return ConstantForceEffect;

    if(type == HL_EFFECT_SPRING)
        return SpringEffect;

    if(type == HL_EFFECT_VISCOUS)
        return ViscousEffect;

    if(type == HL_EFFECT_FRICTION)
        return FrictionEffect;

    if(type == HL_EFFECT_CALLBACK)
        return CustomEffect;

    return CustomEffect;
}

bool CVtkOpenHapticsEffect::isStarted() const
{
    return d->effect->GetStarted();
}

void CVtkOpenHapticsEffect::setEnabled(bool val)
{
    d->effect->SetEnabled(val);
}

bool CVtkOpenHapticsEffect::isEnabled() const
{
    return d->effect->GetEnabled();
}

void CVtkOpenHapticsEffect::setGain(double g)
{
    d->effect->SetGain(g);
}

double CVtkOpenHapticsEffect::gain() const
{
    return d->effect->GetGain();
}

void CVtkOpenHapticsEffect::setFrequency(double f)
{
    d->effect->SetFrequency(f);
}

double CVtkOpenHapticsEffect::frequency() const
{
    return d->effect->GetFrequency();
}

void CVtkOpenHapticsEffect::setMagnitude(double m)
{
    d->effect->SetMagnitude(m);
}

double CVtkOpenHapticsEffect::magnitude() const
{
    return d->effect->GetMagnitude();
}

void CVtkOpenHapticsEffect::setDuration(int dur)
{
    d->effect->SetDuration(dur);
}

int CVtkOpenHapticsEffect::duration() const
{
    return d->effect->GetDuration();
}

void CVtkOpenHapticsEffect::setPosition(double x, double y, double z)
{
    d->effect->SetPosition(x, y, z);
}

void CVtkOpenHapticsEffect::setPosition(Point3D point)
{
    d->effect->SetPosition(point.x, point.y, point.z);
}

Point3D CVtkOpenHapticsEffect::position() const
{
    return Point3D(d->effect->GetPosition());
}

void CVtkOpenHapticsEffect::setDirection(double x, double y, double z)
{
    d->effect->SetDirection(x, y, z);
}

void CVtkOpenHapticsEffect::setDirection(Vector3D vec)
{
    d->effect->SetDirection(vec.x, vec.y, vec.z);
}

Vector3D CVtkOpenHapticsEffect::direction() const
{
    return Vector3D(d->effect->GetDirection());
}

void CVtkOpenHapticsEffect::trigger()
{
    d->effect->Trigger();
}

void CVtkOpenHapticsEffect::command_Trigger()
{
    d->effect->Trigger();
}

bool CVtkOpenHapticsEffect::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    if(path->pathName() == "Effect")
    {
        *outputData = &(d->ioData);
        return true;
    }

    return false;
}

bool CVtkOpenHapticsEffect::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
{
    if(!path || !outputData)
        return false;

    if(path->pathName() == "Effect")
        return true;

    return false;
}


