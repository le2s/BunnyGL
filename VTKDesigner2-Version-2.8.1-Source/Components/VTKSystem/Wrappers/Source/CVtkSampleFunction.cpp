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
#include "CVtkSampleFunction.h"
#include "IVtkImplicitFunctionIOData.h"
#include <QVector>

DEFINE_VTK_OBJECT(CVtkSampleFunction,CVtkImageAlgorithm,vtkSampleFunction)
{
    pDesc->setNodeClassName("vtkSampleFunction");
    pDesc->setNodeClassDescription("Sample an implicit function over a structured point set");
    
    pDesc->setPropertyInfo("OutputScalarType", "SetOutputScalarType", "GetOutputScalarType", QStringList() << "int", QStringList(), "void", "int");
    pDesc->setPropertyInfo("ModelBounds", "SetModelBounds", "GetModelBounds", QStringList() << "double" << "double" << "double" << "double" << "double" << "double", QStringList(), "void", "double*");
    pDesc->setPropertyInfo("Capping", "SetCapping", "GetCapping", QStringList() << "int" , QStringList(), "void", "int");
    pDesc->setPropertyInfo("CapValue", "SetCapValue", "GetCapValue", QStringList() << "double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("ComputeNormals", "SetComputeNormals", "GetComputeNormals", QStringList() << "int" , QStringList(), "void", "int");

// ImplicitFunction Input
    pDesc->addConnectionPath(
        new CVtkObjectConnectionPath("ImplicitFunction", IVisSystemNodeConnectionPath::InputPath, "vtkImplicitFunction", 0)
        );
}

CVtkSampleFunction::CVtkSampleFunction() : m_vtkSampleFunction(0)
{
    CVtkSampleFunction::InitializeObjectDesc();
    setVtkObject(vtkSampleFunction::New());
}

CVtkSampleFunction::~CVtkSampleFunction()
{

}

CVtkSampleFunction::OutputScalarType CVtkSampleFunction::outputScalarType() const
{
    switch(m_vtkSampleFunction->GetOutputScalarType())
    {
        case VTK_DOUBLE:return (Double);
        case VTK_FLOAT:return (Float);
        case VTK_LONG:return (Long);
        case VTK_UNSIGNED_LONG:return (UnsignedLong);
        case VTK_INT:return (Int);
        case VTK_UNSIGNED_INT:return (UnsignedInt);
        case VTK_SHORT:return (Short);
        case VTK_UNSIGNED_SHORT:return (UnsignedShort);
        case VTK_CHAR:return (Char);
        case VTK_UNSIGNED_CHAR:return (UnsignedChar);
        default :return (Double);
    }
}

void CVtkSampleFunction::setOutputScalarType(OutputScalarType val)
{
    switch(val)
    {
        case Double:
            m_vtkSampleFunction->SetOutputScalarType(VTK_DOUBLE);
            break;
        case Float:
            m_vtkSampleFunction->SetOutputScalarType(VTK_FLOAT);
            break;
        case Long:
            m_vtkSampleFunction->SetOutputScalarType(VTK_LONG);
            break;
        case UnsignedLong:
            m_vtkSampleFunction->SetOutputScalarType(VTK_UNSIGNED_LONG);
            break;
        case Int:
            m_vtkSampleFunction->SetOutputScalarType(VTK_INT);
            break;
        case UnsignedInt:
            m_vtkSampleFunction->SetOutputScalarType(VTK_UNSIGNED_INT);
            break;
        case Short:
            m_vtkSampleFunction->SetOutputScalarType(VTK_SHORT);
            break;
        case UnsignedShort:
            m_vtkSampleFunction->SetOutputScalarType(VTK_UNSIGNED_SHORT);
            break;
        case Char:
            m_vtkSampleFunction->SetOutputScalarType(VTK_CHAR);
            break;
        case UnsignedChar:
            m_vtkSampleFunction->SetOutputScalarType(VTK_UNSIGNED_CHAR);
            break;
        default :
            m_vtkSampleFunction->SetOutputScalarType(VTK_DOUBLE);
            break;
     }
}


QList<QVariant> CVtkSampleFunction::modelBounds() const
{
    double *val = 0;
    QList<QVariant> list;
    val = m_vtkSampleFunction->GetModelBounds();
    if(val)
    {
        for(int i = 0; i < 6; i++)
            list.append(val[i]);
    }
    return list;
}


void CVtkSampleFunction::setModelBounds(QList<QVariant> val)
{
    double vec[6];
    for(int i=0; i<6; i++)
    {
        if(i >= val.count())
            vec[i] = 0;
        else
            vec[i] = val[i].toDouble();
    }

    m_vtkSampleFunction->SetModelBounds(vec);
}


bool CVtkSampleFunction::isCapping() const
{
    return m_vtkSampleFunction->GetCapping();
}


void CVtkSampleFunction::setCapping(bool val)
{
    m_vtkSampleFunction->SetCapping(val);
}


double CVtkSampleFunction::capValue() const
{
    return m_vtkSampleFunction->GetCapValue();
}


void CVtkSampleFunction::setCapValue(double val)
{
    m_vtkSampleFunction->SetCapValue(val);
}


bool CVtkSampleFunction::isComputeNormals() const
{
    return m_vtkSampleFunction->GetComputeNormals();
}


void CVtkSampleFunction::setComputeNormals(bool val)
{
    m_vtkSampleFunction->SetComputeNormals(val);
}

QList<QVariant> CVtkSampleFunction::sampleDimensions() const
{
    int *val = 0;
    QList<QVariant> list;
    val = m_vtkSampleFunction->GetSampleDimensions();
    if(val)
    {
        for(int i=0; i<3; i++)
            list.append(val[i]);
    }
    return list;
}

void CVtkSampleFunction::setSampleDimensions(QList<QVariant> val)
{
    int vec[6];
    for(int i=0; i<3; i++)
    {
        if(i >= val.count())
            vec[i] = 0;
        else
            vec[i] = val[i].toInt();
    }

    m_vtkSampleFunction->SetSampleDimensions(vec);
}


bool CVtkSampleFunction::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    if(path->pathName() == "ImplicitFunction")
    {
        return m_vtkSampleFunction->GetImplicitFunction() != 0;
    }

    return CVtkImageAlgorithm::hasInput(path);
}

bool CVtkSampleFunction::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    if(path->pathName() == "ImplicitFunction")
    {
        IVtkImplicitFunctionIOData* implicitFunctionData = 0;
        bool success = inputData->queryInterface("IVtkImplicitFunctionIOData", (void**)&implicitFunctionData);
        if(success && implicitFunctionData)
        {
            m_vtkSampleFunction->SetImplicitFunction(implicitFunctionData->getVtkImplicitFunction());
            return true;
        }

        return false;
    }
    return CVtkImageAlgorithm::setInput(path, inputData);
}

bool CVtkSampleFunction::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    if(path->pathName() == "ImplicitFunction")
    {
        IVtkImplicitFunctionIOData* implicitFunctionData = 0;
        bool success = inputData->queryInterface("IVtkImplicitFunctionIOData", (void**)&implicitFunctionData);
        if(success && implicitFunctionData && implicitFunctionData->getVtkImplicitFunction() == m_vtkSampleFunction->GetImplicitFunction())
        {
            m_vtkSampleFunction->SetImplicitFunction(0);
            return true;
        }

        return false;
    }
    return CVtkImageAlgorithm::removeInput(path, inputData);
}
