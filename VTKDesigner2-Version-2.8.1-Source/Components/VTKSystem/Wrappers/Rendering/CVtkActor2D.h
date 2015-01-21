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

#ifndef C_VTK_ACTOR_2D_H
#define C_VTK_ACTOR_2D_H

#include "CVtkObject.h"
#include "vtkActor2D.h"
#include "CVtkActor2DData.h"
#include "vtkProperty2D.h"

class CVtkActor2D : public CVtkObject
{
    Q_OBJECT
    Q_PROPERTY(bool Visible READ isVisible WRITE setVisible)
    Q_PROPERTY(bool Pickable READ isPickable WRITE setPickable)
    Q_PROPERTY(bool Draggable READ isDraggable WRITE setDraggable)
    Q_PROPERTY(QColor Color READ color WRITE setColor)
    Q_PROPERTY(double Opacity READ opacity WRITE setOpacity)
    Q_PROPERTY(double PointSize READ pointSize WRITE setPointSize)
    Q_PROPERTY(double LineWidth READ lineWidth WRITE setLineWidth)
    Q_ENUMS(DisplayLocation)
    Q_PROPERTY(DisplayLocation DisplayLocation READ displayLocation WRITE setDisplayLocation)
    Q_PROPERTY(QList<QVariant> Position READ position WRITE setPosition)
    Q_PROPERTY(double X READ x WRITE setX)
    Q_PROPERTY(double Y READ y WRITE setY)
    Q_PROPERTY(double Width READ width WRITE setWidth)
    Q_PROPERTY(double Height READ height WRITE setHeight)
    DECLARE_VTK_OBJECT

public:
    CVtkActor2D(bool newVtkActor2D=true);
    ~CVtkActor2D();

    // vtkProp properties
    void setVisible(bool val);
    bool isVisible() const;

    void setPickable(bool val);
    bool isPickable() const;

    void setDraggable(bool val);
    bool isDraggable() const;

    // vtkActor2D stuff. (manipulation of vtkProperty2D)
    void setColor(QColor color);
    QColor color() const;

    void setOpacity(double val);
    double opacity() const;

    void setPointSize(double ps);
    double pointSize() const;

    void setLineWidth(double lw);
    double lineWidth() const;

    enum DisplayLocation
    {
        BackgroundLocation = VTK_BACKGROUND_LOCATION,
        ForegroundLocation = VTK_FOREGROUND_LOCATION
    };
    void setDisplayLocation(DisplayLocation loc);
    DisplayLocation displayLocation() const;

    // vtkActor2D Stuff
    void setLayerNumber(int val);
    int layerNumber() const;

    void setPosition(QList<QVariant> pos);
    QList<QVariant> position() const;
    Q_INVOKABLE void setPosition(double x, double y);

    void setX(double val);
    double x() const;

    void setY(double val);
    double y() const;

    void setWidth(double width);
    double width() const;

    void setHeight(double height);
    double height() const;
    Q_INVOKABLE void setRect(double x, double y, double width, double height);

protected:
    bool hasInput(IVisSystemNodeConnectionPath* path);
    bool setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData);
    bool removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData);
    bool fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData);
    bool outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData);

protected:
    vtkActor2D* m_vtkActor2D;
    CVtkActor2DData m_actor2DData;
};

#endif
