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

#ifndef C_VTK_OPEN_HAPTICS_ACTOR_H
#define C_VTK_OPEN_HAPTICS_ACTOR_H

#include "CGenericVisNodeBase.h"
#include "UserDefinedDataTypes.h"

struct CVtkOpenHapticsActorData;
class CVtkOpenHapticsActor : public CGenericVisNodeBase
{
    Q_OBJECT
    Q_ENUMS(ShapeRenderingType)
    Q_PROPERTY(QColor Color WRITE setColor READ color)
    Q_PROPERTY(double Opacity WRITE setOpacity READ opacity)
    Q_PROPERTY(Point3D Position WRITE setPosition READ position)
    Q_PROPERTY(Point3D Scale WRITE setScale READ scale)
    Q_PROPERTY(Point3D Orientation WRITE setOrientation READ orientation)
    Q_PROPERTY(bool EnableHapticRendering WRITE setEnableHapticRendering READ isEnableHapticRendering)
    Q_PROPERTY(bool MoveWithStylus WRITE setMoveWithStylus READ isMoveWithStylus)
    Q_PROPERTY(bool RotateWithStylus WRITE setRotateWithStylus READ isRotateWithStylus)
    Q_PROPERTY(bool UseHapticTransform WRITE setUseHapticTransform READ isUseHapticTransform)
    Q_PROPERTY(bool DynamicSurfaceChange READ isDynamicSurfaceChange WRITE setDynamicSurfaceChange)
	DECLARE_VIS_NODE

public:
    static CVtkOpenHapticsActor*& GetCurrentHapticsActor();
    static CVtkOpenHapticsActor* GetHapticsActorFor(unsigned id);

    CVtkOpenHapticsActor();
    ~CVtkOpenHapticsActor();

    QColor color() const;
    void setColor(QColor c);

    void setOpacity(double val);
    double opacity() const;

    Point3D position() const;
    void setPosition(Point3D pos);

    Point3D scale() const;
    void setScale(Point3D s);

    Point3D orientation() const;
    void setOrientation(Point3D o);

    void setEnableHapticRendering(bool val);
    bool isEnableHapticRendering() const;

    void setMoveWithStylus(bool val);
    bool isMoveWithStylus() const;

    void setRotateWithStylus(bool val);
    bool isRotateWithStylus() const;

    void setUseHapticTransform(bool val);
    bool isUseHapticTransform() const;

    void setDynamicSurfaceChange(bool val);
    bool isDynamicSurfaceChange() const;

    Q_INVOKABLE int openHapticsShapeID();
    Q_INVOKABLE bool isTouched();
    Q_INVOKABLE bool isOneButtonDown();
    Q_INVOKABLE bool isTwoButtonDown();

    Q_INVOKABLE void makeIdentity();
    Q_INVOKABLE void translate(Point3D dp);
    Q_INVOKABLE void scale(double x, double y, double z);
    Q_INVOKABLE void rotateX(double x);
    Q_INVOKABLE void rotateY(double y);
    Q_INVOKABLE void rotateZ(double z);
    Q_INVOKABLE void rotate(QList<double> quat);
    Q_INVOKABLE void rotate(double w, double x, double y, double z);
    Q_INVOKABLE QList<double> rotation();

signals: // haptic signals
    void OnHapticTouch();
    void OnHapticMotion();
    void OnHapticUnTouch();
    void OnHapticButton1Down();
    void OnHapticButton1Up();
    void OnHapticButton2Down();
    void OnHapticButton2Up();

protected:
    // IVisSystemNode implementation
    bool hasInput(IVisSystemNodeConnectionPath* path);
    bool setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData);
    bool removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData);
    bool fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData);
    bool outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData);
    void initializeNode();
    void finalizeNode();

private:
    friend struct CVtkOpenHapticsActorData;
    CVtkOpenHapticsActorData* d;
};

#endif

