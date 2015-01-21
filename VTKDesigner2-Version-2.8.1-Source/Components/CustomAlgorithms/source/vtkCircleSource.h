#ifndef VTK_CIRCLE_SOURCE_H
#define VTK_CIRCLE_SOURCE_H

#include "vtkPolyDataAlgorithm.h"

class vtkCircleSource : public vtkPolyDataAlgorithm
{
public:
    static vtkCircleSource *New();
    vtkTypeRevisionMacro(vtkCircleSource, vtkPolyDataAlgorithm);
    void PrintSelf(ostream& os, vtkIndent indent);

    vtkSetMacro(Resolution, int);
    vtkGetMacro(Resolution, int);

    vtkSetMacro(Radius, double);
    vtkGetMacro(Radius, double);

protected:
    vtkCircleSource();
    ~vtkCircleSource();
    int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *);

private:
    vtkCircleSource(const vtkCircleSource&);  // Not implemented.
    void operator=(const vtkCircleSource&);  // Not implemented.

private:
    int Resolution;
    double Radius;
};

#endif


