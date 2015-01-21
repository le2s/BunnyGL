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

#include "CVtkStructuredGridGeometryFilter.h"
#include "vtkStructuredGrid.h"
#include "IVtkStructuredGridIOData.h"

DEFINE_VTK_OBJECT(CVtkStructuredGridGeometryFilter, CVtkPolyDataAlgorithm, vtkStructuredGridGeometryFilter)
{
    pDesc->setNodeClassCategory("Filters");
    pDesc->setNodeClassName("vtkStructuredGridGeometryFilter");
    pDesc->setNodeClassDescription("extract geometry for structured grid");
    pDesc->setPropertyInfo("Extent", "setExtent", "extent", QStringList() << "int", QStringList(), "void", "QList<int>");

    pDesc->addConnectionPath(
        new CVtkObjectConnectionPath("StructuredGridInput", IVisSystemNodeConnectionPath::InputPath,  "vtkStructuredGrid", 0)
    );
}

CVtkStructuredGridGeometryFilter::CVtkStructuredGridGeometryFilter() : m_vtkStructuredGridGeometryFilter(0)
{
    CVtkStructuredGridGeometryFilter::InitializeObjectDesc();
    setVtkObject(vtkStructuredGridGeometryFilter::New());
}

CVtkStructuredGridGeometryFilter::~CVtkStructuredGridGeometryFilter()
{

}

void CVtkStructuredGridGeometryFilter::setExtent(const QList<int>& exts)
{
    QVector<int> data = exts.toVector();
    m_vtkStructuredGridGeometryFilter->SetExtent(data.data());
}

QList<int> CVtkStructuredGridGeometryFilter::extent() const
{
    QList<int> ret;

    int* data = m_vtkStructuredGridGeometryFilter->GetExtent();

    for(int i=0; i<6; i++)
        ret << data[i];

    return ret;
}

bool CVtkStructuredGridGeometryFilter::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    if(path->pathName() == "StructuredGridInput")
            return m_vtkStructuredGridGeometryFilter->GetInput() != 0;

    return CVtkPolyDataAlgorithm::hasInput(path);
}

bool CVtkStructuredGridGeometryFilter::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    if(path->pathName() == "StructuredGridInput")
    {
        IVtkStructuredGridIOData* data = 0;
        bool success = inputData->queryInterface("IVtkStructuredGridIOData", (void**)&data);

        if(success)
        {
            vtkStructuredGrid* sGrid = data->getVtkStructuredGrid();
            m_vtkStructuredGridGeometryFilter->SetInput(sGrid);
            return true;
        }

        return false;
    }

    return CVtkPolyDataAlgorithm::setInput(path, inputData);
}

bool CVtkStructuredGridGeometryFilter::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    if(path->pathName() == "StructuredGridInput")
    {
        m_vtkStructuredGridGeometryFilter->SetInput(0);
        return true;
    }

    return CVtkPolyDataAlgorithm::removeInput(path, inputData);
}

