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

#ifndef C_VTK_PROP_3D_H
#define C_VTK_PROP_3D_H

#include "CVtkObject.h"
#include "vtkProp3D.h"
#include "CVtkProp3DData.h"
#include "UserDefinedDataTypes.h"

class CVtkProp3D : public CVtkObject
{
    Q_OBJECT
    Q_PROPERTY(Point3D Position WRITE setPosition READ position)
    Q_PROPERTY(Point3D Scale WRITE setScale READ scale)
    Q_PROPERTY(Point3D Orientation WRITE setOrientation READ orientation)
    Q_PROPERTY(bool Visible READ isVisible WRITE setVisible)
    Q_PROPERTY(bool Pickable READ isPickable WRITE setPickable)
    Q_PROPERTY(bool Dragable READ isDragable WRITE setDragable)
    DECLARE_VTK_OBJECT

public:
    CVtkProp3D();
    ~CVtkProp3D();

    // These things affect the actual matrix
    Point3D position() const;
    void setPosition(Point3D pos);

    Point3D scale() const;
    void setScale(Point3D s);

    Point3D orientation() const;
    void setOrientation(Point3D o);

    void setVisible(bool val);
    bool isVisible() const;

    void setPickable(bool val);
    bool isPickable() const;

    void setDragable(bool val);
    bool isDragable() const;

    // These things affect only the user matrix
    Q_INVOKABLE void makeIdentity();
    Q_INVOKABLE void translate(const Point3D& dp);
    Q_INVOKABLE void translate(double x, double y, double z) { translate(Point3D(x,y,z)); }
    Q_INVOKABLE void scale(double x, double y, double z);
    Q_INVOKABLE void rotateX(double x);
    Q_INVOKABLE void rotateY(double y);
    Q_INVOKABLE void rotateZ(double z);
    Q_INVOKABLE void rotate(QList<double> quat);
    Q_INVOKABLE void rotate(double w, double x, double y, double z);
    Q_INVOKABLE QList<double> rotation();

    // Independent
    Q_INVOKABLE Point3D getCenter();
    Q_INVOKABLE QList<double> getXRange();
    Q_INVOKABLE QList<double> getYRange();
    Q_INVOKABLE QList<double> getZRange();
    Q_INVOKABLE double        getXLength();
    Q_INVOKABLE double        getYLength();
    Q_INVOKABLE double        getZLength();
    Q_INVOKABLE double        getDiagonalLength();
    Q_INVOKABLE QList<double> getBounds() const;

    // Colission Detection
    Q_INVOKABLE bool collidesWith(QObject* anotherProp3D) const;

protected:
    bool hasInput(IVisSystemNodeConnectionPath* path);
    bool setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData);
    bool removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData);
    bool fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData);
    bool outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData);

    vtkTransform* userTransform();

private:
    vtkProp3D* m_vtkProp3D;
    CVtkProp3DData m_prop3DData;
};

#endif
