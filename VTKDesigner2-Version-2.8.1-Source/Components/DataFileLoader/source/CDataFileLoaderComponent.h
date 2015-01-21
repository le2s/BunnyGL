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

#ifndef C_DATA_FILE_LOADER_COMPONENT_H
#define C_DATA_FILE_LOADER_COMPONENT_H

#include <GCF/AbstractComponent>
#include <GCF/IAboutData>

#include "IDataFileLoaderRegistry.h"

struct CDataFileLoaderComponentData;
class CDataFileLoaderComponent : public GCF::AbstractComponent,
                                 virtual public IDataFileLoaderRegistry,
                                 virtual public IAboutData
{
    Q_OBJECT
    GCF_DECLARE_COMPONENT(CDataFileLoaderComponent)
    Q_PROPERTY(QString DefaultDataFilePath READ defaultDataFilePath WRITE setDefaultDataFilePath)
    Q_INTERFACES(IDataFileLoaderRegistry IAboutData)

public:
    static CDataFileLoaderComponent& instance();
    ~CDataFileLoaderComponent();

    void setDefaultDataFilePath(QString path);
    QString defaultDataFilePath() const;

protected:
    CDataFileLoaderComponent();
    void initializeComponent();
    void finalizeComponent();
    QObject* fetchObject(const QString& completeName) const;
    QWidget* fetchWidget(const QString& completeName) const;
    void creationAnnounced();

public slots:
    void loadDataFile();
    void loadDataFile(const QString& fileName, bool newCanvas=false);

public:
    // IContainer implementation
    QObject* containerObject();

    // IDataFileLoaderRegistry implementation
    void registerDataFileLoader(IDataFileLoader* loader);
    void unregisterDataFileLoader(IDataFileLoader* loader);
    int dataFileLoaderCount();
    IDataFileLoader* dataFileLoader(int index);
    IDataFileLoader* dataFileLoader(QString fileName);

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

private:
    CDataFileLoaderComponentData* d;
};

#endif
