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

// Added from the VTK Designer 2 Project

#include "vtkFractalTerrainSource.h"
#include "vtkObjectFactory.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkPoints.h"
#include "vtkCellArray.h"
#include "vtkPointData.h"
#include "vtkDoubleArray.h"
#include "vtkPointLocator.h"

#include <math.h>
#include <stdlib.h>

static const double MaxHeight = 0.1;

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

struct Square
{
    Square(const Square& other) {
        a = other.a;
        b = other.b;
        c = other.c;
        d = other.d;
    }

    Square(const Point &_a, const Point& _b, const Point& _c, const Point& _d) {
        a = _a;
        b = _b;
        c = _c;
        d = _d;
    }

    Square() {
        a = Point(0.5, 0.5, 0);
        b = Point(-0.5, 0.5, 0);
        c = Point(-0.5, -0.5, 0);
        d = Point(0.5, -0.5, 0);
    }

    ~Square() { }

    Square& operator = (const Square& other) {
        a = other.a;
        b = other.b;
        c = other.c;
        d = other.d;

        return *this;
    }

    bool operator == (const Square& other) const {
        return (a==other.a) && (b==other.b) && (c==other.c) && (d==other.d);
    }

    Point midPoint() const {
        return Point( (a.x+b.x+c.x+d.x)/4,
                      (a.y+b.y+c.y+d.y)/4,
                      (a.z+b.z+c.z+d.z)/4 );
    }

public:
    Point a;
    Point b;
    Point c;
    Point d;
};

// I am not using stl::list<> or stl::vector<> for this on purpose.
struct SquareListData
{
    SquareListData() : refCount(1) {
        squareArray = new Square[100];
        size = 100;
        lastIndex = -1;
    }

    ~SquareListData() {
        delete [] squareArray;
    }

    void ref() { ++refCount; }
    void deref() { --refCount; if(refCount <= 0) delete this; }

    void resize(int newSize) {
        if(newSize < size)
            return;

        Square* newArray = new Square[newSize];
        for(int i=0; i<size; i++)
            newArray[i] = squareArray[i];

        delete [] squareArray;
        squareArray = newArray;
        size = newSize;
    }

public:
    Square* squareArray;
    int size;
    int lastIndex;

private:
    int refCount;
};

struct SquareList
{
public:
    SquareList() : d(new SquareListData) { }
    SquareList(const SquareList& other) {
        d = other.d;
        d->ref();
    }
    ~SquareList() { d->deref(); }

    SquareList& operator = (const SquareList& other) {
        d->deref();
        d = other.d;
        d->ref();

        return *this;
    }

    bool operator == (const SquareList& other) {
        return d == other.d;
    }

    void push_back(const Square& sqr) {
        ++d->lastIndex;
        if(d->lastIndex >= d->size)
            d->resize(d->size+100);

        d->squareArray[d->lastIndex] = sqr;
    }

    int size() const { return d->lastIndex+1; }

    Square square(int index) const {
        if(index < 0 || index >= size())
            return Square();

        return d->squareArray[index];
    }

    SquareList& operator += (const SquareList& other) {
        for(int i=0; i<other.size(); i++)
            this->push_back( other.square(i) );

        return *this;
    }

private:
    SquareListData* d;
};

double RandomNumber(double min, double max)
{
    double rand_max = double(RAND_MAX);
    double r = double( rand() );
    double value = (max - min) / (rand_max) * r + min;
    return value;
}

SquareList FractalTerrainDivide(const Square& sqr, int iterations, double step=MaxHeight)
{
    SquareList ret;

    if(iterations == 0)
    {
        ret.push_back(sqr);
        return ret;
    }

    // Find out the pid point of the square
    Point midPt = sqr.midPoint();

    // Find out mid point along each of the edges
    Point e = sqr.a.midPointTo(sqr.b);
    Point f = sqr.b.midPointTo(sqr.c);
    Point g = sqr.c.midPointTo(sqr.d);
    Point h = sqr.d.midPointTo(sqr.a);

    // Offset the mid point by a random value about the Z axis
    midPt.z += RandomNumber(-step, step);

    // Divide into 4 squares
    Square s1(sqr.a, e, midPt, h);
    Square s2(e, sqr.b, f, midPt);
    Square s3(midPt, f, sqr.c, g);
    Square s4(h, midPt, g, sqr.d);

    // Should we divide more?
    if(iterations <= 1)
    {
        // No we dont!
        ret.push_back( s1 );
        ret.push_back( s2 );
        ret.push_back( s3 );
        ret.push_back( s4 );

        return ret;
    }

    // Yes we do.
    ret += FractalTerrainDivide(s1, iterations-1, step/2);
    ret += FractalTerrainDivide(s2, iterations-1, step/2);
    ret += FractalTerrainDivide(s3, iterations-1, step/2);
    ret += FractalTerrainDivide(s4, iterations-1, step/2);

    return ret;
}

///////////////////////////////////////////////////////////////////////////////
// vtkFractalTerrainSource implementation
///////////////////////////////////////////////////////////////////////////////

vtkCxxRevisionMacro(vtkFractalTerrainSource, "$Revision: 1.0 $");
vtkStandardNewMacro(vtkFractalTerrainSource);

vtkFractalTerrainSource::vtkFractalTerrainSource()
{
    this->NumberOfIterations = 1;

    this->SetNumberOfInputPorts(0);
    this->SetNumberOfOutputPorts(1);
}

vtkFractalTerrainSource::~vtkFractalTerrainSource()
{

}

void vtkFractalTerrainSource::PrintSelf(ostream& os, vtkIndent indent)
{
    vtkPolyDataAlgorithm::PrintSelf(os, indent);

    os << "NumberOfIterations: " << this->NumberOfIterations << endl;
}

int vtkFractalTerrainSource::RequestData(
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

    // First divide a square this->NumberOfIterations number of times.
    SquareList squares = FractalTerrainDivide(Square(), this->NumberOfIterations);

    // Now construct a point array and square array
    static double bounds[] = {-0.5, 0.5, -0.5, 0.5, -MaxHeight, MaxHeight};
    vtkPoints* points = vtkPoints::New();
    vtkCellArray* cells = vtkCellArray::New();
    vtkPointLocator* pointLoc = vtkPointLocator::New();
    pointLoc->InitPointInsertion(points, bounds);
    int size = squares.size();
    for(int i=0; i<size; i++)
    {
        Square sqr = squares.square(i);
        vtkIdType abc[3];
        vtkIdType a, b, c, d;

        pointLoc->InsertUniquePoint((double*)(&sqr.a), a);
        pointLoc->InsertUniquePoint((double*)(&sqr.b), b);
        pointLoc->InsertUniquePoint((double*)(&sqr.c), c);
        pointLoc->InsertUniquePoint((double*)(&sqr.d), d);

        abc[0] = a;
        abc[1] = b;
        abc[2] = c;
        cells->InsertNextCell(3, abc);

        abc[0] = a;
        abc[1] = c;
        abc[2] = d;
        cells->InsertNextCell(3, abc);
    }

    output->SetPoints(points);
    points->Delete();
    output->SetStrips(cells);
    cells->Delete();

    return 1;
}


