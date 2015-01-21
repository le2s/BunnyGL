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

#ifndef I_VIS_NETWORK_CANVAS_WIDGET_EXT_H
#define I_VIS_NETWORK_CANVAS_WIDGET_EXT_H

#include <GCF/IContainer>

class IVisNetworkCanvasWidgetExt : virtual public IContainer
{
public:
    virtual void addWidget(QWidget* widget, const QRectF& rect, const QPointF& pos) = 0;
    virtual void removeWidget(QWidget* widget) = 0;
    virtual int widgetCount() const = 0;
    virtual QWidget* widget(int index) const = 0;

    // Must be implemented as signals
    virtual void widgetAdded(QWidget* widget) = 0;
    virtual void widgetRemoved(QWidget* widget) = 0;
};
Q_DECLARE_INTERFACE(IVisNetworkCanvasWidgetExt, "com.vcreatelogic.IVisNetworkCanvasWidgetExt/1.0");

#endif

