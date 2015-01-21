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

#include <GCF/Menu>
#include <QLabel>
#include <QVBoxLayout>
#include <QEvent>
#include <QStyle>

GCF::Menu::Menu(QWidget* parent) : QMenu(parent)
{

}

GCF::Menu::~Menu()
{

}

namespace GCF
{
    class MenuWidgetTitleLabel : public QLabel
    {
    public:
        MenuWidgetTitleLabel(QWidget* parent=0) : QLabel(parent) {
            setAlignment(Qt::AlignCenter);
            setMargin(2);
            setFrameStyle(QFrame::Plain|QFrame::Box);

            static QColor color = palette().highlightedText().color();
            static QColor bgColor = palette().highlight().color();
            static QString ss = QString(
                "color: rgb(%1, %2, %3);\n"
                "background-color: rgb(%4, %5, %6);\n"
                ).arg(color.red()).arg(color.green()).arg(color.blue())
                .arg(bgColor.red()).arg(bgColor.green()).arg(bgColor.blue());

            setStyleSheet(ss);
        }
    };

    struct MenuWidgetData
    {
        QLabel* titleLabel;
        QVBoxLayout* layout;
        QWidget* widget;
    };
}

GCF::MenuWidget::MenuWidget(QWidget* child, QWidget* parent)
: QWidget(parent)
{
    d = new MenuWidgetData;
    d->widget = 0;

    d->titleLabel = new GCF::MenuWidgetTitleLabel(this);

    d->layout = new QVBoxLayout(this);
    d->layout->setMargin(0);
    d->layout->setSpacing(3);
    d->layout->addWidget(d->titleLabel);
    if(child)
        setWidget(child);

    // By default the title is not shown
    d->titleLabel->hide();
}

GCF::MenuWidget::~MenuWidget()
{
    delete d;
}

void GCF::MenuWidget::setShowWindowTitleLabel(bool val)
{
    if(d->titleLabel->isVisible() == val)
        return;

    d->titleLabel->setVisible(val);
}

bool GCF::MenuWidget::isShowWindowTitleLabel() const
{
    return d->titleLabel->isVisible();
}

void GCF::MenuWidget::setWidget(QWidget* widget)
{
    if(d->widget == widget)
        return;

    if(d->widget)
        delete d->widget;
    d->widget = 0;
    d->titleLabel->setText( QString() );

    if(widget)
    {
        d->widget = widget;
        d->widget->installEventFilter(this);
        d->layout->addWidget(d->widget);
        d->widget->show();
        d->titleLabel->setText( d->widget->windowTitle() );
    }
}

QWidget* GCF::MenuWidget::widget() const
{
    return d->widget;
}

bool GCF::MenuWidget::eventFilter(QObject* obj, QEvent* event)
{
    if(obj == d->widget)
    {
        switch(event->type())
        {
        case QEvent::WindowTitleChange:
            d->titleLabel->setText( ((QWidget*)obj)->windowTitle() );
            break;

        default:
            break;
        }
    }

    return false;
}

void GCF::MenuWidget::showEvent(QShowEvent* se)
{
    int space = style()->pixelMetric(QStyle::PM_SmallIconSize);
    int hmargin = style()->pixelMetric(QStyle::PM_MenuHMargin); 
    space += 4+2*hmargin;
    d->layout->setContentsMargins(space, 4, 4, 4);

    QWidget::showEvent(se);
}


