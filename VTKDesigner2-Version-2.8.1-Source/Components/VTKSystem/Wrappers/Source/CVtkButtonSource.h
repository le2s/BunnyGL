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

#ifndef C_VTK_BUTTON_SOURCE_H
#define C_VTK_BUTTON_SOURCE_H

#include "CVtkPolyDataAlgorithm.h"
#include "vtkButtonSource.h"
#include "CUserDefinedDataTypes.h"

class CVtkButtonSource : public CVtkPolyDataAlgorithm
{
	Q_OBJECT
	Q_PROPERTY(bool TwoSided READ isTwoSided WRITE setTwoSided)
	DECLARE_VTK_OBJECT

public:
	CVtkButtonSource();
	~CVtkButtonSource();

	void setTwoSided(bool val);
	bool isTwoSided() const;

private:
	vtkButtonSource* m_vtkButtonSource;
};

#endif


