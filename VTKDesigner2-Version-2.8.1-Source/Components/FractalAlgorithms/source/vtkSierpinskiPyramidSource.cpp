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

#include "vtkSierpinskiPyramidSource.h"
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

struct Pyramid
{
    Pyramid() {
        base1 = Point(1, 0, 0);
        base2 = Point(0, 0, -1);
        base3 = Point(-1, 0, 0);
        base4 = Point(0, 0, 1);
        apex  = Point(0, 1.5, 0);
    }

    Pyramid(const Point& b1, const Point& b2, const Point& b3, const Point& b4, const Point& a)
            : base1(b1), base2(b2), base3(b3),base4(b4), apex(a) { }

    ~Pyramid() { }

public:
    Point base1;
    Point base2;
    Point base3;
    Point base4;
    Point apex;
};

// I am not using stl::list<> or stl::vector<> for this on purpose.
struct PyramidListData
{
    PyramidListData() : refCount(1) {
        PyramidArray = new Pyramid[100];
        size = 100;
        lastIndex = -1;
    }

    ~PyramidListData() {
        delete [] PyramidArray;
    }

    void ref() { ++refCount; }
    void deref() { --refCount; if(refCount <= 0) delete this; }

    void resize(int newSize) {
        if(newSize < size)
            return;

        Pyramid* newArray = new Pyramid[newSize];
        for(int i=0; i<size; i++)
            newArray[i] = PyramidArray[i];

        delete [] PyramidArray;
        PyramidArray = newArray;
        size = newSize;
    }

public:
    Pyramid* PyramidArray;
    int size;
    int lastIndex;

private:
    int refCount;
};

struct PyramidList
{
public:
    PyramidList() : d(new PyramidListData) { }
    PyramidList(const PyramidList& other) {
        d = other.d;
        d->ref();
    }
    ~PyramidList() { d->deref(); }

    PyramidList& operator = (const PyramidList& other) {
        d->deref();
        d = other.d;
        d->ref();

        return *this;
    }

    bool operator == (const PyramidList& other) {
        return d == other.d;
    }

    void push_back(const Pyramid& pyramid) {
        ++d->lastIndex;
        if(d->lastIndex >= d->size)
            d->resize(d->size+100);

        d->PyramidArray[d->lastIndex] = pyramid;
    }

    int size() const { return d->lastIndex+1; }

    Pyramid pyramid(int index) const {
        if(index < 0 || index >= size())
            return Pyramid();

        return d->PyramidArray[index];
    }

    PyramidList& operator += (const PyramidList& other) {
        for(int i=0; i<other.size(); i++)
            this->push_back( other.pyramid(i) );

        return *this;
    }

private:
    PyramidListData* d;
};

PyramidList DivideSierpinski(const Pyramid& pyramid, int iterations=0)
{
    PyramidList ret;

    if(iterations == 0)
    {
        ret.push_back( pyramid );
        return ret;
    }

    Point a = pyramid.base1.midPointTo(pyramid.base2);
    Point b = pyramid.base2.midPointTo(pyramid.base3);
    Point c = pyramid.base3.midPointTo(pyramid.base4);
    Point d = pyramid.base4.midPointTo(pyramid.base1);
    Point e = pyramid.apex.midPointTo(pyramid.base1);
    Point f = pyramid.apex.midPointTo(pyramid.base2);
    Point g = pyramid.apex.midPointTo(pyramid.base3);
    Point h = pyramid.apex.midPointTo(pyramid.base4);
    Point i = pyramid.base2.midPointTo(pyramid.base4);

    Pyramid pyramid1(pyramid.base1, a, i, d, e);
    Pyramid pyramid2(a, pyramid.base2, b, i, f);
    Pyramid pyramid3(i, b, pyramid.base3, c, g);
    Pyramid pyramid4(d, i, c, pyramid.base4, h);
    Pyramid pyramid5(e, f, g, h, pyramid.apex);

    if(iterations <= 1)
    {
        ret.push_back(pyramid1);
        ret.push_back(pyramid2);
        ret.push_back(pyramid3);
        ret.push_back(pyramid4);
        ret.push_back(pyramid5);

        return ret;
    }

    ret += DivideSierpinski(pyramid1, iterations-1);
    ret += DivideSierpinski(pyramid2, iterations-1);
    ret += DivideSierpinski(pyramid3, iterations-1);
    ret += DivideSierpinski(pyramid4, iterations-1);
    ret += DivideSierpinski(pyramid5, iterations-1);

    return ret;
}

///////////////////////////////////////////////////////////////////////////////
// vtkSierpinskiPyramidSource implementation
///////////////////////////////////////////////////////////////////////////////

vtkCxxRevisionMacro(vtkSierpinskiPyramidSource, "$Revision: 1.0 $");
vtkStandardNewMacro(vtkSierpinskiPyramidSource);

vtkSierpinskiPyramidSource::vtkSierpinskiPyramidSource()
{
    this->NumberOfIterations = 1;

    this->SetNumberOfInputPorts(0);
    this->SetNumberOfOutputPorts(1);
}

vtkSierpinskiPyramidSource::~vtkSierpinskiPyramidSource()
{

}

void vtkSierpinskiPyramidSource::PrintSelf(ostream& os, vtkIndent indent)
{
    vtkPolyDataAlgorithm::PrintSelf(os, indent);

    os << "NumberOfIterations: " << this->NumberOfIterations << endl;
}

int vtkSierpinskiPyramidSource::RequestData(
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

    // First divide a Pyramid this->NumberOfIterations number of times.
    PyramidList pyramids = DivideSierpinski(Pyramid(), this->NumberOfIterations);

    // Now construct a point array and Pyramid array
    static double bounds[] = {-1, 1, 0, 1.5, -1, 1};
    vtkPoints* points = vtkPoints::New();
    vtkCellArray* cells = vtkCellArray::New();
    vtkPointLocator* pointLoc = vtkPointLocator::New();
    pointLoc->InitPointInsertion(points, bounds);
    int size = pyramids.size();
    for(int i=0; i<size; i++)
    {
        Pyramid pyramid = pyramids.pyramid(i);
        vtkIdType abc[3];
        vtkIdType a, b, c, d, e;

        pointLoc->InsertUniquePoint((double*)(&pyramid.apex), a);
        pointLoc->InsertUniquePoint((double*)(&pyramid.base1), b);
        pointLoc->InsertUniquePoint((double*)(&pyramid.base2), c);
        pointLoc->InsertUniquePoint((double*)(&pyramid.base3), d);
        pointLoc->InsertUniquePoint((double*)(&pyramid.base4), e);

        abc[0] = a;
        abc[1] = b;
        abc[2] = c;
        cells->InsertNextCell(3, abc);

        abc[0] = a;
        abc[1] = c;
        abc[2] = d;
        cells->InsertNextCell(3, abc);

        abc[0] = a;
        abc[1] = d;
        abc[2] = e;
        cells->InsertNextCell(3, abc);

        abc[0] = a;
        abc[1] = e;
        abc[2] = b;
        cells->InsertNextCell(3, abc);

        abc[0] = b;
        abc[1] = c;
        abc[2] = d;
        cells->InsertNextCell(3, abc);

        abc[0] = b;
        abc[1] = d;
        abc[2] = e;
        cells->InsertNextCell(3, abc);
    }

    output->SetPoints(points);
    points->Delete();
    output->SetStrips(cells);
    cells->Delete();
    pointLoc->Delete();

    return 1;
}


