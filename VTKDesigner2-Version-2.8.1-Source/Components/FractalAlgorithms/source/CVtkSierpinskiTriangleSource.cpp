/****************************************************************************
**
** Copyright (C) VCreate Logic Pvt Ltd
**
** Use of this file is limited according to the terms specified by
** VCreate Logic Pvt Ltd.
**
** Details of those terms are listed in licence.txt included as
** part of the distribution package of this file. This file may not
** be distributed without including the licence.txt file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include "CVtkSierpinskiTriangleSource.h"
#include "FractalAlgorithmsComponent.h"
#include "vtkSierpinskiTriangleSource.h"
#include "CVtkPolyData.h"

DEFINE_VIS_NODE(CVtkSierpinskiTriangleSource, CGenericVisNodeBase)
{
    pDesc->setNodeClassCategory("FractalAlgorithms");
    pDesc->setNodeClassName("vtkSierpinskiTriangleSource");
    pDesc->setNodeClassDescription("vtkSierpinskiTriangleSource");
    pDesc->setNodeIcon( FractalAlgorithmsComponent::instance().nodeIcon() );
    pDesc->setPropertyInfo("NumberOfIterations", "setNumberOfIterations", "numberOfIterations", QStringList() << "int", QStringList(), "void", "int");

    // Uncomment and use the following code template to add input/output paths
    pDesc->addConnectionPath(
        new CGenericVisNodeConnectionPath(
                "SierpinskiTriangle",                       // Name of the path
                IVisSystemNodeConnectionPath::OutputPath,   // Path type can be OutputPath or InputPath
                "vtkPolyData",                              // Data type of the path
                0,                                          // Path index (don't change)
                false                                       // Allow Multiple Inputs Flag
            )
        );
}

struct CVtkSierpinskiTriangleSourceData
{
    vtkSierpinskiTriangleSource *sierpinskiTriangleSource;
    CVtkPolyData output;
};

CVtkSierpinskiTriangleSource::CVtkSierpinskiTriangleSource()
{
    CVtkSierpinskiTriangleSource::InitializeNodeDesc();
    d = new CVtkSierpinskiTriangleSourceData;

    d->sierpinskiTriangleSource = 0;
    setVtkObject( vtkSierpinskiTriangleSource::New() );
}

CVtkSierpinskiTriangleSource::~CVtkSierpinskiTriangleSource()
{
    // This would be a good time to delete the backend object. If the backend object is a vtkObject
    // subclass, then you have to delete it now.
    // d->VtkObjectPointer->Delete();

    d->sierpinskiTriangleSource->Delete();
    delete d;
}

void CVtkSierpinskiTriangleSource::setNumberOfIterations(int count)
{
    d->sierpinskiTriangleSource->SetNumberOfIterations(count);
}

int CVtkSierpinskiTriangleSource::numberOfIterations() const
{
    return d->sierpinskiTriangleSource->GetNumberOfIterations();
}

bool CVtkSierpinskiTriangleSource::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CGenericVisNodeBase::hasInput(path);
}

bool CVtkSierpinskiTriangleSource::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CGenericVisNodeBase::setInput(path, inputData);
}

bool CVtkSierpinskiTriangleSource::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CGenericVisNodeBase::removeInput(path, inputData);
}

bool CVtkSierpinskiTriangleSource::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    if(path->pathName() == "SierpinskiTriangle")
    {
        d->output.setPolyData( d->sierpinskiTriangleSource->GetOutput() );
        *outputData = &d->output;
        return true;
    }

    return CGenericVisNodeBase::fetchOutput(path, outputData);
}

bool CVtkSierpinskiTriangleSource::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
{
    if(!path || !outputData)
        return false;

    if(path->pathName() == "SierpinskiTriangle")
        return true;

    return CGenericVisNodeBase::outputDerefed(path, outputData);
}

#ifdef ENABLE_ADVANCED_PROPERTIES

int CVtkSierpinskiTriangleSource::propertyCount()
{
    return 0;
}

QString CVtkSierpinskiTriangleSource::propertyName(int index)
{
    Q_UNUSED(index);
    return QString();
}

QString CVtkSierpinskiTriangleSource::propertyGroup(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkSierpinskiTriangleSource::propertyValue(int index)
{
    Q_UNUSED(index);
    return QVariant();
}

void CVtkSierpinskiTriangleSource::setPropertyValue(int index, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
}

bool CVtkSierpinskiTriangleSource::hasEditor(int index)
{
    Q_UNUSED(index);
    return false;
}

QWidget* CVtkSierpinskiTriangleSource::createEditor(int index)
{
    Q_UNUSED(index);
    return 0;
}

void CVtkSierpinskiTriangleSource::setEditorValue(int index, QWidget* widget, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(value);
}

QVariant CVtkSierpinskiTriangleSource::editorValue(int index, QWidget* widget)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
}

void CVtkSierpinskiTriangleSource::connectEditor(int index, QWidget* widget, QObject* receiver, const char* member)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(receiver);
    Q_UNUSED(member);
}

QString CVtkSierpinskiTriangleSource::propertyValueString(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkSierpinskiTriangleSource::propertyValueFromString(int index, QString valueStr)
{
    Q_UNUSED(index);
    Q_UNUSED(valueStr);

    return QVariant();
}

bool CVtkSierpinskiTriangleSource::canLoadSaveProperty(int index)
{
    Q_UNUSED(index);

    return false;
}

#endif

void CVtkSierpinskiTriangleSource::setVtkObject(vtkObject* ptr)
{
    d->sierpinskiTriangleSource = dynamic_cast<vtkSierpinskiTriangleSource*>(ptr);
    nodeData()["vtkObject"] = qVariantFromValue<vtkObject*>(d->sierpinskiTriangleSource);
}





