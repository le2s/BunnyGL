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

#include "CVtkActor.h"
#include "IVtkMapperIOData.h"
#include "IVtkTextureIOData.h"
#include "IVtkPropertyIOData.h"
#include "IVtkAbstractTransformIOData.h"
#include "vtkProperty.h"
#include "vtkLinearTransform.h"

DEFINE_VTK_OBJECT(CVtkActor, CVtkProp3D, vtkActor)
{
    pDesc->setNodeClassCategory("Rendering");
    pDesc->setNodeClassDescription("Generic Actor");
    pDesc->setNodeClassName("vtkActor");

    // Mapper Input
    pDesc->addConnectionPath(
        new CVtkObjectConnectionPath("Mapper", IVisSystemNodeConnectionPath::InputPath, "vtkMapper", 0)
        );

    // Texture Input
    pDesc->addConnectionPath(
        new CVtkObjectConnectionPath("Texture", IVisSystemNodeConnectionPath::InputPath, "vtkTexture", 0)
        );

    // Property Input
    pDesc->addConnectionPath(
        new CVtkObjectConnectionPath("Property", IVisSystemNodeConnectionPath::InputPath, "vtkProperty", 0)
        );

    pDesc->setPropertyInfo("Color", "GetProperty()->SetColor", "GetProperty()->GetColor", QStringList() << "color_double" << "color_double" << "color_double", QStringList(),"void", "double*");
    pDesc->setPropertyInfo("Opacity", "GetProperty()->SetOpacity", "GetProperty()->GetOpacity", QStringList() << "double", QStringList(), "void", "double");
}

CVtkActor::CVtkActor() : m_vtkActor(0), m_hasProperty(false)
{
    CVtkActor::InitializeObjectDesc();
    setVtkObject(vtkActor::New());
}

CVtkActor::~CVtkActor()
{

}


QColor CVtkActor::color() const
{
    QColor retColor;
    double* color = m_vtkActor->GetProperty()->GetColor();
    retColor.setRgbF(color[0], color[1], color[2]);
    return retColor;
}

void CVtkActor::setColor(QColor c)
{
    m_vtkActor->GetProperty()->SetColor(c.redF(), c.greenF(), c.blueF());
}

void CVtkActor::setOpacity(double val)
{
    m_vtkActor->GetProperty()->SetOpacity(val);
}

double CVtkActor::opacity() const
{
    return m_vtkActor->GetProperty()->GetOpacity();
}

bool CVtkActor::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    if(path->pathName() == "Mapper")
    {
        return m_vtkActor->GetMapper() != 0;
    }

    if(path->pathName() == "Texture")
    {
        return m_vtkActor->GetTexture() != 0;
    }

    if(path->pathName() == "Property")
    {
        return m_hasProperty;
    }

    return CVtkProp3D::hasInput(path);
}

bool CVtkActor::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    if(path->pathName() == "Mapper")
    {
        IVtkMapperIOData* mapperData = 0;
        bool success = inputData->queryInterface("IVtkMapperIOData", (void**)&mapperData);
        if(success && mapperData)
        {
            m_vtkActor->SetMapper(mapperData->getVtkMapper());
            return true;
        }

        return false;
    }

    if(path->pathName() == "Texture")
    {
        IVtkTextureIOData* texData = 0;
        bool success = inputData->queryInterface("IVtkTextureIOData", (void**)&texData);
        if(success && texData)
        {
            m_vtkActor->SetTexture(texData->getVtkTexture());
            return true;
        }

        return false;
    }

    if(path->pathName() == "Property")
    {
        IVtkPropertyIOData* propData = 0;
        bool success = inputData->queryInterface("IVtkPropertyIOData", (void**)&propData);
        if(success && propData)
        {
            m_vtkActor->SetProperty(propData->getVtkProperty());
            m_hasProperty = true;
            return true;
        }

        return false;
    }

    return CVtkProp3D::setInput(path, inputData);
}

bool CVtkActor::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    if(path->pathName() == "Mapper")
    {
        IVtkMapperIOData* mapperData = 0;
        bool success = inputData->queryInterface("IVtkMapperIOData", (void**)&mapperData);
        if(success && mapperData && mapperData->getVtkMapper() == m_vtkActor->GetMapper())
        {
            m_vtkActor->SetMapper(0);
            return true;
        }

        return false;
    }

    if(path->pathName() == "Texture")
    {
        IVtkTextureIOData* texData = 0;
        bool success = inputData->queryInterface("IVtkTextureIOData", (void**)&texData);
        if(success && texData && texData->getVtkTexture() == m_vtkActor->GetTexture())
        {
            m_vtkActor->SetTexture(0);
            return true;
        }

        return false;
    }

    if(path->pathName() == "Property")
    {
        IVtkPropertyIOData* propData = 0;
        bool success = inputData->queryInterface("IVtkPropertyIOData", (void**)&propData);
        if(success && propData && propData->getVtkProperty() == m_vtkActor->GetProperty())
        {
            m_vtkActor->SetProperty(m_vtkActor->MakeProperty());
            m_hasProperty = false;
            return true;
        }

        return false;
    }

    return CVtkProp3D::removeInput(path, inputData);
}

bool CVtkActor::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    // Copied from parent class.
    if(path->pathName() == "Prop3D")
    {
        m_actorData.setActor(m_vtkActor);
        *outputData = &m_actorData;
        return true;
    }

    return CVtkProp3D::fetchOutput(path, outputData);
}

bool CVtkActor::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
{
    Q_UNUSED(path);
    Q_UNUSED(outputData);
    return CVtkProp3D::outputDerefed(path, outputData);
}

