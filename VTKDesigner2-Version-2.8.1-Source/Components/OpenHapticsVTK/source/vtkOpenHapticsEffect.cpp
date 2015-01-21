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

#include "vtkOpenHapticsEffect.h"

#include "HL/hl.h"
#include "HD/hd.h"

///////////////////////////////////////////////////////////////////////////////
// vtkOpenHapticsEffect
///////////////////////////////////////////////////////////////////////////////

vtkOpenHapticsEffect* vtkOpenHapticsEffect::New()
{
    vtkOpenHapticsEffect* ret = new vtkOpenHapticsEffect;
    return ret;
}

int vtkOpenHapticsEffect::IsValid()
{
    return hlIsEffect(this->EffectId);
}

vtkOpenHapticsEffect::vtkOpenHapticsEffect()
{
    this->EffectId = hlGenEffects(1);

    this->EffectType = HL_EFFECT_CONSTANT;
    this->TriggerFlag = 0;
    this->UpdateEffectFlag = 0;
    this->Started = 0;
    this->Enabled = 0;
    this->Gain = 0.0;
    this->Magnitude = 1.0;
    this->Frequency = 0.0;
    this->Duration = 1000.0; // ms
    this->Position[0] = this->Position[1] = this->Position[2] = 0;
    this->Direction[0] = this->Direction[1] = this->Direction[2] = 0;
    this->Direction[1] = 1;
}

vtkOpenHapticsEffect::~vtkOpenHapticsEffect()
{
    hlDeleteEffects(this->EffectId, 1);
}

void vtkOpenHapticsEffect::Modified()
{
    this->UpdateEffectFlag = 1;
    vtkObject::Modified();
}

void vtkOpenHapticsEffect::RenderEffect()
{
    if(this->TriggerFlag == 1 && this->Started == 0)
    {
        this->SetupEffectParameters();
        hlTriggerEffect(this->GetEffectType());
        this->TriggerFlag = 0;
    }
    else
    {
        if(this->Enabled)
            this->StartEffect();
        else
            this->StopEffect();
    }
}

void vtkOpenHapticsEffect::StartEffect()
{
    if(this->Started == 1)
    {
        if(this->UpdateEffectFlag == 1)
        {
            this->UpdateEffectFlag = 0;
            this->SetupEffectParameters();
            hlUpdateEffect(this->EffectId);
        }

        return;
    }

    this->SetupEffectParameters();
    hlStartEffect(this->GetEffectType(), this->EffectId);
    this->Started = 1;
}

void vtkOpenHapticsEffect::StopEffect()
{
    if(this->Started == 0)
        return;

    hlStopEffect(this->EffectId);
    this->Started = 0;
}

void vtkOpenHapticsEffect::SetupEffectParameters()
{
    hlEffectd(HL_EFFECT_PROPERTY_GAIN, this->Gain);
    hlEffectd(HL_EFFECT_PROPERTY_MAGNITUDE, this->Magnitude);
    hlEffectd(HL_EFFECT_PROPERTY_FREQUENCY, this->Frequency);
    hlEffectdv(HL_EFFECT_PROPERTY_POSITION, this->Position);
    hlEffectdv(HL_EFFECT_PROPERTY_DIRECTION, this->Direction);

    if(this->TriggerFlag == 1)
        hlEffecti(HL_EFFECT_PROPERTY_DURATION, this->Duration);
}

///////////////////////////////////////////////////////////////////////////////
// vtkOpenHapticsEffectCollection
///////////////////////////////////////////////////////////////////////////////

vtkOpenHapticsEffectCollection* vtkOpenHapticsEffectCollection::New()
{
    vtkOpenHapticsEffectCollection* ret = new vtkOpenHapticsEffectCollection;
    return ret;
}

vtkOpenHapticsEffectCollection::vtkOpenHapticsEffectCollection()
{
    // Do nothing
}

vtkOpenHapticsEffectCollection::~vtkOpenHapticsEffectCollection()
{
    // Do nothing
}
