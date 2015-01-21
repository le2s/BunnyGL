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

#ifndef VTK_SIERPINSKI_Pyramid_SOURCE_H
#define VTK_SIERPINSKI_Pyramid_SOURCE_H

#include "vtkPolyDataAlgorithm.h"

class vtkSierpinskiPyramidSource : public vtkPolyDataAlgorithm
{
public:
    static vtkSierpinskiPyramidSource *New();
    vtkTypeRevisionMacro(vtkSierpinskiPyramidSource, vtkPolyDataAlgorithm);
    void PrintSelf(ostream& os, vtkIndent indent);

    vtkSetClampMacro(NumberOfIterations, int, 0, 7)
    vtkGetMacro(NumberOfIterations, int)

protected:
    vtkSierpinskiPyramidSource();
    ~vtkSierpinskiPyramidSource();
    int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *);

private:
    vtkSierpinskiPyramidSource(const vtkSierpinskiPyramidSource&);  // Not implemented.
    void operator=(const vtkSierpinskiPyramidSource&);  // Not implemented.

private:
    int NumberOfIterations;
};

#endif


