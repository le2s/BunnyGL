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

#include "CVtkVolumeRayCastIsosurfaceFunction.h"
#include "VTKVolumeRenderingSystemComponent.h"
#include "vtkVolumeRayCastIsosurfaceFunction.h"

DEFINE_VIS_NODE(CVtkVolumeRayCastIsosurfaceFunction, CVtkVolumeRayCastFunction)
{
    pDesc->setNodeClassCategory("VTKVolumeRenderingSystem");
    pDesc->setNodeClassName("vtkVolumeRayCastIsosurfaceFunction");
    pDesc->setNodeClassDescription("vtkVolumeRayCastIsosurfaceFunction");
    pDesc->setNodeIcon( VTKVolumeRenderingSystemComponent::instance().nodeIcon() );

    pDesc->setPropertyInfo("IsoValue", "SetIsoValue", "GetIsoValue", QStringList() << "double", QStringList(), "void", "double");
    // Uncomment and use the following code template to add input/output paths
    /*
    pDesc->addConnectionPath(
        new CGenericVisNodeConnectionPath(
                "PathName",                                 // Name of the path
                IVisSystemNodeConnectionPath::OutputPath,   // Path type can be OutputPath or InputPath
                "PathType",                                 // Data type of the path
                0,                                          // Path index (don't change)
                false                                       // Allow Multiple Inputs Flag
            )
        );
    */
}

struct CVtkVolumeRayCastIsosurfaceFunctionData
{
    vtkVolumeRayCastIsosurfaceFunction *volumeRayCastIsosurfaceFunction;
};

CVtkVolumeRayCastIsosurfaceFunction::CVtkVolumeRayCastIsosurfaceFunction()
{
    CVtkVolumeRayCastIsosurfaceFunction::InitializeNodeDesc();
    d = new CVtkVolumeRayCastIsosurfaceFunctionData;

    d->volumeRayCastIsosurfaceFunction = 0;
    setVtkObject( vtkVolumeRayCastIsosurfaceFunction::New() );
}

CVtkVolumeRayCastIsosurfaceFunction::~CVtkVolumeRayCastIsosurfaceFunction()
{
    // This would be a good time to delete the backend object. If the backend object is a vtkObject
    // subclass, then you have to delete it now.
    // d->VtkObjectPointer->Delete();
    d->volumeRayCastIsosurfaceFunction->Delete();
    delete d;
}

void CVtkVolumeRayCastIsosurfaceFunction::setIsoValue(double val)
{
    d->volumeRayCastIsosurfaceFunction->SetIsoValue(val);
}

double CVtkVolumeRayCastIsosurfaceFunction::isoValue() const
{
    return d->volumeRayCastIsosurfaceFunction->GetIsoValue();
}

bool CVtkVolumeRayCastIsosurfaceFunction::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CVtkVolumeRayCastFunction::hasInput(path);
}

bool CVtkVolumeRayCastIsosurfaceFunction::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CVtkVolumeRayCastFunction::setInput(path, inputData);
}

bool CVtkVolumeRayCastIsosurfaceFunction::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CVtkVolumeRayCastFunction::removeInput(path, inputData);
}

bool CVtkVolumeRayCastIsosurfaceFunction::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    /*
    If you have added output paths in the description block at the header of this file,
    then you will have to handle outputs here
    */

    return CVtkVolumeRayCastFunction::fetchOutput(path, outputData);
}

bool CVtkVolumeRayCastIsosurfaceFunction::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
{
    if(!path || !outputData)
        return false;

    /*
    If you have added output paths in the description block at the header of this file,
    then you will have to handle outputs here
    */

    return CVtkVolumeRayCastFunction::outputDerefed(path, outputData);
}

#ifdef ENABLE_ADVANCED_PROPERTIES

int CVtkVolumeRayCastIsosurfaceFunction::propertyCount()
{
    return 0;
}

QString CVtkVolumeRayCastIsosurfaceFunction::propertyName(int index)
{
    Q_UNUSED(index);
    return QString();
}

QString CVtkVolumeRayCastIsosurfaceFunction::propertyGroup(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkVolumeRayCastIsosurfaceFunction::propertyValue(int index)
{
    Q_UNUSED(index);
    return QVariant();
}

void CVtkVolumeRayCastIsosurfaceFunction::setPropertyValue(int index, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
}

bool CVtkVolumeRayCastIsosurfaceFunction::hasEditor(int index)
{
    Q_UNUSED(index);
    return false;
}

QWidget* CVtkVolumeRayCastIsosurfaceFunction::createEditor(int index)
{
    Q_UNUSED(index);
    return 0;
}

void CVtkVolumeRayCastIsosurfaceFunction::setEditorValue(int index, QWidget* widget, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(value);
}

QVariant CVtkVolumeRayCastIsosurfaceFunction::editorValue(int index, QWidget* widget)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
}

void CVtkVolumeRayCastIsosurfaceFunction::connectEditor(int index, QWidget* widget, QObject* receiver, const char* member)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(receiver);
    Q_UNUSED(member);
}

QString CVtkVolumeRayCastIsosurfaceFunction::propertyValueString(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkVolumeRayCastIsosurfaceFunction::propertyValueFromString(int index, QString valueStr)
{
    Q_UNUSED(index);
    Q_UNUSED(valueStr);

    return QVariant();
}

bool CVtkVolumeRayCastIsosurfaceFunction::canLoadSaveProperty(int index)
{
    Q_UNUSED(index);

    return false;
}

#endif

void CVtkVolumeRayCastIsosurfaceFunction::setVtkObject(vtkObject* ptr)
{
    d->volumeRayCastIsosurfaceFunction = dynamic_cast<vtkVolumeRayCastIsosurfaceFunction*>(ptr);
    CVtkVolumeRayCastFunction::setVtkObject(ptr);
}





