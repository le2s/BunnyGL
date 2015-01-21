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

#ifndef C_VTK_HYPER_STREAM_LINE_H
#define C_VTK_HYPER_STREAM_LINE_H

#include "CVtkPolyDataFilter.h"
#include "vtkHyperStreamline.h"
#include "UserDefinedDataTypes.h"

class CVtkHyperStreamline : public CVtkPolyDataFilter
{
    Q_OBJECT
    Q_ENUMS(IntegrationEigenvector)
    Q_ENUMS(IntegrationDirection)
    Q_PROPERTY(Point3D StartPosition READ startPosition WRITE setStartPosition)
    Q_PROPERTY(IntegrationEigenvector IntegrationEigenvector READ integrationEigenvector WRITE setIntegrationEigenvector)
    Q_PROPERTY(double IntegrationStepLength READ integrationStepLength WRITE setIntegrationStepLength)
    Q_PROPERTY(double StepLength READ stepLength WRITE setStepLength)
    Q_PROPERTY(IntegrationDirection IntegrationDirection READ integrationDirection WRITE setIntegrationDirection)
    Q_PROPERTY(double TerminalEigenvalue READ terminalEigenvalue WRITE setTerminalEigenvalue)
    Q_PROPERTY(int NumberOfSides READ numberOfSides WRITE setNumberOfSides)
    Q_PROPERTY(double Radius READ radius WRITE setRadius)
    Q_PROPERTY(bool LogScaling READ isLogScaling WRITE setLogScaling)
    DECLARE_VTK_OBJECT

public:
    CVtkHyperStreamline();
    ~CVtkHyperStreamline();

    enum IntegrationEigenvector
    {
        Major =  VTK_INTEGRATE_MAJOR_EIGENVECTOR,
        Medium = VTK_INTEGRATE_MEDIUM_EIGENVECTOR,
        Minor =  VTK_INTEGRATE_MINOR_EIGENVECTOR
    };

    enum IntegrationDirection
    {
        Forward = VTK_INTEGRATE_FORWARD,
        Backward = VTK_INTEGRATE_BACKWARD,
        BothDirections = VTK_INTEGRATE_BOTH_DIRECTIONS
    };

    Point3D startPosition() const;
    void setStartPosition(Point3D p);

    IntegrationEigenvector integrationEigenvector() const;
    void setIntegrationEigenvector(IntegrationEigenvector val);

    double integrationStepLength() const;
    void setIntegrationStepLength(double val);

    double stepLength() const;
    void setStepLength(double val);

    IntegrationDirection integrationDirection() const;
    void setIntegrationDirection(IntegrationDirection val);

    double terminalEigenvalue() const;
    void setTerminalEigenvalue(double val);

    int numberOfSides() const;
    void setNumberOfSides(int val);

    double radius() const;
    void setRadius(double val);

    bool isLogScaling() const;
    void setLogScaling(bool val);

private:
    vtkHyperStreamline *m_vtkHyperStreamline;

};

#endif