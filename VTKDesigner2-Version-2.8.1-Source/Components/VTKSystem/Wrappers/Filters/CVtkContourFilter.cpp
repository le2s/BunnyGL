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

#include "CVtkContourFilter.h"
                  
DEFINE_VTK_OBJECT(CVtkContourFilter, CVtkPolyDataFilter, vtkContourFilter)
{
    pDesc->setNodeClassName("vtkContourFilter");
    pDesc->setNodeClassDescription("Generate isosurfaces/isolines from scalar values");

    pDesc->setPropertyInfo("NumberOfContours","SetNumberOfContours","GetNumberOfContours",QStringList() << "int", QStringList(),"void","int");
    pDesc->setPropertyInfo("ComputeNormals", "SetComputeNormals", "GetComputeNormals", QStringList() << "int", QStringList(), "void", "int");
    pDesc->setPropertyInfo("ComputeGradients", "SetComputeGradients", "GetComputeGradients", QStringList() << "int", QStringList(), "void", "int");
    pDesc->setPropertyInfo("ComputeScalars", "SetComputeScalars", "GetComputeScalars", QStringList() << "int", QStringList(), "void", "int");
    pDesc->setPropertyInfo("ScalarArrayComponent", "SetArrayComponent", "GetArrayComponent", QStringList() << "int", QStringList(), "void", "int");
    pDesc->setPropertyInfo("ValueRange", "SetValueRange", "GetValueRange", QStringList() << "double" << "double", QStringList(), "void", "ValueRange");
}

CVtkContourFilter::CVtkContourFilter() : m_vtkContourFilter(0)
{
    CVtkContourFilter::InitializeObjectDesc();
    setVtkObject(vtkContourFilter::New());
}

CVtkContourFilter::~CVtkContourFilter()
{

}

void CVtkContourFilter::setValue(int index, double value)
{
    if(index < 0 || index >= m_vtkContourFilter->GetNumberOfContours())
        return;

    m_vtkContourFilter->SetValue(index, value);
}

double CVtkContourFilter::value(int index) const
{
    if(index < 0 || index >= m_vtkContourFilter->GetNumberOfContours())
        return 0.0f;

    return m_vtkContourFilter->GetValue(index);
}

void CVtkContourFilter::setNumberOfContours(int number)
{
    ValueRange range = this->valueRange();
    m_vtkContourFilter->SetNumberOfContours(number);
    m_vtkContourFilter->GenerateValues(number, range.min, range.max);
}

int CVtkContourFilter::numberOfContours() const
{
    return m_vtkContourFilter->GetNumberOfContours();
}


void CVtkContourFilter::setComputeNormals(bool val)
{
    m_vtkContourFilter->SetComputeNormals(val);    
}

bool CVtkContourFilter::isComputeNormals() const
{
    return (m_vtkContourFilter->GetComputeNormals());
}


void CVtkContourFilter::setComputeGradients(bool val)
{
    m_vtkContourFilter->SetComputeGradients(val);
}

bool CVtkContourFilter::isComputeGradients() const
{
    return (m_vtkContourFilter->GetComputeGradients());
}

void CVtkContourFilter::setComputeScalars(bool val)
{
    m_vtkContourFilter->SetComputeScalars(val);
}

bool CVtkContourFilter::isComputeScalars() const
{
    return (m_vtkContourFilter->GetComputeScalars());
}

void CVtkContourFilter::setScalarArrayComponent(int comp)
{
    m_vtkContourFilter->SetArrayComponent(comp);
}

int CVtkContourFilter::scalarArrayComponent() const
{
    return m_vtkContourFilter->GetArrayComponent();
}

void CVtkContourFilter::setValueRange(ValueRange range)
{
    m_vtkContourFilter->GenerateValues(m_vtkContourFilter->GetNumberOfContours(), range.min, range.max);
}

ValueRange CVtkContourFilter::valueRange() const
{
    double* values = m_vtkContourFilter->GetValues();
    ValueRange range;
    if(!values)
        return range;

    range.min = values[0];
    range.max = values[m_vtkContourFilter->GetNumberOfContours()-1];
    return range;
}

bool CVtkContourFilter::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    bool ret = CVtkPolyDataFilter::setInput(path, inputData);

    if(ret && path->pathName() == "DataObject")
    {
        vtkDataObject* input = m_vtkContourFilter->GetInput();
        vtkDataSet* inputDS = vtkDataSet::SafeDownCast( input );
        if(inputDS)
        {
            double* range = inputDS->GetScalarRange();
            m_vtkContourFilter->SetNumberOfContours(3);
            m_vtkContourFilter->GenerateValues(3, range[0], range[1]);
        }
    }

    return ret;
}


