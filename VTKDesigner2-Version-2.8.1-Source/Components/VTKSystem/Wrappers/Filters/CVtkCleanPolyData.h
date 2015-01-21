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

#ifndef C_VTK_CLEAN_POLY_DATA_H
#define C_VTK_CLEAN_POLY_DATA_H

#include "vtkCleanPolyData.h"
#include "CVtkPolyDataFilter.h"

class CVtkCleanPolyData : public CVtkPolyDataFilter
{
    Q_OBJECT
    Q_PROPERTY(bool ToleranceIsAbsolute READ isToleranceIsAbsolute WRITE setToleranceIsAbsolute)
    Q_PROPERTY(bool PieceInvariant READ isPieceInvariant WRITE setPieceInvariant)
    Q_PROPERTY(double Tolerance READ tolerance WRITE setTolerance)
    Q_PROPERTY(double AbsoluteTolerance READ absoluteTolerance WRITE setAbsoluteTolerance)
    Q_PROPERTY(bool ConvertLinesToPoints READ isConvertLinesToPoints WRITE setConvertLinesToPoints)
    Q_PROPERTY(bool ConvertPolysToLines READ isConvertPolysToLines WRITE setConvertPolysToLines)
    Q_PROPERTY(bool ConvertStripsToPolys READ isConvertStripsToPolys WRITE setConvertStripsToPolys)
    Q_PROPERTY(bool PointMerging READ isPointMerging WRITE setPointMerging)
    DECLARE_VTK_OBJECT

public:

    CVtkCleanPolyData();
    ~CVtkCleanPolyData();

    bool isToleranceIsAbsolute() const;
    void setToleranceIsAbsolute(bool val);

    bool isPieceInvariant() const;
    void setPieceInvariant(bool val);

    double tolerance() const;
    void setTolerance(double val);

    double absoluteTolerance() const;
    void setAbsoluteTolerance(double val);

    bool isConvertLinesToPoints() const;
    void setConvertLinesToPoints(bool val);

    bool isConvertPolysToLines() const;
    void setConvertPolysToLines(bool val);

    bool isConvertStripsToPolys() const;
    void setConvertStripsToPolys(bool val);

    bool isPointMerging() const;
    void setPointMerging(bool val);


private:
    vtkCleanPolyData *m_vtkCleanPolyData;
};
#endif//C_VTK_CLEAN_POLY_DATA_H