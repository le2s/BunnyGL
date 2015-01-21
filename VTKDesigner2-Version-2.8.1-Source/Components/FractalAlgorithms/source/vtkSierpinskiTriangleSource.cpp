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

/**
Authors:
    Prashanth N Udupa (prashanth@vcreatelogic.com)
*/

#include "vtkSierpinskiTriangleSource.h"
#include "vtkObjectFactory.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkPoints.h"
#include "vtkCellArray.h"
#include "vtkPointData.h"
#include "vtkDoubleArray.h"
#include "vtkPointLocator.h"
#include <math.h>

///////////////////////////////////////////////////////////////////////////////
// Helper structures and methods
///////////////////////////////////////////////////////////////////////////////

struct Point
{
    Point() : x(0), y(0), z(0) { }
    Point(const Point& other) : x(other.x), y(other.y), z(other.z) { }
    Point(double _x, double _y, double _z) : x(_x), y(_y), z(_z) { }
    Point(double* v) : x(v[0]), y(v[1]), z(v[2]) { }

    bool operator == (const Point& other) const { return (x==other.x) && (y==other.y) && (z==other.z); }
    Point& operator = (const Point& other) { x = other.x; y = other.y; z = other.z; return *this; }

    Point operator + (const Point& other) const { return Point(x+other.x, y+other.y, z+other.z); }
    Point& operator += (const Point& other) { x += other.x; y += other.y; z += other.z; return *this; }

    Point operator - (const Point& other) const { return Point(x-other.x, y-other.y, z-other.z); }
    Point& operator -= (const Point& other) { x -= other.x; y -= other.y; z -= other.z; return *this; }

    Point operator * (const Point& other) const { return Point(x*other.x, y*other.y, z*other.z); }
    Point& operator *= (const Point& other) { x *= other.x; y *= other.y; z *= other.z; return *this; }

    Point operator - () const { return Point(-x, -y, -z); }

    Point operator * (double s) { return Point(x*s, y*s, z*s); }

    const double* toArray() const { return (double*)(this); }
    double* toArray() { return (double*)(this); }

    Point midPointTo(const Point& other) const {
        return Point( (x+other.x)/2,
                      (y+other.y)/2,
                      (z+other.z)/2 );
    }

public:
    double x;
    double y;
    double z;
};

struct Triangle
{
    Triangle(const Triangle& other) {
        a = other.a;
        b = other.b;
        c = other.c;
    }

    Triangle(const Point &_a, const Point& _b, const Point& _c) {
        a = _a;
        b = _b;
        c = _c;
    }

    Triangle() {
        // We consider a simple triangle with 3 equal sides. Distance
        // from center of the triangle to any of the points is 1 units
        a = Point(0, 1, 0);
        b = Point(cos( (90+120)*3.1415/180.0 ),
                  sin( (90+120)*3.1415/180.0 ),
                  0 );
        c = Point(cos( (90+240)*3.1415/180.0 ),
                  sin( (90+240)*3.1415/180.0 ),
                  0 );
    }

    ~Triangle() { }

    Triangle& operator = (const Triangle& other) {
        a = other.a;
        b = other.b;
        c = other.c;

        return *this;
    }

    bool operator == (const Triangle& other) const {
        return (a==other.a) && (b==other.b) && (c==other.c);
    }

public:
    Point a;
    Point b;
    Point c;
};

// I am not using stl::list<> or stl::vector<> for this on purpose.
struct TriangleListData
{
    TriangleListData() : refCount(1) {
        triangleArray = new Triangle[100];
        size = 100;
        lastIndex = -1;
    }

    ~TriangleListData() {
        delete [] triangleArray;
    }

    void ref() { ++refCount; }
    void deref() { --refCount; if(refCount <= 0) delete this; }

    void resize(int newSize) {
        if(newSize < size)
            return;

        Triangle* newArray = new Triangle[newSize];
        for(int i=0; i<size; i++)
            newArray[i] = triangleArray[i];

        delete [] triangleArray;
        triangleArray = newArray;
        size = newSize;
    }

public:
    Triangle* triangleArray;
    int size;
    int lastIndex;

private:
    int refCount;
};

struct TriangleList
{
public:
    TriangleList() : d(new TriangleListData) { }
    TriangleList(const TriangleList& other) {
        d = other.d;
        d->ref();
    }
    ~TriangleList() { d->deref(); }

    TriangleList& operator = (const TriangleList& other) {
        d->deref();
        d = other.d;
        d->ref();

        return *this;
    }

    bool operator == (const TriangleList& other) {
        return d == other.d;
    }

    void push_back(const Triangle& tgl) {
        ++d->lastIndex;
        if(d->lastIndex >= d->size)
            d->resize(d->size+100);

        d->triangleArray[d->lastIndex] = tgl;
    }

    int size() const { return d->lastIndex+1; }

    Triangle triangle(int index) const {
        if(index < 0 || index >= size())
            return Triangle();

        return d->triangleArray[index];
    }

    TriangleList& operator += (const TriangleList& other) {
        for(int i=0; i<other.size(); i++)
            this->push_back( other.triangle(i) );

        return *this;
    }

private:
    TriangleListData* d;
};

TriangleList DivideSierpinski(const Triangle& tgl, int iterations=0)
{
    TriangleList ret;

    if(iterations == 0)
    {
        ret.push_back(tgl);
        return ret;
    }

    // Sierpinski this triangle into three parts.
    Point d = tgl.a.midPointTo(tgl.b);
    Point e = tgl.b.midPointTo(tgl.c);
    Point f = tgl.c.midPointTo(tgl.a);

    Triangle t1(tgl.a, d, f);
    Triangle t2(d, tgl.b, e);
    Triangle t3(f, e, tgl.c);

    // Should we divide more?
    if(iterations <= 1)
    {
        // Now we dont!
        ret.push_back(t1);
        ret.push_back(t2);
        ret.push_back(t3);

        return ret;
    }

    // Yes we do.
    ret += DivideSierpinski(t1, iterations-1);
    ret += DivideSierpinski(t2, iterations-1);
    ret += DivideSierpinski(t3, iterations-1);

    return ret;
}

///////////////////////////////////////////////////////////////////////////////
// vtkSierpinskiTriangleSource implementation
///////////////////////////////////////////////////////////////////////////////

vtkCxxRevisionMacro(vtkSierpinskiTriangleSource, "$Revision: 1.0 $");
vtkStandardNewMacro(vtkSierpinskiTriangleSource);

vtkSierpinskiTriangleSource::vtkSierpinskiTriangleSource()
{
    this->NumberOfIterations = 1;

    this->SetNumberOfInputPorts(0);
    this->SetNumberOfOutputPorts(1);
}

vtkSierpinskiTriangleSource::~vtkSierpinskiTriangleSource()
{

}

void vtkSierpinskiTriangleSource::PrintSelf(ostream& os, vtkIndent indent)
{
    vtkPolyDataAlgorithm::PrintSelf(os, indent);

    os << "NumberOfIterations: " << this->NumberOfIterations << endl;
}

int vtkSierpinskiTriangleSource::RequestData(
            vtkInformation *vtkNotUsed(request),
            vtkInformationVector **vtkNotUsed(inputVector),
            vtkInformationVector *outputVector
        )
{
    // Common stuff..
    vtkInformation *outputInfo = outputVector->GetInformationObject(0);
    if(!outputInfo)
        return 1;
    vtkPolyData *output = vtkPolyData::SafeDownCast(outputInfo->Get(vtkDataObject::DATA_OBJECT()));

    // First divide a triangle this->NumberOfIterations number of times.
    TriangleList triangles = DivideSierpinski(Triangle(), this->NumberOfIterations);

    // Now construct a point array and triangle array
    static double bounds[] = {-1, 1, 0, 1, -1, 1};
    vtkPointLocator* pointLoc = vtkPointLocator::New();
    vtkPoints* points = vtkPoints::New();
    vtkCellArray* cells = vtkCellArray::New();
    pointLoc->InitPointInsertion(points, bounds);
    int size = triangles.size();
    for(int i=0; i<size; i++)
    {
        Triangle tgl = triangles.triangle(i);

        vtkIdType abc[3];
        pointLoc->InsertUniquePoint(tgl.a.toArray(), abc[0]);
        pointLoc->InsertUniquePoint(tgl.b.toArray(), abc[1]);
        pointLoc->InsertUniquePoint(tgl.c.toArray(), abc[2]);

        cells->InsertNextCell(3, abc);
    }

    output->SetPoints(points);
    points->Delete();
    output->SetStrips(cells);
    cells->Delete();
    pointLoc->Delete();

    return 1;
}


