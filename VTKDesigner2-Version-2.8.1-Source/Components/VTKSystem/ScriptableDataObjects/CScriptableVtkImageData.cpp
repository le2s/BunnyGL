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

#include "CScriptableVtkImageData.h"
#include "vtkPoints.h"
#include "vtkPointData.h"
#include "vtkDoubleArray.h"

CScriptableVtkImageData::CScriptableVtkImageData(vtkImageData* imageData, CVtkObject* source)
:CScriptableVtkDataSet(imageData, source), m_vtkImageData(imageData)
{

}

CScriptableVtkImageData::~CScriptableVtkImageData()
{

}

QList<int> CScriptableVtkImageData::dimensions() const
{
    QList<int> ret;
    int* dim = m_vtkImageData->GetDimensions();
    ret << dim[0] << dim[1] << dim[2];
    return ret;
}

void CScriptableVtkImageData::setDimensions(QList<int> dim)
{
    if(dim.size() != 3)
        return;

    m_vtkImageData->SetDimensions( dim[0], dim[1], dim[2] );
}

QList<double> CScriptableVtkImageData::spacing() const
{
    QList<double> ret;
    double* spacing = m_vtkImageData->GetSpacing();
    ret << spacing[0] << spacing[1] << spacing[2];
    return ret;
}

void CScriptableVtkImageData::setSpacing(QList<double> sp)
{
    m_vtkImageData->SetSpacing(sp[0], sp[1], sp[2]);
}

Point3D CScriptableVtkImageData::origin() const
{
    return Point3D(m_vtkImageData->GetOrigin());
}

void CScriptableVtkImageData::setOrigin(Point3D point)
{
    m_vtkImageData->SetOrigin(point.x, point.y, point.z);
}

QString CScriptableVtkImageData::scalarType() const
{
    return QString("%1").arg(m_vtkImageData->GetScalarTypeAsString());
}

void CScriptableVtkImageData::setScalarTypeToDouble()
{
    m_vtkImageData->SetScalarTypeToDouble();
}

void CScriptableVtkImageData::setScalarTypeToChar()
{
    m_vtkImageData->SetScalarTypeToChar();
}

void CScriptableVtkImageData::setScalarTypeToInt()
{
    m_vtkImageData->SetScalarTypeToInt();
}

void CScriptableVtkImageData::setScalarTypeToUnsignedChar()
{
    m_vtkImageData->SetScalarTypeToUnsignedChar();
}

void CScriptableVtkImageData::setScalarTypeToUnsignedInt()
{
    m_vtkImageData->SetScalarTypeToUnsignedInt();
}

int CScriptableVtkImageData::numberOfScalarComponents() const
{
    return m_vtkImageData->GetNumberOfScalarComponents();
}

void CScriptableVtkImageData::setNumberOfScalarComponents(int val)
{
    m_vtkImageData->SetNumberOfScalarComponents(val);
}

void CScriptableVtkImageData::reset()
{
    m_vtkImageData->Initialize();
}

void CScriptableVtkImageData::allocateScalars()
{
    m_vtkImageData->AllocateScalars();
}

int CScriptableVtkImageData::numberOfPoints() const
{
    return m_vtkImageData->GetNumberOfPoints();
}

Point3D CScriptableVtkImageData::point(int index) const
{
    return Point3D(m_vtkImageData->GetPoint(vtkIdType(index)));
}

Point3D CScriptableVtkImageData::point(int x, int y, int z) const
{
    int* dim = m_vtkImageData->GetDimensions();
    int pts_per_line = dim[0];
    int pts_per_plane = dim[0]*dim[1];
    int index = z*pts_per_plane + y*pts_per_line + x;
    return Point3D(m_vtkImageData->GetPoint(vtkIdType(index)));
}

QList<double> CScriptableVtkImageData::scalarComponent(int x, int y, int z) const
{
    QList<double> ret;
    for(int i=0; i<m_vtkImageData->GetNumberOfScalarComponents(); i++)
        ret << m_vtkImageData->GetScalarComponentAsDouble(x, y, z, i);
    return ret;
}

void CScriptableVtkImageData::setScalarComponent(int x, int y, int z, QList<double> scalar)
{
    if(scalar.count() < m_vtkImageData->GetNumberOfScalarComponents())
        return;
    for(int i=0; i<m_vtkImageData->GetNumberOfScalarComponents(); i++)
    {
        double scalar_value = scalar[i];
        m_vtkImageData->SetScalarComponentFromDouble(x, y, z, i, scalar_value);
    }
}


