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

#include "GCF/DomAccess.h"
#include <QAtomicInt>
#include <QStringList>
#include <QFile>

namespace GCF
{
    struct SharedData
    {
        SharedData() {
            m_refCount.ref();
        }

        void ref() { m_refCount.ref(); }
        void deref() { if(!m_refCount.deref()) delete this; }
        
    private:
        QAtomicInt m_refCount;
    };

    struct DomElementData : public SharedData
    {
        DomElementData() : oneLookupDone(false),
            allLookupDone(false) { }

        QString tagName;
        QDomElement parentElement;
        QList<QDomElement> elementList;
        bool oneLookupDone;
        bool allLookupDone;
        
        void oneLookup() {
            if(oneLookupDone)
                return;
            
            QDomElement e = parentElement.firstChildElement(tagName);
            if( !e.isNull() )
                elementList.append( e );

            oneLookupDone = true;
        }
        
        void allLookup() {
            if(allLookupDone)
                return;
                
            oneLookup();
            if(!elementList.count())
            {
                allLookupDone = true;
                return;
            }
            
            QDomElement e = elementList.at(0);
            while(1)
            {
                e = e.nextSiblingElement(tagName);
                if(e.isNull())
                    break;

                elementList.append(e);
            }
            
            allLookupDone = true;
        }
    };
}

GCF::DomElement::DomElement()
{
    d = new DomElementData;
}

GCF::DomElement::DomElement(const DomElement& other)
{
    d = other.d;
    d->ref();
}

GCF::DomElement::~DomElement()
{
    d->deref();
}

GCF::DomElement& GCF::DomElement::operator = (const GCF::DomElement& other)
{
    d->deref();
    d = other.d;
    d->ref();
    return *this;
}

bool GCF::DomElement::operator == (const GCF::DomElement& other) const
{
    return d == other.d;
}

bool GCF::DomElement::isValid() const
{
    d->oneLookup();
    if( !d->elementList.count() )
        return false;
    return true;
}

QString GCF::DomElement::tagName() const
{
    return d->tagName;
}

QDomElement GCF::DomElement::element() const
{
    if( this->isValid() )
        return d->elementList.at(0);
        
    return QDomElement();
}

QList<QDomElement> GCF::DomElement::elements() const
{
    d->allLookup();
    return d->elementList;
}

GCF::DomElement GCF::DomElement::operator [] (const QString& tagName) const
{
    int index = tagName.split('/').last().toInt();
    if(index < 0)
        index = 0;

    DomElement newElement;
    newElement.d->tagName = tagName;
    if(!this->isValid())
        return newElement;

    if(index > 0 && index >= this->elements().count())
        return newElement;
        
    newElement.d->parentElement = d->elementList.at(index);
    return newElement;
}

///////////////////////////////////////////////////////////////////////////////
// DomAccess
///////////////////////////////////////////////////////////////////////////////

namespace GCF
{        
    struct DomAccessData : public SharedData
    {
        QDomDocument doc;
        DomElement rootElement;
    };
}

GCF::DomAccess::DomAccess(const QString& fileName)
{
    d = new DomAccessData;
    
    QFile file(fileName);
    file.open(QFile::ReadOnly);
    d->doc.setContent(&file);
    
    d->rootElement.d->parentElement = QDomElement();
    d->rootElement.d->tagName = d->doc.documentElement().tagName();
    d->rootElement.d->elementList.append( d->doc.documentElement() );
    d->rootElement.d->oneLookupDone = true;
    d->rootElement.d->allLookupDone = true;
}

GCF::DomAccess::DomAccess(QDomDocument doc)
{
    d = new DomAccessData;
    d->doc = doc;

    d->rootElement.d->parentElement = QDomElement();
    d->rootElement.d->tagName = d->doc.documentElement().tagName();
    d->rootElement.d->elementList.append( d->doc.documentElement() );
    d->rootElement.d->oneLookupDone = true;
    d->rootElement.d->allLookupDone = true;
}

GCF::DomAccess::~DomAccess()
{
    d->deref();
}

GCF::DomAccess& GCF::DomAccess::operator = (const GCF::DomAccess& other)
{
    d->deref();
    d = other.d;
    d->ref();
    return *this;
}

bool GCF::DomAccess::operator == (const GCF::DomAccess& other) const
{
    return d == other.d;
}

GCF::DomElement GCF::DomAccess::operator [] (const QString& tagName) const
{
    if(tagName == d->rootElement.tagName())
        return d->rootElement;
        
    return d->rootElement[tagName];
}

GCF::DomElement GCF::DomAccess::query(const QString& name) const
{
    QStringList comps = name.split('.');
    if(!comps.count())
        return GCF::DomElement();

    GCF::DomElement ret = (*this)[comps.first()];
    comps.takeFirst();

    while(comps.count())
    {
        ret = ret[ comps.first() ];
        comps.takeFirst();
    }

    return ret;
}

