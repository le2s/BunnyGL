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
#include "CVtkSmoothPolyDataFilter.h"
#include "IVtkPolyDataIOData.h"

DEFINE_VTK_OBJECT(CVtkSmoothPolyDataFilter,CVtkPolyDataFilter,vtkSmoothPolyDataFilter)
{
    pDesc->setNodeClassName("vtkSmoothPolyDataFilter");
    pDesc->setNodeClassDescription("Adjust point positions using Laplacian smoothing");

    //  PolyData input
    pDesc->addConnectionPath(
        new CVtkObjectConnectionPath("PolyData", IVisSystemNodeConnectionPath::InputPath, "vtkPolyData", 0)
        );

    pDesc->setPropertyInfo("Convergence", "SetConvergence", "GetConvergence", QStringList() << "double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("NumberOfIterations", "SetNumberOfIterations", "GetNumberOfIterations", QStringList() << "int", QStringList(), "void", "int");
    pDesc->setPropertyInfo("RelaxationFactor", "SetRelaxationFactor", "GetRelaxationFactor", QStringList() << "double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("FeatureEdgeSmoothing", "SetFeatureEdgeSmoothing", "GetFeatureEdgeSmoothing", QStringList() << "int" , QStringList(), "void", "int");
    pDesc->setPropertyInfo("FeatureAngle", "SetFeatureAngle", "GetFeatureAngle", QStringList() << "double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("EdgeAngle", "SetEdgeAngle", "GetEdgeAngle", QStringList() << "double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("BoundarySmoothing", "SetBoundarySmoothing", "GetBoundarySmoothing", QStringList() << "int" , QStringList(), "void", "int");
    pDesc->setPropertyInfo("GenerateErrorScalars", "SetGenerateErrorScalars", "GetGenerateErrorScalars", QStringList() << "int" , QStringList(), "void", "int");
    pDesc->setPropertyInfo("GenerateErrorVectors", "SetGenerateErrorVectors", "GetGenerateErrorVectors", QStringList() << "int" , QStringList(), "void", "int");
}

CVtkSmoothPolyDataFilter::CVtkSmoothPolyDataFilter() : m_vtkSmoothPolyDataFilter(0)
{
    CVtkSmoothPolyDataFilter::InitializeObjectDesc();
    setVtkObject(vtkSmoothPolyDataFilter::New());
}

CVtkSmoothPolyDataFilter::~CVtkSmoothPolyDataFilter()
{

}

double CVtkSmoothPolyDataFilter::convergence() const
{
    return m_vtkSmoothPolyDataFilter->GetConvergence();
}


void CVtkSmoothPolyDataFilter::setConvergence(double val)
{
    m_vtkSmoothPolyDataFilter->SetConvergence(val);
}


int CVtkSmoothPolyDataFilter::numberOfIterations() const
{
    return m_vtkSmoothPolyDataFilter->GetNumberOfIterations();
}


void CVtkSmoothPolyDataFilter::setNumberOfIterations(int val)
{
    m_vtkSmoothPolyDataFilter->SetNumberOfIterations(val);
}


double CVtkSmoothPolyDataFilter::relaxationFactor() const
{
    return m_vtkSmoothPolyDataFilter->GetRelaxationFactor();
}


void CVtkSmoothPolyDataFilter::setRelaxationFactor(double val)
{
    m_vtkSmoothPolyDataFilter->SetRelaxationFactor(val);
}


bool CVtkSmoothPolyDataFilter::isFeatureEdgeSmoothing() const
{
    return m_vtkSmoothPolyDataFilter->GetFeatureEdgeSmoothing();
}


void CVtkSmoothPolyDataFilter::setFeatureEdgeSmoothing(bool val)
{
    m_vtkSmoothPolyDataFilter->SetFeatureEdgeSmoothing(val);
}


double CVtkSmoothPolyDataFilter::featureAngle() const
{
    return m_vtkSmoothPolyDataFilter->GetFeatureAngle();
}


void CVtkSmoothPolyDataFilter::setFeatureAngle(double val)
{
    m_vtkSmoothPolyDataFilter->SetFeatureAngle(val);
}


double CVtkSmoothPolyDataFilter::edgeAngle() const
{
    return m_vtkSmoothPolyDataFilter->GetEdgeAngle();
}


void CVtkSmoothPolyDataFilter::setEdgeAngle(double val)
{
    m_vtkSmoothPolyDataFilter->SetEdgeAngle(val);
}


bool CVtkSmoothPolyDataFilter::isBoundarySmoothing() const
{
    return m_vtkSmoothPolyDataFilter->GetBoundarySmoothing();
}


void CVtkSmoothPolyDataFilter::setBoundarySmoothing(bool val)
{
    m_vtkSmoothPolyDataFilter->SetBoundarySmoothing(val);
}


bool CVtkSmoothPolyDataFilter::isGenerateErrorScalars() const
{
    return m_vtkSmoothPolyDataFilter->GetGenerateErrorScalars();
}


void CVtkSmoothPolyDataFilter::setGenerateErrorScalars(bool val)
{
    m_vtkSmoothPolyDataFilter->SetGenerateErrorScalars(val);
}


bool CVtkSmoothPolyDataFilter::isGenerateErrorVectors() const
{
    return m_vtkSmoothPolyDataFilter->GetGenerateErrorVectors();
}


void CVtkSmoothPolyDataFilter::setGenerateErrorVectors(bool val)
{
    m_vtkSmoothPolyDataFilter->SetGenerateErrorVectors(val);
}



bool CVtkSmoothPolyDataFilter::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    if(path->pathName() == "PolyData")
        return m_vtkSmoothPolyDataFilter->GetSource() != 0;

    return CVtkPolyDataFilter::hasInput(path);
}

bool CVtkSmoothPolyDataFilter::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    if(path->pathName() == "PolyData")
    {
        IVtkPolyDataIOData* polyData = 0;
        bool success = inputData->queryInterface("IVtkPolyDataIOData", (void**)&polyData);
        if(success && polyData)
        {
            m_vtkSmoothPolyDataFilter->SetSource(polyData->getVtkPolyData());
            return true;
        }
    }

    return CVtkPolyDataFilter::setInput(path, inputData);
}

bool CVtkSmoothPolyDataFilter::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    if(path->pathName() == "PolyData")
    {
        IVtkPolyDataIOData* polyData = 0;
        bool success = inputData->queryInterface("IVtkPolyDataIOData", (void**)&polyData);
        if(success && polyData && m_vtkSmoothPolyDataFilter->GetSource() == polyData->getVtkPolyData())
        {
            m_vtkSmoothPolyDataFilter->SetSource(0);
            return true;
        }
    }

    return CVtkPolyDataFilter::removeInput(path, inputData);
}

