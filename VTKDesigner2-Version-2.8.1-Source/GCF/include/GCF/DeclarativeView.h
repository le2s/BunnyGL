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

#ifndef DECLARATIVEVIEW_H
#define DECLARATIVEVIEW_H

#include <QDeclarativeView>
#include "Common.h"
#include "IContainer.h"

namespace GCF
{

class AbstractComponent;
class DeclarativeView;

class DeclarativeViewListener : virtual public IContainer
{
public:
    virtual void declarativeViewCreated(GCF::DeclarativeView* view) = 0;
    virtual void declarativeViewDestroyed(GCF::DeclarativeView* view) = 0;
};

struct DeclarativeViewData;
class GCF_EXPORT_CLASS DeclarativeView : public QDeclarativeView
{
    Q_OBJECT

public:
    DeclarativeView(QWidget *parent = 0);
    ~DeclarativeView();

    void setSourceLater(const QUrl& source);

    Q_PROPERTY(bool transparentBackground READ isTransparentBackground WRITE setTransparentBackground)
    void setTransparentBackground(bool val);
    bool isTransparentBackground() const;

protected slots:
    void onAllComponentsLoaded();

public:
    static void initialzieWithQmlObjects(QDeclarativeEngine* engine);

private:
    DeclarativeViewData* d;
};

struct DeclarativeEnvironmentData;
class DeclarativeEnvironment : public QObject
{
    Q_OBJECT

public:
    ~DeclarativeEnvironment();

    Q_INVOKABLE QObject* findObject(const QString& iface) const;
    Q_INVOKABLE QObject* getObject(const QString& completeName) const;

private:
    static DeclarativeEnvironment& instance();
    DeclarativeEnvironment(QObject* parent=0);

    void addView(DeclarativeView* view);
    void removeView(DeclarativeView* view);
    void initializeContext(DeclarativeView* view);
    void initializeContext(QDeclarativeContext* context);

    void addNamedObject(const QString& qmlName, QObject* obj);
    void removeNamedObject(const QString& qmlName);

private slots:
    void slotComponentCreated(GCF::AbstractComponent* comp);
    void slotComponentDestroyed(GCF::AbstractComponent* comp);

private:
    friend class DeclarativeView;
    DeclarativeEnvironmentData* d;
};

}

Q_DECLARE_INTERFACE(GCF::DeclarativeViewListener, "com.vcreatelogic.DeclarativeViewListener/1.0")

#endif // DECLARATIVEVIEW_H
