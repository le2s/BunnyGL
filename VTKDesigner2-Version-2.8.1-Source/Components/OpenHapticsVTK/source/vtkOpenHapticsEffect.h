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

#ifndef VTK_OPEN_HAPTICS_EFFECT_H
#define VTK_OPEN_HAPTICS_EFFECT_H

#include "vtkObject.h"
#include "vtkCollection.h"

class vtkOpenHapticsRenderer;

class vtkOpenHapticsEffect : public vtkObject
{
public:
    static vtkOpenHapticsEffect* New();

    vtkSetMacro(EffectType, const char*);
    vtkGetMacro(EffectType, const char*);

    vtkGetMacro(EffectId, unsigned int);
    vtkGetMacro(Started, int);
    int IsValid();

    vtkSetMacro(Enabled, int);
    vtkGetMacro(Enabled, int);
    vtkBooleanMacro(Enabled, int);

    void Trigger() { TriggerFlag = 1; }

    vtkGetMacro(Gain, double);
    vtkSetMacro(Gain, double);

    vtkGetMacro(Magnitude, double);
    vtkSetMacro(Magnitude, double);

    vtkSetMacro(Frequency, double);
    vtkGetMacro(Frequency, double);

    vtkSetMacro(Duration, int);
    vtkGetMacro(Duration, int);

    vtkSetVector3Macro(Position, double);
    vtkGetVector3Macro(Position, double);

    vtkSetVector3Macro(Direction, double);
    vtkGetVector3Macro(Direction, double);

protected:
    vtkOpenHapticsEffect();
    virtual ~vtkOpenHapticsEffect();
    void Modified();

    void RenderEffect();
    void StartEffect();
    void StopEffect();
    virtual void SetupEffectParameters();

protected:
    unsigned int EffectId;
    const char* EffectType;
    int UpdateEffectFlag;
    int TriggerFlag;
    int Started;
    int Enabled;
    double Gain;
    double Magnitude;
    double Frequency;
    int    Duration;
    double Position[3];
    double Direction[3];

    friend class vtkOpenHapticsRenderer;
};

class vtkOpenHapticsEffectCollection : public vtkCollection
{
public:
    static vtkOpenHapticsEffectCollection* New();

    void AddItem(vtkOpenHapticsEffect* effect) {
        this->vtkCollection::AddItem((vtkObject*)effect);
    }

    void RemoveItem(vtkOpenHapticsEffect* effect) {
        this->vtkCollection::RemoveItem((vtkObject*)effect);
    }

    vtkOpenHapticsEffect* GetNextEffect() {
        return static_cast<vtkOpenHapticsEffect*>(this->GetNextItemAsObject());
    }

    int GetNumberOfEffects() {
        return this->GetNumberOfItems();
    }

protected:
    vtkOpenHapticsEffectCollection();
    ~vtkOpenHapticsEffectCollection();
};

#endif

