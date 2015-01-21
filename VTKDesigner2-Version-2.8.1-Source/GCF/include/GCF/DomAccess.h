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

#ifndef DOM_ACCESS_H
#define DOM_ACCESS_H

#include <GCF/Common>
#include <QDomDocument>
#include <QDomElement>
#include <QList>

namespace GCF
{
class DomAccess;
struct DomAccessData;

struct DomElementData;
class GCF_EXPORT_CLASS DomElement
{
public:
    DomElement(const DomElement& other);
    ~DomElement();

    DomElement& operator = (const DomElement& other);
    bool operator == (const DomElement& other) const;

    bool isValid() const;
    QString tagName() const;
    QDomElement element() const;
    QList<QDomElement> elements() const;

    QString attribute(const QString& name, const QString& defValue=QString()) const {
        return element().attribute(name, defValue);
    }

    DomElement operator [] (const QString& tagName) const;

private:
    friend class DomAccess;
    friend struct DomAccessData;

    DomElement();
    DomElementData* d;
};

class GCF_EXPORT_CLASS DomAccess
{
public:
    DomAccess(const QString& fileName);
    DomAccess(QDomDocument doc);
    ~DomAccess();

    DomAccess& operator = (const DomAccess& other);
    bool operator == (const DomAccess& other) const;

    DomElement operator [] (const QString& tagName) const;
    DomElement query(const QString& name) const;

private:
    DomAccessData* d;
};
}

#endif

