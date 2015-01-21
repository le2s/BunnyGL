#ifndef I_BROWSER_WINDOW_H
#define I_BROWSER_WINDOW_H

#include <GCF/IContainer>

class QWebView;
class QLineEdit;
class QStatusBar;
class QPushButton;

class IBrowserWindow : public IContainer
{
public:
    virtual QWebView* browserControl() const = 0;
    virtual QLineEdit* urlEdit() const = 0;
    virtual QStatusBar* statusBar() const = 0;
    virtual QPushButton* goButton() const = 0;
};

Q_DECLARE_INTERFACE(IBrowserWindow, "com.vcreatelogic.IBrowserWindow/1.0");

#endif
