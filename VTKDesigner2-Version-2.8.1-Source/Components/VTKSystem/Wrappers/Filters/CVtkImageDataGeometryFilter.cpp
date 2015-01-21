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

#include "CVtkImageDataGeometryFilter.h"
#include "CVtkPolyDataAlgorithm.h"
#include "vtkImageDataGeometryFilter.h"
#include "IVtkImageDataIOData.h"
#include "vtkImageData.h"


DEFINE_VTK_OBJECT(CVtkImageDataGeometryFilter, CVtkPolyDataAlgorithm, vtkImageDataGeometryFilter)
{
    pDesc->setNodeClassName("vtkImageDataGeometryFilter");
    pDesc->setNodeClassDescription("extract geometry for structured points");
    pDesc->setNodeClassCategory("Image Filters");

    pDesc->setPropertyInfo("Extent", "SetExtent", "GetExtent", QStringList()<<"int*",QStringList(),"void","int*");
    pDesc->setPropertyInfo("ThresholdCells", "SetThresholdCells", "GetThresholdCells", QStringList()<<"bool",QStringList(),"void","bool");
    pDesc->setPropertyInfo("ThresholdValue", "SetThresholdValue", "GetThresholdValue", QStringList()<<"double",QStringList(),"void","double");
    pDesc->setPropertyInfo("OutputTriangles", "SetOutputTriangles", "GetOutputTriangles", QStringList()<<"bool",QStringList(),"void","bool");
    
    pDesc->addConnectionPath(
        new CVtkObjectConnectionPath("ImageData", IVisSystemNodeConnectionPath::InputPath, "vtkImageData", 0)
    );
    pDesc->setPropertyInfo("Extent", "setExtent", "getExtent", QStringList() << "int" << "int" << "int" << "int" << "int" << "int", QStringList(),"void", "QList<int>");
};

CVtkImageDataGeometryFilter::CVtkImageDataGeometryFilter() : m_vtkImageDataGeometryFilter(0)
{
    CVtkImageDataGeometryFilter::InitializeObjectDesc();
    setVtkObject(vtkImageDataGeometryFilter::New());
}

CVtkImageDataGeometryFilter::~CVtkImageDataGeometryFilter()
{
}

void CVtkImageDataGeometryFilter::setExtent(const QList<int>& exts)
{
    QVector<int> data = exts.toVector();

    m_vtkImageDataGeometryFilter->SetExtent(data.data());
}

QList<int> CVtkImageDataGeometryFilter::extent() const
{
    QList<int> list;

    int *ext = 0;
    ext = m_vtkImageDataGeometryFilter->GetExtent();

    for(int i=0; i<6; i++)
        list.append(ext[i]);

    return list;
}

void CVtkImageDataGeometryFilter::setThresholdCells(int val)
{
    m_vtkImageDataGeometryFilter->SetThresholdCells(val);
}

bool CVtkImageDataGeometryFilter::thresholdCells() const
{
    return m_vtkImageDataGeometryFilter->GetThresholdCells(); 
}

void CVtkImageDataGeometryFilter::setThresholdValue(double val)
{
    m_vtkImageDataGeometryFilter->SetThresholdValue(val);
}

double CVtkImageDataGeometryFilter::thresholdValue() const
{
    return m_vtkImageDataGeometryFilter->GetThresholdValue();
}

void CVtkImageDataGeometryFilter::setOutputTriangles(int val)
{
    m_vtkImageDataGeometryFilter->SetOutputTriangles(val);
}

bool CVtkImageDataGeometryFilter::outputTriangles() const
{
    return  m_vtkImageDataGeometryFilter->GetOutputTriangles();
}

bool CVtkImageDataGeometryFilter::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    if(path->pathName() == "ImageData")
        return m_vtkImageDataGeometryFilter->GetInput() != 0;

    return CVtkPolyDataAlgorithm::hasInput(path);

}

bool CVtkImageDataGeometryFilter::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    if(path->pathName() == "ImageData")
    {
        IVtkImageDataIOData* data = 0;
        bool success = inputData->queryInterface("IVtkImageDataIOData", (void**)&data);

        if(success)
        {
           vtkImageData* imData = data->getVtkImageData();
           m_vtkImageDataGeometryFilter->SetInput(imData);
           return true;
        }

        return false;
    }

    return CVtkPolyDataAlgorithm::setInput(path, inputData);
}

bool CVtkImageDataGeometryFilter::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || ! inputData)
        return false;

    if(path->pathName() == "ImageData")
    {
        m_vtkImageDataGeometryFilter->SetInput(0);
        return true;
    }

    return CVtkPolyDataAlgorithm::removeInput(path, inputData);
}