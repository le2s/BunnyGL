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
#include "CVtkWeightedTransformFilter.h"

DEFINE_VTK_OBJECT(CVtkWeightedTransformFilter,CVtkPointSetFilter,vtkWeightedTransformFilter)
{
    pDesc->setNodeClassName("vtkWeightedTransformFilter");
    pDesc->setNodeClassDescription("Transform based on per-point or per-cell weighting functions");
    pDesc->setNodeClassCategory("PolyData Filters");

    pDesc->setPropertyInfo("WeightArray", "SetWeightArray", "GetWeightArray", QStringList() << "QString", QStringList(), "void", "QString");
    pDesc->setPropertyInfo("TransformIndexArray", "SetTransformIndexArray", "GetTransformIndexArray", QStringList() << "QString", QStringList(), "void", "QString");
    pDesc->setPropertyInfo("CellDataWeightArray", "SetCellDataWeightArray", "GetCellDataWeightArray", QStringList() << "QString", QStringList(), "void", "QString");
    pDesc->setPropertyInfo("CellDataTransformIndexArray", "SetCellDataTransformIndexArray", "GetCellDataTransformIndexArray", QStringList() << "QString", QStringList(), "void", "QString");
    pDesc->setPropertyInfo("NumberOfTransforms", "SetNumberOfTransforms", "GetNumberOfTransforms", QStringList() << "int", QStringList(), "void", "int");
    pDesc->setPropertyInfo("AddInputValues", "SetAddInputValues", "GetAddInputValues", QStringList() << "int" , QStringList(), "void", "int");
}

CVtkWeightedTransformFilter::CVtkWeightedTransformFilter() : m_vtkWeightedTransformFilter(0)
{
    CVtkWeightedTransformFilter::InitializeObjectDesc();
    setVtkObject(vtkWeightedTransformFilter::New());
}

CVtkWeightedTransformFilter::~CVtkWeightedTransformFilter()
{

}

QString CVtkWeightedTransformFilter::weightArray() const
{
    return m_vtkWeightedTransformFilter->GetWeightArray();
}


void CVtkWeightedTransformFilter::setWeightArray(QString val)
{
    m_vtkWeightedTransformFilter->SetWeightArray(qPrintable(val));
}


QString CVtkWeightedTransformFilter::transformIndexArray() const
{
    return m_vtkWeightedTransformFilter->GetTransformIndexArray();
}


void CVtkWeightedTransformFilter::setTransformIndexArray(QString val)
{
    m_vtkWeightedTransformFilter->SetTransformIndexArray(qPrintable(val));
}


QString CVtkWeightedTransformFilter::cellDataWeightArray() const
{
    return m_vtkWeightedTransformFilter->GetCellDataWeightArray();
}


void CVtkWeightedTransformFilter::setCellDataWeightArray(QString val)
{
    m_vtkWeightedTransformFilter->SetCellDataWeightArray(qPrintable(val));
}


QString CVtkWeightedTransformFilter::cellDataTransformIndexArray() const
{
    return m_vtkWeightedTransformFilter->GetCellDataTransformIndexArray();
}


void CVtkWeightedTransformFilter::setCellDataTransformIndexArray(QString val)
{
    m_vtkWeightedTransformFilter->SetCellDataTransformIndexArray(qPrintable(val));
}


int CVtkWeightedTransformFilter::numberOfTransforms() const
{
    return m_vtkWeightedTransformFilter->GetNumberOfTransforms();
}


void CVtkWeightedTransformFilter::setNumberOfTransforms(int val)
{
    m_vtkWeightedTransformFilter->SetNumberOfTransforms(val);
}


bool CVtkWeightedTransformFilter::isAddInputValues() const
{
    return m_vtkWeightedTransformFilter->GetAddInputValues();
}


void CVtkWeightedTransformFilter::setAddInputValues(bool val)
{
    m_vtkWeightedTransformFilter->SetAddInputValues(val);
}

