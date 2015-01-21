#ifndef BROWSER_WINDOW_H
#define BROWSER_WINDOW_H

#include <QMainWindow>
#include "IBrowserWindow.h"

struct BrowserWindowData;
class BrowserWindow : public QMainWindow, public IBrowserWindow
{
    Q_OBJECT
    Q_INTERFACES(IBrowserWindow)

public:
    static BrowserWindow& instance();
    ~BrowserWindow();

protected slots:
    void on_cmdGo_clicked();
    void on_browser_titleChanged(const QString& title);

public:
    // IContainer implementation
    QObject* containerObject();

    // IBrowserWindow implementation
    QWebView* browserControl() const;
    QLineEdit* urlEdit() const;
    QStatusBar* statusBar() const;
    QPushButton* goButton() const;

protected:
    BrowserWindow();
    bool eventFilter(QObject* obj, QEvent* event);
    void closeEvent(QCloseEvent* ce);

private:
    BrowserWindowData* d;
};

#endif
