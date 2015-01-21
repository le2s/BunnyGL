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

#include "COpenHapticsVTKComponent.h"
#include "IVisSystemNodeFactoryRegistry.h"
#include "IVisSystemNodeDesc.h"
#include "IVisNetwork.h"
#include "IVisNetworkCanvas.h"
#include "IVisNetworkConnection.h"
#include "IScriptEngineManager.h"

#include <GCF/IComponentPlugin>
#include <GCF/ComponentFactory>

#include "CGenericVisNodeDesc.h"
#include "CGenericVisNodeBase.h"
#include "CVtkOpenHapticsCursorActor.h"
#include "CVtkOpenHapticsActor.h"
#include "CVtkOpenHapticsRenderer.h"
#include "CVtkOpenHapticsProperty.h"
#include "CVtkOpenHapticsEffect.h"
#include "vtkOpenHapticsRenderer.h"

#include "CShapeRenderModeSelector.h"

#include <QMessageBox>
#include <QMainWindow>
#include <QSlider>
#include <QWidgetAction>
#include <QDate>
#include <QApplication>
#include <QFile>
#include <QCheckBox>
#include <QProcess>

#include "HL/hl.h"
#include "HD/hd.h"

#define OPEN_HAPTICS_VTK_LICENSE_OEM
// #define VERIFY_DEVICE_SERIAL_NO
// #define VERIFY_HOST_ID

struct COpenHapticsVTKComponentData
{
	COpenHapticsVTKComponentData() : nodeFactoryRegistryComp(0),
        nodeFactoryRegistry(0), createdNodeCount(0), hapticRendererCreated(false),
        hapticPollIntervalSlider(0), enableHapticsCheckBox(0), visNetworkCanvas(0),
        visNetwork(0), scriptEngineManager(0), shapeRenderModeSelector(0), deviceHandle(0), 
        deviceContext(0), validityFlag(false), validityFlagChecked(false) { }

    QString licenseFile;
    QMap<QString, CGenericVisNodeDesc*> nodeDescMap;
    GCF::AbstractComponent* nodeFactoryRegistryComp;
    IVisSystemNodeFactoryRegistry* nodeFactoryRegistry;
    int createdNodeCount;
    bool hapticRendererCreated;
    QSlider* hapticPollIntervalSlider;
    QCheckBox* enableHapticsCheckBox;
    IVisNetworkCanvas* visNetworkCanvas;
    IVisNetwork* visNetwork;
    IScriptEngineManager* scriptEngineManager;
    CShapeRenderModeSelector* shapeRenderModeSelector;

    // Haptics device stuff
    bool deviceInitialized;
    QString deviceName;
    HHD deviceHandle;
    HHLRC deviceContext;
    QString deviceInfoString;
    int hapticPollInterval;

    bool validityFlag;
    bool validityFlagChecked;
    bool isValid(bool checkAgain=false);
    int findConnectionPath(IVisSystemNodeDesc* desc, const QString& pathName);
    IVisSystemNode* createCursor();
    void deleteCursor(IVisSystemNode* node);
    QList<IVisSystemNode*> inputNodes(IVisSystemNode* node);
};

GCF_DEFINE_COMPONENT(COpenHapticsVTKComponent)

COpenHapticsVTKComponent & COpenHapticsVTKComponent::instance()
{
	static COpenHapticsVTKComponent* theInstance = GCF_CREATE_COMPONENT(COpenHapticsVTKComponent);
	return *theInstance;
}

COpenHapticsVTKComponent::COpenHapticsVTKComponent()
{
	d = new COpenHapticsVTKComponentData;
    d->deviceInitialized = false;
    d->deviceHandle = HD_INVALID_HANDLE;
    d->deviceContext = 0;
    d->hapticPollInterval = 100;
    d->licenseFile = QString("%1/ohlic.dat").arg(qApp->applicationDirPath());
}

COpenHapticsVTKComponent::~COpenHapticsVTKComponent()
{
	delete d;
}

void COpenHapticsVTKComponent::setDefaultDeviceName(const QString& name)
{
    d->deviceName = name;
}

const QString& COpenHapticsVTKComponent::defaultDeviceName() const
{
    return d->deviceName;
}

void COpenHapticsVTKComponent::setHapticPollInterval(int mSec)
{
    if(d->hapticPollInterval == mSec)
        return;

    d->hapticPollInterval = mSec;

    if(CVtkOpenHapticsRenderer::GetInstancePointer())
        CVtkOpenHapticsRenderer::GetInstancePointer()->setHapticPollInterval(mSec);

    if(d->hapticPollIntervalSlider)
        d->hapticPollIntervalSlider->setValue(d->hapticPollIntervalSlider->maximum() - mSec);
}

int COpenHapticsVTKComponent::hapticPollInterval() const
{
    return d->hapticPollInterval;
}

QIcon COpenHapticsVTKComponent::nodeIcon() const
{
    static QIcon icon(":/OpenHapticsVTK/haptics.png");
    return icon;
}

bool COpenHapticsVTKComponent::isHapticDeviceInitialized() const
{
    return d->deviceInitialized;
}

unsigned int COpenHapticsVTKComponent::hapticsDeviceHandle() const
{
    return d->deviceHandle;
}

void* COpenHapticsVTKComponent::hapticsDeviceContext() const
{
    return d->deviceContext;
}

QString COpenHapticsVTKComponent::hapticsDeviceInfo() const
{
    return d->deviceInfoString;
}

void COpenHapticsVTKComponent::initializeHapticsDevice()
{
    bool valid = d->isValid();
    if(!valid)
        return;

    if(d->deviceInitialized)
        return;

    // Initialize the device.
    if(d->deviceName.isEmpty())
        d->deviceHandle = hdInitDevice(HD_DEFAULT_DEVICE);
    else
        d->deviceHandle = hdInitDevice( qPrintable(d->deviceName) );
    HDErrorInfo errorInfo = hdGetError();
    if(HD_DEVICE_ERROR(errorInfo))
    {
        HDstring errMsg = hdGetErrorString(errorInfo.errorCode);
        qWarning("No PHANToM devices found: %s", errMsg);
        d->deviceHandle = 0;
        d->deviceInitialized = false;
    }
    else
    {
        d->deviceInitialized = true;

        d->deviceContext = hlCreateContext(d->deviceHandle);
        hlMakeCurrent(d->deviceContext);
        errorInfo = hdGetError();
        if(HD_DEVICE_ERROR(errorInfo))
        {
            HDstring errMsg = hdGetErrorString(errorInfo.errorCode);
            qWarning("Could not make the haptic context current: %s", errMsg);
        }
        hlEnable(HL_HAPTIC_CAMERA_VIEW);

        HDint statusLight = 1;
        hdSetIntegerv(HD_USER_STATUS_LIGHT, &statusLight);
        d->deviceInfoString = QString("%1 Version: %3. Serial No: %4. Vendor: %5. Device Driver Version: %6")
                                .arg(hdGetString(HD_DEVICE_MODEL_TYPE))
                                .arg(hdGetString(HD_VERSION))
                                .arg(hdGetString(HD_DEVICE_SERIAL_NUMBER))
                                .arg(hdGetString(HD_DEVICE_VENDOR))
                                .arg(hdGetString(HD_DEVICE_DRIVER_VERSION));

        if( !d->isValid(true) )
        {
            finalizeHapticsDevice();
            return;
        }

        qDebug("Initialized %s", qPrintable(d->deviceInfoString));
    }
}

void COpenHapticsVTKComponent::finalizeHapticsDevice()
{
    if(!d->deviceInitialized)
        return;

    HDint statusLight = 0;
    hdSetIntegerv(HD_USER_STATUS_LIGHT, &statusLight);
    hlMakeCurrent(NULL);
    hlDeleteContext(d->deviceContext);
    hdDisableDevice(d->deviceHandle);
    qDebug("Finalized %s", qPrintable(d->deviceInfoString));
    d->deviceInfoString.clear();
    d->deviceInitialized = false;
}

QObject* COpenHapticsVTKComponent::containerObject()
{
	return this;
}

QStringList COpenHapticsVTKComponent::nodeClassNameList()
{
	return d->nodeDescMap.keys();
}

IVisSystemNodeDesc* COpenHapticsVTKComponent::nodeDesc(QString nodeClassName)
{
	if(d->nodeDescMap.contains(nodeClassName))
		return d->nodeDescMap[nodeClassName];
	return 0;
}

bool COpenHapticsVTKComponent::canCreate(QString nodeClassName)
{
	IVisSystemNodeDesc* desc = nodeDesc(nodeClassName);
	CGenericVisNodeDesc* desc2 = 0;
	desc->queryInterface("CGenericVisNodeDesc", (void**)&desc2);
	if(desc2)
		return desc2->createFunction();
    return 0;
}

IVisSystemNode* COpenHapticsVTKComponent::createNode(QString nodeClassName)
{
    bool validated = d->validityFlagChecked;
    if(!d->isValid())
    {
        if(validated)
        QMessageBox::information(mainWindow(), "Invalid License",
         "Could not validate the license of the OpenHaptics component\n"
         "on your machine. Please write to info@vcreatelogic.com.");
        return 0;
    }

    if(nodeClassName == "vtkOpenHapticsRenderer" && d->hapticRendererCreated)
    {
        QMessageBox::information(mainWindow(), "Implementation Limitation",
                "Currently we support only one haptic renderer per pipeline.\n"
                "We hope to remove this limitation in a future revision of the software.");
        return 0;
    }

	IVisSystemNodeDesc* desc = nodeDesc(nodeClassName);
	CGenericVisNodeDesc* desc2 = 0;
	desc->queryInterface("CGenericVisNodeDesc", (void**)&desc2);
	if(!desc2)
		return false;

    if(!d->createdNodeCount)
        initializeHapticsDevice();

    if(!d->deviceInitialized)
    {
        QMessageBox::information(GCF::AbstractComponent::componentHostWidget(),
            "Device not initialized",
            "Please install and connect a PHANToM Haptic Device in order to\n"
            "use any of the algorithms provided by this component.");
        return 0;
    }

    IVisSystemNode* node = desc2->createNode();
	if(node)
	{
		node->initializeNode();
		CGenericVisNodeBase* nodeBase = qobject_cast<CGenericVisNodeBase*>(node->containerObject());
		if(nodeBase)
			nodeBase->setNodeFactory(this);

        ++d->createdNodeCount;
        if(nodeClassName == "vtkOpenHapticsRenderer")
        {
            d->hapticRendererCreated = true;

            CVtkOpenHapticsRenderer* ren = qobject_cast<CVtkOpenHapticsRenderer*>(nodeBase);
            ren->setHapticPollInterval(d->hapticPollInterval);
        }
    }
    else if(!d->createdNodeCount)
        finalizeHapticsDevice();

    if(d->enableHapticsCheckBox && d->createdNodeCount)
    {
        d->enableHapticsCheckBox->blockSignals(true);
        d->enableHapticsCheckBox->setChecked(true);
        d->enableHapticsCheckBox->blockSignals(false);
    }

	return node;
}

void COpenHapticsVTKComponent::deleteNode(IVisSystemNode* node)
{
    if(node->nodeDesc()->nodeClassName() == "vtkOpenHapticsRenderer")
        d->hapticRendererCreated = false;

    node->finalizeNode();
    --d->createdNodeCount;
    if(d->createdNodeCount <= 0)
        finalizeHapticsDevice();

    if(d->enableHapticsCheckBox && !d->createdNodeCount)
    {
        d->enableHapticsCheckBox->blockSignals(true);
        d->enableHapticsCheckBox->setChecked(false);
        d->enableHapticsCheckBox->blockSignals(false);
    }
}

QString COpenHapticsVTKComponent::productName() const
{
    return "Open Haptics VTK Classes";
}

QString COpenHapticsVTKComponent::organization() const
{
    return "VCreate Logic (P) Ltd";
}

QImage COpenHapticsVTKComponent::programLogo() const
{
    return QImage();
}

QString COpenHapticsVTKComponent::version() const
{
    return "1.0";
}

QString COpenHapticsVTKComponent::shortDescription() const
{
    return "This component provides classes for the VTK system that can be used "
           "in conjunction with the OpenHaptics toolkit and PHANToM Haptic Devices";
}

QString COpenHapticsVTKComponent::homepage() const
{
    return "http://www.vcreatelogic.com/oss/vtkdesigner";
}

QString COpenHapticsVTKComponent::bugAddress() const
{
    return "http://www.vcreatelogic.com/oss/vtkdesigner/feedback/bugs";
}

const QList<GCF::AboutPerson> COpenHapticsVTKComponent::authors() const
{
    static QList<GCF::AboutPerson> retList;
    return retList;
}

const QList<GCF::AboutPerson> COpenHapticsVTKComponent::credits() const
{
    static QList<GCF::AboutPerson> retList;
    return retList;
}

QString COpenHapticsVTKComponent::license() const
{
    return "Commercial, closed source license only.";
}

QString COpenHapticsVTKComponent::copyrightStatement() const
{
    return QString();
}

void COpenHapticsVTKComponent::initializeComponent()
{
    REGISTER_NODE(CVtkOpenHapticsCursorActor, d->nodeDescMap);
    REGISTER_NODE(CVtkOpenHapticsRenderer, d->nodeDescMap);
    REGISTER_NODE(CVtkOpenHapticsActor, d->nodeDescMap);
    REGISTER_NODE(CVtkOpenHapticsProperty, d->nodeDescMap);
    REGISTER_NODE(CVtkOpenHapticsEffect, d->nodeDescMap);
}

void COpenHapticsVTKComponent::finalizeComponent()
{
    this->finalizeHapticsDevice();
    if(d->nodeFactoryRegistry)
        d->nodeFactoryRegistry->unregisterNodeFactory(qobject_cast<IVisSystemNodeFactory*>(this));
}

void COpenHapticsVTKComponent::creationAnnounced()
{
	QString completeName;
	GCF::AbstractComponent* compPtr = 0;
	QObject* objectPtr = findObject("IVisSystemNodeFactoryRegistry", completeName, &compPtr);
	if(objectPtr)
	{
		IVisSystemNodeFactoryRegistry* nodeFacReg = qobject_cast<IVisSystemNodeFactoryRegistry*>(objectPtr);
		nodeFacReg->registerNodeFactory(qobject_cast<IVisSystemNodeFactory*>(this));
        d->nodeFactoryRegistry = nodeFacReg;
        d->nodeFactoryRegistryComp = compPtr;
		GCF::AbstractComponent::addDependency(this, compPtr);
	}


	objectPtr = findObject("IVisNetwork", completeName, &compPtr);
	if(objectPtr)
	{
        d->visNetwork = qobject_cast<IVisNetwork*>(objectPtr);
		// This should be done to ensure that this component is finalized
		// after the network is destroyed.
		GCF::AbstractComponent::addDependency(this, compPtr);
	}


	objectPtr = findObject("IVisNetworkCanvas", completeName, &compPtr);
	if(objectPtr)
	{
        d->visNetworkCanvas = qobject_cast<IVisNetworkCanvas*>(objectPtr);
		// This should be done to ensure that this component is finalized
		// after the network is destroyed.
		GCF::AbstractComponent::addDependency(this, compPtr);
	}

	objectPtr = findObject("IScriptEngineManager", completeName, &compPtr);
	if(objectPtr)
	{
        d->scriptEngineManager = qobject_cast<IScriptEngineManager*>(objectPtr);
		// This should be done to ensure that this component is finalized
		// after the network is destroyed.
		GCF::AbstractComponent::addDependency(this, compPtr);
	}
}

void COpenHapticsVTKComponent::componentDestroyed(GCF::AbstractComponent* comp)
{
    if(comp == d->nodeFactoryRegistryComp)
    {
        d->nodeFactoryRegistry = 0;
    }
}

QAction* COpenHapticsVTKComponent::fetchAction(const QString& completeName) const
{
	QStringList comps = completeName.split('.');
	COpenHapticsVTKComponent* that = const_cast<COpenHapticsVTKComponent*>(this);

    if(comps.last() == "hapticPollIntervalW")
    {
        if(!d->hapticPollIntervalSlider)
            fetchWidget("hapticPollIntervalSlider");

        QWidgetAction* action = new QWidgetAction(that);
        action->setDefaultWidget(d->hapticPollIntervalSlider);
        // action->setEnabled( d->isValid() );
        return action;
    }

    if(comps.last() == "enableHapticsW")
    {
        if(!d->enableHapticsCheckBox)
           fetchWidget("enableHapticsCheckBox");

        QWidgetAction* action = new QWidgetAction(that);
        action->setDefaultWidget(d->enableHapticsCheckBox);
        // action->setEnabled( d->isValid() );
        return action;
    }

    return GCF::AbstractComponent::fetchAction(completeName);
}

QObject* COpenHapticsVTKComponent::fetchObject(const QString& completeName) const
{
	QStringList comps = completeName.split('.');
    COpenHapticsVTKComponent* that = const_cast<COpenHapticsVTKComponent*>(this);

    if(comps.last() == "controller")
        return that;

	return 0;
}

QWidget* COpenHapticsVTKComponent::fetchWidget(const QString& completeName) const
{
	QStringList comps = completeName.split('.');

    if(comps.last() == "hapticPollIntervalSlider")
    {
        if(d->hapticPollIntervalSlider)
            return d->hapticPollIntervalSlider;

        d->hapticPollIntervalSlider = new QSlider(Qt::Horizontal);
        d->hapticPollIntervalSlider->setTickPosition(QSlider::TicksBothSides);
        d->hapticPollIntervalSlider->setTickInterval(10);
        d->hapticPollIntervalSlider->setRange(0, 100);
        d->hapticPollIntervalSlider->setValue(100-d->hapticPollInterval);
        d->hapticPollIntervalSlider->setMinimumWidth(250);
        connect(d->hapticPollIntervalSlider, SIGNAL(sliderReleased()),
                this, SLOT(on_hapticPollIntervalSlider_sliderReleased()));
        // d->hapticPollIntervalSlider->setEnabled( d->isValid() );
        return d->hapticPollIntervalSlider;
    }

    if(comps.last() == "enableHapticsCheckBox")
    {
        if(d->enableHapticsCheckBox)
            return d->enableHapticsCheckBox;

        d->enableHapticsCheckBox = new QCheckBox;
        d->enableHapticsCheckBox->setText("Enable Haptics");
        connect(d->enableHapticsCheckBox, SIGNAL(clicked(bool)), this, SLOT(slotEnableHaptics(bool)));
        // d->enableHapticsCheckBox->setEnabled( d->isValid() );
        return d->enableHapticsCheckBox;
    }

    if(comps.last() == "shapeRenderModeSelector")
    {
        if(!d->shapeRenderModeSelector)
            d->shapeRenderModeSelector = new CShapeRenderModeSelector;

        return d->shapeRenderModeSelector;
    }

	return 0;
}

void COpenHapticsVTKComponent::on_hapticPollIntervalSlider_sliderReleased()
{
    int max = d->hapticPollIntervalSlider->maximum();
    int val = d->hapticPollIntervalSlider->value();
    val = max - val;

    // We should not allow val=0 to be set from within the slider.
    if(val <= 0)
        val = 1;
    setHapticPollInterval(val);
}

class BusyCursor
{
public:
    BusyCursor() {
        QApplication::setOverrideCursor( Qt::BusyCursor );
    }

    ~BusyCursor() {
        QApplication::restoreOverrideCursor();
    }
};

void COpenHapticsVTKComponent::slotEnableHaptics(bool val)
{
    bool validated = d->validityFlagChecked;
    if(!d->isValid())
    {
        d->enableHapticsCheckBox->blockSignals(true);

        if( validated )
        QMessageBox::information(mainWindow(), "Invalid License",
         "Could not validate the license of the OpenHaptics component\n"
         "on your machine. Please write to info@vcreatelogic.com.");

        d->enableHapticsCheckBox->setChecked(false);
        d->enableHapticsCheckBox->blockSignals(false);

        return;
    }

    BusyCursor busyCursor;
    if(!d->visNetwork || !d->nodeFactoryRegistry)
        return;

    if(d->visNetwork->nodeCount() == 0)
    {
        qDebug("No nodes in the network to enable haptics");
        d->enableHapticsCheckBox->setChecked(false);
        return;
    }

    static QStringList ohList = QStringList() << "vtkOpenHapticsActor" << "vtkOpenHapticsRenderer"
            << "vtkOpenHapticsProperty" << "vtkOpenHapticsCursorActor";
    static QStringList vtkList = QStringList() << "vtkActor" << "vtkRenderer" << "NoClass" << "NoClass";

    const QStringList & classList = val ? vtkList : ohList;
    const QStringList & otherClassList = val ? ohList : vtkList;
    QMap< IVisSystemNode*, QList<IVisNetworkConnection*> > nodeConnectionMap;

    // first figure out the nodes that we must replace.
    QList<IVisNetworkConnection*> conList;
    for(int i=0; i<d->visNetwork->nodeCount(); i++)
    {
        IVisSystemNode* node = d->visNetwork->node(i);
        IVisSystemNodeDesc* nodeDesc = node->nodeDesc();
        if( classList.contains( nodeDesc->nodeClassName() ) )
            nodeConnectionMap[node] = conList;
    }

    if(nodeConnectionMap.count() == 0)
    {
        qDebug("No nodes in the network to enable haptics");
        d->enableHapticsCheckBox->setChecked(false);
        return;
    }

    if(!d->deviceInitialized)
        this->initializeHapticsDevice();

    if(!d->deviceInitialized)
    {
        qDebug("Device not connected");
        return;
    }

    // Now identify connections that need to be replaced.
    QList<IVisSystemNode*> nodeList = nodeConnectionMap.keys();
    for(int i=0; i<d->visNetwork->connectionCount(); i++)
    {
        IVisNetworkConnection* con = d->visNetwork->connection(i);
        if( nodeList.contains(con->senderNode()) )
            nodeConnectionMap[con->senderNode()].append(con);
        if(nodeList.contains(con->receiverNode()) )
            nodeConnectionMap[con->receiverNode()].append(con);
    }

    // Now replace names from classList with classes from otherClassList.
    QMap< IVisSystemNode*, QList<IVisNetworkConnection*> >::iterator it = nodeConnectionMap.begin();
    QMap< IVisSystemNode*, QList<IVisNetworkConnection*> >::iterator end = nodeConnectionMap.end();
    QMap< IVisSystemNode*, IVisSystemNode* > oldNewNodeMap;
    while(it != end)
    {
        IVisSystemNode* node = it.key();
        QString className = node->nodeDesc()->nodeClassName();
        QString otherClassName = otherClassList[ classList.indexOf(className) ];
        IVisSystemNode* otherNode = d->nodeFactoryRegistry->createNode(otherClassName);
        if(!otherNode)
        {
            ++it;
            continue;
        }
        d->visNetwork->addNode(otherNode);
        oldNewNodeMap[node] = otherNode;
        ++it;
    }

    it = nodeConnectionMap.begin();
    while(it != end)
    {
        IVisSystemNode* node = it.key();
        if(!oldNewNodeMap.contains(node))
        {
            ++it;
            continue;
        }

        QList<IVisNetworkConnection*> cons = it.value();
        for(int i=0; i<cons.count(); i++)
        {
            IVisNetworkConnection* con = cons[i];
            IVisSystemNode* sender = con->senderNode();
            IVisSystemNode* receiver = con->receiverNode();
            int senderIdx = con->senderPathIndex();
            int receiverIdx = con->receiverPathIndex();

            if(oldNewNodeMap.contains(sender))
            {
                QString pathName = sender->nodeDesc()->connectionPath(senderIdx)->pathName();
                sender = oldNewNodeMap[sender];
                senderIdx = d->findConnectionPath(sender->nodeDesc(), pathName);
            }
            if(oldNewNodeMap.contains(receiver))
            {
                QString pathName = receiver->nodeDesc()->connectionPath(receiverIdx)->pathName();
                receiver = oldNewNodeMap[receiver];
                receiverIdx = d->findConnectionPath(receiver->nodeDesc(), pathName);
            }
            if(senderIdx < 0 || receiverIdx < 0)
                continue;

            bool success = d->visNetwork->connectNode(sender, senderIdx, receiver, receiverIdx, &con);
            if(success)
                qWarning("Success");
            else
                qWarning("Failure");
        }

        ++it;
    }

    for(int i=0; i<nodeList.count(); i++)
    {
        IVisSystemNode* node = nodeList[i];
        if(!val && node->nodeDesc()->nodeClassName() == "vtkOpenHapticsCursorActor")
            d->deleteCursor(node);
        d->visNetwork->removeNode(node);
    }

    if(val)
    {
        IVisSystemNode* cursorActor = d->createCursor();
        if(CVtkOpenHapticsRenderer::GetInstancePointer())
            d->visNetwork->connectNode(cursorActor, CVtkOpenHapticsRenderer::GetInstancePointer());
    }

    if(val && CVtkOpenHapticsRenderer::GetInstancePointer())
    {
        CVtkOpenHapticsRenderer::GetInstancePointer()->setNodeName("vtkOpenHapticsRenderer");
        CVtkOpenHapticsRenderer::GetInstancePointer()->resetCamera();
        CVtkOpenHapticsRenderer::GetInstancePointer()->render();
    }

    if(d->visNetworkCanvas)
        d->visNetworkCanvas->layoutNodes();

    // Update the script editor with some script.
    if(!d->scriptEngineManager)
        return;

    // First get hold of the current script.
    QString script = d->scriptEngineManager->script();

    if(val)
    {
        static QString hrScript =
            "// Begin Haptic Rendering Script\n"
            "var currentActor;\n"
            "var isCurrentActor = false;\n"
            "\n"
            "// Event handler for OnHapticButton1Down event of vtkOpenHapticsRenderer\n"
            "function vtkOpenHapticsRenderer_OnHapticButton1Down()\n"
            "{\n"
            "    isCurrentActor = vtkOpenHapticsRenderer.hasCurrentHapticActor();\n"
            "    if(isCurrentActor == false)\n"
            "        return;\n"
            "\n"
            "    currentActor = vtkOpenHapticsRenderer.currentHapticActor();\n"
            "    oldOpacity = currentActor.Opacity;\n"
            "\n"
            "    currentActor.EnableHapticRendering = false;\n"
            "    currentActor.MoveWithStylus = true;\n"
            "    currentActor.RotateWithStylus = true;\n"
            "    currentActor.UseHapticTransform = true;\n"
            "}\n"
            "vtkOpenHapticsRenderer.OnHapticButton1Down.connect(vtkOpenHapticsRenderer_OnHapticButton1Down);\n"
            "\n"
            "// Event handler for OnHapticButton1Up event of vtkOpenHapticsRenderer\n"
            "function vtkOpenHapticsRenderer_OnHapticButton1Up()\n"
            "{\n"
            "    if(isCurrentActor == false)\n"
            "        return;\n"
            "\n"
            "    currentActor.EnableHapticRendering = true;\n"
            "    currentActor.MoveWithStylus = false;\n"
            "    currentActor.RotateWithStylus = false;\n"
            "    isCurrentActor = false;\n"
            "}\n"
            "vtkOpenHapticsRenderer.OnHapticButton1Up.connect(vtkOpenHapticsRenderer_OnHapticButton1Up);\n"
            "\n"
            "// End Haptic Rendering Script\n";

        script += hrScript;
        d->scriptEngineManager->setScript(script);
        d->scriptEngineManager->executeScript();
    }
    else
    {
        // Remove script added by this function (if any)
        int startIndex = script.indexOf("// Begin Haptic Rendering Script");
        int endIndex = script.indexOf("// End Haptic Rendering Script");
        endIndex += 31; // length of the end haptic thingy
        script = script.remove(startIndex, (endIndex-startIndex));
        d->scriptEngineManager->setScript(script);
        d->scriptEngineManager->executeScript();
    }
}

bool COpenHapticsVTKComponentData::isValid(bool checkAgain)
{
#ifndef OPEN_HAPTICS_VTK_LICENSE_OEM
    if( checkAgain )
    {
        this->validityFlag = false;
        this->validityFlagChecked = false;
    }

    if( this->validityFlagChecked )
        return this->validityFlag;

    if( this->validityFlag )
        return true;

    this->validityFlagChecked = true;

#ifdef VERIFY_DEVICE_SERIAL_NO
    // Verify device serial number
    const char* ExpectedSlNo = "30707300003";
    if( this->deviceInitialized )
    {
        HDstring slNo = hdGetString(HD_DEVICE_SERIAL_NUMBER);
        this->validityFlag = (qstrcmp(slNo, ExpectedSlNo) == 0);
        if( !this->validityFlag )
        {
            QMessageBox::information(GCF::AbstractComponent::componentHostWidget(),
                "Invalid device", "Please connect a device against which "
                "this component\nhas been licensed to use");
            return false;
        }
    }
    else
    {
        this->validityFlag = true;
        return true;
    }
#endif

#ifdef VERIFY_HOST_ID
    // Verify host-id
    const QString ExpectedHostId = "6871-00E3";
    QProcess dirProc;
    dirProc.setWorkingDirectory("C:\\");
    dirProc.start("dir C:\\", QProcess::ReadOnly);
    dirProc.waitForStarted();
    dirProc.waitForFinished();
    QString output = dirProc.readAllStandardOutput();
    QString hostId = output; //output.section("Volume Serial Number is", 1, 1).section('\n', 0, 0);
    hostId = hostId.simplified();

    qDebug("Error = %s", qPrintable(dirProc.errorString()));
    qDebug("Expected %s, Got %s", qPrintable(ExpectedHostId), qPrintable(output));

    this->validityFlag &= (hostId == ExpectedHostId);

    if( !this->validityFlag )
    {
        QMessageBox::information(GCF::AbstractComponent::componentHostWidget(),
            "Invalid device", "This component can be used only on the "
            "machine\nthat it has been licensed to use");
        return false;
    }
#endif

    return this->validityFlag;
#endif

    return true;
}

int COpenHapticsVTKComponentData::findConnectionPath(IVisSystemNodeDesc* desc, const QString& pathName)
{
    if(!desc)
        return -1;

    if(pathName.isEmpty())
        return -1;

    for(int i=0; i<desc->connectionPathCount(); i++)
    {
        IVisSystemNodeConnectionPath* path = desc->connectionPath(i);
        if(path->pathName() == pathName)
            return i;
    }

    return -1;
}

IVisSystemNode* COpenHapticsVTKComponentData::createCursor()
{
    IVisSystemNode* sphereNode = nodeFactoryRegistry->createNode("vtkSphereSource");
    if(!sphereNode)
        return 0;
    visNetwork->addNode(sphereNode);

    IVisSystemNode* mapperNode = nodeFactoryRegistry->createNode("vtkDataSetMapper");
    if(!mapperNode)
        return 0;
    visNetwork->addNode(mapperNode);

    IVisSystemNode* actorNode = nodeFactoryRegistry->createNode("vtkOpenHapticsCursorActor");
    if(!actorNode)
        return 0;
    visNetwork->addNode(actorNode);

    double radius = 0.1;

    vtkOpenHapticsRenderer* ren = vtkOpenHapticsRenderer::LastCreatedInstance();
    if(ren)
    {
        double bounds[6];
        ren->ComputeVisiblePropBounds(bounds);
        radius = bounds[1] - bounds[0];
        if(radius > bounds[3] - bounds[2])
            radius = bounds[3] - bounds[2];
        if(radius > bounds[5] - bounds[4])
            radius = bounds[5] - bounds[4];
        radius /= 10;
    }

    sphereNode->containerObject()->setProperty("Radius", radius);
    sphereNode->containerObject()->setProperty("ThetaResolution", 32);
    sphereNode->containerObject()->setProperty("PhiResolution", 32);
    visNetwork->connectNode(sphereNode, mapperNode);
    visNetwork->connectNode(mapperNode, actorNode);

    return actorNode;
}

void COpenHapticsVTKComponentData::deleteCursor(IVisSystemNode* node)
{
    // Trace all input nodes and delete them.
    QList<IVisSystemNode*> toRemove = inputNodes(node);
    for(int i=0; i<toRemove.count(); i++)
        visNetwork->removeNode( toRemove[i] );
}

QList<IVisSystemNode*> COpenHapticsVTKComponentData::inputNodes(IVisSystemNode* node)
{
    QList<IVisSystemNode*> ret;
    for(int i=0; i<visNetwork->connectionCount(); i++)
    {
        IVisNetworkConnection* con = visNetwork->connection(i);
        if(con->receiverNode() == node)
        {
            ret.append( con->senderNode() );
            ret += inputNodes(con->senderNode());
        }
    }

    return ret;
}

GCF_EXPORT_COMPONENT_PLUGIN(COpenHapticsVTKComponent)


