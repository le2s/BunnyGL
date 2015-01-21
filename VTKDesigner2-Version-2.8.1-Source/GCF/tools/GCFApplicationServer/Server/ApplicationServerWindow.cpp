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

#include "ApplicationServerWindow.h"

#include "RequestHandlerPool.h"
#include "ServerLoadInfoDialog.h"
#include "ProcessPoolSizeConfigurationDialog.h"
#include "ui_ApplicationServerWindow.h"
#include "ApplicationServerComponent.h"

#include <QCloseEvent>
#include <QMetaObject>
#include <QMetaMethod>
#include <QHeaderView>
#include <QTreeWidgetItem>
#include <QTextStream>

#include <GCF/AppAccessPoint>
#include <GCF/ComponentFactory>
#include <GCF/ComponentGui>
#include <GCF/AbstractComponent>
#include <GCF/AppAccessPointComponent>
#include <GCF/ComponentLoaderComponent>

struct ApplicationServerWindowData
{
    Ui::ApplicationServerWindow ui;
    const RequestHandlerPool* requestHandlerPool;
    QString windowTitlePrefix;

    QString genObjectInfoHTML(QObject* object) const;
    QString genMethodInfoHTML(QObject* object, QMetaMethod method) const;
    QString genObjectCallInfoHTML(QObject* object) const;
};

ApplicationServerWindow& ApplicationServerWindow::instance()
{
    static ApplicationServerWindow* theInstance = new ApplicationServerWindow(0);
    return *theInstance;
}

ApplicationServerWindow::ApplicationServerWindow(QWidget* parent)
: QMainWindow(parent)
{
    d = new ApplicationServerWindowData;
    d->ui.setupUi(this);
    d->requestHandlerPool = 0;
    d->windowTitlePrefix = "GCF Application Server";

    connect(d->ui.lstServerObjects, SIGNAL(itemActivated(QTreeWidgetItem*,int)),
            this, SLOT(slotItemActivated(QTreeWidgetItem*)));
    connect(d->ui.lstServerObjects, SIGNAL(itemClicked(QTreeWidgetItem*,int)),
            this, SLOT(slotItemActivated(QTreeWidgetItem*)));
    connect(d->ui.lstServerObjects, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),
            this, SLOT(slotItemActivated(QTreeWidgetItem*)));
}

ApplicationServerWindow::~ApplicationServerWindow()
{
    delete d;
}

void ApplicationServerWindow::setWindowTitlePrefix(const QString& title)
{
    if(title == d->windowTitlePrefix)
        return;

    d->windowTitlePrefix = title;
    if(d->windowTitlePrefix.isEmpty())
        d->windowTitlePrefix = "GCF Application Server";

    QString windowTitle = QString("%2 [PID : %1]").arg( qApp->applicationPid() )
                                            .arg( d->windowTitlePrefix );
    this->setWindowTitle(windowTitle);
}

QString ApplicationServerWindow::windowTitlePrefix() const
{
    return d->windowTitlePrefix;
}

void ApplicationServerWindow::setRequestHandlerPool(const RequestHandlerPool* requestHanderPool)
{
    if(!requestHanderPool || d->requestHandlerPool == requestHanderPool)
        return;

    d->requestHandlerPool = requestHanderPool;
}

const RequestHandlerPool* ApplicationServerWindow::requestHandlerPool() const
{
    return d->requestHandlerPool;
}

QTabWidget* ApplicationServerWindow::tabWidget() const
{
    return d->ui.tabWidget;
}

void ApplicationServerWindow::showEvent(QShowEvent* se)
{
    QString windowTitle = QString("%2 [PID : %1]").arg( qApp->applicationPid() )
                                            .arg( d->windowTitlePrefix );
    this->setWindowTitle(windowTitle);

    if(!d->ui.lstServerObjects->topLevelItemCount())
        this->loadObjectTree();

    Q_UNUSED(se);
}

void ApplicationServerWindow::closeEvent(QCloseEvent* ce)
{
    ce->setAccepted(false);
    this->hide();
}

void ApplicationServerWindow::loadObjectTree()
{
    QTreeWidget* treeWidget = d->ui.lstServerObjects;
    treeWidget->clear();

    treeWidget->setColumnCount(1);
    treeWidget->header()->hide();

    QList<GCF::AbstractComponent*> comps = GCF::ComponentFactory::instancePointer()->components();
    Q_FOREACH(GCF::AbstractComponent* comp, comps)
    {
        if(!GCF::AppAccessPoint::instance().isAllowObjectAccess(comp, comp, NULL))
            continue;

        GCF::ComponentGui compGui = comp->componentGui();

        QTreeWidgetItem* compItem = new QTreeWidgetItem(treeWidget);
        compItem->setText(0, comp->componentName());
        compItem->setData(0, Qt::UserRole, qVariantFromValue<GCF::AbstractComponent*>(comp));
        compItem->setData(0, Qt::UserRole+1, qVariantFromValue<GCF::ComponentGui>(compGui));

        for(int i=0; i<compGui.nodeCount(); i++)
        {
            GCF::ComponentGuiNode node = compGui.node(i);
            if(node.type() == GCF::ComponentGuiNode::Object || node.type() == GCF::ComponentGuiNode::Widget)
            {
                QObject* object = 0;
                if(node.type() == GCF::ComponentGuiNode::Object)
                    object = node["QObject"].value<QObject*>();
                else
                    object = node["QWidget"].value<QWidget*>();

                if(!GCF::AppAccessPoint::instance().isAllowObjectAccess(comp, object, NULL))
                    continue;

                QTreeWidgetItem* nodeItem = new QTreeWidgetItem(compItem);
                nodeItem->setText(0, node.completeName());
                nodeItem->setData(0, Qt::UserRole+1, qVariantFromValue<GCF::ComponentGuiNode>(node));
                nodeItem->setData(0, Qt::UserRole, qVariantFromValue<QObject*>(object));
            }
        }

        if(compGui.nodeCount())
            compItem->setExpanded(true);

        if(compItem->text(0) == "ApplicationServer")
        {
            compItem->setSelected(true);
            slotItemActivated(compItem);
        }
    }
}

void ApplicationServerWindow::showComponentInfo(GCF::AbstractComponent* comp, QTreeWidgetItem* item)
{
    d->ui.wvObjectInfo->setContent("");
    if(!comp || !item)
        return;

    QString compInfoHTML;
    QTextStream ts(&compInfoHTML, QIODevice::WriteOnly);

    ts << "<H1>" << comp->componentName() << "</H1>\n";
    ts << "<p>" << comp->componentName() << " has the following objects</p>\n";
    ts << "<ul>\n";
    for(int i=0; i<item->childCount(); i++)
    {
        QTreeWidgetItem* childItem = item->child(i);
        ts << "<li><a href=\"#" << childItem->text(0) << "Info\">" << childItem->text(0) << "</a></li>\n";
    }
    ts << "</ul>\n";

    for(int i=0; i<item->childCount(); i++)
    {
        QTreeWidgetItem* childItem = item->child(i);
        QObject* childObject = childItem->data(0, Qt::UserRole).value<QObject*>();
        ts << "<a name=\"" << childItem->text(0) << "Info\">\n";
        ts << "<h2>" << childItem->text(0) << " - Object Information</h2>\n";
        ts << "</a>\n";
        ts << d->genObjectInfoHTML(childObject) << "\n";
    }

    ts.flush();
    d->ui.wvObjectInfo->setHtml(compInfoHTML);

    d->ui.wvCallInfo->setContent("Select an object to view call statistics");
}

void ApplicationServerWindow::showObjectInfo(QObject* object, QTreeWidgetItem* item)
{
    d->ui.wvObjectInfo->setContent("");
    if(!object || !item)
        return;

    // Prepare a string that dumps information about an object.
    QString objectInfoHTML;
    QTextStream ts(&objectInfoHTML, QIODevice::WriteOnly);

    ts << "<h1>" << item->text(0) << "</h1>\n";
    ts << d->genObjectInfoHTML(object);

    ts.flush();
    d->ui.wvObjectInfo->setHtml(objectInfoHTML);

    // Prepare a string that dumps information about the number
    // of calls made to various methods on the object.
    QString objectCallInfoHTML;
    QTextStream ts2(&objectCallInfoHTML, QIODevice::WriteOnly);
    ts2 << "<h1>" << item->text(0) << " - Calls</h1><br>\n";
    ts2 << d->genObjectCallInfoHTML(object);
    ts2.flush();
    d->ui.wvCallInfo->setHtml(objectCallInfoHTML);
}

void ApplicationServerWindow::slotItemActivated(QTreeWidgetItem* item)
{
    GCF::HourGlass hourGlass;

    if(!item->parent())
        showComponentInfo(item->data(0, Qt::UserRole).value<GCF::AbstractComponent*>(), item);
    else
        showObjectInfo(item->data(0, Qt::UserRole).value<QObject*>(), item);
}

void ApplicationServerWindow::on_quitAction_triggered()
{
    this->hide();
    QMetaObject::invokeMethod( &ApplicationServerComponent::instance(),
        "shutdown", Qt::QueuedConnection );
}

void ApplicationServerWindow::on_showServerInfoAction_triggered()
{
    ServerLoadInfoDialog* serverLoadInfoDialog = new ServerLoadInfoDialog(d->requestHandlerPool, this);

    serverLoadInfoDialog->exec();

    delete serverLoadInfoDialog;
}

void ApplicationServerWindow::on_configureProcessPoolSizeAction_triggered()
{
    RequestHandlerPool* requestHandlerPool = const_cast<RequestHandlerPool*>(d->requestHandlerPool);
    ProcessPoolSizeConfigurationDialog* processPoolSizeConfigurationDialog = new ProcessPoolSizeConfigurationDialog(requestHandlerPool, this);
    processPoolSizeConfigurationDialog->exec();
    delete processPoolSizeConfigurationDialog;
}

QString ApplicationServerWindowData::genObjectInfoHTML(QObject* object) const
{
    QString retStr;
    QTextStream ts(&retStr, QIODevice::WriteOnly);

    int startMethodIdx = QObject::staticMetaObject.methodCount();
    if(qobject_cast<GCF::AbstractComponent*>(object))
        startMethodIdx = GCF::AbstractComponent::staticMetaObject.methodCount();

    const QMetaObject* mo = object->metaObject();
    QList<QMetaMethod> methodList;

    ts << "<ul>\n";
    for(int i=startMethodIdx; i<mo->methodCount(); i++)
    {
        QMetaMethod method = mo->method(i);

        if(method.access() != QMetaMethod::Public)
            continue;

        if(method.methodType() == QMetaMethod::Signal ||
           method.methodType() == QMetaMethod::Constructor )
            continue;

        if( !qstrcmp(method.tag(), "GCF_NOT_WS") )
            continue;

        methodList.append(method);

        ts << "<li><a href=\"#" << object->objectName() << "_method_" << method.methodIndex()
           << "\">" << QString(method.signature()).section('(', 0, 0) << "</a></li>\n";
    }
    ts << "</ul>\n";

    ts << "<p>Function documentation</p><p>&nbsp;</p>\n";

    ts << "<table border=\"0\" cellspacing=\"0\" cellpadding=\"3\">\n";

    for(int i=0; i<methodList.count(); i++)
    {
        QMetaMethod method = methodList.at(i);
        ts << "<a name=\"" << object->objectName() << "_method_" << method.methodIndex() << "\"></a>\n";
        ts << this->genMethodInfoHTML(object, method) << "\n\n";
        ts << "<tr><td colspan=\"2\">&nbsp;</td></tr>\n";
    }

    ts << "</table>\n";

    return retStr;
}

QString ApplicationServerWindowData::genMethodInfoHTML(QObject* object, QMetaMethod method) const
{
    QString methodTable;
    QTextStream ts(&methodTable, QIODevice::WriteOnly);

    QString methodType;
    switch(method.methodType())
    {
    case QMetaMethod::Signal: methodType = "Signal"; break;
    case QMetaMethod::Slot: methodType = "Slot"; break;
    case QMetaMethod::Method: methodType = "Method"; break;
    case QMetaMethod::Constructor: methodType = "Constructor"; break;
    };

    // First row is method name.
    QString methodName = QString(method.signature()).section('(', 0, 0);
    ts << "<tr><td colspan=\"2\" style=\"background-color: #ababab;\"><font size=\"+1\"><b>"
        << methodName
        << "&nbsp;&nbsp;&nbsp;[" << methodType << "] [Tag: "
        << (qstrlen(method.tag()) ? method.tag() : "None") << "]</b></font></td></tr>\n";

    // Second row is parameter types
    ts << "<tr><td width=\"40%\">Parameters</td><td>";
    if(method.parameterNames().count())
    {
        for(int i=0; i<method.parameterTypes().count(); i++)
        {
            if(i)
                ts << "<br/>\n";
            ts << method.parameterTypes().at(i) << " "
                << method.parameterNames().at(i);
        }
    }
    else
        ts << "<i>None</i>";
    ts << "</td></tr>\n";

    // Thid row is return type.
    QString retVal;
    if(method.typeName())
        retVal = QString(method.typeName());
    else
        retVal = "<i>void</i>";

    ts << "<tr><td width=\"40%\">Return Type</td><td>"
       << retVal << "</td></tr>\n";

    // Fourth row is usage
    ts << "<tr><td width=\"40%\">AJAX Usage</td><td><div style=\"overflow: auto;\"><pre>";
    ts << "var msg = new GCFRequestMessage();\n";
    ts << "msg.setObjectName(\"" << object->objectName() << "\");\n";
    ts << "msg.setMethodName(\"" << methodName << "\");\n";
    if(method.parameterTypes().count())
    {
        for(int i=0; i<method.parameterTypes().count(); i++)
        {
            QByteArray paramType = method.parameterTypes().at(i);
            QString paramName = method.parameterNames().at(i);
            if(paramType == "int")
                ts << "msg.addIntegerArg( /*" << paramName << " arg*/ );\n";
            else if(paramType == "float" || paramType == "double")
                ts << "msg.addDoubleArg( /*" << paramName << " arg*/ );\n";
            else if(paramType == "bool")
                ts << "msg.addBooleanArg( /*" << paramName << " arg*/ );\n";
            else if(paramType == "QString")
                ts << "msg.addStringArg( /*" << paramName << " arg*/ );\n";
            else if(paramType == "QColor")
                ts << "msg.addColorArg( /*" << paramName << " arg*/ );\n";
            else if(paramType == "QUrl")
                ts << "msg.addUrlArg( /*" << paramName << " arg*/ );\n";
            else if(paramType == "QDate")
                ts << "msg.addDateArg( /*" << paramName << " arg*/ );\n";
            else if(paramType == "QTime")
                ts << "msg.addTimeArg( /*" << paramName << " arg*/ );\n";
            else if(paramType == "QDateTime")
                ts << "msg.addDateTimeArg( /*" << paramName << " arg*/ );\n";
            else
                ts << "msg.addStringArg( /*" << paramName << " arg (unsupported arg type)*/ );\n";
        }
    };

    if(method.typeName())
    {
        ts << "msg.onReply = function(reply) {\n";
        ts << "    if(!reply.isValid() || !reply.hasResult()) {\n"
           << "        // show error message\n"
           << "        return;\n"
           << "    }\n\n";
        if(retVal == "QPixmap" || retVal == "QImage")
        {
            ts << "    // Returned value is a image\n";
            ts << "    document.getElementById('divId').innerHTML = '';\n";
            ts << "    var img = reply.getResult();\n";
            ts << "    document.getElementById('divId').appendChild( img );\n";
        }
        else if(retVal == "QVariantMap")
        {
            ts << "    // Returned value is a GCFDictionary object\n";
            ts << "    document.getElementById('abc').innerHTML = reply.getResult().valueOf('abc').getValue();\n";
            ts << "    // ....\n";
        }
        else if(retVal == "QVariantList")
        {
            ts << "    // Returned value is a GCFList object\n";
            ts << "    document.getElementById('abc').innerHTML = reply.getResult().at(0).getValue();\n";
            ts << "    // ....\n";
        }
        else
        {
            ts << "    // Returned value is a " << retVal << "\n";
            ts << "    document.getElementById('abc').innerHTML = reply.getResult();\n";
        }

        ts << "}\n";
    }

    ts << "msg.send();\n";
    ts << "</pre></div></td></tr>\n";

    ts.flush();
    return methodTable;
}

QString ApplicationServerWindowData::genObjectCallInfoHTML(QObject* object) const
{
    GCF::DataStore callCounterStore;
    if( object->dynamicPropertyNames().contains("_CallCounterStore_") )
        callCounterStore = object->property("_CallCounterStore_").value<GCF::DataStore>();

    QString ret;
    QTextStream ts(&ret, QIODevice::WriteOnly);

    QStringList keys = callCounterStore.keys();
    if(!keys.count())
        ts << "<p>No calls have been made to this object yet</p>";
    else
    {
        ts << "<table border=\"0\" cellspacing=\"0\" cellpadding=\"3\">\n"
           << "<tr><td style=\"background-color: #ababab;\">Method Name</td>"
           << "<td style=\"background-color: #ababab;\">Call Count</td></tr>\n";
        Q_FOREACH(QString key, keys)
        {
            ts << "<tr><td>" << key
               << "</td><td><code>" << callCounterStore[key].toInt()
               << "</code></td></tr>\n";
        }
        ts << "</table>";
    }

    ts.flush();

    return ret;
}
