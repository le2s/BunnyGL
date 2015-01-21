/****************************************************************************
**
** Copyright (C) VCreate Logic Private Limited, Bangalore
**
** Use of this file is limited according to the terms specified by
** VCreate Logic Private Limited, Bangalore.  Details of those terms
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

#ifndef C_VTK_MATH_H
#define C_VTK_MATH_H

#include <QObject>
#include "vtkMath.h"
#include "UserDefinedDataTypes.h"

class CVtkMath : public QObject
{
    Q_OBJECT

public:
    static CVtkMath& instance();

    CVtkMath(QObject* parent);
    ~CVtkMath();

    Q_INVOKABLE Vector3D cross(Vector3D v1, Vector3D v2);
    Q_INVOKABLE Vector3D normalize(Vector3D vec);
    Q_INVOKABLE double     distanceBetweenPoints(Point3D p1, Point3D p2);
    Q_INVOKABLE void    setRandomNumberSeed(double s);
    Q_INVOKABLE double    randomNumber();
    Q_INVOKABLE double    randomNumber(double min, double max);
    Q_INVOKABLE double    degreesToRadians();
    Q_INVOKABLE double    radiansToDegrees();
    Q_INVOKABLE double    toDegrees(double rad);
    Q_INVOKABLE double    toRadians(double deg);
    Q_INVOKABLE double    pi();
    Q_INVOKABLE double    vectorLength(Vector3D vec);
    Q_INVOKABLE double    dotProduct(Vector3D v1, Vector3D v2);
    Q_INVOKABLE Vector3D    perpendicularVector(Vector3D vec);
    Q_INVOKABLE QObject* newTransformationMatrix();

    Q_INVOKABLE Point3D pointDifference(Point3D a, Point3D b);
    Q_INVOKABLE QList<double> quatDifference(QList<double> quat1, QList<double> quat2);
    Q_INVOKABLE Point3D pointSum(Point3D a, Point3D b);
    Q_INVOKABLE QList<double> quatSum(QList<double> quat1, QList<double> quat2);
    Q_INVOKABLE Point3D pointInverse(Point3D pt);
    Q_INVOKABLE QList<double> quatInverse(QList<double> quat);
    Q_INVOKABLE QList<double> normalizeQuat(QList<double> quat);
};

struct CTransformationMatrixData;
class CTransformationMatrix : public QObject
{
    Q_OBJECT

public:
    CTransformationMatrix(QObject* parent=0);
    ~CTransformationMatrix();

    Q_INVOKABLE void makeIdentity();
    Q_INVOKABLE void translate(double x, double y, double z);
    Q_INVOKABLE void rotateX(double angle);
    Q_INVOKABLE void rotateY(double angle);
    Q_INVOKABLE void rotateZ(double angle);
    Q_INVOKABLE void rotate(QList<double> quat);
    Q_INVOKABLE void rotate(double w, double x, double y, double z);
    Q_INVOKABLE void scale(double x, double y, double z);
    Q_INVOKABLE Point3D transform(Point3D input);
    Q_INVOKABLE void concatenate(QList<double> elements);
    Q_INVOKABLE void setMatrix(QList<double> matrix);
    Q_INVOKABLE QList<double> getMatrix();

private:
    CTransformationMatrixData* d;
};

#endif

