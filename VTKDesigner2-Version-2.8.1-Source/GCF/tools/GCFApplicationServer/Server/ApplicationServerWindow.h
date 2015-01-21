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

#ifndef APPLICATION_SERVER_WINDOW_H
#define APPLICATION_SERVER_WINDOW_H

#include <QMainWindow>

namespace GCF { class AbstractComponent; }
class QTreeWidgetItem;

class RequestHandlerPool;
class QTabWidget;

struct ApplicationServerWindowData;
class ApplicationServerWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    static ApplicationServerWindow& instance();

    ~ApplicationServerWindow();

    void setWindowTitlePrefix(const QString& title);
    QString windowTitlePrefix() const;

    void setRequestHandlerPool(const RequestHandlerPool* requestHanderPool);
    const RequestHandlerPool* requestHandlerPool() const;

    QTabWidget* tabWidget() const;

protected:
    void showEvent(QShowEvent* se);
    void closeEvent(QCloseEvent* ce);
    
protected:
    ApplicationServerWindow(QWidget* parent=0);
    void loadObjectTree();
    
    void showComponentInfo(GCF::AbstractComponent* comp, QTreeWidgetItem* item);
    void showObjectInfo(QObject* object, QTreeWidgetItem* item);

private slots:
    void slotItemActivated(QTreeWidgetItem* item);
    void on_quitAction_triggered();
    void on_showServerInfoAction_triggered();
    void on_configureProcessPoolSizeAction_triggered();
    
private:
    ApplicationServerWindowData* d;
};

#endif

