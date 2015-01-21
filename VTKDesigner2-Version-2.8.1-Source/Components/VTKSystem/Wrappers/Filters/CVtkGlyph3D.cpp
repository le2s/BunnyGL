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
#include "CVtkGlyph3D.h"

DEFINE_VTK_OBJECT(CVtkGlyph3D,CVtkPolyDataFilter,vtkGlyph3D)
{
    pDesc->setNodeClassName("vtkGlyph3D");
    pDesc->setNodeClassDescription("Copy oriented and scaled glyph geometry to every input point");

    pDesc->addConnectionPath(
        new CVtkObjectConnectionPath("Source", IVisSystemNodeConnectionPath::InputPath, "vtkPolyData", 0)
        );

    pDesc->setPropertyInfo("Scaling", "SetScaling", "GetScaling", QStringList() << "int" , QStringList(), "void", "int");
    pDesc->setPropertyInfo("ScaleMode", "SetScaleMode", "GetScaleMode", QStringList() << "int", QStringList(), "void", "int");
    pDesc->setPropertyInfo("ColorMode", "SetColorMode", "GetColorMode", QStringList() << "int", QStringList(), "void", "int");
    pDesc->setPropertyInfo("ScaleFactor", "SetScaleFactor", "GetScaleFactor", QStringList() << "double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("Range", "SetRange", "GetRange", QStringList() << "double" << "double", QStringList(), "void", "double*");
    pDesc->setPropertyInfo("Orient", "SetOrient", "GetOrient", QStringList() << "int" , QStringList(), "void", "int");
    pDesc->setPropertyInfo("Clamping", "SetClamping", "GetClamping", QStringList() << "int" , QStringList(), "void", "int");
    pDesc->setPropertyInfo("VectorMode", "SetVectorMode", "GetVectorMode", QStringList() << "int", QStringList(), "void", "int");
    pDesc->setPropertyInfo("IndexMode", "SetIndexMode", "GetIndexMode", QStringList() << "int", QStringList(), "void", "int");
    pDesc->setPropertyInfo("GeneratePointIds", "SetGeneratePointIds", "GetGeneratePointIds", QStringList() << "int" , QStringList(), "void", "int");
    pDesc->setPropertyInfo("PointIdsName", "SetPointIdsName", "GetPointIdsName", QStringList() << "QString", QStringList(), "void", "QString");
}

CVtkGlyph3D::CVtkGlyph3D() : m_vtkGlyph3D(0), m_emptyPolyData(0)
{
    CVtkGlyph3D::InitializeObjectDesc();
    setVtkObject(vtkGlyph3D::New());
}

CVtkGlyph3D::~CVtkGlyph3D()
{
    if(m_emptyPolyData)
        m_emptyPolyData->Delete();
}

bool CVtkGlyph3D::isScaling() const
{
    return m_vtkGlyph3D->GetScaling();
}


void CVtkGlyph3D::setScaling(bool val)
{
    m_vtkGlyph3D->SetScaling(val);
}


CVtkGlyph3D::ScaleMode CVtkGlyph3D::scaleMode() const
{
    switch(m_vtkGlyph3D->GetScaleMode())
    {
        case VTK_SCALE_BY_SCALAR:return (Scalar);
        case VTK_SCALE_BY_VECTOR:return (Vector);
        case VTK_SCALE_BY_VECTORCOMPONENTS:return (VectorComponents);
        case VTK_DATA_SCALING_OFF:return (ScalingOff);
        default :return (Scalar);
    }
}

void CVtkGlyph3D::setScaleMode(ScaleMode val)
{
    switch(val)
    {
        case Scalar:
            m_vtkGlyph3D->SetScaleMode(VTK_SCALE_BY_SCALAR);
            break;
        case Vector:
            m_vtkGlyph3D->SetScaleMode(VTK_SCALE_BY_VECTOR);
            break;
        case VectorComponents:
            m_vtkGlyph3D->SetScaleMode(VTK_SCALE_BY_VECTORCOMPONENTS);
            break;
        case ScalingOff:
            m_vtkGlyph3D->SetScaleMode(VTK_DATA_SCALING_OFF);
            break;
        default :
            m_vtkGlyph3D->SetScaleMode(VTK_SCALE_BY_SCALAR);
            break;
     }
}


CVtkGlyph3D::ColorMode CVtkGlyph3D::colorMode() const
{
    switch(m_vtkGlyph3D->GetColorMode())
    {
        case VTK_COLOR_BY_SCALE:return (Scale);
        case VTK_COLOR_BY_SCALAR:return (CScalar);
        case VTK_COLOR_BY_VECTOR:return (Vector3D);
        default :return (Scale);
    }
}

void CVtkGlyph3D::setColorMode(ColorMode val)
{
    switch(val)
    {
        case Scale:
            m_vtkGlyph3D->SetColorMode(VTK_COLOR_BY_SCALE);
            break;
        case CScalar:
            m_vtkGlyph3D->SetColorMode(VTK_COLOR_BY_SCALAR);
            break;
        case Vector3D:
            m_vtkGlyph3D->SetColorMode(VTK_COLOR_BY_VECTOR);
            break;
        default :
            m_vtkGlyph3D->SetColorMode(VTK_COLOR_BY_SCALE);
            break;
     }
}


double CVtkGlyph3D::scaleFactor() const
{
    return m_vtkGlyph3D->GetScaleFactor();
}


void CVtkGlyph3D::setScaleFactor(double val)
{
    m_vtkGlyph3D->SetScaleFactor(val);
}


ValueRange CVtkGlyph3D::range() const
{
    return ValueRange(m_vtkGlyph3D->GetRange());
}


void CVtkGlyph3D::setRange(ValueRange val)
{
    m_vtkGlyph3D->SetRange(val.min, val.max);
}


bool CVtkGlyph3D::isOrient() const
{
    return m_vtkGlyph3D->GetOrient();
}


void CVtkGlyph3D::setOrient(bool val)
{
    m_vtkGlyph3D->SetOrient(val);
}


bool CVtkGlyph3D::isClamping() const
{
    return m_vtkGlyph3D->GetClamping();
}


void CVtkGlyph3D::setClamping(bool val)
{
    m_vtkGlyph3D->SetClamping(val);
}


CVtkGlyph3D::VectorMode CVtkGlyph3D::vectorMode() const
{
    switch(m_vtkGlyph3D->GetVectorMode())
    {
        case VTK_USE_VECTOR:return (VVector);
        case VTK_USE_NORMAL:return (Normal);
        case VTK_VECTOR_ROTATION_OFF:return (VectorRotationOff);
        default :return (VVector);
    }
}

void CVtkGlyph3D::setVectorMode(VectorMode val)
{
    switch(val)
    {
        case VVector:
            m_vtkGlyph3D->SetVectorMode(VTK_USE_VECTOR);
            break;
        case Normal:
            m_vtkGlyph3D->SetVectorMode(VTK_USE_NORMAL);
            break;
        case VectorRotationOff:
            m_vtkGlyph3D->SetVectorMode(VTK_VECTOR_ROTATION_OFF);
            break;
        default :
            m_vtkGlyph3D->SetVectorMode(VTK_USE_VECTOR);
            break;
     }
}


CVtkGlyph3D::IndexMode CVtkGlyph3D::indexMode() const
{
    switch(m_vtkGlyph3D->GetIndexMode())
    {
        case VTK_INDEXING_OFF:return (Off);
        case VTK_INDEXING_BY_SCALAR:return (IScalar);
        case VTK_INDEXING_BY_VECTOR:return (IVector);
        default :return (Off);
    }
}

void CVtkGlyph3D::setIndexMode(IndexMode val)
{
    switch(val)
    {
        case Off:
            m_vtkGlyph3D->SetIndexMode(VTK_INDEXING_OFF);
            break;
        case IScalar:
            m_vtkGlyph3D->SetIndexMode(VTK_INDEXING_BY_SCALAR);
            break;
        case IVector:
            m_vtkGlyph3D->SetIndexMode(VTK_INDEXING_BY_VECTOR);
            break;
        default :
            m_vtkGlyph3D->SetIndexMode(VTK_INDEXING_OFF);
            break;
     }
}


bool CVtkGlyph3D::isGeneratePointIds() const
{
    return m_vtkGlyph3D->GetGeneratePointIds();
}


void CVtkGlyph3D::setGeneratePointIds(bool val)
{
    m_vtkGlyph3D->SetGeneratePointIds(val);
}


QString CVtkGlyph3D::pointIdsName() const
{
    return m_vtkGlyph3D->GetPointIdsName();
}


void CVtkGlyph3D::setPointIdsName(QString val)
{
    m_vtkGlyph3D->SetPointIdsName(qPrintable(val));
}

bool CVtkGlyph3D::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    if(path->pathName() == "Source")
        return m_vtkGlyph3D->GetSource() != 0 && m_vtkGlyph3D->GetSource() != m_emptyPolyData;

    return CVtkPolyDataFilter::hasInput(path);
}

bool CVtkGlyph3D::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    if(path->pathName() == "Source")
    {
        IVtkPolyDataIOData* polyData = 0;
        bool success = inputData->queryInterface("IVtkPolyDataIOData", (void**)&polyData);
        if(success && polyData)
        {
            m_vtkGlyph3D->SetSource(polyData->getVtkPolyData());
            return true;
        }

        return false;
    }

    return CVtkPolyDataFilter::setInput(path, inputData);
}

bool CVtkGlyph3D::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    if(path->pathName() == "Source")
    {
        IVtkPolyDataIOData* polyData = 0;
        bool success = inputData->queryInterface("IVtkPolyDataIOData", (void**)&polyData);
        if(success && polyData && m_vtkGlyph3D->GetSource() == polyData->getVtkPolyData())
        {
            if(!m_emptyPolyData)
                m_emptyPolyData = vtkPolyData::New();
            m_vtkGlyph3D->SetSource(m_emptyPolyData);
            return true;
        }

        return false;
    }

    return CVtkPolyDataFilter::removeInput(path, inputData);
}
