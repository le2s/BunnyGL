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

#include "GradientGenerator.h"

#include <QBrush>
#include <QPainter>
#include <QLinearGradient>

GradientGenerator::GradientGenerator(QObject* parent)
: QObject(parent)
{

}

GradientGenerator::~GradientGenerator()
{

}

QPixmap GradientGenerator::generateVerticalGradient(int length, int size,
                                const QColor& color1, const QColor& color2)
{
    QPixmap pm(size, length);
    pm.fill(Qt::white);
    
    QPainter paint(&pm);
    
    QLinearGradient grad(0, 0, 0, length);
    grad.setColorAt(0, color1);
    grad.setColorAt(1, color2);
    
    paint.fillRect(0, 0, size, length, QBrush(grad));
    
    return pm;
}
                                             
QPixmap GradientGenerator::generateHorizontalGradient(int length, int size,
                                const QColor& color1, const QColor& color2)
{
    QPixmap pm(length, size);
    pm.fill(Qt::white);
    
    QPainter paint(&pm);
    
    QLinearGradient grad(0, 0, length, 0);
    grad.setColorAt(0, color1);
    grad.setColorAt(1, color2);
    
    paint.fillRect(0, 0, length, size, QBrush(grad));
    
    return pm;
}


