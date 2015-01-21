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

#include "vtkMengerSquareSource.h"
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

void MengerSquareTrivide(const Point& p1, const Point& p2, Point& q1, Point& q2)
{
    // In this algorithm we assume that the line between p1 and p2
    // is either parallel to X axis or Y axis. So we can optimize
    // trivide without using complex vector math
    if(p1.x == p2.x)
    {
        // Parallel to Y axis
        double dist = p2.y - p1.y;
        double trividedDist = dist/3.0;

        q1 = p1 + Point(0, trividedDist, 0);
        q2 = p2 - Point(0, trividedDist, 0);
    }
    else
    {
        // Parallel to X axis
        double dist = p2.x - p1.x;
        double trividedDist = dist/3.0;

        q1 = p1 + Point(trividedDist, 0, 0);
        q2 = p2 - Point(trividedDist, 0, 0);
    }
}

SquareList DivideMenger(const Square& sqr, int iterations=0)
{
    SquareList ret;

    if(iterations == 0)
    {
        ret.push_back(sqr);
        return ret;
    }

    // Menger divide this square
    Point e, f, g, h, i, j, k, l, m, n, o, p;
    MengerSquareTrivide(sqr.a, sqr.b, e, f);
    MengerSquareTrivide(sqr.b, sqr.c, g, h);
    MengerSquareTrivide(sqr.c, sqr.d, i, j);
    MengerSquareTrivide(sqr.d, sqr.a, k, l);
    MengerSquareTrivide(    h,     k, o, p);
    MengerSquareTrivide(    g,     l, n, m);

    Square s1(sqr.a, e, m, l);
    Square s2(e, f, n, m);
    Square s3(f, sqr.b, g, n);
    Square s4(n, g, h, o);
    Square s5(o, h, sqr.c, i);
    Square s6(p, o, i, j);
    Square s7(k, p, j, sqr.d);
    Square s8(l, m, p, k);

    // Should we divide more?
    if(iterations <= 1)
    {
        // Now we dont!
        ret.push_back(s1);
        ret.push_back(s2);
        ret.push_back(s3);
        ret.push_back(s4);
        ret.push_back(s5);
        ret.push_back(s6);
        ret.push_back(s7);
        ret.push_back(s8);

        return ret;
    }

    // Yes we do.
    ret += DivideMenger(s1, iterations-1);
    ret += DivideMenger(s2, iterations-1);
    ret += DivideMenger(s3, iterations-1);
    ret += DivideMenger(s4, iterations-1);
    ret += DivideMenger(s5, iterations-1);
    ret += DivideMenger(s6, iterations-1);
    ret += DivideMenger(s7, iterations-1);
    ret += DivideMenger(s8, iterations-1);

    return ret;
}

///////////////////////////////////////////////////////////////////////////////
// vtkMengerSquareSource implementation
///////////////////////////////////////////////////////////////////////////////

vtkCxxRevisionMacro(vtkMengerSquareSource, "$Revision: 1.0 $");
vtkStandardNewMacro(vtkMengerSquareSource);

vtkMengerSquareSource::vtkMengerSquareSource()
{
    this->NumberOfIterations = 1;

    this->SetNumberOfInputPorts(0);
    this->SetNumberOfOutputPorts(1);
}

vtkMengerSquareSource::~vtkMengerSquareSource()
{

}

void vtkMengerSquareSource::PrintSelf(ostream& os, vtkIndent indent)
{
    vtkPolyDataAlgorithm::PrintSelf(os, indent);

    os << "NumberOfIterations: " << this->NumberOfIterations << endl;
}

int vtkMengerSquareSource::RequestData(
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
    SquareList squares = DivideMenger(Square(), this->NumberOfIterations);

    // Now construct a point array and square array
    static double bounds[] = {-0.5, 0.5, -0.5, 0.5, -1, 1};
    vtkPoints* points = vtkPoints::New();
    vtkCellArray* cells = vtkCellArray::New();
    vtkPointLocator* pointLoc = vtkPointLocator::New();
    pointLoc->InitPointInsertion(points, bounds);
    int size = squares.size();
    for(int i=0; i<size; i++)
    {
        Square sqr = squares.square(i);
        vtkIdType abcd[4];

        pointLoc->InsertUniquePoint((double*)(&sqr.a), abcd[0]);
        pointLoc->InsertUniquePoint((double*)(&sqr.b), abcd[1]);
        pointLoc->InsertUniquePoint((double*)(&sqr.c), abcd[2]);
        pointLoc->InsertUniquePoint((double*)(&sqr.d), abcd[3]);

        cells->InsertNextCell(4, abcd);
    }

    output->SetPoints(points);
    points->Delete();
    output->SetPolys(cells);
    cells->Delete();

    return 1;
}


