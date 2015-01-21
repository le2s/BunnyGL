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
#ifndef C_VTK_BROWNIAN_POINTS_H
#define C_VTK_BROWNIAN_POINTS_H

#include "vtkBrownianPoints.h"
#include "CVtkDataSetFilter.h"

class CVtkBrownianPoints : public CVtkDataSetFilter
{
    Q_OBJECT
    Q_PROPERTY(double MinimumSpeed READ minimumSpeed WRITE setMinimumSpeed)
    Q_PROPERTY(double MaximumSpeed READ maximumSpeed WRITE setMaximumSpeed)
    DECLARE_VTK_OBJECT

public:
    CVtkBrownianPoints();
    ~CVtkBrownianPoints();

    double minimumSpeed() const;
    void setMinimumSpeed(double val);

    double maximumSpeed() const;
    void setMaximumSpeed(double val);

private:
    vtkBrownianPoints *m_vtkBrownianPoints;
};
#endif//C_VTK_BROWNIAN_POINTS_H