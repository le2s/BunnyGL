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

#ifndef C_VTK_STREAM_TRACER_H
#define C_VTK_STREAM_TRACER_H

#include "vtkStreamTracer.h"
#include "CVtkPolyDataFilter.h"
#include "UserDefinedDataTypes.h"
#include "CVtkGenericStreamTracer.h"
/*
enum Units
  {
    TIME_UNIT,
    LENGTH_UNIT,
    CELL_LENGTH_UNIT
  };

  enum Solvers
  {
    RUNGE_KUTTA2,
    RUNGE_KUTTA4,
    RUNGE_KUTTA45,
    NONE,
    UNKNOWN
  };

  enum ReasonForTermination
  {
    OUT_OF_DOMAIN = vtkInitialValueProblemSolver::OUT_OF_DOMAIN,
    NOT_INITIALIZED = vtkInitialValueProblemSolver::NOT_INITIALIZED ,
    UNEXPECTED_VALUE = vtkInitialValueProblemSolver::UNEXPECTED_VALUE,
    OUT_OF_TIME = 4,
    OUT_OF_STEPS = 5,
    STAGNATION = 6
  };

    enum
  {
    FORWARD,
    BACKWARD,
    BOTH
  };
*/
class CVtkStreamTracer : public CVtkPolyDataFilter
{
    Q_OBJECT
    Q_ENUMS(Solvers)
    Q_ENUMS(Units)
    Q_ENUMS(Direction)
    Q_PROPERTY(Point3D StartPosition READ startPosition WRITE setStartPosition)
    Q_PROPERTY(Solvers IntegratorType READ integratorType WRITE setIntegratorType)
    Q_PROPERTY(Units MaximumPropagation READ maximumPropagationUnit WRITE setMaximumPropagationUnit)
    Q_PROPERTY(Units MinimumIntegrationStep READ minimumIntegrationStepUnit WRITE setMinimumIntegrationStepUnit)
    Q_PROPERTY(Units MaximumIntegrationStep READ maximumIntegrationStepUnit WRITE setMaximumIntegrationStepUnit)
    Q_PROPERTY(Units InitialIntegrationStep READ initialIntegrationStepUnit WRITE setInitialIntegrationStepUnit)
    Q_PROPERTY(double MaximumError READ maximumError WRITE setMaximumError)
    Q_PROPERTY(int MaximumNumberOfSteps READ maximumNumberOfSteps WRITE setMaximumNumberOfSteps)
    Q_PROPERTY(double TerminalSpeed READ terminalSpeed WRITE setTerminalSpeed)
    Q_PROPERTY(Direction IntegrationDirection READ integrationDirection WRITE setIntegrationDirection)
    Q_PROPERTY(bool ComputeVorticity READ isComputeVorticity WRITE setComputeVorticity)
    Q_PROPERTY(double RotationScale READ rotationScale WRITE setRotationScale)
    DECLARE_VTK_OBJECT

public:
    enum Solvers
    {
        RK2=RUNGE_KUTTA2,
        RK4=RUNGE_KUTTA4,
        RK5=RUNGE_KUTTA45
    };
    enum Units
    {
        Time=TIME_UNIT,
        Length=LENGTH_UNIT,
        CellLength=CELL_LENGTH_UNIT
    };

    enum Direction
    {
        Forward=FORWARD,
        Backward=BACKWARD,
        Both=BOTH
    };

    CVtkStreamTracer();
    ~CVtkStreamTracer();

    Point3D startPosition() const;
    void setStartPosition(Point3D val);

    Solvers integratorType() const;
    void setIntegratorType(Solvers val);

    Units maximumPropagationUnit() const;
    void setMaximumPropagationUnit(Units val);

    Units minimumIntegrationStepUnit() const;
    void setMinimumIntegrationStepUnit(Units val);

    Units maximumIntegrationStepUnit() const;
    void setMaximumIntegrationStepUnit(Units val);

    Units initialIntegrationStepUnit() const;
    void setInitialIntegrationStepUnit(Units val);

    double maximumError() const;
    void setMaximumError(double val);

    int maximumNumberOfSteps() const;
    void setMaximumNumberOfSteps(int val);

    double terminalSpeed() const;
    void setTerminalSpeed(double val);

    Direction integrationDirection() const;
    void setIntegrationDirection(Direction val);

    bool isComputeVorticity() const;
    void setComputeVorticity(bool val);

    double rotationScale() const;
    void setRotationScale(double val);


private:
    vtkStreamTracer *m_vtkStreamTracer;
};
#endif//C_VTK_GENERIC_STREAM_TRACER_H