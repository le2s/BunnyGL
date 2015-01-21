/****************************************************************************
**
** Copyright (C) VCreate Logic Private Limited, Bangalore
**
** Use of this file is limited according to the terms specified by
** VCreate Logic Private Limited, Bangalore.  Details of those terms
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

#include "PropertyEditorComponent.h"
#include "PropertyEditor.h"
#include <GCF/ComponentFactory>
#include <GCF/IComponentPlugin>
#include "ValueEditorCreators.h"
#include "ValueEditorFactory.h"
#include "IEditableProperties.h"

struct GCF::Components::PropertyEditorComponentData
{
    PropertyEditorComponentData() : propertyEditor(0),
        valueEditorFactory(0) { }

    GCF::Components::PropertyEditor* propertyEditor;
    GCF::Components::ValueEditorFactory* valueEditorFactory;

    QStringList getSkipClassList(QObject* object);
    int indexOfEditableProperty(IEditableProperties* editableProps, const QString& propName);
};

GCF_DEFINE_STD_COMPONENT(PropertyEditorComponent)

GCF::Components::PropertyEditorComponent & GCF::Components::PropertyEditorComponent::instance()
{
    static GCF::Components::PropertyEditorComponent* theInstance = GCF_CREATE_COMPONENT(PropertyEditorComponent);
    return *theInstance;
}

GCF::Components::PropertyEditorComponent::PropertyEditorComponent()
{
    d = new GCF::Components::PropertyEditorComponentData;
    d->propertyEditor = 0; // new GCF::Components::PropertyEditor;
}

GCF::Components::PropertyEditorComponent::~PropertyEditorComponent()
{
    delete d->valueEditorFactory;
    delete d;
}

QObject* GCF::Components::PropertyEditorComponent::fetchObject(const QString& completeName) const
{
    QStringList comps = completeName.split('.');
    GCF::Components::PropertyEditorComponent* that = const_cast<GCF::Components::PropertyEditorComponent*>(this);

    if(comps.last() == "controller")
        return that;

    return 0;
}

QWidget* GCF::Components::PropertyEditorComponent::fetchWidget(const QString& completeName) const
{
    QStringList comps = completeName.split('.');

    if(comps.last() == "propertyEditor")
    {
        if(!d->propertyEditor)
        {
            d->propertyEditor = new GCF::Components::PropertyEditor;
            d->propertyEditor->setEditorFactory(d->valueEditorFactory);
        }

        return d->propertyEditor;
    }

    return 0;
}

void GCF::Components::PropertyEditorComponent::addChildWidget(QWidget* parent, QWidget* child, const QString& hint)
{
    Q_UNUSED(hint);
    Q_UNUSED(parent);
    child->setParent(0);
}

void GCF::Components::PropertyEditorComponent::initializeComponent()
{
    GCF::Components::ValueEditorFactory* factory = new GCF::Components::ValueEditorFactory;
    // d->propertyEditor->setEditorFactory(factory);
    d->valueEditorFactory = factory;

    // Default editor creator set
#define REGISTER_NUMBER_CREATOR(x) \
    static GCF::Components::NumberEditorCreator x##Creator(qMetaTypeId<x>(), #x); \
    factory->registerCreator(&x##Creator)

    REGISTER_NUMBER_CREATOR(int);
    REGISTER_NUMBER_CREATOR(uint);
    REGISTER_NUMBER_CREATOR(double);
    REGISTER_NUMBER_CREATOR(qreal);
    // REGISTER_NUMBER_CREATOR(qsreal);
#if QT_VERSION > 0x040704
    REGISTER_NUMBER_CREATOR(qint8);
    REGISTER_NUMBER_CREATOR(qint16);
    REGISTER_NUMBER_CREATOR(qint32);
    REGISTER_NUMBER_CREATOR(qint64);
    REGISTER_NUMBER_CREATOR(quint8);
    REGISTER_NUMBER_CREATOR(quint16);
    REGISTER_NUMBER_CREATOR(quint32);
    REGISTER_NUMBER_CREATOR(quint64);
    REGISTER_NUMBER_CREATOR(long);
    REGISTER_NUMBER_CREATOR(ulong);
    REGISTER_NUMBER_CREATOR(qlonglong);
    REGISTER_NUMBER_CREATOR(qulonglong);
#endif

#undef REGISTER_NUMBER_CREATOR

    static GCF::Components::BoolEditorCreator boolCreator;
    factory->registerCreator(&boolCreator);
    static GCF::Components::StringEditorCreator stringCreator;
    factory->registerCreator(&stringCreator);
    static GCF::Components::PointEditorCreator pointCreator;
    factory->registerCreator(&pointCreator);
    static GCF::Components::ColorEditorCreator colorCreator;
    factory->registerCreator(&colorCreator);
    static GCF::Components::VectorEditorCreator vectorCreator;
    factory->registerCreator(&vectorCreator);
    static GCF::Components::TriangleEditorCreator triangleCreator;
    factory->registerCreator(&triangleCreator);
    static GCF::Components::BoundingBoxEditorCreator boundingBoxCreator;
    factory->registerCreator(&boundingBoxCreator);
    static GCF::Components::ValueRangeEditorCreator valueRangeEditorCreator;
    factory->registerCreator(&valueRangeEditorCreator);
    static GCF::Components::FileNameEditorCreator fileNameCreator;
    factory->registerCreator(&fileNameCreator);
    static GCF::Components::FontEditorCreator fontCreator;
    factory->registerCreator(&fontCreator);
    static GCF::Components::SizeEditorCreator sizeCreator;
    factory->registerCreator(&sizeCreator);
    static GCF::Components::SizeFEditorCreator sizeFCreator;
    factory->registerCreator(&sizeFCreator);
    static GCF::Components::Point2DEditorCreator point2dCreator;
    factory->registerCreator(&point2dCreator);
    static GCF::Components::PointF2DEditorCreator pointF2dCreator;
    factory->registerCreator(&pointF2dCreator);
    static GCF::Components::ListEditorCreator doubleListEditorCreator(true);
    factory->registerCreator(&doubleListEditorCreator);
    static GCF::Components::ListEditorCreator intListEditorCreator(false);
    factory->registerCreator(&intListEditorCreator);
    static GCF::Components::EnumEditorCreator enumCreator;
    factory->setEnumCreator(&enumCreator);
}

void GCF::Components::PropertyEditorComponent::finalizeComponent()
{
    if(d->propertyEditor)
        d->propertyEditor->setObject(0);
}

QWidget* GCF::Components::PropertyEditorComponent::createWidget(const QString& completeName) const
{
    QStringList comps = completeName.split('.');

    if(comps.last() == "propertyEditor")
    {
        GCF::Components::PropertyEditor* propEditor = new GCF::Components::PropertyEditor;
        propEditor->setEditorFactory(d->valueEditorFactory);
        return propEditor;
    }

    return 0;
}

QObject* GCF::Components::PropertyEditorComponent::containerObject()
{
    return this;
}

QString GCF::Components::PropertyEditorComponent::productName() const
{
    return "Property Editor";
}

QString GCF::Components::PropertyEditorComponent::organization() const
{
    return "VCreate Logic (P) Ltd";
}

QImage GCF::Components::PropertyEditorComponent::programLogo() const
{
    return QImage();
}

QString GCF::Components::PropertyEditorComponent::version() const
{
    return "1.0";
}

QString GCF::Components::PropertyEditorComponent::shortDescription() const
{
    return "This component provides a property editor widget that can show properties of "
        "any QObject in the system.";
}

QString GCF::Components::PropertyEditorComponent::homepage() const
{
    return "http://www.vcreatelogic.com/products/gcf";
}

QString GCF::Components::PropertyEditorComponent::bugAddress() const
{
    return "http://www.vcreatelogic.com/products/gcf/feedback/bugs";
}

const QList<GCF::AboutPerson> GCF::Components::PropertyEditorComponent::authors() const
{
    static QList<GCF::AboutPerson> retList;
    if(retList.count() == 0)
        retList << GCF::AboutPerson("Prashanth N Udupa", "prashanth@vcreatelogic.com", "http://www.vcreatelogic.com", "Founder, Vision, Design, Development");

    return retList;
}

const QList<GCF::AboutPerson> GCF::Components::PropertyEditorComponent::credits() const
{
    static QList<GCF::AboutPerson> retList;
    return retList;
}

QString GCF::Components::PropertyEditorComponent::license() const
{
    return "Dual License: GPL Version 2 for FOSS development, Commercial for others.";
}

QString GCF::Components::PropertyEditorComponent::copyrightStatement() const
{
    return QString();
}

bool GCF::Components::PropertyEditorComponent::saveProperties(QObject* obj, QDomDocument doc, QDomElement parentE)
{
    // Adopted from VTK Designer project from VCreate Logic.
    IValueEditorFactory* veFactory = d->valueEditorFactory;

    QStringList skipClassList = d->getSkipClassList(obj);

    // First lets save all the Q_PROPERTY properties in obj
    const QMetaObject* mo = obj->metaObject();
    while( mo && mo != &QObject::staticQtMetaObject )
    {
        if(!(mo->propertyCount()-mo->propertyOffset()))
        {
            mo = mo->superClass();
            continue;
        }

        QString sectionName(mo->className());
        if( skipClassList.contains(sectionName) )
        {
            mo = mo->superClass();
            continue;
        }

        for(int i=mo->propertyOffset(); i<mo->propertyCount(); i++)
        {
            QMetaProperty prop = mo->property(i);
            if(!prop.isStored())
                continue;

            QString propName = QString("%1").arg(prop.name());
            QString propType = QString("%1").arg(prop.typeName());
            QString propValueStr;
            QVariant propValue = prop.read(obj);
            bool forceWritePropertyValue = false;

            if(veFactory)
            {
                IValueEditorCreator* vec = prop.isEnumType() ? veFactory->enumCreator() : veFactory->creator(prop.typeName());
                if(vec)
                {
                    propValueStr = vec->toString(propValue);
                    forceWritePropertyValue = true;
                }
                else
                    propValueStr = propValue.toString();
            }
            else
                propValueStr = propValue.toString();

            if(propValueStr.isEmpty() && !forceWritePropertyValue)
                continue;

            QDomElement propE = doc.createElement("Property");
            parentE.appendChild(propE);
            propE.setAttribute("Name", propName);
            propE.setAttribute("Type", propType);
            propE.setAttribute("Value", propValueStr);
        }

        mo = mo->superClass();
    }

    // Now let us save editable properties.
    IEditableProperties* editableProps = qobject_cast<IEditableProperties*>(obj);
    if(!editableProps)
        return true;

    IEditablePropertiesExt* editablePropsExt = qobject_cast<IEditablePropertiesExt*>(obj);

    for(int i=0; i<editableProps->propertyCount(); i++)
    {
        if(editablePropsExt && !editablePropsExt->canLoadSaveProperty(i))
            continue;

        QString propName = editableProps->propertyName(i);
        QVariant propValue = editableProps->propertyValue(i);
        QString propType = propValue.typeName();
        QString propValueStr;

        if(editableProps->hasEditor(i))
            propValueStr = editableProps->propertyValueString(i);
        else if(veFactory)
        {
            IValueEditorCreator* vec = veFactory->creator(propValue.typeName());
            if(vec)
                propValueStr = vec->toString(propValue);
            else
                propValueStr = propValue.toString();
        }
        else
            propValueStr = propValue.toString();

        QDomElement propE = doc.createElement("EProperty");
        parentE.appendChild(propE);
        propE.setAttribute("Index", QString("%1").arg(i));
        propE.setAttribute("Name", propName);
        propE.setAttribute("Type", propType);
        propE.setAttribute("Value", propValueStr);
    }

    return true;
}

bool GCF::Components::PropertyEditorComponent::loadProperties(QObject* object, QDomDocument doc, QDomElement parentE)
{
    // First manage the default properties of this object.
    // defaultProperty(object);

    QDomNodeList childNodes = parentE.childNodes();

    const QMetaObject* mo = object->metaObject();
    IEditableProperties* editableProps = qobject_cast<IEditableProperties*>(object);
    IEditablePropertiesExt* editablePropsExt = qobject_cast<IEditablePropertiesExt*>(object);
    IValueEditorFactory* veFactory = d->valueEditorFactory;

    for(int i=0; i<childNodes.count(); i++)
    {
        QDomNode childNode = childNodes.item(i);
        QDomElement childE = childNode.toElement();

        if(childE.tagName() == "Property")
        {
            // Standard Qt property
            QString propName = childE.attribute("Name");
            QString propValueStr = childE.attribute("Value");
            QVariant propValue;
            int propIndex = mo->indexOfProperty( qPrintable(propName) );
            QMetaProperty prop = mo->property(propIndex);
            if(!prop.isValid() || !prop.isStored())
                continue; // we ignore loading of properties that are not stored

            if(veFactory)
            {
                IValueEditorCreator* vec = prop.isEnumType() ? veFactory->enumCreator() :
                                           veFactory->creator(prop.typeName());
                if(vec)
                    propValue = vec->fromString(propValueStr);
                else
                    propValue = propValueStr;
            }
            else
                propValue = propValueStr;

            object->setProperty( qPrintable(propName), propValue );
        }
        else if(editableProps)
        {
            int propIndex = childE.attribute("Index").toInt();
            QString propName = childE.attribute("Name");

            // Check to see if propIndex has the same property name is propName
            if( editableProps->propertyName(propIndex) != propName )
                propIndex = d->indexOfEditableProperty(editableProps, propName);

            if(propIndex < 0 || propIndex >= editableProps->propertyCount())
                continue;

            if(editablePropsExt && !editablePropsExt->canLoadSaveProperty(propIndex))
                continue;

            QString propValueStr = childE.attribute("Value");
            QString propType = childE.attribute("Type");
            QVariant propValue;

            if(editableProps->hasEditor(propIndex))
                propValue = editableProps->propertyValueFromString(propIndex, propValueStr);
            else if(veFactory)
            {
                IValueEditorCreator* vec = veFactory->creator( qPrintable(propType) );
                if(vec)
                    propValue = vec->fromString(propValueStr);
                else
                    propValue = propValueStr;
            }
            else
                propValue = propValueStr;

            editableProps->setPropertyValue(propIndex, propValue);
        }
    }

    return true;
}

IValueEditorFactory* GCF::Components::PropertyEditorComponent::valueEditorFactory() const
{
    return d->valueEditorFactory;
}

QStringList GCF::Components::PropertyEditorComponentData::getSkipClassList(QObject* object)
{
    QStringList ret;

    const QMetaObject* mo = object->metaObject();
    while( mo && mo != &QObject::staticMetaObject )
    {
        for(int i=mo->classInfoOffset(); i<mo->classInfoCount(); i++)
        {
            QMetaClassInfo ci = mo->classInfo(i);
            if( qstrcmp(ci.name(), "SkipClassProperties") )
                continue;

            QString value( ci.value() );
            QStringList valueList = value.split(" ", QString::SkipEmptyParts);
            ret += valueList;
        }

        mo = mo->superClass();
    }

    return ret;
}

int GCF::Components::PropertyEditorComponentData::indexOfEditableProperty(IEditableProperties* editableProps, const QString& propName)
{
    if( !editableProps || propName.isEmpty() )
        return -1;

    for(int i=0; i<editableProps->propertyCount(); i++)
    {
        QString pName = editableProps->propertyName(i);
        if( pName == propName )
            return i;
    }

    return -1;
}

GCF_EXPORT_STD_COMPONENT_PLUGIN(PropertyEditorComponent)

