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


#include "vtkOpenHapticsProperty.h"
#include "vtkOpenHapticsRenderer.h"

#include "HL/hl.h"
#include "HD/hd.h"

vtkOpenHapticsProperty* vtkOpenHapticsProperty::New()
{
	return new vtkOpenHapticsProperty;
}

vtkOpenHapticsProperty::vtkOpenHapticsProperty()
{
	this->ApplyToFace = VTK_FRONT_FACE;
	this->Stiffness = 0.6;
	this->Damping = 0.6;
	this->StaticFriction = 0.2;
	this->DynamicFriction = 0.3;
	this->Popthrough = 0.5;
	this->TouchModel = VTK_TOUCH_MODEL_CONTACT;
	this->SnapDistance = 1.5;
}

vtkOpenHapticsProperty::~vtkOpenHapticsProperty()
{

}

void vtkOpenHapticsProperty::Render(vtkOpenHapticsRenderer *ren)
{
	if(!ren)
		return;

	HLenum face = HL_FRONT;

	switch(this->ApplyToFace)
	{
	case VTK_FRONT_FACE: face = HL_FRONT; break;
	case VTK_BACK_FACE: face = HL_BACK; break;
	case VTK_FRONT_AND_BACK_FACE: face = HL_FRONT_AND_BACK; break;
	}

	hlMaterialf(face, HL_STIFFNESS, (float)this->Stiffness);
	hlMaterialf(face, HL_DAMPING, (float)this->Damping);
	hlMaterialf(face, HL_STATIC_FRICTION, (float)this->StaticFriction);
	hlMaterialf(face, HL_DYNAMIC_FRICTION, (float)this->DynamicFriction);
	hlMaterialf(face, HL_POPTHROUGH, (float)this->Popthrough);

	if(this->TouchModel == VTK_TOUCH_MODEL_CONSTRAINT)
		hlTouchModel(HL_CONSTRAINT);
	else if(this->TouchModel == VTK_TOUCH_MODEL_CONTACT)
		hlTouchModel(HL_CONTACT);
	hlTouchModelf(HL_SNAP_DISTANCE, this->SnapDistance);
    hlTouchableFace(face);
}

