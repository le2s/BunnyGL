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

#include "CVtkGaussianSplatter.h"
#include "CVtkImageAlgorithm.h"
#include "vtkDataSet.h"
#include "vtkUnstructuredGrid.h"
#include "IVtkUnstructuredGridIOData.h"

DEFINE_VTK_OBJECT(CVtkGaussianSplatter, CVtkImageAlgorithm, vtkGaussianSplatter)
{
    pDesc->setNodeClassCategory("Filters");
    pDesc->setNodeClassName("vtkGaussianSplatter");
    pDesc->setNodeClassDescription("splat points into a volume with an elliptical, Gaussian distribution");
    pDesc->setPropertyInfo("Radius", "SetRadius", "radius", QStringList() << "double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("ScaleFactor", "SetScaleFactor", "scaleFactor", QStringList() << "double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("ScalarWarping", "SetScalarWarping", "IsScalarWarping", QStringList() << "bool", QStringList(), "void", "bool");
    pDesc->setPropertyInfo("ModelBounds", "SetModelBounds", "ModelBounds", QStringList() << "double" << "double" << "double" << "double" << "double" << "double", QStringList(), "void", "QList<double>");
    pDesc->setPropertyInfo("SampleDimensions", "SetSampleDimensions", "sampleDimensions", QStringList() << "int" << "int" << "int", QStringList(), "void", "QList<int>");
    pDesc->setPropertyInfo("ExponentFactor", "SetExponentFactor", "GetExponentFactor", QStringList() << "double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("NormalWarping", "SetNormalWarping", "GetNormalWarping", QStringList() << "bool", QStringList(), "void", "bool");
    pDesc->setPropertyInfo("Eccentricity", "SetEccentricity", "GetEccentricity", QStringList() << "double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("Capping", "SetCapping", "GetCapping", QStringList() << "int", QStringList(), "void", "int");
    
    pDesc->addConnectionPath(
        new CVtkObjectConnectionPath("UnstructuredGridInput", IVisSystemNodeConnectionPath::InputPath,  "vtkUnstructuredGrid", 0)
    );
}

CVtkGaussianSplatter::CVtkGaussianSplatter()
{
    CVtkGaussianSplatter::InitializeObjectDesc();
    setVtkObject(vtkGaussianSplatter::New());
}

CVtkGaussianSplatter::~CVtkGaussianSplatter()
{

}

QList<double> CVtkGaussianSplatter::modelBounds() const
{
    double* val = 0;
    QList<double> list;
    val = m_vtkGaussianSplatter->GetModelBounds();
    if(val)
    {
        for(int i=0; i<6; i++)
            list.append(val[i]);
    }
    return list;
}

void CVtkGaussianSplatter::setModelBounds(const QList<double>& val)
{
    QVector<double> data = val.toVector();
    m_vtkGaussianSplatter->SetModelBounds(data.data());
}

double CVtkGaussianSplatter::radius() const
{
    return m_vtkGaussianSplatter->GetRadius();
}

void CVtkGaussianSplatter::setRadius(double rad)
{
    m_vtkGaussianSplatter->SetRadius(rad);
}

double CVtkGaussianSplatter::scaleFactor() const
{
    return m_vtkGaussianSplatter->GetScaleFactor();
}

void CVtkGaussianSplatter::setScaleFactor(double fact)
{
    m_vtkGaussianSplatter->SetScaleFactor(fact);
}

double CVtkGaussianSplatter::exponentFactor() const
{
    return m_vtkGaussianSplatter->GetExponentFactor();
}

void CVtkGaussianSplatter::setExponentFactor(double fact)
{
    m_vtkGaussianSplatter->SetExponentFactor(fact);
}

bool CVtkGaussianSplatter::isNormalWarping()
{
    return m_vtkGaussianSplatter->GetNormalWarping();
}

void CVtkGaussianSplatter::setNormalWarping(bool val)
{
    m_vtkGaussianSplatter->SetNormalWarping(val);
}

double CVtkGaussianSplatter::eccentricity() const
{
    return m_vtkGaussianSplatter->GetEccentricity();
}

void CVtkGaussianSplatter::setEccentricity(double ecc)
{
    m_vtkGaussianSplatter->SetEccentricity(ecc);
}

bool CVtkGaussianSplatter::isScalarWarping()
{
    return m_vtkGaussianSplatter->GetScalarWarping();
}

void CVtkGaussianSplatter::setScalarWarping(bool val)
{
    m_vtkGaussianSplatter->SetScalarWarping(val);
}

bool CVtkGaussianSplatter::isCapping()
{
    return m_vtkGaussianSplatter->GetCapping();
}

void CVtkGaussianSplatter::setCapping(bool val)
{
    m_vtkGaussianSplatter->SetCapping(val);
}

QList<int> CVtkGaussianSplatter::sampleDimensions() const
{
    int* dim = 0;
    QList<int> list;

    dim = m_vtkGaussianSplatter->GetSampleDimensions();
    if(dim)
    {
        for(int i=0; i<3; i++)
            list.append(dim[i]);
    }
    return list;
}

void CVtkGaussianSplatter::setSampleDimensions(const QList<int>& dim)
{
    QVector<int> data = dim.toVector();
    m_vtkGaussianSplatter->SetSampleDimensions(data.data());
}

bool CVtkGaussianSplatter::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    if(path->pathName() == "UnstructuredGridInput")
        return m_vtkGaussianSplatter->GetInput() != 0;

    return CVtkImageAlgorithm::hasInput(path);
}

bool CVtkGaussianSplatter::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    if(path->pathName() == "UnstructuredGridInput")
    {
        IVtkUnstructuredGridIOData* data = 0;
        bool success = inputData->queryInterface("IVtkUnstructuredGridIOData", (void**)&data);
        if(success)
        {
            vtkUnstructuredGrid* uGrid = data->getVtkUnstructuredGrid();
            m_vtkGaussianSplatter->SetInput(uGrid);
            return true;
        }

        return false;
    }

    return CVtkImageAlgorithm::setInput(path, inputData);
}

bool CVtkGaussianSplatter::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    if(path->pathName() == "UnstructuredGridInput")
    {
        m_vtkGaussianSplatter->SetInput(0);
        return true;
    }

    return CVtkImageAlgorithm::removeInput(path, inputData);
}






