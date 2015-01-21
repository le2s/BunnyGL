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

#include "vtkMengerSpongeSource.h"
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
    Point& operator *= (double s) { x *= s; y *= s; z *= s; return *this; }

    const double* toArray() const { return (double*)(this); }
    double* toArray() { return (double*)(this); }

public:
    double x;
    double y;
    double z;
};

struct Cube
{
    Cube(const Point& _center, double _size) {
        center = _center;
        size = _size;
    }

    Cube() {
        center = Point(0, 0, 0);
        size = 1.0;
    }

    Cube(const Cube& other) {
        center = other.center;
        size = other.size;
    }

    Cube& operator = (const Cube& other) {
        center = other.center;
        size = other.size;
        return *this;
    }

    bool operator == (const Cube& other) const {
        return (center == other.center) && (size == other.size);
    }

    Point a() const { return center + Point(size/2, size/2, size/2); }
    Point b() const { return center + Point(-size/2, size/2, size/2); }
    Point c() const { return center + Point(-size/2, -size/2, size/2); }
    Point d() const { return center + Point(size/2, -size/2, size/2); }
    Point e() const { return center + Point(size/2, size/2, -size/2); }
    Point f() const { return center + Point(-size/2, size/2, -size/2); }
    Point g() const { return center + Point(-size/2, -size/2, -size/2); }
    Point h() const { return center + Point(size/2, -size/2, -size/2); }

public:
    Point center;
    double size;
};

// I am not using stl::list<> or stl::vector<> for this on purpose.
struct CubeListData
{
    CubeListData() : refCount(1) {
        cubeArray = new Cube[100];
        size = 100;
        lastIndex = -1;
    }

    ~CubeListData() {
        delete [] cubeArray;
    }

    void ref() { ++refCount; }
    void deref() { --refCount; if(refCount <= 0) delete this; }

    void resize(int newSize) {
        if(newSize < size)
            return;

        Cube* newArray = new Cube[newSize];
        for(int i=0; i<size; i++)
            newArray[i] = cubeArray[i];

        delete [] cubeArray;
        cubeArray = newArray;
        size = newSize;
    }

public:
    Cube* cubeArray;
    int size;
    int lastIndex;

private:
    int refCount;
};

struct CubeList
{
public:
    CubeList() : d(new CubeListData) { }
    CubeList(const CubeList& other) {
        d = other.d;
        d->ref();
    }
    ~CubeList() { d->deref(); }

    CubeList& operator = (const CubeList& other) {
        d->deref();
        d = other.d;
        d->ref();

        return *this;
    }

    bool operator == (const CubeList& other) {
        return d == other.d;
    }

    void push_back(const Cube& cube) {
        ++d->lastIndex;
        if(d->lastIndex >= d->size)
            d->resize(d->size+100);

        d->cubeArray[d->lastIndex] = cube;
    }

    int size() const { return d->lastIndex+1; }

    Cube cube(int index) const {
        if(index < 0 || index >= size())
            return Cube();

        return d->cubeArray[index];
    }

    CubeList& operator += (const CubeList& other) {
        for(int i=0; i<other.size(); i++)
            this->push_back( other.cube(i) );

        return *this;
    }

private:
    CubeListData* d;
};

CubeList DivideMengerSponge(const Cube& cube, int iterations=0)
{
    CubeList ret;

    if(iterations == 0)
    {
        ret.push_back(cube);
        return ret;
    }

    // We will now need to create 20 menger sponge cubes within the first cube
    // We know that along the vertical and horizontal axis we have 3 divided
    // cube centers. 
    static double c1 = -0.5+(1.0/3.0)*0.5;
    static double c2 = 0;
    static double c3 = 0.5-(1.0/3.0)*0.5;
    static double cubeCenters[] =
        {
            c1, c1, c1,
            c1, c2, c1,
            c1, c3, c1,
            c2, c1, c1,
            c2, c3, c1,
            c3, c1, c1,
            c3, c2, c1,
            c3, c3, c1,

            c1, c1, c3,
            c1, c2, c3,
            c1, c3, c3,
            c2, c1, c3,
            c2, c3, c3,
            c3, c1, c3,
            c3, c2, c3,
            c3, c3, c3,

            c1, c1, c2,
            c1, c3, c2,
            c3, c1, c2,
            c3, c3, c2
        };

    double size = cube.size/3.0;
    for(int i=0; i<20; i++)
    {
        Point subCubeCenter(cubeCenters+i*3);
        subCubeCenter *= cube.size;
        subCubeCenter += cube.center;

        Cube subCube(subCubeCenter, size);

        if(iterations <= 1)
            ret.push_back( subCube );
        else
            ret += DivideMengerSponge( subCube, iterations-1 );
    }

    return ret;
}

vtkIdType AddSpongePoint(vtkPoints* points, const Point& pt)
{
    vtkIdType count = points->GetNumberOfPoints();
    points->InsertNextPoint((double*)&pt);
    return count;
}

///////////////////////////////////////////////////////////////////////////////
// vtkMengerSpongeSource implementation
///////////////////////////////////////////////////////////////////////////////

vtkCxxRevisionMacro(vtkMengerSpongeSource, "$Revision: 1.0 $");
vtkStandardNewMacro(vtkMengerSpongeSource);

vtkMengerSpongeSource::vtkMengerSpongeSource()
{
    this->NumberOfIterations = 1;

    this->SetNumberOfInputPorts(0);
    this->SetNumberOfOutputPorts(1);
}

vtkMengerSpongeSource::~vtkMengerSpongeSource()
{

}

void vtkMengerSpongeSource::PrintSelf(ostream& os, vtkIndent indent)
{
    vtkPolyDataAlgorithm::PrintSelf(os, indent);

    os << "NumberOfIterations: " << this->NumberOfIterations << endl;
}

int vtkMengerSpongeSource::RequestData(
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
    CubeList cubes = DivideMengerSponge(Cube(), this->NumberOfIterations);

    // Now construct a point array and square array
    static double bounds[] = {-0.5, 0.5, -0.5, 0.5, -0.5, 0.5};
    vtkPoints* points = vtkPoints::New();
    vtkCellArray* cells = vtkCellArray::New();
    vtkPointLocator* pointLoc = vtkPointLocator::New();
    pointLoc->InitPointInsertion(points, bounds);
    int size = cubes.size();
    for(int i=0; i<size; i++)
    {
        Cube cube = cubes.cube(i);
        vtkIdType abcd[4];
        vtkIdType a, b, c, d, e, f, g, h;

        pointLoc->InsertUniquePoint(cube.a().toArray(), a);
        pointLoc->InsertUniquePoint(cube.b().toArray(), b);
        pointLoc->InsertUniquePoint(cube.c().toArray(), c);
        pointLoc->InsertUniquePoint(cube.d().toArray(), d);
        pointLoc->InsertUniquePoint(cube.e().toArray(), e);
        pointLoc->InsertUniquePoint(cube.f().toArray(), f);
        pointLoc->InsertUniquePoint(cube.g().toArray(), g);
        pointLoc->InsertUniquePoint(cube.h().toArray(), h);

        abcd[0] = a; 
        abcd[1] = b; 
        abcd[2] = c; 
        abcd[3] = d; 
        cells->InsertNextCell(4, abcd);

        abcd[0] = b; 
        abcd[1] = f; 
        abcd[2] = g; 
        abcd[3] = c; 
        cells->InsertNextCell(4, abcd);

        abcd[0] = f; 
        abcd[1] = e; 
        abcd[2] = h; 
        abcd[3] = g; 
        cells->InsertNextCell(4, abcd);

        abcd[0] = e; 
        abcd[1] = a; 
        abcd[2] = d; 
        abcd[3] = h; 
        cells->InsertNextCell(4, abcd);

        abcd[0] = e; 
        abcd[1] = f; 
        abcd[2] = b; 
        abcd[3] = a; 
        cells->InsertNextCell(4, abcd);

        abcd[0] = h; 
        abcd[1] = d; 
        abcd[2] = c; 
        abcd[3] = g; 
        cells->InsertNextCell(4, abcd);
    }

    output->SetPoints(points);
    points->Delete();
    output->SetPolys(cells);
    cells->Delete();
    pointLoc->Delete();

    return 1;
}


