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

#ifndef C_VTK_OPEN_HAPTICS_EFFECT_H
#define C_VTK_OPEN_HAPTICS_EFFECT_H

#include "CGenericVisNodeBase.h"
#include "UserDefinedDataTypes.h"

struct CVtkOpenHapticsEffectData;
class CVtkOpenHapticsEffect : public CGenericVisNodeBase
{
    Q_OBJECT
    Q_ENUMS(EffectType)
    Q_PROPERTY(EffectType EffectType READ effectType WRITE setEffectType)
    Q_PROPERTY(bool Enabled READ isEnabled WRITE setEnabled)
    Q_PROPERTY(double Gain READ gain WRITE setGain)
    Q_PROPERTY(double Magnitude READ magnitude WRITE setMagnitude)
    Q_PROPERTY(double Frequency READ frequency WRITE setFrequency)
    Q_PROPERTY(int Duration READ duration WRITE setDuration)
    Q_PROPERTY(Point3D Position READ position WRITE setPosition)
    Q_PROPERTY(Vector3D Direction READ direction WRITE setDirection)
    
    DECLARE_VIS_NODE

public:
    enum EffectType
    {
        ConstantForceEffect,
        SpringEffect,
        ViscousEffect,
        FrictionEffect,
        CustomEffect /* not supported yet */
    };

    CVtkOpenHapticsEffect();
    ~CVtkOpenHapticsEffect();

    void setEffectType(EffectType type);
    EffectType effectType() const;

    Q_INVOKABLE bool isStarted() const;

    void setEnabled(bool val);
    bool isEnabled() const;

    void setGain(double g);
    double gain() const;

    void setFrequency(double f);
    double frequency() const;

    void setMagnitude(double m);
    double magnitude() const;

    void setDuration(int dur);
    int duration() const;

    Q_INVOKABLE void setPosition(double x, double y, double z);
    void setPosition(Point3D point);
    Point3D position() const;

    Q_INVOKABLE void setDirection(double x, double y, double z);
    void setDirection(Vector3D vec);
    Vector3D direction() const;

    Q_INVOKABLE void trigger();

protected slots:
    void command_Trigger();

protected:
    bool fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData);
    bool outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData);

private:
    CVtkOpenHapticsEffectData* d;
};

#endif
