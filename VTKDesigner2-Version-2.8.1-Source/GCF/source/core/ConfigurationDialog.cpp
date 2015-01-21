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

#include <GCF/ConfigurationDialog>
#include <GCF/ComponentFactory>
#include <GCF/AbstractComponent>
#include <GCF/ComponentGui>
// #include "ui_ConfigurationDialog.h"
#include "ui_ComponentPropertyEditorPage.h"
#include "ui_AddPropertyDialog.h"

#include <QDomDocument>
#include <QDomElement>
#include <QStyle>
#include <QApplication>
#include <QTimeLine>
#include <QPlastiqueStyle>

#include <QMap>
#include <QMenu>
#include <QFile>
#include <QPainter>
#include <QComboBox>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QPushButton>
#include <QMetaProperty>
#include <QTreeWidgetItem>
#include <QAbstractItemDelegate>
#include <QStyleOptionViewItem>

#include <QStyle>
#include <QPixmap>
#include <QPainter>
#include <QChildEvent>
#include <QVBoxLayout>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QApplication>
#include <QResizeEvent>
#include <QStackedWidget>
#include <QLinearGradient>

///////////////////////////////////////////////////////////////////////////////
// ConfigDialog
///////////////////////////////////////////////////////////////////////////////

struct PageInfo
{
    PageInfo() : widget(0), height(0) { }

    PageInfo(QWidget* _widget,
             const QString& _title=QString(),
             const QIcon& _icon=QIcon())
    {
        this->widget = _widget;

        if(_title.isEmpty())
            this->title = this->widget->windowTitle();
        else
            this->title = _title;

        if(_icon.isNull())
        {
            this->icon = this->widget->windowIcon();
            if(this->icon.isNull())
                this->icon = qApp->style()->standardIcon(QStyle::SP_ComputerIcon);
        }
        else
            this->icon = _icon;

        this->height = _widget->sizeHint().height();
    }

    QWidget* widget;
    QString title;
    QIcon icon;
    int height;

    bool operator == (const PageInfo& other) const {
        return this->widget == other.widget;
    }
};

namespace GCF
{
    struct ConfigDialogData
    {
        ConfigDialogData() : currentPageIndex(-1) { }

        QVBoxLayout* layout;
        GCF::ConfigDialogToolbar* toolBar;

        QList<PageInfo> pageList;
        int currentPageIndex;

        int startHeight, endHeight;
    };
}

GCF::ConfigDialog::ConfigDialog(QWidget* parent)
        :QDialog(parent)
{
    d = new ConfigDialogData;

    d->layout = new QVBoxLayout(this);
    d->layout->setMargin(0);

    d->toolBar = new ConfigDialogToolbar(this);
    d->layout->addWidget(d->toolBar);
    d->toolBar->hide();

    connect(d->toolBar, SIGNAL(currentToolButtonChanged(int)),
            this, SLOT(setCurrentPageFromIndex(int)));
    connect(d->toolBar, SIGNAL(toolButtonAdded(int,QString)),
            this, SIGNAL(pageAdded(int)));
    connect(d->toolBar, SIGNAL(toolButtonRemoved(int,QString)),
            this, SIGNAL(pageRemoved(int)));

    this->setAutoFillBackground(true);
    /*
#ifdef Q_WS_WIN
    this->setBackgroundRole(QPalette::Dark);
#else
    this->setBackgroundRole(QPalette::Midlight);
#endif
    */
}

GCF::ConfigDialog::~ConfigDialog()
{
    delete d;
}

void GCF::ConfigDialog::addPage(QWidget* page, const QString& title, const QIcon& icon)
{
    if(!page)
        return;

    int pageIndex = this->indexOfPage(page);
    if(pageIndex >= 0)
        return;

    PageInfo info(page,title,icon);
    page->setParent(this);
    page->hide();

    d->pageList.append( info );
    d->toolBar->addToolButton(info.title, info.icon);
    d->toolBar->setVisible( d->pageList.count() > 1 );
}

void GCF::ConfigDialog::removePage(QWidget* page)
{
    if(!page)
        return;

    int pageIndex = this->indexOfPage(page);
    if(pageIndex < 0)
        return;

    PageInfo info = d->pageList.at(pageIndex);
    if(pageIndex == d->currentPageIndex)
    {
        d->layout->removeWidget(info.widget);
        info.widget->hide();
    }

    d->pageList.removeAt(pageIndex);
    d->toolBar->removeToolButton(info.title);
    d->toolBar->setVisible( d->pageList.count() > 1 );
}

int GCF::ConfigDialog::pageCount() const
{
    return d->pageList.count();
}

QWidget* GCF::ConfigDialog::pageAt(int index) const
{
    if(index < 0 || index >= d->pageList.count())
        return 0;

    return d->pageList.at(index).widget;
}

int GCF::ConfigDialog::indexOfPage(QWidget* widget) const
{
    for(int i=0; i<d->pageList.count(); i++)
        if(d->pageList.at(i).widget == widget)
            return i;

    return -1;
}

QWidget* GCF::ConfigDialog::currentPage() const
{
    return this->pageAt(d->currentPageIndex);
}

int GCF::ConfigDialog::currentPageIndex() const
{
    return d->currentPageIndex;
}

void GCF::ConfigDialog::setCurrentPage(QWidget* page)
{
    int pageIndex = this->indexOfPage(page);
    this->setCurrentPageFromIndex(pageIndex);
}

void GCF::ConfigDialog::setCurrentPageFromIndex(int index)
{
    if(index < 0 || index >= d->pageList.count())
        return;

    PageInfo& info = d->pageList[index];
    if(info.height <= 0)
        info.height = info.widget->sizeHint().height();

    QWidget* oldPage = this->currentPage();
    if(oldPage)
    {
        d->layout->removeWidget(oldPage);
        oldPage->hide();
    }

    d->currentPageIndex = index;
    d->toolBar->setCurrentToolButton(index);

    if(d->pageList.count() > 1)
    {
        d->toolBar->setReadOnly(true);

        d->startHeight = this->height();
        d->endHeight = d->toolBar->height() + d->layout->spacing() + info.height;

        QWidget* dummyWidget = new QWidget(this);
        d->layout->addWidget(dummyWidget);

        if(!qobject_cast<QPlastiqueStyle*>(style()))
        {
            info.widget->show();
            info.widget->setGeometry(0, d->toolBar->height()+d->layout->spacing(),
                                     width(), d->endHeight-(d->toolBar->height()+d->layout->spacing()));
        }

        QTimeLine timeLine(300);
        connect(&timeLine, SIGNAL(valueChanged(qreal)), this, SLOT(resizeTimeLineChanged(qreal)));
        timeLine.start();
        while(timeLine.state() == QTimeLine::Running)
            qApp->processEvents();

        d->toolBar->setReadOnly(false);

        delete dummyWidget;
    }

    info.widget->show();
    d->layout->addWidget(info.widget);

    resize( width(), d->endHeight );
}

void GCF::ConfigDialog::childEvent(QChildEvent* ce)
{
    if(ce->type() == QEvent::ChildRemoved)
    {
        QObject* child = ce->child();
        if(child->isWidgetType())
        {
            QWidget* widget = (QWidget*)child;
            int pageIndex = this->indexOfPage(widget);
            if(pageIndex >= 0)
                this->removePage(widget);
        }
    }

    QDialog::childEvent(ce);
}

void GCF::ConfigDialog::resizeTimeLineChanged(qreal value)
{
    if( !qobject_cast<QTimeLine*>(sender()) )
        return;

    int height = d->startHeight + (d->endHeight - d->startHeight)*value;
    resize( width(), height );
}

///////////////////////////////////////////////////////////////////////////////
// ConfigDialogToolbar
///////////////////////////////////////////////////////////////////////////////

struct ToolButtonInfo
{
    ToolButtonInfo(const QString& title, const QIcon& icon) {
        this->title = title;
        this->icon = icon;

        if(this->icon.isNull())
            this->icon = qApp->style()->standardIcon(QStyle::SP_ComputerIcon);
    }

    QString title;
    QIcon icon;

    bool operator == (const ToolButtonInfo& other) const {
        return this->title == other.title;
    }
};

namespace GCF
{
    struct ConfigDialogToolbarData
    {
        ConfigDialogToolbarData() :
            toolButtonWidth(ConfigDialogToolbarData::minToolButtonWidth),
            currentToolButton(-1), readOnly(false) { }

        static int minToolButtonWidth;
        static int fixedToolBarHeight;

        QList<ToolButtonInfo> toolButtonList;
        int toolButtonWidth;
        int currentToolButton;
        bool readOnly;
    };
}

int GCF::ConfigDialogToolbarData::minToolButtonWidth = 80;
int GCF::ConfigDialogToolbarData::fixedToolBarHeight = 65;

GCF::ConfigDialogToolbar::ConfigDialogToolbar(QWidget* parent)
        : QWidget(parent)
{
    d = new ConfigDialogToolbarData;

    setFixedHeight(ConfigDialogToolbarData::fixedToolBarHeight);
}

GCF::ConfigDialogToolbar::~ConfigDialogToolbar()
{
    delete d;
}

void GCF::ConfigDialogToolbar::addToolButton(const QString& title, const QIcon& icon)
{
    int index = this->indexOfToolButton(title);
    if(index >= 0)
        return;

    d->toolButtonList.append( ToolButtonInfo(title,icon) );
    this->updateToolButtonWidth();

    emit toolButtonAdded(d->toolButtonList.count()-1, title);

    if(d->currentToolButton < 0)
        setCurrentToolButton(d->toolButtonList.count()-1);
}

void GCF::ConfigDialogToolbar::removeToolButton(const QString& title)
{
    int index = this->indexOfToolButton(title);
    if(index < 0)
        return;

    d->toolButtonList.removeAt(index);
    this->updateToolButtonWidth();

    emit toolButtonRemoved(index, title);

    if(d->currentToolButton == index)
    {
        if(d->currentToolButton >= d->toolButtonList.count())
            setCurrentToolButton(d->toolButtonList.count()-1);
    }
}

int GCF::ConfigDialogToolbar::toolButtonCount() const
{
    return d->toolButtonList.count();
}

QString GCF::ConfigDialogToolbar::toolButtonText(int index) const
{
    if(index < 0 || index >= d->toolButtonList.count())
        return QString();

    return d->toolButtonList.at(index).title;
}

QIcon GCF::ConfigDialogToolbar::toolButtonIcon(int index) const
{
    if(index < 0 || index >= d->toolButtonList.count())
        return QIcon();

    return d->toolButtonList.at(index).icon;
}

int GCF::ConfigDialogToolbar::indexOfToolButton(const QString& title)
{
    for(int i=0; i<d->toolButtonList.count(); i++)
        if(d->toolButtonList.at(i).title == title)
            return i;

    return -1;
}

bool GCF::ConfigDialogToolbar::isReadOnly() const
{
    return d->readOnly;
}

int GCF::ConfigDialogToolbar::currentToolButton() const
{
    return d->currentToolButton;
}

QSize GCF::ConfigDialogToolbar::sizeHint() const
{
    QSize size(d->toolButtonWidth * d->toolButtonList.count(), this->minimumHeight());
    if(size.width() == 0)
        size.setWidth(d->toolButtonWidth);

    return size;
}

void GCF::ConfigDialogToolbar::setReadOnly(bool val)
{
    d->readOnly = val;
}

void GCF::ConfigDialogToolbar::setCurrentToolButton(int index)
{
    if(d->readOnly)
        return;

    if(index == d->currentToolButton)
        return;

    if(index < 0 || index >= d->toolButtonList.count())
        d->currentToolButton = d->toolButtonList.count()-1;
    else
        d->currentToolButton = index;

    update();

    emit currentToolButtonChanged(d->currentToolButton);
}

void GCF::ConfigDialogToolbar::updateToolButtonWidth()
{
    int width = 0;

    Q_FOREACH(ToolButtonInfo tbInfo, d->toolButtonList)
    {
        int textWidth = this->fontMetrics().width(tbInfo.title) + 4;
        if(textWidth > width)
            width = textWidth;
    }

    if(width < ConfigDialogToolbarData::minToolButtonWidth)
        d->toolButtonWidth = ConfigDialogToolbarData::minToolButtonWidth;
    else
        d->toolButtonWidth = width;

    this->update();
}

void GCF::ConfigDialogToolbar::resizeEvent(QResizeEvent* re)
{
    QWidget::resizeEvent(re);
}

void GCF::ConfigDialogToolbar::paintEvent(QPaintEvent* pe)
{
    Q_UNUSED(pe);

    QPalette pal = this->palette();
    QColor bottomColor = pal.button().color().darker(120);

    QColor color1 = pal.light().color();
    QColor color2 = pal.button().color();

    color1.setAlphaF(0.5);
    color2.setAlphaF(0.5);

    QLinearGradient backgroundGradient( this->rect().topLeft(), this->rect().bottomLeft() );
    backgroundGradient.setColorAt(0, color1);
    backgroundGradient.setColorAt(1, color2);

    QPainter paint(this);
    paint.fillRect(this->rect(), backgroundGradient);

    QLinearGradient borderGradient( this->rect().topLeft(), this->rect().bottomLeft() );
    borderGradient.setColorAt(0, pal.button().color());
    borderGradient.setColorAt(0.5, pal.shadow().color());
    borderGradient.setColorAt(1, bottomColor);

    int textHeight = this->fontMetrics().height();
    int iconSize = ConfigDialogToolbarData::minToolButtonWidth - textHeight - 10;
    if(iconSize > 52)
        iconSize = 52;

    QRect tbRect(0, 0, d->toolButtonWidth, height());
    QRect currentTBRect;

    for(int i=0; i<d->toolButtonList.count(); i++)
    {
        ToolButtonInfo tbInfo = d->toolButtonList.at(i);

        if(i == d->currentToolButton)
        {
            paint.fillRect(tbRect, pal.button());
            currentTBRect = tbRect;
        }

        QRect iconRect = tbRect.adjusted(0, 0, 0, -textHeight-2);
        QRect textRect = tbRect;
        textRect.setTop( iconRect.bottom() + 1 );

        QPixmap tbPixmap = tbInfo.icon.pixmap(iconSize,iconSize);

        QPoint iconRectCenter = iconRect.center();
        iconRect.setLeft(iconRectCenter.x()-tbPixmap.width()/2);
        iconRect.setRight(iconRect.left()+tbPixmap.width());
        iconRect.setTop(iconRectCenter.y()-tbPixmap.height()/2);
        iconRect.setBottom(iconRect.top()+tbPixmap.height());

        paint.drawPixmap(iconRect, tbPixmap);
        paint.drawText(textRect, Qt::AlignCenter, tbInfo.title);

        if(i == d->currentToolButton)
        {
            QRectF leftBorder = tbRect;
            leftBorder.setRight(leftBorder.left()+0.5);
            paint.fillRect(leftBorder, borderGradient);

            QRectF rightBorder = tbRect;
            rightBorder.setLeft(rightBorder.right()-1);
            rightBorder.setRight(rightBorder.right()-0.5);
            paint.fillRect(rightBorder, borderGradient);
        }

        tbRect.translate(d->toolButtonWidth, 0);
    }


    if(currentTBRect.isEmpty())
        return;

    paint.setPen(bottomColor);
    paint.drawLine(0, this->rect().bottom(), currentTBRect.left(), this->rect().bottom());
    paint.drawLine(currentTBRect.right(), this->rect().bottom(), width(), this->rect().bottom());
}

void GCF::ConfigDialogToolbar::mousePressEvent(QMouseEvent* me)
{
    int tbIndex = int(me->pos().x() / d->toolButtonWidth);
    if(tbIndex < 0 || tbIndex >= d->toolButtonList.count())
        return;

    setCurrentToolButton(tbIndex);
}

void GCF::ConfigDialogToolbar::mouseReleaseEvent(QMouseEvent* me)
{
    Q_UNUSED(me);
}

///////////////////////////////////////////////////////////////////////////////
// ConfigurationDialog
///////////////////////////////////////////////////////////////////////////////

struct GCF::ConfigurationDialogData
{
    // Ui::ConfigurationDialog ui;
    GCF::ComponentPropertyEditorPage* propEditorPage;
};

GCF::ConfigurationDialog::ConfigurationDialog(QWidget* parent)
:GCF::ConfigDialog(parent)
{
    d = new GCF::ConfigurationDialogData;
    // d->firstWidget = 0;
    // d->ui.setupUi(this);

    // Properties page.
    if(qApp->dynamicPropertyNames().contains("HideComponentPropertyEditorPage")
            && qApp->property("HideComponentPropertyEditorPage").toBool() )
        d->propEditorPage = 0;
    else
    {
        d->propEditorPage = new GCF::ComponentPropertyEditorPage;
        addPage(d->propEditorPage);
    }
    // connect(okButton(), SIGNAL(clicked()), d->propEditorPage, SLOT(saveProperties()));

    resize(640, 480);
}

GCF::ConfigurationDialog::~ConfigurationDialog()
{
    delete d;
}

QWidget* GCF::ConfigurationDialog::pageContainer() const
{
    return 0;
}

QPushButton* GCF::ConfigurationDialog::okButton() const
{
    return 0;
}

QPushButton* GCF::ConfigurationDialog::cancelButton() const
{
    return 0;
}

void GCF::ConfigurationDialog::showEvent(QShowEvent* se)
{
    if(d->propEditorPage)
        QMetaObject::invokeMethod(d->propEditorPage, "loadProperties", Qt::QueuedConnection);
    QDialog::showEvent(se);
}

void GCF::ConfigurationDialog::closeEvent(QCloseEvent* se)
{
    if(d->propEditorPage)
        d->propEditorPage->saveProperties();
    QDialog::closeEvent(se);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// GCF::ComponentPropertyEditorPage
////////////////////////////////////////////////////////////////////////////////////////////////////

struct GCF::ComponentPropertyEditorPageData
{
    Ui::ComponentPropertyEditorPage ui;

    QMap< QString, QMap<QString, QString> > propertyMap;
    QString currentObject;
    GCF::ComponentGui currentGui;
    QMap<QString, QDomDocument> guiXmlMap;

    void loadComponentTree(GCF::AbstractComponent* comp);
    void saveProperties();
    void saveProperties(GCF::ComponentGuiNode node, const QMap<QString, QString> & propMap);
    void showProperties(const QString& completeName);
    QComboBox* createComboBox(const QString& completeName, QWidget* parent);
    QObject* getObject(const QString& completeName);
};

GCF::ComponentPropertyEditorPage::ComponentPropertyEditorPage(QWidget* parent)
:QWidget(parent)
{
    d = new GCF::ComponentPropertyEditorPageData;
    d->ui.setupUi(this);
    setWindowIcon(QIcon(":/GCF/configure.png"));

    d->ui.componentTree->setHeaderLabel("Component/Object");
    QStringList colNames = (QStringList() << "Property" << "Value");
    d->ui.propertyEditor->setHeaderLabels(colNames);
}

GCF::ComponentPropertyEditorPage::~ComponentPropertyEditorPage()
{
    delete d;
}

void GCF::ComponentPropertyEditorPage::loadProperties()
{
    if(d->ui.componentTree->topLevelItemCount())
        return;

    // First Load the components and their objects/widgets
    QList<GCF::AbstractComponent*> comps = GCF::ComponentFactory::instance().components();
    for(int i=0; i<comps.count(); i++)
    {
        GCF::AbstractComponent* comp = comps[i];
        d->loadComponentTree(comp);
    }
}

void GCF::ComponentPropertyEditorPage::saveProperties()
{
    // First Load the components and their objects/widgets
    d->saveProperties();

    // QMessageBox::information(this, "Notification",
    //    "The changes will be applied when you restart the application");
}

void GCF::ComponentPropertyEditorPage::on_componentTree_itemClicked(QTreeWidgetItem* item)
{
    d->ui.propertyEditor->clear();
    d->currentObject.clear();
    d->currentGui = GCF::ComponentGui();

    if(!item->parent() || !item)
    {
        d->ui.cmdAddProperty->setEnabled(false);
        d->ui.cmdRemoveProperty->setEnabled(false);
        return;
    }

    d->currentObject = item->data(0, Qt::UserRole).toString();
    d->currentGui = item->parent()->data(0, Qt::UserRole).value<GCF::ComponentGui>();
    d->showProperties(d->currentObject);
    d->ui.cmdAddProperty->setEnabled(true);
    d->ui.cmdRemoveProperty->setEnabled(true);
}

void GCF::ComponentPropertyEditorPage::on_cmdAddProperty_clicked()
{
    if(d->currentObject.isEmpty())
        return;

    QDialog newPropDialog(this);
    Ui::AddPropertyDialog ui;
    ui.setupUi(&newPropDialog);
    QComboBox* comboBox = d->createComboBox(d->currentObject, ui.inputWidgetContainer);

    QHBoxLayout* layout = new QHBoxLayout(ui.inputWidgetContainer);
    layout->setMargin(0);
    layout->addWidget(comboBox);

    if(newPropDialog.exec() == QDialog::Rejected)
        return;

    QTreeWidgetItem* item = new QTreeWidgetItem(d->ui.propertyEditor);
    item->setText(0, comboBox->currentText());
    item->setSizeHint(0, QSize(20, 20));
    item->setSizeHint(1, QSize(20, 20));
    item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEditable|Qt::ItemIsEnabled);
    d->propertyMap[d->currentObject][item->text(0)] = "";
}

void GCF::ComponentPropertyEditorPage::on_cmdRemoveProperty_clicked()
{
    QTreeWidgetItem* item = d->ui.propertyEditor->currentItem();
    if(!item || item->text(0) == "title")
        return;

    d->propertyMap[d->currentObject].remove(item->text(0));
    delete item;
}

void GCF::ComponentPropertyEditorPage::on_propertyEditor_itemChanged(QTreeWidgetItem* item, int)
{
    if(d->currentObject.isEmpty() || !item)
        return;

    d->propertyMap[d->currentObject][item->text(0)] = item->text(1);
}

void GCF::ComponentPropertyEditorPageData::loadComponentTree(GCF::AbstractComponent* comp)
{
    QTreeWidgetItem* compItem = new QTreeWidgetItem(ui.componentTree);
    GCF::ComponentGui gui = comp->componentGui();

    compItem->setText(0, comp->componentName());
    compItem->setData(0, Qt::UserRole, qVariantFromValue<GCF::ComponentGui>(gui));
    compItem->setIcon(0, qApp->style()->standardIcon(QStyle::SP_DirIcon));

    // Store the title property
    for(int i=0; i<gui.nodeCount(); i++)
    {
        GCF::ComponentGuiNode node = gui.node(i);
        if(node.type() == GCF::ComponentGuiNode::Object || node.type() == GCF::ComponentGuiNode::Widget)
        {
            QTreeWidgetItem* item = new QTreeWidgetItem(compItem);
            item->setText(0, node.name());
            item->setData(0, Qt::UserRole, node.completeName());
            item->setIcon(0, qApp->style()->standardIcon(QStyle::SP_ComputerIcon));

            propertyMap[node.completeName()]["title"] = node["title"].toString();
        }
        else if(node.type() == GCF::ComponentGuiNode::Property)
        {
            QString completePropName = node["key"].toString();
            QString objName = completePropName.section("::", 0, 0);
            QString propName = completePropName.section("::", 1, 1);
            QString propValue = node["value"].toString();
            propertyMap[objName][propName] = propValue;
        }
    }

    compItem->setExpanded(true);
}

void GCF::ComponentPropertyEditorPageData::saveProperties()
{
    GCF::AbstractComponent* firstComp = GCF::ComponentFactory::instance().components().first();
    QMap< QString, QMap<QString, QString> >::iterator it = propertyMap.begin();
    QMap< QString, QMap<QString, QString> >::iterator end = propertyMap.end();

    guiXmlMap.clear();
    while(it != end)
    {
        QString objectName = it.key();
        QMap<QString, QString> props = it.value();
        GCF::ComponentGuiNode node = firstComp->componentGui().findNode(objectName, true);
        saveProperties(node, props);
        ++it;
    }

    QMap<QString, QDomDocument>::iterator git = guiXmlMap.begin();
    QMap<QString, QDomDocument>::iterator gend = guiXmlMap.end();
    while(git != gend)
    {
        QFile file(git.key());
        file.open(QFile::WriteOnly);
        QString fc = git.value().toString(2);
        file.write(fc.toAscii());
        ++git;
    }
    guiXmlMap.clear();
}

void GCF::ComponentPropertyEditorPageData::saveProperties(GCF::ComponentGuiNode node, const QMap<QString, QString> & pm)
{
    if(!node.isValid())
        return;

    QMap<QString, QString> propMap = pm;
    QString guiFileName = QString("%1/%2").arg(GCF::AbstractComponent::guiXmlFileDirectory()).arg(node.component()->guiXmlFileName());
    QDomDocument doc;

    if(guiXmlMap.contains(guiFileName))
        doc = guiXmlMap[guiFileName];
    else
    {
        QFile guiFile(guiFileName);
        guiFile.open(QFile::ReadOnly);
        QString guiFileContents(guiFile.readAll());
        doc.setContent(guiFileContents);
        guiXmlMap[guiFileName] = doc;
    }

    // Update title and other properties that were already declared
    QDomNodeList nodes = doc.documentElement().childNodes();
    QList<QDomNode> nodesToRemove;
    for(int i=0; i<nodes.count(); i++)
    {
        QDomElement nodeE = nodes.item(i).toElement();
        if(nodeE.isNull())
            continue;

        if(nodeE.tagName()=="object" || nodeE.tagName()=="widget")
        {
            if(nodeE.attribute("name") == node.name())
            {
                nodeE.setAttribute("title", propMap["title"]);
                propMap.remove("title");
            }
        }
        else if(nodeE.tagName()=="property")
        {
            QString key = nodeE.attribute("key");
            if(key.section("::", 0, 0) != node.completeName())
                continue;

            QString key2 = key.section("::", 1, 1);
            if(propMap.contains(key2))
            {
                nodeE.setAttribute("value", propMap[key2]);
                propMap.remove(key2);
            }
            else
            {
                nodesToRemove.push_back(nodeE);
            }
        }
    }

    // Remove unwanted nodes.
    for(int i=0; i<nodesToRemove.count(); i++)
        doc.documentElement().removeChild(nodesToRemove[i]);

    // Append new properties.
    QMap<QString, QString>::iterator it = propMap.begin();
    QMap<QString, QString>::iterator end = propMap.end();
    while(it != end)
    {
        QDomElement propE = doc.createElement("property");
        propE.setAttribute("key", QString("%1::%2").arg(node.completeName()).arg(it.key()));
        propE.setAttribute("value", it.value());
        doc.documentElement().appendChild(propE);
        ++it;
    }
}

void GCF::ComponentPropertyEditorPageData::showProperties(const QString& completeName)
{
    ui.propertyEditor->clear();

    QMap<QString, QString> map = propertyMap[completeName];
    QMap<QString, QString>::iterator it = map.begin();
    QMap<QString, QString>::iterator end = map.end();
    while(it != end)
    {
        QTreeWidgetItem* item = new QTreeWidgetItem(ui.propertyEditor);
        item->setText(0, it.key());
        item->setText(1, it.value());
        item->setSizeHint(0, QSize(20, 20));
        item->setSizeHint(1, QSize(20, 20));
        item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEditable|Qt::ItemIsEnabled);
        ++it;
    }
}

QObject* GCF::ComponentPropertyEditorPageData::getObject(const QString& completeName)
{
    QObject* ret = 0;
    if(completeName.isEmpty())
        return ret;

    GCF::ComponentGuiNode node;
    if(!currentGui.component())
    {
        GCF::AbstractComponent* firstComp = GCF::ComponentFactory::instance().components().first();
        if(!firstComp)
            return ret;
        GCF::ComponentGui gui = firstComp->componentGui();
        node = gui.findNode(completeName, true);
    }
    else
        node = currentGui.findNode(completeName, true);
    if(!node.isValid())
        return ret;

    switch(node.type())
    {
    case GCF::ComponentGuiNode::Object:
        ret = node["QObject"].value<QObject*>();
        break;
    case GCF::ComponentGuiNode::Widget:
        ret = node["QWidget"].value<QWidget*>();
        break;
    default:
        break;
    }

    return ret;
}

QComboBox* GCF::ComponentPropertyEditorPageData::createComboBox(const QString& completeName, QWidget* parent)
{
    QComboBox* combo = new QComboBox(parent);
    QObject* object = getObject(completeName);
    combo->addItem("title");

    if(!object)
        return combo;

    QStringList propNames;
    const QMetaObject* mo = object->metaObject();
    for(int i=0; i<mo->propertyCount(); i++)
    {
        QMetaProperty prop = mo->property(i);
        propNames.push_back(prop.name());
    }

    qSort(propNames);
    combo->addItems(propNames);

    return combo;
}


