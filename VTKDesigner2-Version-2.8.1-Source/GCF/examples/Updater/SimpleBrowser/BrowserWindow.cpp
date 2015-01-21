#include "BrowserWindow.h"
#include "ui_BrowserWindow.h"

#include <QUrl>
#include <QMessageBox>
#include <QWheelEvent>

#include <GCF/AbstractComponent>

struct BrowserWindowData
{
    Ui::BrowserWindow ui;
};

BrowserWindow& BrowserWindow::instance()
{
    static BrowserWindow* theInstance = new BrowserWindow;
    return *theInstance;
}

BrowserWindow::BrowserWindow()
{
    d = new BrowserWindowData;
    d->ui.setupUi(this);

    d->ui.browser->installEventFilter(this);
}

BrowserWindow::~BrowserWindow()
{
    delete d;
}

void BrowserWindow::on_cmdGo_clicked()
{
    QUrl url( d->ui.txtUrl->text() );
    if( !url.isValid() )
    {
        QMessageBox::information(this, "Invalid URL",
            QString("Url %1 is invalid").arg( d->ui.txtUrl->text() ) );
        return;
    }

    d->ui.browser->load(url);
}

void BrowserWindow::on_browser_titleChanged(const QString& title)
{
    QString title2;

    if(title.isEmpty())
        title2 = "Simple Browser";
    else
        title2 = QString("%1 - Simple Browser").arg(title);

    setWindowTitle(title2);
}

QObject* BrowserWindow::containerObject()
{
    return this;
}

QWebView* BrowserWindow::browserControl() const
{
    return d->ui.browser;
}

QLineEdit* BrowserWindow::urlEdit() const
{
    return d->ui.txtUrl;
}

QStatusBar* BrowserWindow::statusBar() const
{
    return statusBar();
}

QPushButton* BrowserWindow::goButton() const
{
    return d->ui.cmdGo;
}

bool BrowserWindow::eventFilter(QObject* obj, QEvent* event)
{
    if(obj == d->ui.browser && event->type() == QEvent::Wheel)
    {
        QWheelEvent* we = (QWheelEvent*)event;
        if( !(we->modifiers()&Qt::ControlModifier) )
            return false;

        qreal tsm = d->ui.browser->textSizeMultiplier();
        if(we->delta() > 0)
            tsm *= 1.1;
        else
            tsm *= 0.9;

        if(tsm < 0.3)
            tsm = 0.3;

        d->ui.browser->setTextSizeMultiplier(tsm);
    }

    return false;
}

void BrowserWindow::closeEvent(QCloseEvent* ce)
{
    GCF::finalize();
    Q_UNUSED(ce);
}


