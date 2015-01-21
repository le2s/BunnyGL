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

#ifndef C_OPEN_HAPTICS_VTK_COMPONENT_H
#define C_OPEN_HAPTICS_VTK_COMPONENT_H

#include <GCF/AbstractComponent>
#include "IVisSystemNodeFactory.h"
#include <GCF/IAboutData>

struct COpenHapticsVTKComponentData;
class COpenHapticsVTKComponent : public GCF::AbstractComponent,
                                 virtual public IVisSystemNodeFactory,
                                 virtual public IAboutData
{
    Q_OBJECT
    Q_PROPERTY(QString HDDefaultDevice READ defaultDeviceName WRITE setDefaultDeviceName)
    Q_PROPERTY(int HapticPollInterval READ hapticPollInterval WRITE setHapticPollInterval)
    Q_INTERFACES(IVisSystemNodeFactory IAboutData)
    GCF_DECLARE_COMPONENT(COpenHapticsVTKComponent)

public:
    static COpenHapticsVTKComponent & instance();
    ~COpenHapticsVTKComponent();

    void setDefaultDeviceName(const QString& name);
    const QString& defaultDeviceName() const;

    void setHapticPollInterval(int mSec);
    int hapticPollInterval() const;

    QIcon nodeIcon() const;

    // Custom methods.
    bool isHapticDeviceInitialized() const;
    unsigned int hapticsDeviceHandle() const;
    void* hapticsDeviceContext() const;
    QString hapticsDeviceInfo() const;
    void initializeHapticsDevice();
    void finalizeHapticsDevice();

    // IContainer implementation
    QObject* containerObject();

    // IVisSystemNodeFactory implementation
    QStringList nodeClassNameList();
    IVisSystemNodeDesc* nodeDesc(QString nodeClassName);
    bool canCreate(QString nodeClassName);
    IVisSystemNode* createNode(QString nodeClassName);
    void  deleteNode(IVisSystemNode* node);

    // IAboutData implementation
    QString productName() const;
    QString organization() const;
    QImage programLogo() const;
    QString version() const;
    QString shortDescription() const;
    QString homepage() const;
    QString bugAddress() const;
    const QList<GCF::AboutPerson> authors() const;
    const QList<GCF::AboutPerson> credits() const;
    QString license() const;
    QString copyrightStatement() const;

protected:
    COpenHapticsVTKComponent();
    void initializeComponent();
    void finalizeComponent();
    void creationAnnounced();
    QAction* fetchAction(const QString& completeName) const;
    QObject* fetchObject(const QString& completeName) const;
    QWidget* fetchWidget(const QString& completeName) const;
    void componentDestroyed(GCF::AbstractComponent* comp);

protected slots:
    void on_hapticPollIntervalSlider_sliderReleased();
    void slotEnableHaptics(bool val);

private:
    COpenHapticsVTKComponentData* d;
};

#endif

