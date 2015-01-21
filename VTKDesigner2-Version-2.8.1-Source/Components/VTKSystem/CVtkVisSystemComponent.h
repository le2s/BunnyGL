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

#ifndef C_VTK_VIS_SYSTEM_COMPONENT_H
#define C_VTK_VIS_SYSTEM_COMPONENT_H

#include <GCF/AbstractComponent>
#include <GCF/IAboutData>

#include "IVisSystemNodeFactory.h"
#include "IProjectPart.h"
#include "IVtkXmlUtils.h"
#include "IDataPathResolver.h"

#include <QTabWidget>

class IVisNetworkConnection;
class QMenu;

struct CVtkVisSystemComponentData;
class CVtkVisSystemComponent : public GCF::AbstractComponent,
                               virtual public IVisSystemNodeFactory,
                               virtual public IAboutData,
                               virtual public IProjectPart,
                               virtual public IVtkXmlUtils,
                               virtual public IDataPathResolver
{
    Q_OBJECT
    Q_INTERFACES(IVisSystemNodeFactory IAboutData IProjectPart IVtkXmlUtils IDataPathResolver)
    Q_PROPERTY(QString DataPath READ dataPath WRITE setDataPath)
    GCF_DECLARE_COMPONENT(CVtkVisSystemComponent)

public:
    static CVtkVisSystemComponent& instance();
    ~CVtkVisSystemComponent();

    QTabWidget* vtkOutputDisplay() const;
    QString resolvePath(const QString& path); // IDataPathResolver implementation

    void setDataPath(const QString& path);
    QString dataPath() const;

    Q_INVOKABLE void recordStart(QObject* renNode, QString fileName, QString format, int interval, int nrFrames);

public slots:
    void aboutVtk();
    void recordStart();
    void recordEnd();

protected:
    CVtkVisSystemComponent();
    void initializeComponent();
    void finalizeComponent();
    void justActivated();
    void aboutToDeactivate();
    QAction* fetchAction(const QString& completeName) const;
    QObject* fetchObject(const QString& completeName) const;
    QWidget* fetchWidget(const QString& completeName) const;
    void addChildWidget(QWidget* parent, QWidget* child, const QString& hint);
    void removeChildWidget(QWidget* parent, QWidget* child, const QString& hint);
    void creationAnnounced();
    void componentDestroyed(GCF::AbstractComponent* comp);

public:
    // IContainer implementation
    QObject* containerObject();

    // IVisSystemNodeFactory implementation
    QStringList nodeClassNameList();
    IVisSystemNodeDesc* nodeDesc(QString nodeClassName);
    bool canCreate(QString nodeClassName);
    IVisSystemNode* createNode(QString nodeClassName);
    void deleteNode(IVisSystemNode* node);

protected:
    void timerEvent(QTimerEvent* te);

public: // IAboutData
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

public: // IProjectPart implementation
    QString projectPartID();
    bool newProjectPart();
    bool loadProjectPart(QDomDocument doc, QDomElement partE);
    bool initializeProjectPart();
    bool saveProjectPart(QDomDocument doc, QDomElement partParentE);
    bool closeProjectPart();

signals:
    void projectPartModified();

public:
    // IVtkXmlUtils implementation
    void savePoints(vtkPoints* points, QDomDocument doc, QDomElement ele);
    void loadPoints(vtkPoints* points, QDomDocument doc, QDomElement ele);
    void saveCells(vtkCellArray* cells, QDomDocument doc, QDomElement ele);
    void loadCells(vtkCellArray* cells, QDomDocument doc, QDomElement ele);
    void saveData(vtkDataArray* dataArray, QDomDocument doc, QDomElement ele);
    void loadData(vtkDataArray* dataArray, QDomDocument doc, QDomElement ele);
    void savePolyData(vtkPolyData* polyData, QDomDocument doc, QDomElement ele);
    void loadPolyData(vtkPolyData* polyData, QDomDocument doc, QDomElement ele);
    void saveImageData(vtkImageData* imageData, QDomDocument doc, QDomElement ele);
    void loadImageData(vtkImageData* imageData, QDomDocument doc, QDomElement ele);

public slots:
    // For capturing snapshots to the clipboard
    void captureSnapshotToClipboard();
    void slotWorkspaceWidgetActivated(QWidget* widget);
    void slotConnectionContextMenuEvent(IVisNetworkConnection* node, QMenu* menu, Qt::KeyboardModifiers modifiers);
    void slotExploreData();

private:
    CVtkVisSystemComponentData* d;
};

#endif

