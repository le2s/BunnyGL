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

#ifndef C_VTK_OPEN_HAPTICS_PROPERTY_H
#define C_VTK_OPEN_HAPTICS_PROPERTY_H

#include "UserDefinedDataTypes.h"
#include "CGenericVisNodeBase.h"
#include "vtkOpenHapticsProperty.h"
#include "IVtkOpenHapticsPropertyIOData.h"

struct CVtkOpenHapticsPropertyData;
class CVtkOpenHapticsProperty : public CGenericVisNodeBase, virtual public IVtkOpenHapticsPropertyIOData
{
    Q_OBJECT
	Q_ENUMS(Face TouchModel);
	Q_PROPERTY(Face ApplyToFace READ applyToFace WRITE setApplyToFace)
	Q_PROPERTY(double Stiffness READ stiffness WRITE setStiffness)
	Q_PROPERTY(double StaticFriction READ staticFriction WRITE setStaticFriction)
	Q_PROPERTY(double DynamicFriction READ dynamicFriction WRITE setDynamicFriction)
	Q_PROPERTY(double Popthrough READ popthrough WRITE setPopthrough)
    Q_PROPERTY(TouchModel TouchModel READ touchModel WRITE setTouchModel)
    Q_PROPERTY(double SnapDistance READ snapDistance WRITE setSnapDistance)
	DECLARE_VIS_NODE

public:
	enum Face
	{
		FrontFace = VTK_FRONT_FACE,
		BackFace = VTK_BACK_FACE,
		FrontAndBackFace = VTK_FRONT_AND_BACK_FACE
	};

    enum TouchModel
    {
        Constraint = VTK_TOUCH_MODEL_CONSTRAINT,
        Contact = VTK_TOUCH_MODEL_CONTACT
    };

	CVtkOpenHapticsProperty();
	~CVtkOpenHapticsProperty();

	void setApplyToFace(Face f);
	Face applyToFace() const;

	void setStiffness(double val);
	double stiffness() const;

	void setDamping(double val);
	double damping() const;

	void setStaticFriction(double val);
	double staticFriction() const;

	void setDynamicFriction(double val);
	double dynamicFriction() const;

	void setPopthrough(double val);
	double popthrough() const;

    void setTouchModel(TouchModel val);
    TouchModel touchModel() const;

    void setSnapDistance(double val);
    double snapDistance() const;

protected:
    bool fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData);
    bool outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData);

public:
	// IQueryable implementation
	bool queryInterface(QString ifaceName, void** ifacePtr);
	bool destroySelf();

	// IVisSystemNodeIOData implementation
	void* dataPointer();
	QString dataTypeName();

	// IVtkOpenHapticsPropertyIOData implementation
	vtkOpenHapticsProperty* getVtkOpenHapticsProperty();

private:
    vtkOpenHapticsProperty* m_vtkOpenHapticsProperty;
};

#endif

