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

#include "CVtkMath.h"
#include "CVtkVisSystemComponent.h"
#include "vtkMatrix4x4.h"
#include "vtkTransform.h"
#include <QVector>

static CVtkMath* CVtkMathObjectPtr = 0;

inline void toArray(Vector3D vec, double arr[3])
{
    arr[0] = vec.x;
    arr[1] = vec.y;
    arr[2] = vec.z;
}

inline void toArray(Point3D point, double arr[3])
{
    arr[0] = point.x;
    arr[1] = point.y;
    arr[2] = point.z;
}

CVtkMath& CVtkMath::instance()
{
    if(!CVtkMathObjectPtr)
        new CVtkMath(&CVtkVisSystemComponent::instance());
    return *CVtkMathObjectPtr;
}

CVtkMath::CVtkMath(QObject* parent)
:QObject(parent)
{
    if(CVtkMathObjectPtr == 0)
        CVtkMathObjectPtr = this;
}

CVtkMath::~CVtkMath()
{
    if(CVtkMathObjectPtr == this)
        CVtkMathObjectPtr = 0;
}

Vector3D CVtkMath::cross(Vector3D v1, Vector3D v2)
{
    double v1_vec[3], v2_vec[3], v3_vec[3];
    toArray(v1, v1_vec);
    toArray(v2, v2_vec);
    vtkMath::Cross(v1_vec, v2_vec, v3_vec);
    return Vector3D(v3_vec);
}

Vector3D CVtkMath::normalize(Vector3D vec)
{
    double v[3];
    toArray(vec, v);
    vtkMath::Normalize(v);
    return v;
}

double CVtkMath::distanceBetweenPoints(Point3D p1, Point3D p2)
{
    double p1_vec[3], p2_vec[3];
    toArray(p1, p1_vec);
    toArray(p2, p2_vec);
    return sqrt( vtkMath::Distance2BetweenPoints(p1_vec, p2_vec) );
}

void CVtkMath::setRandomNumberSeed(double s)
{
    vtkMath::RandomSeed(s);
}

double CVtkMath::randomNumber()
{
    return vtkMath::Random();
}

double CVtkMath::randomNumber(double min, double max)
{
    return vtkMath::Random(min, max);
}

double CVtkMath::degreesToRadians()
{
    return vtkMath::RadiansFromDegrees(1.0);
}

double CVtkMath::radiansToDegrees()
{
    return vtkMath::DegreesFromRadians(1.0);
}

double CVtkMath::toDegrees(double rad)
{
    return rad*180.0/vtkMath::Pi();
}

double CVtkMath::toRadians(double deg)
{
    return deg*vtkMath::Pi()/180.0;
}

double CVtkMath::pi()
{
    return vtkMath::Pi();
}

double CVtkMath::vectorLength(Vector3D vec)
{
    double v[3];
    toArray(vec, v);
    return vtkMath::Norm(v);
}

double CVtkMath::dotProduct(Vector3D v1, Vector3D v2)
{
    double v1_vec[3], v2_vec[3];
    toArray(v1, v1_vec);
    toArray(v2, v2_vec);
    return vtkMath::Dot(v1_vec, v2_vec);
}

Vector3D CVtkMath::perpendicularVector(Vector3D vec)
{
    // A vector has no position. First lets take a random line that actually 
    // a direction vector vec.
    // Such a line would be
    //    (0, 0, 0) -> (vec.x, vec.y, vec.z)

    // Now lets pick one point in the line above p1(0, 0, 0)

    double leastDotProduct = 0;
    Point3D p;

    // Now lets tie an imaginary string about 'point' and rotate it about the x axis
    // at a radius of 1 units.
    double radius = 1.0;
    for(int i=0; i<360; i++)
    {
        Point3D p2;
        p2.x = 0.0;
        p2.y = radius * sin( double(i) * 3.1415 / 180.0 );
        p2.z = radius * cos( double(i) * 3.1415 / 180.0 );

        // Now find out if the vector p1->p2 is perpendicular to vec
        double dot_product = p2.x*vec.x + p2.y * vec.y + p2.z * vec.z;

        if(!i)
        {
            leastDotProduct = dot_product;
            p = p2;
        }
        else if(qAbs(dot_product) < qAbs(leastDotProduct))
        {
            leastDotProduct = qAbs(dot_product);
            p = p2;
        }
    }

    Vector3D retVec;
    double length = sqrt(p.x*p.x + p.y*p.y + p.z*p.z);
    retVec.x = p.x / length;
    retVec.y = p.y / length;
    retVec.z = p.z / length;

    return retVec;    
}

QObject* CVtkMath::newTransformationMatrix()
{
    return new CTransformationMatrix(this);
}

Point3D CVtkMath::pointDifference(Point3D a, Point3D b)
{
    return Point3D(a.x - b.x, a.y-b.y, a.z-b.z);
}

QList<double> CVtkMath::quatDifference(QList<double> quat1, QList<double> quat2)
{
    QList<double> ret;
    if(quat1.count() != 4 || quat2.count() != 4)
        return ret;

    QList<double> quat2i = this->quatInverse(quat2);
    return quatSum(quat2i, quat1);
}

Point3D CVtkMath::pointSum(Point3D a, Point3D b)
{
    return Point3D(a.x+b.x, a.y+b.y, a.z+b.z);
}

QList<double> CVtkMath::quatSum(QList<double> quat1, QList<double> quat2)
{
    QList<double> ret;
    if(quat1.count() != 4 || quat2.count() != 4)
        return ret;

    double s1 = quat1[0];
    double s2 = quat2[0];
    double v1[3];
    double v2[3];
    double cross[3];
    double res[4];

    for(int i=0; i<3; i++)
    {
        v1[i] = quat1[i+1];
        v2[i] = quat2[i+1];
    }

    vtkMath::Cross(v2, v1, cross);

    res[0] = s1*s2 - vtkMath::Dot(v1, v2);
    res[1] = s1*v2[0] + s2*v1[0] + cross[0];
    res[2] = s1*v2[1] + s2*v1[1] + cross[1];
    res[3] = s1*v2[2] + s2*v1[2] + cross[2];

    for(int i=0; i<4; i++)
        ret << res[i];
    ret = this->normalizeQuat(ret);

    return ret;
}

Point3D CVtkMath::pointInverse(Point3D pt)
{
    return Point3D(-pt.x, -pt.y, -pt.z);
}

QList<double> CVtkMath::quatInverse(QList<double> quat)
{
    QList<double> ret;
    if(quat.count() != 4)
        return ret;

    double res[4];
    for(int i=0; i<4; i++)
        res[i] = quat[i];

    double norm = res[0]*res[0] + res[1]*res[1] + res[2]*res[2] + res[3]*res[3];
    if(norm == 0)
    {
        res[0] = 0;
        res[1] = 0;
        res[2] = 0;
        res[3] = 0;
    }
    else
    {
        // basically (1/norm) * conjugate(res);
        double onebynorm = 1.0/norm;
        res[0] *= onebynorm;
        res[1] *= -onebynorm;
        res[2] *= -onebynorm;
        res[3] *= -onebynorm;
    }

    for(int i=0; i<4; i++)
        ret << res[i];

    return ret;
}

QList<double> CVtkMath::normalizeQuat(QList<double> quat)
{
    QList<double> ret;
    if(quat.count() != 4)
        return ret;

    double length = sqrt( quat[0]*quat[0] + quat[1]*quat[1] + quat[2]*quat[2] + quat[3]*quat[3] );
    ret << quat[0] / length;
    ret << quat[1] / length;
    ret << quat[2] / length;
    ret << quat[3] / length;

    return ret;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTransformationMatrix
////////////////////////////////////////////////////////////////////////////////////////////////////////

struct CTransformationMatrixData
{
    vtkTransform* transform;
};

CTransformationMatrix::CTransformationMatrix(QObject* parent)
:QObject(parent)
{
    d = new CTransformationMatrixData;
    d->transform = vtkTransform::New();
}

CTransformationMatrix::~CTransformationMatrix()
{
    d->transform->Delete();
    delete d;
}

void CTransformationMatrix::makeIdentity()
{
    d->transform->Identity();
}

void CTransformationMatrix::translate(double x, double y, double z)
{
    d->transform->Translate(x, y, z);
}

void CTransformationMatrix::rotateX(double angle)
{
    d->transform->RotateX(angle);
}

void CTransformationMatrix::rotateY(double angle)
{
    d->transform->RotateY(angle);
}

void CTransformationMatrix::rotateZ(double angle)
{
    d->transform->RotateZ(angle);
}

void CTransformationMatrix::rotate(QList<double> quat)
{
    d->transform->RotateWXYZ(quat[0], quat[1], quat[2], quat[3]);
}

void CTransformationMatrix::rotate(double w, double x, double y, double z)
{
    d->transform->RotateWXYZ(w, x, y, z);
}

void CTransformationMatrix::scale(double x, double y, double z)
{
    d->transform->Scale(x, y, z);
}

Point3D CTransformationMatrix::transform(Point3D input)
{
    double in[3], out[3];
    in[0] = input.x;
    in[1] = input.y;
    in[2] = input.z;

    d->transform->TransformPoint(in, out);
    return Point3D(out);
}

void CTransformationMatrix::concatenate(QList<double> elements)
{
    QVector<double> eVec = elements.toVector();
    d->transform->Concatenate(eVec.data());
}

void CTransformationMatrix::setMatrix(QList<double> matrix)
{
    QVector<double> eVec = matrix.toVector();
    d->transform->SetMatrix( eVec.data() );
}

QList<double> CTransformationMatrix::getMatrix()
{
    QList<double> ret;
    vtkMatrix4x4* matrix = d->transform->GetMatrix();
    for(int i=0; i<4; i++)
        for(int j=0; j<4; j++)
            ret << matrix->Element[i][j];
    return ret;
}



