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

#include "CVtkOpenHapticsProperty.h"
#include "COpenHapticsVTKComponent.h"

DEFINE_VIS_NODE(CVtkOpenHapticsProperty, CGenericVisNodeBase)
{
	pDesc->setNodeClassCategory("Open Haptics + VTK");
	pDesc->setNodeClassName("vtkOpenHapticsProperty");
	pDesc->setNodeClassDescription("Open Haptics Property");
    pDesc->setNodeIcon( COpenHapticsVTKComponent::instance().nodeIcon() );

    // Property Output
    pDesc->addConnectionPath(
        new CGenericVisNodeConnectionPath("OpenHapticsProperty", IVisSystemNodeConnectionPath::OutputPath, "vtkOpenHapticsProperty", 0)
        );
}

CVtkOpenHapticsProperty::CVtkOpenHapticsProperty() : m_vtkOpenHapticsProperty(0)
{
    CVtkOpenHapticsProperty::InitializeNodeDesc();
    m_vtkOpenHapticsProperty = vtkOpenHapticsProperty::New();
    nodeData()["vtkObject"] = qVariantFromValue<vtkObject*>(m_vtkOpenHapticsProperty);
}

CVtkOpenHapticsProperty::~CVtkOpenHapticsProperty()
{
    m_vtkOpenHapticsProperty->Delete();
}

void CVtkOpenHapticsProperty::setApplyToFace(CVtkOpenHapticsProperty::Face f)
{
    m_vtkOpenHapticsProperty->SetApplyToFace( (int)f );
}

CVtkOpenHapticsProperty::Face CVtkOpenHapticsProperty::applyToFace() const
{
    return (CVtkOpenHapticsProperty::Face)(m_vtkOpenHapticsProperty->GetApplyToFace());
}

void CVtkOpenHapticsProperty::setStiffness(double val)
{
    m_vtkOpenHapticsProperty->SetStiffness(val);
}

double CVtkOpenHapticsProperty::stiffness() const
{
    return m_vtkOpenHapticsProperty->GetStiffness();
}

void CVtkOpenHapticsProperty::setDamping(double val)
{
    m_vtkOpenHapticsProperty->SetDamping(val);
}

double CVtkOpenHapticsProperty::damping() const
{
    return m_vtkOpenHapticsProperty->GetDamping();
}

void CVtkOpenHapticsProperty::setStaticFriction(double val)
{
    m_vtkOpenHapticsProperty->SetStaticFriction(val);
}

double CVtkOpenHapticsProperty::staticFriction() const
{
    return m_vtkOpenHapticsProperty->GetStaticFriction();
}

void CVtkOpenHapticsProperty::setDynamicFriction(double val)
{
    m_vtkOpenHapticsProperty->SetDynamicFriction(val);
}

double CVtkOpenHapticsProperty::dynamicFriction() const
{
    return m_vtkOpenHapticsProperty->GetDynamicFriction();
}

void CVtkOpenHapticsProperty::setPopthrough(double val)
{
    m_vtkOpenHapticsProperty->SetPopthrough(val);
}

double CVtkOpenHapticsProperty::popthrough() const
{
    return m_vtkOpenHapticsProperty->GetPopthrough();
}

void CVtkOpenHapticsProperty::setTouchModel(CVtkOpenHapticsProperty::TouchModel val)
{
    m_vtkOpenHapticsProperty->SetTouchModel(val);
}

CVtkOpenHapticsProperty::TouchModel CVtkOpenHapticsProperty::touchModel() const
{
    return CVtkOpenHapticsProperty::TouchModel(m_vtkOpenHapticsProperty->GetTouchModel());
}

void CVtkOpenHapticsProperty::setSnapDistance(double val)
{
    m_vtkOpenHapticsProperty->SetSnapDistance(val);
}

double CVtkOpenHapticsProperty::snapDistance() const
{
    return m_vtkOpenHapticsProperty->GetSnapDistance();
}

bool CVtkOpenHapticsProperty::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
	if(!path || !outputData)
		return false;

	if(path->pathName() == "OpenHapticsProperty")
	{
		*outputData = this;
		return true;
	}

	return false;
}

bool CVtkOpenHapticsProperty::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
{
	Q_UNUSED(path);
	Q_UNUSED(outputData);
	return true;
}

GCF_BEGIN_QUERY_TABLE(CVtkOpenHapticsProperty)
	GCF_IMPLEMENTS(IVtkOpenHapticsPropertyIOData)
GCF_END_QUERY_TABLE(CVtkOpenHapticsProperty)

void* CVtkOpenHapticsProperty::dataPointer()
{
	return m_vtkOpenHapticsProperty;
}

QString CVtkOpenHapticsProperty::dataTypeName()
{
	return "vtkOpenHapticsProperty";
}

vtkOpenHapticsProperty* CVtkOpenHapticsProperty::getVtkOpenHapticsProperty()
{
	return m_vtkOpenHapticsProperty;
}

