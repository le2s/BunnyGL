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

#include "CVtkVolume.h"
#include "VTKVolumeRenderingSystemComponent.h"
#include "vtkVolume.h"

#include "IVtkObjectIOData.h"
#include "IVtkVolumeIOData.h"
#include "IVtkProp3DIOData.h"
#include "vtkVolumeProperty.h"
#include "vtkAbstractVolumeMapper.h"

DEFINE_VIS_NODE(CVtkVolume, CGenericVisNodeBase)
{
    pDesc->setNodeClassCategory("VTKVolumeRenderingSystem");
    pDesc->setNodeClassName("vtkVolume");
    pDesc->setNodeClassDescription("vtkVolume");
    pDesc->setNodeIcon( VTKVolumeRenderingSystemComponent::instance().nodeIcon() );

    pDesc->addConnectionPath(
        new CGenericVisNodeConnectionPath(
                "Volume",                                   // Name of the path
                IVisSystemNodeConnectionPath::OutputPath,   // Path type can be OutputPath or InputPath
                "vtkVolume",                                // Data type of the path
                0,                                          // Path index (don't change)
                false                                       // Allow Multiple Inputs Flag
            )
        );

    pDesc->addConnectionPath(
        new CGenericVisNodeConnectionPath(
                "Volume Property",                          // Name of the path
                IVisSystemNodeConnectionPath::InputPath,    // Path type can be OutputPath or InputPath
                "vtkVolumeProperty",                        // Data type of the path
                0,                                          // Path index (don't change)
                false                                       // Allow Multiple Inputs Flag
            )
        );

    pDesc->addConnectionPath(
        new CGenericVisNodeConnectionPath(
                "Volume Mapper",                            // Name of the path
                IVisSystemNodeConnectionPath::InputPath,    // Path type can be OutputPath or InputPath
                "vtkAbstractVolumeMapper",                  // Data type of the path
                0,                                          // Path index (don't change)
                false                                       // Allow Multiple Inputs Flag
            )
        );
}

struct CVtkVolumeIOData : virtual public IVtkVolumeIOData, 
                          virtual public IVtkObjectIOData,
                          virtual public IVtkProp3DIOData
{
    CVtkVolumeIOData() : m_vtkVolume(0) { }
    ~CVtkVolumeIOData() { }

    void setVolume(vtkVolume* vol) { m_vtkVolume = vol; }

    bool queryInterface(QString ifaceName, void** ifacePtr);
    bool destroySelf();

    void* dataPointer() { return m_vtkVolume; }
    QString dataTypeName() { return "vtkVolume"; }

    vtkVolume* getVtkVolume() { return m_vtkVolume; }
    vtkProp3D* getVtkProp3D() { return vtkProp3D::SafeDownCast(m_vtkVolume); }
    vtkObject* getVtkObject() { return vtkObject::SafeDownCast(m_vtkVolume); }

private:
    vtkVolume* m_vtkVolume;
};

GCF_BEGIN_QUERY_TABLE(CVtkVolumeIOData)
    GCF_IMPLEMENTS(IVtkVolumeIOData)
    GCF_IMPLEMENTS(IVtkObjectIOData)
    GCF_IMPLEMENTS(IVtkProp3DIOData)
GCF_END_QUERY_TABLE(CVtkVolumeIOData)

struct CVtkVolumeData
{
    vtkVolume *volume;
    CVtkVolumeIOData outputData;
    bool hasProperty;
};

CVtkVolume::CVtkVolume()
{
    CVtkVolume::InitializeNodeDesc();
    d = new CVtkVolumeData;

    d->volume = 0;
    d->hasProperty = 0;
    setVtkObject( vtkVolume::New() );
}

CVtkVolume::~CVtkVolume()
{
    // This would be a good time to delete the backend object. If the backend object is a vtkObject
    // subclass, then you have to delete it now.
    // d->VtkObjectPointer->Delete();
    d->volume->Delete();
    delete d;
}

QList<double> CVtkVolume::bounds() const
{
    QList<double> ret;
    double* b = d->volume->GetBounds();
    ret << b[0] << b[1] << b[2] << b[3] << b[4] << b[5];
    return ret;
}

double CVtkVolume::minX() const
{
    return d->volume->GetBounds()[0];
}

double CVtkVolume::maxX() const
{
    return d->volume->GetBounds()[1];
}

double CVtkVolume::minY() const
{
    return d->volume->GetBounds()[2];
}

double CVtkVolume::maxY() const
{
    return d->volume->GetBounds()[3];
}

double CVtkVolume::minZ() const
{
    return d->volume->GetBounds()[4];
}

double CVtkVolume::maxZ() const
{
    return d->volume->GetBounds()[5];
}

void CVtkVolume::update()
{
    d->volume->Update();
}

void CVtkVolume::command_Update()
{
    d->volume->Update();
}


bool CVtkVolume::hasInput(IVisSystemNodeConnectionPath* path)
{
    if(!path)
        return false;

    if(path->pathName() == "Volume Property")
        return d->hasProperty;

    if(path->pathName() == "Volume Mapper")
        return d->volume->GetMapper();

    return CGenericVisNodeBase::hasInput(path);
}

bool CVtkVolume::setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    if(path->pathName() == "Volume Property")
    {
        IVtkObjectIOData* data = 0;
        bool success = inputData->queryInterface("IVtkObjectIOData", (void**)&data);
        if(success && data)
        {
            vtkObject* objPtr = data->getVtkObject();
            vtkVolumeProperty* prop = vtkVolumeProperty::SafeDownCast(objPtr);
            if(!prop)
                return false;

            d->volume->SetProperty(prop);
            d->hasProperty = true;
            return true;
        }

        return false;
    }

    if(path->pathName() == "Volume Mapper")
    {
        IVtkObjectIOData* data = 0;
        bool success = inputData->queryInterface("IVtkObjectIOData", (void**)&data);
        if(success && data)
        {
            vtkObject* objPtr = data->getVtkObject();
            vtkAbstractVolumeMapper* mapper = vtkAbstractVolumeMapper::SafeDownCast(objPtr);
            if(!mapper)
                return false;

            d->volume->SetMapper(mapper);
            return true;
        }

        return false;
    }

    return CGenericVisNodeBase::setInput(path, inputData);
}

bool CVtkVolume::removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData)
{
    if(!path || !inputData)
        return false;

    if(path->pathName() == "Volume Property")
    {
        IVtkObjectIOData* data = 0;
        bool success = inputData->queryInterface("IVtkObjectIOData", (void**)&data);
        if(success && data)
        {
            vtkObject* objPtr = data->getVtkObject();
            vtkVolumeProperty* prop = vtkVolumeProperty::SafeDownCast(objPtr);
            if(prop != d->volume->GetProperty())
                return false;

            d->volume->SetProperty(0);
            d->hasProperty = false;
            return true;
        }

        return false;
    }

    if(path->pathName() == "Volume Mapper")
    {
        IVtkObjectIOData* data = 0;
        bool success = inputData->queryInterface("IVtkObjectIOData", (void**)&data);
        if(success && data)
        {
            vtkObject* objPtr = data->getVtkObject();
            vtkAbstractVolumeMapper* mapper = vtkAbstractVolumeMapper::SafeDownCast(objPtr);
            if(mapper != d->volume->GetMapper())
                return false;

            d->volume->SetMapper(0);
            return true;
        }

        return false;
    }

    return CGenericVisNodeBase::removeInput(path, inputData);
}

bool CVtkVolume::fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData)
{
    if(!path || !outputData)
        return false;

    if(path->pathName() == "Volume")
    {
        *outputData = &d->outputData;
        return true;
    }

    return CGenericVisNodeBase::fetchOutput(path, outputData);
}

bool CVtkVolume::outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData)
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

int CVtkVolume::propertyCount()
{
    return 0;
}

QString CVtkVolume::propertyName(int index)
{
    Q_UNUSED(index);
    return QString();
}

QString CVtkVolume::propertyGroup(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkVolume::propertyValue(int index)
{
    Q_UNUSED(index);
    return QVariant();
}

void CVtkVolume::setPropertyValue(int index, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
}

bool CVtkVolume::hasEditor(int index)
{
    Q_UNUSED(index);
    return false;
}

QWidget* CVtkVolume::createEditor(int index)
{
    Q_UNUSED(index);
    return 0;
}

void CVtkVolume::setEditorValue(int index, QWidget* widget, QVariant value)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(value);
}

QVariant CVtkVolume::editorValue(int index, QWidget* widget)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
}

void CVtkVolume::connectEditor(int index, QWidget* widget, QObject* receiver, const char* member)
{
    Q_UNUSED(index);
    Q_UNUSED(widget);
    Q_UNUSED(receiver);
    Q_UNUSED(member);
}

QString CVtkVolume::propertyValueString(int index)
{
    Q_UNUSED(index);
    return QString();
}

QVariant CVtkVolume::propertyValueFromString(int index, QString valueStr)
{
    Q_UNUSED(index);
    Q_UNUSED(valueStr);

    return QVariant();
}

bool CVtkVolume::canLoadSaveProperty(int index)
{
    Q_UNUSED(index);

    return false;
}

#endif

void CVtkVolume::setVtkObject(vtkObject* ptr)
{
    d->volume = dynamic_cast<vtkVolume*>(ptr);
    d->outputData.setVolume(d->volume);
}





