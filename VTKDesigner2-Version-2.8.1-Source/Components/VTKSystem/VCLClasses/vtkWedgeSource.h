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

#ifndef VTK_WEDGE_SOURCE_H
#define VTK_WEDGE_SOURCE_H

#include "vtkPolyDataAlgorithm.h"

class vtkWedgeSource : public vtkPolyDataAlgorithm
{
public:
    static vtkWedgeSource *New();
    vtkTypeRevisionMacro(vtkWedgeSource, vtkPolyDataAlgorithm);
    void PrintSelf(ostream& os, vtkIndent indent);

    vtkSetMacro(Width, double);
    vtkGetMacro(Width, double);

    vtkSetMacro(Height, double);
    vtkGetMacro(Height, double);

    vtkSetMacro(Depth, double);
    vtkGetMacro(Depth, double);

    void SetHeightFromBaseAngle(double angleDeg);
    double GetBaseAngle();

protected:
    vtkWedgeSource();
    ~vtkWedgeSource();
    int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *);

private:
    vtkWedgeSource(const vtkWedgeSource&);  // Not implemented.
    void operator=(const vtkWedgeSource&);  // Not implemented.

private:
    double Width;
    double Height;
    double Depth;
};

#endif


