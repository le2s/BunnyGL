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

#include "CVtkAxisActor2D.h"
#include "IVtkMapper2DIOData.h"

DEFINE_VTK_OBJECT(CVtkAxisActor2D, CVtkActor2D, vtkAxisActor2D)
{
    pDesc->setNodeClassCategory("Rendering");
    pDesc->setNodeClassName("vtkAxisActor2D");
    
    pDesc->setPropertyInfo("Range", "SetRange", "GetRange", QStringList() << "double" << "double", QStringList(), "void", "double*");
    pDesc->setPropertyInfo("Point1", "SetPoint1", "GetPoint1", QStringList() << "double" << "double", QStringList(), "void", "double*");
    pDesc->setPropertyInfo("Point2", "SetPoint2", "GetPoint2", QStringList() << "double" << "double", QStringList(), "void", "double*");
    pDesc->setPropertyInfo("AdjustLabels", "SetAdjustLabels", "GetAdjustLabels", QStringList() << "bool", QStringList(), "void", "bool");
    pDesc->setPropertyInfo("NumberOfLabels", "SetNumberOfLabels", "GetNumberOfLabels", QStringList() << "int", QStringList(), "void", "int");
    pDesc->setPropertyInfo("LabelFormat", "SetLabelFormat", "GetLabelFormat", QStringList() << "QString", QStringList(), "void", "QString");
    pDesc->setPropertyInfo("Title", "SetTitle", "GetTitle", QStringList() << "QString", QStringList(), "void", "QString");
    pDesc->setPropertyInfo("NumberOfLabels", "SetNumberOfLabels", "GetNumberOfLabels", QStringList() << "int", QStringList(), "void", "int");
    pDesc->setPropertyInfo("TickLength", "SetTickLength", "GetTickLength", QStringList() << "int", QStringList(), "void", "int");
    pDesc->setPropertyInfo("TickOffset", "SetTickOffset", "GetTickOffset", QStringList() << "int", QStringList(), "void", "int");
    pDesc->setPropertyInfo("TickVisible", "SetTickVisibility", "GetTickVisibility", QStringList() << "bool", QStringList(), "void", "bool");
    pDesc->setPropertyInfo("TitleVisible", "SetTitleVisibility", "GetTitleVisibility", QStringList() << "bool", QStringList(), "void", "bool");
    pDesc->setPropertyInfo("LabelVisible", "SetLabelVisibility", "GetLabelVisibility", QStringList() << "bool", QStringList(), "void", "bool");
    pDesc->setPropertyInfo("FontFactor", "SetFontFactor", "GetFontFactor", QStringList() << "double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("LabelColor", "GetLabelTextProperty()->SetColor", "GetLabelTextProperty()->GetColor", QStringList() << "color_double" << "color_double" << "color_double", QStringList(),"void", "double*");
    pDesc->setPropertyInfo("TitleColor", "GetTitleTextProperty()->SetColor", "GetTitleTextProperty()->GetColor", QStringList() << "color_double" << "color_double" << "color_double", QStringList(),"void", "double*");    
    pDesc->setPropertyInfo("LabelFont", "GetLabelTextProperty()->SetFontFamily", "GetLabelTextProperty()->GetFontFamily", QStringList() << "int", QStringList(), "void", "int");
    pDesc->setPropertyInfo("TitleFont", "GetTitleTextProperty()->SetFontFamily", "GetTitleTextProperty()->GetFontFamily", QStringList() << "int", QStringList(), "void", "int");
}

CVtkAxisActor2D::CVtkAxisActor2D() : CVtkActor2D(false), m_vtkAxisActor2D(0)
{
    CVtkAxisActor2D::InitializeObjectDesc();
    setVtkObject(vtkAxisActor2D::New());
}

CVtkAxisActor2D::~CVtkAxisActor2D()
{

}

void CVtkAxisActor2D::setRange(QList<QVariant> range)
{
    m_vtkAxisActor2D->SetRange( range[0].toDouble(), range[1].toDouble() );
}

QList<QVariant> CVtkAxisActor2D::range() const
{
    double* r = m_vtkAxisActor2D->GetRange();
    QList<QVariant> ret = QList<QVariant>() << r[0] << r[1];
    return ret;
}

void CVtkAxisActor2D::setPoint1(QList<QVariant> pt)
{
    m_vtkAxisActor2D->SetPoint1( pt[0].toDouble(), pt[1].toDouble() );
}

QList<QVariant> CVtkAxisActor2D::point1() const
{
    double* pt = m_vtkAxisActor2D->GetPoint1();
    return QList<QVariant>() << pt[0] << pt[1];
}

void CVtkAxisActor2D::setPoint2(QList<QVariant> pt)
{
    m_vtkAxisActor2D->SetPoint2( pt[0].toDouble(), pt[1].toDouble() );
}

QList<QVariant> CVtkAxisActor2D::point2() const
{
    double* pt = m_vtkAxisActor2D->GetPoint2();
    return QList<QVariant>() << pt[0] << pt[1];
}

void CVtkAxisActor2D::setNumberOfLabels(int val)
{
    m_vtkAxisActor2D->SetNumberOfLabels(val);
}

int CVtkAxisActor2D::numberOfLabels() const
{
    return m_vtkAxisActor2D->GetNumberOfLabels();
}

void CVtkAxisActor2D::setAdjustLabels(bool val)
{
    m_vtkAxisActor2D->SetAdjustLabels(val);
}

bool CVtkAxisActor2D::isAdjustLabels() const
{
    return m_vtkAxisActor2D->GetAdjustLabels();
}

void CVtkAxisActor2D::setLabelFormat(QString format)
{
    m_vtkAxisActor2D->SetLabelFormat( qPrintable(format) );
}

QString CVtkAxisActor2D::labelFormat() const
{
    return QString("%1").arg( m_vtkAxisActor2D->GetLabelFormat() );
}

void CVtkAxisActor2D::setTitle(QString title)
{
    m_vtkAxisActor2D->SetTitle( qPrintable(title) );
}

QString CVtkAxisActor2D::title() const
{
    return QString("%1").arg( m_vtkAxisActor2D->GetTitle() );
}

void CVtkAxisActor2D::setTickLength(int length)
{
    m_vtkAxisActor2D->SetTickLength( length );
}

int CVtkAxisActor2D::tickLength() const
{
    return m_vtkAxisActor2D->GetTickLength();
}

void CVtkAxisActor2D::setTickOffset(int off)
{
    m_vtkAxisActor2D->SetTickOffset( off );
}

int CVtkAxisActor2D::tickOffset() const
{
    return m_vtkAxisActor2D->GetTickOffset();
}

void CVtkAxisActor2D::setTickVisible(bool val)
{
    m_vtkAxisActor2D->SetTickVisibility( val );
}

bool CVtkAxisActor2D::isTickVisible() const
{
    return m_vtkAxisActor2D->GetTickVisibility();
}

void CVtkAxisActor2D::setLabelVisible(bool val)
{
    m_vtkAxisActor2D->SetLabelVisibility(val);
}

bool CVtkAxisActor2D::isLabelVisible() const
{
    return m_vtkAxisActor2D->GetLabelVisibility();
}

void CVtkAxisActor2D::setTitleVisible(bool val)
{
    m_vtkAxisActor2D->SetTitleVisibility(val);
}

bool CVtkAxisActor2D::isTitleVisible() const
{
    return m_vtkAxisActor2D->GetTitleVisibility();
}

void CVtkAxisActor2D::setFontFactor(double val)
{
    m_vtkAxisActor2D->SetFontFactor(val);
}

double CVtkAxisActor2D::fontFactor() const
{
    return m_vtkAxisActor2D->GetFontFactor();
}

void CVtkAxisActor2D::setLabelFactor(double val)
{
    m_vtkAxisActor2D->SetLabelFactor( val );
}

double CVtkAxisActor2D::labelFactor() const
{
    return m_vtkAxisActor2D->GetLabelFactor();
}

void CVtkAxisActor2D::setLabelColor(QColor color)
{
    m_vtkAxisActor2D->GetLabelTextProperty()->SetColor( color.redF(), color.greenF(), color.blueF() );
}

QColor CVtkAxisActor2D::labelColor() const
{
    QColor ret;
    double* color = m_vtkAxisActor2D->GetLabelTextProperty()->GetColor();
    ret.setRgbF(color[0], color[1], color[2]);
    return ret;
}

void CVtkAxisActor2D::setLabelFont(CVtkAxisActor2D::FontFamily family)
{
    m_vtkAxisActor2D->GetLabelTextProperty()->SetFontFamily( int(family) );
}

CVtkAxisActor2D::FontFamily CVtkAxisActor2D::labelFont() const
{
    return CVtkAxisActor2D::FontFamily( m_vtkAxisActor2D->GetLabelTextProperty()->GetFontFamily() );
}

void CVtkAxisActor2D::setTitleColor(QColor color)
{
    m_vtkAxisActor2D->GetTitleTextProperty()->SetColor( color.redF(), color.greenF(), color.blueF() );
}

QColor CVtkAxisActor2D::titleColor() const
{
    QColor ret;
    double* color = m_vtkAxisActor2D->GetTitleTextProperty()->GetColor();
    ret.setRgbF(color[0], color[1], color[2]);
    return ret;
}

void CVtkAxisActor2D::setTitleFont(CVtkAxisActor2D::FontFamily family)
{
    m_vtkAxisActor2D->GetTitleTextProperty()->SetFontFamily( int(family) );
}

CVtkAxisActor2D::FontFamily CVtkAxisActor2D::titleFont() const
{
    return CVtkAxisActor2D::FontFamily( m_vtkAxisActor2D->GetTitleTextProperty()->GetFontFamily() );
}


bool CVtkAxisActor2D::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    if(path->pathName() == "Mapper2D")
        return true;

    return CVtkActor2D::hasInput(path);
}

bool CVtkAxisActor2D::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    if(path->pathName() == "Mapper2D")
        return false;

    return CVtkActor2D::setInput(path, inputData);
}

bool CVtkAxisActor2D::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    if(path->pathName() == "Mapper2D")
        return false;

    return CVtkActor2D::removeInput(path, inputData);
}

