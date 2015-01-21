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

#ifndef C_VTK_AXIS_ACTOR2D_H
#define C_VTK_AXIS_ACTOR2D_H

#include "CVtkActor2D.h"
#include "vtkAxisActor2D.h"
#include "vtkTextProperty.h"

class CVtkAxisActor2D : public CVtkActor2D
{
    Q_OBJECT
    Q_PROPERTY(QList<QVariant> Range READ range WRITE setRange)
    Q_PROPERTY(QList<QVariant> Point1 READ point1 WRITE setPoint1)
    Q_PROPERTY(QList<QVariant> Point2 READ point2 WRITE setPoint2)
    Q_PROPERTY(int NumberOfLabels READ numberOfLabels WRITE setNumberOfLabels)
    Q_PROPERTY(bool AdjustLabels READ isAdjustLabels WRITE setAdjustLabels)
    Q_PROPERTY(QString LabelFormat READ labelFormat WRITE setLabelFormat)
    Q_PROPERTY(QString Title READ title WRITE setTitle)
    Q_PROPERTY(int TickLength READ tickLength WRITE setTickLength)
    Q_PROPERTY(int TickOffset READ tickOffset WRITE setTickOffset)
    Q_PROPERTY(bool TickVisible READ isTickVisible WRITE setTickVisible)
    Q_PROPERTY(bool TitleVisible READ isTitleVisible WRITE setTitleVisible)
    Q_PROPERTY(bool LabelVisible READ isLabelVisible WRITE setLabelVisible)
    Q_PROPERTY(double FontFactor READ fontFactor WRITE setFontFactor)
    Q_PROPERTY(double LabelFactor READ labelFactor WRITE setFontFactor)
    Q_ENUMS(FontFamily)
    Q_PROPERTY(QColor LabelColor READ labelColor WRITE setLabelColor)
    Q_PROPERTY(QColor TitleColor READ titleColor WRITE setTitleColor)
    Q_PROPERTY(FontFamily LabelFont READ labelFont WRITE setLabelFont)
    Q_PROPERTY(FontFamily TitleFont READ titleFont WRITE setTitleFont)
    DECLARE_VTK_OBJECT

public:
    CVtkAxisActor2D();
    ~CVtkAxisActor2D();

    // vtkAxisActor2D properties
    void setRange(QList<QVariant> range);
    QList<QVariant> range() const;

    void setPoint1(QList<QVariant> pt);
    QList<QVariant> point1() const;

    void setPoint2(QList<QVariant> pt);
    QList<QVariant> point2() const;

    void setNumberOfLabels(int val);
    int numberOfLabels() const;

    void setAdjustLabels(bool val);
    bool isAdjustLabels() const;

    void setLabelFormat(QString format);
    QString labelFormat() const;

    void setTitle(QString title);
    QString title() const;

    void setTickLength(int length);
    int tickLength() const;

    void setTickOffset(int off);
    int tickOffset() const;

    void setTickVisible(bool val);
    bool isTickVisible() const;

    void setLabelVisible(bool val);
    bool isLabelVisible() const;

    void setTitleVisible(bool val);
    bool isTitleVisible() const;

    void setFontFactor(double val);
    double fontFactor() const;

    void setLabelFactor(double val);
    double labelFactor() const;

    enum FontFamily
    {
        Arial = VTK_ARIAL,
        Courier = VTK_COURIER,
        Times = VTK_TIMES
    };

    void setLabelColor(QColor color);
    QColor labelColor() const;

    void setLabelFont(FontFamily family);
    FontFamily labelFont() const;

    void setTitleColor(QColor color);
    QColor titleColor() const;

    void setTitleFont(FontFamily family);
    FontFamily titleFont() const;

protected:
    bool hasInput(IVisSystemNodeConnectionPath* path);
    bool setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData);
    bool removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData);

protected:
    vtkAxisActor2D* m_vtkAxisActor2D;
};

#endif
