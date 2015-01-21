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

#ifndef VTK_IMAGE_TO_TERRAIN_H
#define VTK_IMAGE_TO_TERRAIN_H

#include "vtkPolyDataAlgorithm.h"

class vtkImageData;
class vtkImageToTerrain : public vtkPolyDataAlgorithm
{
public:
    static vtkImageToTerrain *New();
    vtkTypeRevisionMacro(vtkImageToTerrain, vtkPolyDataAlgorithm);
    void PrintSelf(ostream& os, vtkIndent indent);

    void SetDimensions(int x, int y);
    void GetDimensions(int& x, int& y);
    int* GetDimensions() { return this->Dimensions; }

    void SetDistance(double x, double y);
    void GetDistance(double &x, double &y);
    double* GetDistance() { return this->Distance; }

    void SetHeightScaleFactor(double val);
    double GetHeightScaleFactor();

    double* GetHeightRange() { return this->HeightRange; }

    void SetImage(vtkImageData* imageData);
    vtkImageData* GetImage();

protected:
    vtkImageToTerrain();
    ~vtkImageToTerrain();
    int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *);

private:
    vtkImageToTerrain(const vtkImageToTerrain&);  // Not implemented.
    void operator=(const vtkImageToTerrain&);  // Not implemented.
    double GetHeightValueAt(int s, int t, double scale);
    void ComputeHeightRange();

private:
    int Dimensions[2];
    double Distance[2];
    double HeightRange[2];
    vtkImageData* ImageData;
    double HeightScaleFactor;
};

#endif


