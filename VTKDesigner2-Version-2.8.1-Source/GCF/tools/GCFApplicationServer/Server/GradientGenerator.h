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

#ifndef GRADIENT_GENERATOR_H
#define GRADIENT_GENERATOR_H

#include <QObject>
#include <QPixmap>

class GradientGenerator : public QObject
{
    Q_OBJECT
    
public:
    GradientGenerator(QObject* parent=0);
    ~GradientGenerator();
    
    Q_INVOKABLE QPixmap generateVerticalGradient(int length, int size,
                                                 const QColor& color1, 
                                                 const QColor& color2);

    Q_INVOKABLE QPixmap generateHorizontalGradient(int length, int size,
                                                   const QColor& color1, 
                                                   const QColor& color2);
};

#endif

