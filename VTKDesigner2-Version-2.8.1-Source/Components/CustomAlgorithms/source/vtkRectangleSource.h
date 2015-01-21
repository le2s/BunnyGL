#ifndef VTK_RECTANGLE_SOURCE_H
#define VTK_RECTANGLE_SOURCE_H

#include "vtkPolyDataAlgorithm.h"

class vtkRectangleSource : public vtkPolyDataAlgorithm
{
public:
    static vtkRectangleSource *New();
    vtkTypeRevisionMacro(vtkRectangleSource, vtkPolyDataAlgorithm);
    void PrintSelf(ostream& os, vtkIndent indent);

    vtkSetMacro(Length, double);
    vtkGetMacro(Length, double);

    vtkSetMacro(Breadth, double);
    vtkGetMacro(Breadth, double);

protected:
    vtkRectangleSource();
    ~vtkRectangleSource();
    int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *);

private:
    vtkRectangleSource(const vtkRectangleSource&);  // Not implemented.
    void operator=(const vtkRectangleSource&);  // Not implemented.

private:
    double Length;
    double Breadth;
};

#endif


