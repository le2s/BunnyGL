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

#ifndef UTILS_COMPONENT_H
#define UTILS_COMPONENT_H

#include "IXmlUtils.h"
#include "IMathUtils.h"
#include "IColorUtils.h"
#include "IFileSystemUtils.h"

#include <GCF/AbstractComponent>
#include <GCF/IAboutData>

namespace GCF
{
namespace Components
{
struct UtilsComponentData;
class UtilsComponent : public AbstractComponent,
        virtual public IXmlUtils,
        virtual public IMathUtils,
        virtual public IAboutData,
        virtual public IColorUtils,
        virtual public IFileSystemUtils
{
    Q_OBJECT
    Q_INTERFACES(IXmlUtils IMathUtils IAboutData IColorUtils IFileSystemUtils)
    GCF_DECLARE_STD_COMPONENT(UtilsComponent)

    public:
        static UtilsComponent& instance();
    ~UtilsComponent();

    // IContainer implementation
    QObject* containerObject();

    // IXmlUtils
    QList<QDomElement> findElements(QDomElement parentE, QString tagName);
    QDomElement findElement(QDomElement parentE, QString tagName);
    bool savePixmap(QDomDocument doc, QDomElement element, QPixmap pixmap);
    bool loadPixmap(QDomElement element, QPixmap& pixmap);
    bool parseDom(QDomDocument doc, QStringList tagNames, QMap<QString, QList<QDomElement> > & elementMap);
    bool saveData(QDomDocument doc, QDomElement element, QByteArray array);
    bool loadData(QDomElement element, QByteArray& array);

    // IMathUtils
    Q_INVOKABLE double toRadians(double angle);
    Q_INVOKABLE double toDegrees(double radians);
    Q_INVOKABLE double sinAngle(double angle);
    Q_INVOKABLE double cosAngle(double angle);
    Q_INVOKABLE double tanAngle(double angle);
    Q_INVOKABLE double aSinAngle(double val);
    Q_INVOKABLE double aCosAngle(double val);
    Q_INVOKABLE double aTanAngle(double val);
    Q_INVOKABLE double randomNumber(double min, double max);

    // IAboutData implementation
    QString productName() const;
    QString organization() const;
    QImage programLogo() const;
    QString version() const;
    QString shortDescription() const;
    QString homepage() const;
    QString bugAddress() const;
    const QList<AboutPerson> authors() const;
    const QList<AboutPerson> credits() const;
    QString license() const;
    QString copyrightStatement() const;

protected:
    UtilsComponent();
    QObject* fetchObject(const QString& completeName) const;
    void initializeComponent();
    void finalizeComponent();

private:
    UtilsComponentData* d;

public:
    // IColorUtils implementation
    QColor randomColor(const QPair<qreal,qreal>& redRange = qMakePair(0.0,1.0),
                       const QPair<qreal,qreal>& greenRange = qMakePair(0.0,1.0),
                       const QPair<qreal,qreal>& blueRange = qMakePair(0.0,1.0),
                       const QPair<qreal,qreal>& alphaRange = qMakePair(1.0,1.0));
    QColor interpolatedColor(qreal value, const QColor& start, const QColor& end);

    // IFileSystemUtils implementation
    QString findFileInPath(const QString& fileName);
    QStringList findFilesInPath(const QString& fileName);
    void deleteDirectory(const QString& dir, bool deleteDir=false);
};
}
}

#endif

