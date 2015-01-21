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

#include "CVtkImageReslice.h"
#include "IVtkAbstractTransformIOData.h"

#include "vtkTransform.h"

DEFINE_VTK_OBJECT(CVtkImageReslice, CVtkThreadedImageAlgorithm, vtkImageReslice)
{
    pDesc->setNodeClassName("vtkImageReslice");
    pDesc->setNodeClassCategory("Image Filters");
    pDesc->setNodeClassDescription("Extract slices from a 3D image");
    
    pDesc->setPropertyInfo("TransformInputSampling", "SetTransformInputSampling", "GetTransformInputSampling", QStringList()<<"bool",QStringList(),"void","bool");
    pDesc->setPropertyInfo("AutoCropOutput", "SetAutoCropOutput", "GetAutoCropOutput", QStringList()<<"bool",QStringList(),"void","bool");
    pDesc->setPropertyInfo("Wrap", "SetWrap", "GetWrap", QStringList()<<"bool",QStringList(),"void","bool");
    pDesc->setPropertyInfo("Mirror", "SetMirror", "GetMirror", QStringList()<<"bool",QStringList(),"void","bool");
    pDesc->setPropertyInfo("Border", "SetBorder", "GetBorder", QStringList()<<"bool",QStringList(),"void","bool");
    pDesc->setPropertyInfo("InterpolationMode", "SetInterpolationMode", "GetInterpolationMode", QStringList()<<"int",QStringList(),"void","int");
    pDesc->setPropertyInfo("UseOptimization", "SetOptimization", "GetOptimization", QStringList()<<"bool",QStringList(),"void","bool");
    pDesc->setPropertyInfo("OutputOrigin", "SetOutputOrigin", "GetOutputOrigin", QStringList()<<"double"<<"double"<<"double",QStringList(),"void","double*");
    pDesc->setPropertyInfo("OutputDimensionality", "SetOutputDimensionality", "GetOutputDimensionality", QStringList()<<"int",QStringList(),"void","int");
    pDesc->setPropertyInfo("Interpolate", "SetInterpolate", "GetInterpolate", QStringList()<<"bool",QStringList(),"void","bool");
}

CVtkImageReslice::CVtkImageReslice()
{
    CVtkImageReslice::InitializeObjectDesc();
    setVtkObject(vtkImageReslice::New());

    m_resliceTransform = vtkTransform::New();
}

CVtkImageReslice::~CVtkImageReslice()
{
    m_resliceTransform->Delete();
}

void CVtkImageReslice::setTransformInputSampling(int val)
{
    m_vtkImageReslice->SetTransformInputSampling(val);
}

bool CVtkImageReslice::isTransformInputSampling() const
{
    return m_vtkImageReslice->GetTransformInputSampling();
}

void CVtkImageReslice::setAutoCropOutput(bool val)
{
    m_vtkImageReslice->SetAutoCropOutput(val);
}

bool CVtkImageReslice::isAutoCropOutput() const
{
    return m_vtkImageReslice->GetAutoCropOutput();
}

void CVtkImageReslice::setWrap(bool val)
{
    m_vtkImageReslice->SetWrap(val);
}

bool CVtkImageReslice::isWrap() const
{
    return m_vtkImageReslice->GetWrap();
}

void CVtkImageReslice::setMirror(bool val)
{
    m_vtkImageReslice->SetMirror(val);
}

bool CVtkImageReslice::isMirror() const
{
    return m_vtkImageReslice->GetMirror();
}

void CVtkImageReslice::setBorder(bool val)
{
    m_vtkImageReslice->SetBorder(val);
}

bool CVtkImageReslice::isBorder() const
{
    return m_vtkImageReslice->GetBorder();
}

void CVtkImageReslice::setInterpolationMode(CVtkImageReslice::InterpolationMode mode)
{
    m_vtkImageReslice->SetInterpolationMode( int(mode) );
}

CVtkImageReslice::InterpolationMode CVtkImageReslice::interpolationMode() const
{
    return CVtkImageReslice::InterpolationMode(m_vtkImageReslice->GetInterpolationMode());
}

void CVtkImageReslice::setUseOptimization(bool val)
{
    m_vtkImageReslice->SetOptimization(val);
}

bool CVtkImageReslice::isUseOptimazation() const
{
    return m_vtkImageReslice->GetOptimization();
}

void CVtkImageReslice::setBackgroundColor(const QColor& color)
{
    m_vtkImageReslice->SetBackgroundColor(color.redF(), color.greenF(), color.blueF(), backgroundColorAlpha());
}

QColor CVtkImageReslice::backgroundColor() const
{
    double color[4];
    m_vtkImageReslice->GetBackgroundColor(color);

    QColor ret;
    ret.setRgbF(color[0], color[1], color[2], color[3]);

    return ret;
}

void CVtkImageReslice::setBackgroundColorAlpha(double val)
{
    double color[4];
    m_vtkImageReslice->GetBackgroundColor(color);

    if(val < 0)
        color[3] = 0;
    else if(val > 1)
        color[3] = 1;
    else
        color[3] = val;

    m_vtkImageReslice->SetBackgroundColor(color);
}

double CVtkImageReslice::backgroundColorAlpha() const
{
    double color[4];
    m_vtkImageReslice->GetBackgroundColor(color);

    return color[3];
}

void CVtkImageReslice::setOutputSpacing(const QList<double>& val)
{
    if(val.count() != 3)
        return;

    m_vtkImageReslice->SetOutputSpacing(val.at(0), val.at(1), val.at(2));
}

QList<double> CVtkImageReslice::outputSpacing() const
{
    double* spacing = m_vtkImageReslice->GetOutputSpacing();
    return QList<double>() << spacing[0] << spacing[1] << spacing[2];
}

void CVtkImageReslice::setOutputOrigin(const Point3D& val)
{
    m_vtkImageReslice->SetOutputOrigin(val.x, val.y, val.z);
}

Point3D CVtkImageReslice::outputOrigin() const
{
    return Point3D(m_vtkImageReslice->GetOutputOrigin());
}

void CVtkImageReslice::setOutputExtent(const QList<double>& val)
{
    if(val.count() != 6)
        return;

    m_vtkImageReslice->SetOutputExtent(val.at(0), val.at(1),
                                       val.at(2), val.at(3),
                                       val.at(4), val.at(5));
}

QList<double> CVtkImageReslice::outputExtent() const
{
    double* extent = m_vtkImageReslice->GetOutputSpacing();
    return QList<double>() << extent[0] << extent[1] << extent[2]
                           << extent[3] << extent[4] << extent[5];
}

void CVtkImageReslice::setOutputDimensionality(int val)
{
    if(val < 0 || val > 3)
        return;

    m_vtkImageReslice->SetOutputDimensionality(val);
}

int CVtkImageReslice::outputDimensionality() const
{
    return m_vtkImageReslice->GetOutputDimensionality();
}

void CVtkImageReslice::setInterpolate(bool val)
{
    m_vtkImageReslice->SetInterpolate(val);
}

bool CVtkImageReslice::isInterpolate() const
{
    return m_vtkImageReslice->GetInterpolate();
}

void CVtkImageReslice::resetTransform()
{
    m_resliceTransform->Identity();
    m_vtkImageReslice->SetResliceAxes(m_resliceTransform->GetMatrix());
}

void CVtkImageReslice::translate(double x, double y, double z)
{
    m_resliceTransform->Translate(x, y, z);
    m_vtkImageReslice->SetResliceAxes(m_resliceTransform->GetMatrix());
}

void CVtkImageReslice::rotateX(double x)
{
    m_resliceTransform->RotateX(x);
    m_vtkImageReslice->SetResliceAxes(m_resliceTransform->GetMatrix());
}

void CVtkImageReslice::rotateY(double y)
{
    m_resliceTransform->RotateY(y);
    m_vtkImageReslice->SetResliceAxes(m_resliceTransform->GetMatrix());
}

void CVtkImageReslice::rotateZ(double z)
{
    m_resliceTransform->RotateZ(z);
    m_vtkImageReslice->SetResliceAxes(m_resliceTransform->GetMatrix());
}

void CVtkImageReslice::rotateWXYZ(double w, double x, double y, double z)
{
    m_resliceTransform->RotateWXYZ(w, x, y, z);
    m_vtkImageReslice->SetResliceAxes(m_resliceTransform->GetMatrix());
}
