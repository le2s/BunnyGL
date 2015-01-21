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

#include "CVtkVolumeTextureMapper3D.h"
#include "VTKVolumeRenderingSystemComponent.h"
#include "vtkVolumeTextureMapper3D.h"

DEFINE_VIS_NODE(CVtkVolumeTextureMapper3D, CVtkVolumeMapper)
{
    pDesc->setNodeClassCategory("VTKVolumeRenderingSystem");
    pDesc->setNodeClassName("vtkVolumeTextureMapper3D");
    pDesc->setNodeClassDescription("vtkVolumeTextureMapper3D");
    pDesc->setNodeIcon( VTKVolumeRenderingSystemComponent::instance().nodeIcon() );

    pDesc->setPropertyInfo("RenderMethod", "SetPreferredRenderMethod", "GetPreferredRenderMethod", QStringList() << "int", QStringList(), "void", "int");
    
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

struct CVtkVolumeTextureMapper3DData
{
    vtkVolumeTextureMapper3D *volumeTextureMapper3D;
};

CVtkVolumeTextureMapper3D::CVtkVolumeTextureMapper3D()
{
    CVtkVolumeTextureMapper3D::InitializeNodeDesc();
    d = new CVtkVolumeTextureMapper3DData;

    d->volumeTextureMapper3D = 0;
    setVtkObject( vtkVolumeTextureMapper3D::New() );
}

CVtkVolumeTextureMapper3D::~CVtkVolumeTextureMapper3D()
{
    // This would be a good time to delete the backend object. If the backend object is a vtkObject
    // subclass, then you have to delete it now.
    // d->VtkObjectPointer->Delete();
    d->volumeTextureMapper3D->Delete();
    delete d;
}

QList<QVariant> CVtkVolumeTextureMapper3D::volumeDimensions() const
{
	int* r = d->volumeTextureMapper3D->GetVolumeDimensions();

	QList<QVariant> ret = QList<QVariant>() << r[0] << r[1] << r[2];
    return ret;
}

QList<QVariant> CVtkVolumeTextureMapper3D::volumeSpacing() const
{
	float* r = d->volumeTextureMapper3D->GetVolumeSpacing();

	QList<QVariant> ret = QList<QVariant>() << r[0] << r[1] << r[2];
    return ret;
}

int CVtkVolumeTextureMapper3D::numberOfPolygons() const
{
	return d->volumeTextureMapper3D->GetNumberOfPolygons();
}

float CVtkVolumeTextureMapper3D::actualSampleDistance() const
{
	return d->volumeTextureMapper3D->GetActualSampleDistance();
}

void CVtkVolumeTextureMapper3D::setPreferredRenderMethod(CVtkVolumeTextureMapper3D::RenderMethod meth)
{
    d->volumeTextureMapper3D->SetPreferredRenderMethod( int(meth) );
}

CVtkVolumeTextureMapper3D::RenderMethod CVtkVolumeTextureMapper3D::preferredRenderMethod() const
{
    return CVtkVolumeTextureMapper3D::RenderMethod(
            d->volumeTextureMapper3D->GetPreferredRenderMethod()
        );
}

bool CVtkVolumeTextureMapper3D::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CVtkVolumeMapper::hasInput(path);
}

bool CVtkVolumeTextureMapper3D::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CVtkVolumeMapper::setInput(path, inputData);
}

bool CVtkVolumeTextureMapper3D::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    /*
    If you have added input paths in the description block at the header of this file,
    then you will have to handle inputs here
    */

    return CVtkVolumeMapper::removeInput(path, inputData);
}

bool CVtkVolumeTextureMapper3D::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    /*
    If you have added output paths in the description block at the header of this file,
    then you will have to handle outputs here
    */

    return CVtkVolumeMapper::fetchOutput(path, outputData);
}

bool CVtkVolumeTextureMapper3D::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
{
    if(!path || !outputData)
        return false;

    /*
    If you have added output paths in the description block at the header of this file,
    then you will have to handle outputs here
    */

    return CVtkVolumeMapper::outputDerefed(path, outputData);
}

#ifdef ENABLE_ADVANCED_PROPERTIES

int CVtkVolumeTextureMapper3D::propertyCount()
{
    return 0;
}

QString CVtkVolumeTextureMapper3D::propertyName(int index)
{
    Q_UNUSED(index);
    return QString();
}

QString CVtkVolumeTextureMapper3D::propertyGroup(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkVolumeTextureMapper3D::propertyValue(int index)
{
    Q_UNUSED(index);
    return QVariant();
}

void CVtkVolumeTextureMapper3D::setPropertyValue(int index, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
}

bool CVtkVolumeTextureMapper3D::hasEditor(int index)
{
    Q_UNUSED(index);
    return false;
}

QWidget* CVtkVolumeTextureMapper3D::createEditor(int index)
{
    Q_UNUSED(index);
    return 0;
}

void CVtkVolumeTextureMapper3D::setEditorValue(int index, QWidget* widget, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(value);
}

QVariant CVtkVolumeTextureMapper3D::editorValue(int index, QWidget* widget)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
}

void CVtkVolumeTextureMapper3D::connectEditor(int index, QWidget* widget, QObject* receiver, const char* member)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(receiver);
    Q_UNUSED(member);
}

QString CVtkVolumeTextureMapper3D::propertyValueString(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkVolumeTextureMapper3D::propertyValueFromString(int index, QString valueStr)
{
    Q_UNUSED(index);
    Q_UNUSED(valueStr);

    return QVariant();
}

bool CVtkVolumeTextureMapper3D::canLoadSaveProperty(int index)
{
    Q_UNUSED(index);

    return false;
}

#endif

void CVtkVolumeTextureMapper3D::setVtkObject(vtkObject* ptr)
{
    d->volumeTextureMapper3D = dynamic_cast<vtkVolumeTextureMapper3D*>(ptr);
    CVtkVolumeMapper::setVtkObject(ptr);
}





