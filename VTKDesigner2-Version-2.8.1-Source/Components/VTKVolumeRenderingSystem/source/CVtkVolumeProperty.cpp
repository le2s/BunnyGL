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

#include "CVtkVolumeProperty.h"
#include "VTKVolumeRenderingSystemComponent.h"
#include "vtkVolumeProperty.h"

#include "vtkPiecewiseFunction.h"
#include "vtkColorTransferFunction.h"

#include "IVtkObjectIOData.h"
#include "CVtkObjectIOData.h"

DEFINE_VIS_NODE(CVtkVolumeProperty, CGenericVisNodeBase)
{
    pDesc->setNodeClassCategory("VTKVolumeRenderingSystem");
    pDesc->setNodeClassName("vtkVolumeProperty");
    pDesc->setNodeClassDescription("vtkVolumeProperty");
    pDesc->setNodeIcon( VTKVolumeRenderingSystemComponent::instance().nodeIcon() );
    pDesc->setPropertyInfo("Shade", "setShade", "shade", QStringList() << "int", QStringList(), "void", "int");
    pDesc->setPropertyInfo("IndependentComponents", "SetIndependentComponents", "GetIndependentComponents", QStringList() << "int", QStringList(), "void", "int");
    pDesc->setPropertyInfo("ScalarOpacityUnitDistance", "SetScalarOpacityUnitDistance", "GetScalarOpacityUnitDistance", QStringList() << "double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("DisableGradientOpacity", "SetDisableGradientOpacity", "GetDisableGradientOpacity", QStringList() << "int", QStringList(), "void", "int");
    pDesc->setPropertyInfo("Ambient", "SetAmbient", "GetAmbient", QStringList() << "double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("Diffuse", "SetDiffuse", "GetDiffuse", QStringList() << "double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("Specular", "SetSpecular", "GetSpecular", QStringList() << "double", QStringList(), "void", "double");
    pDesc->setPropertyInfo("SpecularPower", "SetSpecularPower", "GetSpecularPower", QStringList() << "double", QStringList(), "void", "double");
    
    pDesc->addConnectionPath(
        new CGenericVisNodeConnectionPath(
                "Color Transfer Function",               // Name of the path
                IVisSystemNodeConnectionPath::InputPath, // Path type can be OutputPath or InputPath
                "vtkPiecewiseFunction, vtkColorTransferFunction",// Data type of the path
                0,                                       // Path index (don't change)
                false                                    // Allow Multiple Inputs Flag
            )
        );

    pDesc->addConnectionPath(
        new CGenericVisNodeConnectionPath(
                "Opacity Function",             // Name of the path
                IVisSystemNodeConnectionPath::InputPath, // Path type can be OutputPath or InputPath
                "vtkPiecewiseFunction",                  // Data type of the path
                0,                                       // Path index (don't change)
                false                                    // Allow Multiple Inputs Flag
            )
    );

    pDesc->addConnectionPath(
        new CGenericVisNodeConnectionPath(
                "Gradient Opacity",                      // Name of the path
                IVisSystemNodeConnectionPath::InputPath, // Path type can be OutputPath or InputPath
                "vtkPiecewiseFunction",                  // Data type of the path
                0,                                       // Path index (don't change)
                false                                    // Allow Multiple Inputs Flag
            )
    );

    pDesc->addConnectionPath(
        new CGenericVisNodeConnectionPath(
                "Volume Property",                       // Name of the path
                IVisSystemNodeConnectionPath::OutputPath,// Path type can be OutputPath or InputPath
                "vtkVolumeProperty",                     // Data type of the path
                0,                                       // Path index (don't change)
                false                                    // Allow Multiple Inputs Flag
            )
    );

}

struct CVtkVolumePropertyData
{
    vtkVolumeProperty *volumeProperty;
    CVtkObjectIOData outputData;

    bool hasColorTransferFn;
    bool hasOpacityTransferFn;
    bool hasGradientOpacityTransferFn;
};

CVtkVolumeProperty::CVtkVolumeProperty()
{
    CVtkVolumeProperty::InitializeNodeDesc();
    d = new CVtkVolumePropertyData;

    d->volumeProperty = 0;
    d->hasColorTransferFn = false;
    d->hasOpacityTransferFn = false;
    d->hasGradientOpacityTransferFn = false;

    setVtkObject( vtkVolumeProperty::New() );
}

CVtkVolumeProperty::~CVtkVolumeProperty()
{
    // This would be a good time to delete the backend object. If the backend object is a vtkObject
    // subclass, then you have to delete it now.
    // d->VtkObjectPointer->Delete();
    d->volumeProperty->Delete();
    delete d;
}

void CVtkVolumeProperty::setIndependentComponents(int val)
{
    d->volumeProperty->SetIndependentComponents(val);
}

int CVtkVolumeProperty::independentComponents() const
{
    return d->volumeProperty->GetIndependentComponents();
}

void CVtkVolumeProperty::setInterpolationType(CVtkVolumeProperty::InterpolationType type)
{
    d->volumeProperty->SetInterpolationType( int(type) );
}

CVtkVolumeProperty::InterpolationType CVtkVolumeProperty::interpolationType() const
{
    return CVtkVolumeProperty::InterpolationType(
                d->volumeProperty->GetInterpolationType()
            );
}

void CVtkVolumeProperty::setComponentWeight(int index, double val)
{
    Q_UNUSED(index);
    Q_UNUSED(val);
    // TODO
}

double CVtkVolumeProperty::componentWeight(int index) const
{
    Q_UNUSED(index);
    // TODO

    return 0.0;
}

void CVtkVolumeProperty::setScalarOpacityUnitDistance(double dist)
{
    d->volumeProperty->SetScalarOpacityUnitDistance(dist);
}

double CVtkVolumeProperty::scalarOpacityUnitDistance() const
{
    return d->volumeProperty->GetScalarOpacityUnitDistance();
}

void CVtkVolumeProperty::setDisableGradientOpacity(int val)
{
    d->volumeProperty->SetDisableGradientOpacity(val);
}

int CVtkVolumeProperty::disableGradientOpacity() const
{
    return d->volumeProperty->GetDisableGradientOpacity();
}

void CVtkVolumeProperty::setShade(int val)
{
    d->volumeProperty->SetShade(val);
}

int CVtkVolumeProperty::shade() const
{
    return d->volumeProperty->GetShade();
}

void CVtkVolumeProperty::setAmbient(double amb)
{
    d->volumeProperty->SetAmbient(amb);
}

double CVtkVolumeProperty::ambient() const
{
    return d->volumeProperty->GetAmbient();
}

void CVtkVolumeProperty::setDiffuse(double diff)
{
    d->volumeProperty->SetDiffuse(diff);
}

double CVtkVolumeProperty::diffuse() const
{
    return d->volumeProperty->GetDiffuse();
}

void CVtkVolumeProperty::setSpecular(double spec)
{
    d->volumeProperty->SetSpecular(spec);
}

double CVtkVolumeProperty::specular() const
{
    return d->volumeProperty->GetSpecular();
}

void CVtkVolumeProperty::setSpecularPower(double pow)
{
    d->volumeProperty->SetSpecularPower(pow);
}

double CVtkVolumeProperty::specularPower() const
{
    return d->volumeProperty->GetSpecularPower();
}

bool CVtkVolumeProperty::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    if(path->pathName() == "Color Transfer Function")
    {
        return d->hasColorTransferFn;
    }
    
    if(path->pathName() == "Opacity Function")
    {
        return d->hasOpacityTransferFn;
    }
    
    if(path->pathName() == "Gradient Opacity")
    {
        return d->hasGradientOpacityTransferFn;
    }

    return CGenericVisNodeBase::hasInput(path);
}

bool CVtkVolumeProperty::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    if(path->pathName() == "Color Transfer Function")
    {
        IVtkObjectIOData* data = 0;
        bool success = inputData->queryInterface("IVtkObjectIOData", (void**)&data);
        if(!success || !data)
            return false;

        vtkObject* objPtr = data->getVtkObject();
        vtkPiecewiseFunction* pwfn = vtkPiecewiseFunction::SafeDownCast(objPtr);
        if(pwfn)
        {
            d->volumeProperty->SetColor(pwfn);
            d->hasColorTransferFn = true;
            return true;
        }

        vtkColorTransferFunction* ctfn = vtkColorTransferFunction::SafeDownCast(objPtr);
        if(ctfn)
        {
            d->volumeProperty->SetColor(ctfn);
            d->hasColorTransferFn = true;
            return true;
        }

        return false;
    }
    
    if(path->pathName() == "Opacity Function")
    {
        IVtkObjectIOData* data = 0;
        bool success = inputData->queryInterface("IVtkObjectIOData", (void**)&data);
        if(!success || !data)
            return false;

        vtkObject* objPtr = data->getVtkObject();
        vtkPiecewiseFunction* pwfn = vtkPiecewiseFunction::SafeDownCast(objPtr);
        if(pwfn)
        {
            d->volumeProperty->SetScalarOpacity(pwfn);
            d->hasOpacityTransferFn = true;
            return true;
        }

        return false;
    }
    
    if(path->pathName() == "Gradient Opacity")
    {
        IVtkObjectIOData* data = 0;
        bool success = inputData->queryInterface("IVtkObjectIOData", (void**)&data);
        if(!success || !data)
            return false;

        vtkObject* objPtr = data->getVtkObject();
        vtkPiecewiseFunction* pwfn = vtkPiecewiseFunction::SafeDownCast(objPtr);
        if(pwfn)
        {
            d->volumeProperty->SetGradientOpacity(pwfn);
            d->hasGradientOpacityTransferFn = true;
            return true;
        }

        return false;
    }

    return CGenericVisNodeBase::setInput(path, inputData);
}

bool CVtkVolumeProperty::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    if(path->pathName() == "Color Transfer Function")
    {
        IVtkObjectIOData* data = 0;
        bool success = inputData->queryInterface("IVtkObjectIOData", (void**)&data);
        if(!success || !data)
            return false;

        vtkObject* objPtr = data->getVtkObject();
        vtkPiecewiseFunction* pwfn = vtkPiecewiseFunction::SafeDownCast(objPtr);
        vtkColorTransferFunction* ctfn = pwfn ? 0 : vtkColorTransferFunction::SafeDownCast(objPtr);
        if(!pwfn && !ctfn)
            return false;

        if(pwfn && d->volumeProperty->GetGrayTransferFunction() == pwfn)
        {
            d->volumeProperty->SetColor( (vtkPiecewiseFunction*)0 );
            d->hasColorTransferFn = false;
            return true;
        }

        if(ctfn && d->volumeProperty->GetRGBTransferFunction() == ctfn)
        {
            d->volumeProperty->SetColor( (vtkColorTransferFunction*)0 );
            d->hasColorTransferFn = false;
            return true;
        }

        return false;
    }
    
    if(path->pathName() == "Opacity Function")
    {
        IVtkObjectIOData* data = 0;
        bool success = inputData->queryInterface("IVtkObjectIOData", (void**)&data);
        if(!success || !data)
            return false;

        vtkObject* objPtr = data->getVtkObject();
        vtkPiecewiseFunction* pwfn = vtkPiecewiseFunction::SafeDownCast(objPtr);
        if(pwfn && d->volumeProperty->GetScalarOpacity() == pwfn)
        {
            d->volumeProperty->SetScalarOpacity((vtkPiecewiseFunction*)0);
            d->hasOpacityTransferFn = false;
            return true;
        }

        return false;
    }
    
    if(path->pathName() == "Gradient Opacity")
    {
        IVtkObjectIOData* data = 0;
        bool success = inputData->queryInterface("IVtkObjectIOData", (void**)&data);
        if(!success || !data)
            return false;

        vtkObject* objPtr = data->getVtkObject();
        vtkPiecewiseFunction* pwfn = vtkPiecewiseFunction::SafeDownCast(objPtr);
        if(pwfn && d->volumeProperty->GetGradientOpacity() == pwfn)
        {
            d->volumeProperty->SetGradientOpacity((vtkPiecewiseFunction*)0);
            d->hasOpacityTransferFn = false;
            return true;
        }

        return false;
    }

    return CGenericVisNodeBase::removeInput(path, inputData);
}

bool CVtkVolumeProperty::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    if(path->pathName() == "Volume Property")
    {
        *outputData = &d->outputData;
        return true;
    }

    return CGenericVisNodeBase::fetchOutput(path, outputData);
}

bool CVtkVolumeProperty::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
{
    if(!path || !outputData)
        return false;

    /*
    If you have added output paths in the description block at the header of this file,
    then you will have to handle outputs here
    */

    return CGenericVisNodeBase::outputDerefed(path, outputData);
}

#ifdef ENABLE_ADVANCED_PROPERTIES

int CVtkVolumeProperty::propertyCount()
{
    return 0;
}

QString CVtkVolumeProperty::propertyName(int index)
{
    Q_UNUSED(index);
    return QString();
}

QString CVtkVolumeProperty::propertyGroup(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkVolumeProperty::propertyValue(int index)
{
    Q_UNUSED(index);
    return QVariant();
}

void CVtkVolumeProperty::setPropertyValue(int index, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
}

bool CVtkVolumeProperty::hasEditor(int index)
{
    Q_UNUSED(index);
    return false;
}

QWidget* CVtkVolumeProperty::createEditor(int index)
{
    Q_UNUSED(index);
    return 0;
}

void CVtkVolumeProperty::setEditorValue(int index, QWidget* widget, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(value);
}

QVariant CVtkVolumeProperty::editorValue(int index, QWidget* widget)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
}

void CVtkVolumeProperty::connectEditor(int index, QWidget* widget, QObject* receiver, const char* member)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(receiver);
    Q_UNUSED(member);
}

QString CVtkVolumeProperty::propertyValueString(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkVolumeProperty::propertyValueFromString(int index, QString valueStr)
{
    Q_UNUSED(index);
    Q_UNUSED(valueStr);

    return QVariant();
}

bool CVtkVolumeProperty::canLoadSaveProperty(int index)
{
    Q_UNUSED(index);

    return false;
}

#endif

void CVtkVolumeProperty::setVtkObject(vtkObject* ptr)
{
    d->volumeProperty = dynamic_cast<vtkVolumeProperty*>(ptr);
    d->outputData.setObject(ptr);
}





