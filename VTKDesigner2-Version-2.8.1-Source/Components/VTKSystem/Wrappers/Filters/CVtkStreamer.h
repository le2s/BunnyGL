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

#ifndef C_VTK_STREAMER_H
#define C_VTK_STREAMER_H

#include "vtkStreamer.h"
#include "CVtkPolyDataFilter.h"
#include "UserDefinedDataTypes.h"

class CVtkStreamer : public CVtkPolyDataFilter
{
    Q_OBJECT
    Q_ENUMS(Direction)
    Q_PROPERTY(int NumberOfThreads READ numberOfThreads WRITE setNumberOfThreads)
    Q_PROPERTY(Point3D StartPosition READ startPosition WRITE setStartPosition)
    Q_PROPERTY(double MaximumPropagationTime READ maximumPropagationTime WRITE setMaximumPropagationTime)
    Q_PROPERTY(Direction IntegrationDirection READ integrationDirection WRITE setIntegrationDirection)
    Q_PROPERTY(double IntegrationStepLength READ integrationStepLength WRITE setIntegrationStepLength)
    Q_PROPERTY(bool SpeedScalars READ isSpeedScalars WRITE setSpeedScalars)
    Q_PROPERTY(bool OrientationScalars READ isOrientationScalars WRITE setOrientationScalars)
    Q_PROPERTY(double TerminalSpeed READ terminalSpeed WRITE setTerminalSpeed)
    Q_PROPERTY(bool Vorticity READ isVorticity WRITE setVorticity)
    DECLARE_VTK_OBJECT

public:
    enum Direction
    {
        Forward=VTK_INTEGRATE_FORWARD,
        Backword=VTK_INTEGRATE_BACKWARD,
        BothDirections=VTK_INTEGRATE_BOTH_DIRECTIONS
    };

    CVtkStreamer();
    ~CVtkStreamer();

    int numberOfThreads() const;
    void setNumberOfThreads(int val);

    Point3D startPosition() const;
    void setStartPosition(Point3D val);

    double maximumPropagationTime() const;
    void setMaximumPropagationTime(double val);

    Direction integrationDirection() const;
    void setIntegrationDirection(Direction val);

    double integrationStepLength() const;
    void setIntegrationStepLength(double val);

    bool isSpeedScalars() const;
    void setSpeedScalars(bool val);

    bool isOrientationScalars() const;
    void setOrientationScalars(bool val);

    double terminalSpeed() const;
    void setTerminalSpeed(double val);

    bool isVorticity() const;
    void setVorticity(bool val);

protected:
    bool hasInput(IVisSystemNodeConnectionPath* path);
    bool setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData);
    bool removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData);

private:
    vtkStreamer *m_vtkStreamer;
};
#endif//C_VTK_STREAMER_H