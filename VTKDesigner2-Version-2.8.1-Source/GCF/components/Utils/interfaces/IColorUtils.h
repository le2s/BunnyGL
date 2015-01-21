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

#ifndef I_COLOR_UTILS_H
#define I_COLOR_UTILS_H

#include <GCF/IContainer>
#include <QColor>
#include <QPair>

class IColorUtils : virtual public IContainer
{
public:
	virtual QColor randomColor(const QPair<qreal,qreal>& redRange = qMakePair(0.0,1.0),
							   const QPair<qreal,qreal>& greenRange = qMakePair(0.0,1.0),
							   const QPair<qreal,qreal>& blueRange = qMakePair(0.0,1.0),
                               const QPair<qreal,qreal>& alphaRange = qMakePair(1.0,1.0)) = 0;
							   
    QColor randomColor(qreal rStart, qreal rEnd, qreal gStart, qreal gEnd, qreal bStart, qreal bEnd, qreal aStart, qreal aEnd) {
        return randomColor(qMakePair(rStart,rEnd), qMakePair(gStart,gEnd), qMakePair(bStart,bEnd), qMakePair(aStart,aEnd));
	}
	
    QColor randomColor(const QColor& start, const QColor& end) {
		qreal rStart, gStart, bStart, aStart;
		qreal rEnd, gEnd, bEnd, aEnd;
		start.getRgbF(&rStart, &gStart, &bStart, &aStart);
		end.getRgbF(&rEnd, &gEnd, &bEnd, &aEnd);
		return randomColor(rStart, rEnd, gStart, gEnd, bStart, bEnd, aStart, aEnd);
	}
	
    virtual QColor interpolatedColor(qreal value, const QColor& start, const QColor& end) = 0;
};

Q_DECLARE_INTERFACE(IColorUtils, "com.vcreatelogic.IColorUtils/1.0")

#endif

