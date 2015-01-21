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

#include "XMLGCFMessage_p.h"
#include "XMLGCFMessage.h"
#include <GCF/IXmlDataSerializer>

// Include all supported types..
#include <QVariant>
#include <QColor>
#include <QPixmap>
#include <QTime>
#include <QDate>
#include <QDateTime>
#include <QString>
#include <QByteArray>
#include <QBitArray>
#include <QImage>
#include <QBitmap>
#include <QPointF>
#include <QPoint>
#include <QLineF>
#include <QLine>
#include <QSizeF>
#include <QSize>
#include <QRectF>
#include <QRect>
#include <QMatrix>
#include <QTransform>
#include <QUrl>
#include <QSizePolicy>
#include <QBrush>
#include <QPolygon>
#include <QPolygonF>
#include <QTextStream>
#include <QPen>
#include <QGradient>
#include <QBuffer>

static bool SaveByteArray(QDomDocument doc, QDomElement element, QByteArray array)
{
    QByteArray base64 = array.toBase64();

    QDomElement base64E = doc.createElement("Base64");
    base64E.appendChild(doc.createTextNode(base64));

    element.appendChild(base64E);

    return true;
}

static bool LoadByteArray(QDomElement element, QByteArray& array)
{
    QDomElement base64E = element.firstChildElement("Base64");
    QByteArray base64 = base64E.text().toLatin1();

    array = QByteArray::fromBase64(base64);

    return true;
}

QDomElement AbstractMessageAdapter::encodeVariant(const QVariant& variant, QDomDocument doc)
{
    QDomElement variantE = doc.createElement("Variant");

    if(variant.type() == QVariant::Invalid)
    {
        variantE.setAttribute("Type", "Unsupported");
        variantE.setAttribute("NativeType", QString(variant.typeName()));
        return variantE;
    }

    variantE.setAttribute("Type", variant.typeName());

    // Serialize value on to the byte array
    switch( variant.type() )
    {
    // These types are readily convertible to strings from QVariant
    // class itself!
    case QVariant::Bool:
    case QVariant::Char:
    case QVariant::Date:
    case QVariant::DateTime:
    case QVariant::String:
    case QVariant::Double:
    case QVariant::Int:
    case QVariant::LongLong:
    case QVariant::Time:
    case QVariant::UInt:
    case QVariant::ULongLong:
        variantE.appendChild(doc.createTextNode(variant.toString()));
        break;

    case QVariant::Color:
        variantE.appendChild(doc.createTextNode(variant.value<QColor>().name()));
        break;

    case QVariant::Pixmap: {
        // We need to save and load Pixmaps only in base64 format.
        QImage image = variant.value<QPixmap>().toImage();

        // Save the JPG representation into a buffer
        QByteArray ba;
        QBuffer buffer(&ba);
        buffer.open(QIODevice::WriteOnly);
        image.save(&buffer, "PNG");

        // Convert ba to base64.
        ba = ba.toBase64();

        // Now prefix the image type information to ba.
        ba = ba.prepend("data:image/png;base64;charset=utf-8,");

        // Write this into the XML element.
        variantE.appendChild( doc.createTextNode(ba) );
        } break;

    case QVariant::ByteArray:
        SaveByteArray(doc, variantE, variant.toByteArray());
        break;

    case QVariant::BitArray: {
        QBitArray bits = variant.toBitArray();
        QByteArray ba;
        QDataStream ds(&ba, QIODevice::WriteOnly);
        ds << bits;
        SaveByteArray(doc, variantE, ba);
        } break;

    case QVariant::Image: {
        // We need to save and load Pixmaps only in base64 format.
        QImage image = variant.value<QImage>();

        // Save the JPG representation into a buffer
        QByteArray ba;
        QBuffer buffer(&ba);
        buffer.open(QIODevice::WriteOnly);
        image.save(&buffer, "PNG");

        // Convert ba to base64.
        ba = ba.toBase64();

        // Now prefix the image type information to ba.
        ba = ba.prepend("data:image/png;base64;charset=utf-8,");

        // Write this into the XML element.
        variantE.appendChild( doc.createTextNode(ba) );
        } break;

    case QVariant::Url:
        variantE.appendChild( doc.createTextNode(variant.toUrl().toString()) );
        break;

    case QVariant::StringList: {
        QStringList list = variant.toStringList();
        Q_FOREACH(QString item, list)
        {
            QDomElement itemE = doc.createElement("Item");
            itemE.appendChild(doc.createTextNode(item));
            variantE.appendChild(itemE);
        }
        } break;

    case QVariant::SizePolicy: {
        QSizePolicy sp = variant.value<QSizePolicy>();
        QDomElement element;

        element = doc.createElement("ControlType");
        element.appendChild(doc.createTextNode(QString::number(sp.controlType())));
        variantE.appendChild(element);

        element = doc.createElement("HeightForWidth");
        element.appendChild(doc.createTextNode(sp.hasHeightForWidth() ? "true" : "false"));
        variantE.appendChild(element);

        element = doc.createElement("HorizontalPolicy");
        element.appendChild(doc.createTextNode(QString::number(sp.horizontalPolicy())));
        variantE.appendChild(element);

        element = doc.createElement("HorizontalStretch");
        element.appendChild(doc.createTextNode(QString::number(sp.horizontalStretch())));
        variantE.appendChild(element);

        element = doc.createElement("VerticalPolicy");
        element.appendChild(doc.createTextNode(QString::number(sp.verticalPolicy())));
        variantE.appendChild(element);

        element = doc.createElement("VerticalStretch");
        element.appendChild(doc.createTextNode(QString::number(sp.verticalStretch())));
        variantE.appendChild(element);

        } break;

    case QVariant::SizeF: {
        QSizeF size = variant.value<QSizeF>();

        QDomElement widthE = doc.createElement("Width");
        widthE.appendChild(doc.createTextNode(QString::number(size.width())));
        variantE.appendChild(widthE);

        QDomElement heightE = doc.createElement("Height");
        heightE.appendChild(doc.createTextNode(QString::number(size.height())));
        variantE.appendChild(heightE);
        } break;

    case QVariant::Size: {
        QSize size = variant.value<QSize>();

        QDomElement widthE = doc.createElement("Width");
        widthE.appendChild(doc.createTextNode(QString::number(size.width())));
        variantE.appendChild(widthE);

        QDomElement heightE = doc.createElement("Height");
        heightE.appendChild(doc.createTextNode(QString::number(size.height())));
        variantE.appendChild(heightE);
        } break;

    case QVariant::RegExp: {
        variantE.appendChild( doc.createTextNode(variant.toRegExp().pattern()) );
        } break;

    case QVariant::RectF: {
        QRectF rect = variant.value<QRectF>();

        QDomElement xE = doc.createElement("X");
        xE.appendChild(doc.createTextNode(QString::number(rect.x())));
        variantE.appendChild(xE);

        QDomElement yE = doc.createElement("Y");
        yE.appendChild(doc.createTextNode(QString::number(rect.y())));
        variantE.appendChild(yE);

        QDomElement widthE = doc.createElement("Width");
        widthE.appendChild(doc.createTextNode(QString::number(rect.width())));
        variantE.appendChild(widthE);

        QDomElement heightE = doc.createElement("Height");
        heightE.appendChild(doc.createTextNode(QString::number(rect.height())));
        variantE.appendChild(heightE);
        } break;

    case QVariant::Rect: {
        QRect rect = variant.value<QRect>();

        QDomElement xE = doc.createElement("X");
        xE.appendChild(doc.createTextNode(QString::number(rect.x())));
        variantE.appendChild(xE);

        QDomElement yE = doc.createElement("Y");
        yE.appendChild(doc.createTextNode(QString::number(rect.y())));
        variantE.appendChild(yE);

        QDomElement widthE = doc.createElement("Width");
        widthE.appendChild(doc.createTextNode(QString::number(rect.width())));
        variantE.appendChild(widthE);

        QDomElement heightE = doc.createElement("Height");
        heightE.appendChild(doc.createTextNode(QString::number(rect.height())));
        variantE.appendChild(heightE);
        } break;

    case QVariant::Polygon: {
        QPolygon poly = variant.value<QPolygon>();

        Q_FOREACH(QPoint pt, poly)
        {
            QDomElement itemE = doc.createElement("Item");
            variantE.appendChild(itemE);

            QDomElement xE = doc.createElement("X");
            xE.appendChild(doc.createTextNode(QString::number(pt.x())));
            itemE.appendChild(xE);

            QDomElement yE = doc.createElement("Y");
            yE.appendChild(doc.createTextNode(QString::number(pt.y())));
            itemE.appendChild(yE);
        }
        } break;

    case QVariant::PointF: {
        QPointF pt = variant.value<QPointF>();

        QDomElement xE = doc.createElement("X");
        xE.appendChild(doc.createTextNode(QString::number(pt.x())));
        variantE.appendChild(xE);

        QDomElement yE = doc.createElement("Y");
        yE.appendChild(doc.createTextNode(QString::number(pt.y())));
        variantE.appendChild(yE);
        } break;

    case QVariant::Point: {
        QPoint pt = variant.value<QPoint>();

        QDomElement xE = doc.createElement("X");
        xE.appendChild(doc.createTextNode(QString::number(pt.x())));
        variantE.appendChild(xE);

        QDomElement yE = doc.createElement("Y");
        yE.appendChild(doc.createTextNode(QString::number(pt.y())));
        variantE.appendChild(yE);
        } break;

    case QVariant::Matrix: {
        QMatrix mat = variant.value<QMatrix>();
        QDomElement element;

        element = doc.createElement("M11");
        element.appendChild(doc.createTextNode(QString::number(mat.m11())));
        variantE.appendChild(element);

        element = doc.createElement("M12");
        element.appendChild(doc.createTextNode(QString::number(mat.m12())));
        variantE.appendChild(element);

        element = doc.createElement("M21");
        element.appendChild(doc.createTextNode(QString::number(mat.m21())));
        variantE.appendChild(element);

        element = doc.createElement("M22");
        element.appendChild(doc.createTextNode(QString::number(mat.m22())));
        variantE.appendChild(element);

        element = doc.createElement("DX");
        element.appendChild(doc.createTextNode(QString::number(mat.dx())));
        variantE.appendChild(element);

        element = doc.createElement("DY");
        element.appendChild(doc.createTextNode(QString::number(mat.dy())));
        variantE.appendChild(element);

        } break;

    case QVariant::LineF: {
        QLineF line = variant.value<QLineF>();

        QDomElement x1E = doc.createElement("X1");
        x1E.appendChild(doc.createTextNode(QString::number(line.x1())));
        variantE.appendChild(x1E);

        QDomElement y1E = doc.createElement("Y1");
        y1E.appendChild(doc.createTextNode(QString::number(line.y1())));
        variantE.appendChild(y1E);

        QDomElement x2E = doc.createElement("X2");
        x2E.appendChild(doc.createTextNode(QString::number(line.x2())));
        variantE.appendChild(x2E);

        QDomElement y2E = doc.createElement("Y2");
        y2E.appendChild(doc.createTextNode(QString::number(line.y2())));
        variantE.appendChild(y2E);

        } break;

    case QVariant::Line: {
        QLine line = variant.value<QLine>();

        QDomElement x1E = doc.createElement("X1");
        x1E.appendChild(doc.createTextNode(QString::number(line.x1())));
        variantE.appendChild(x1E);

        QDomElement y1E = doc.createElement("Y1");
        y1E.appendChild(doc.createTextNode(QString::number(line.y1())));
        variantE.appendChild(y1E);

        QDomElement x2E = doc.createElement("X2");
        x2E.appendChild(doc.createTextNode(QString::number(line.x2())));
        variantE.appendChild(x2E);

        QDomElement y2E = doc.createElement("Y2");
        y2E.appendChild(doc.createTextNode(QString::number(line.y2())));
        variantE.appendChild(y2E);

        } break;

    case QVariant::Pen: {
        QPen pen = variant.value<QPen>();
        QDomElement element;

        element = doc.createElement("Color");
        element.appendChild(doc.createTextNode(pen.color().name()));
        variantE.appendChild(element);

        element = doc.createElement("Width");
        element.appendChild(doc.createTextNode(QString::number(pen.widthF())));
        variantE.appendChild(element);

        element = doc.createElement("Style");
        element.appendChild(doc.createTextNode(QString::number(pen.style())));
        variantE.appendChild(element);

        element = doc.createElement("CapStyle");
        element.appendChild(doc.createTextNode(QString::number(pen.capStyle())));
        variantE.appendChild(element);

        element = doc.createElement("JoinStyle");
        element.appendChild(doc.createTextNode(QString::number(pen.joinStyle())));
        variantE.appendChild(element);

        } break;

    case QVariant::Brush: {
        QBrush brush = variant.value<QBrush>();
        QDomElement element;

        element = doc.createElement("Type");
        if(!brush.texture().isNull())
            element.appendChild(doc.createTextNode("Pixmap"));
        else if(!brush.textureImage().isNull())
            element.appendChild(doc.createTextNode("Image"));
        else if(brush.gradient())
            element.appendChild(doc.createTextNode("Gradient"));
        else
            element.appendChild(doc.createTextNode("Default"));
        variantE.appendChild(element);

        if(brush.gradient())
        {
            const QGradient* grad = brush.gradient();
            QDomElement gradE = doc.createElement("Gradient");
            variantE.appendChild(gradE);

            element = doc.createElement("Type");
            element.appendChild(doc.createTextNode(QString::number(grad->type())));
            gradE.appendChild(element);

            element = doc.createElement("CoordinateMode");
            element.appendChild(doc.createTextNode(QString::number(grad->coordinateMode())));
            gradE.appendChild(element);

            element = doc.createElement("Spread");
            element.appendChild(doc.createTextNode(QString::number(grad->spread())));
            gradE.appendChild(element);

            QDomElement stopsE = doc.createElement("Stops");
            gradE.appendChild(stopsE);

            Q_FOREACH(QGradientStop stop, grad->stops())
            {
                QDomElement itemE = doc.createElement("Item");
                stopsE.appendChild(itemE);

                QDomElement posE = doc.createElement("Position");
                posE.appendChild(doc.createTextNode(QString::number(stop.first)));
                itemE.appendChild(posE);

                QDomElement colorE = doc.createElement("Color");
                colorE.appendChild(doc.createTextNode(stop.second.name()));
                itemE.appendChild(colorE);
            }
        }
        else if( !brush.texture().isNull() )
        {
            QByteArray ba;
            QDataStream ds(&ba, QIODevice::WriteOnly);
            ds << brush.texture();
            SaveByteArray(doc, variantE, ba);
        }
        else if( !brush.textureImage().isNull() )
        {
            QByteArray ba;
            QDataStream ds(&ba, QIODevice::WriteOnly);
            ds << brush.textureImage();
            SaveByteArray(doc, variantE, ba);
        }
        else
        {
            element = doc.createElement("Color");
            element.appendChild(doc.createTextNode(brush.color().name()));
            variantE.appendChild(element);

            element = doc.createElement("Style");
            element.appendChild(doc.createTextNode(QString::number(brush.style())));
            variantE.appendChild(element);
        }

        } break;

    case QVariant::Bitmap: {
        // We need to save and load Pixmaps only in base64 format.
        QImage image = variant.value<QBitmap>().toImage();

        // Save the JPG representation into a buffer
        QByteArray ba;
        QBuffer buffer(&ba);
        buffer.open(QIODevice::WriteOnly);
        image.save(&buffer, "PNG");

        // Convert ba to base64.
        ba = ba.toBase64();

        // Now prefix the image type information to ba.
        ba = ba.prepend("data:image/png;base64;charset=utf-8,");

        // Write this into the XML element.
        variantE.appendChild( doc.createTextNode(ba) );
        } break;

    case QVariant::Transform: {
        QTransform tx = variant.value<QTransform>();
        QDomElement element;

        element = doc.createElement("M11");
        element.appendChild(doc.createTextNode(QString::number(tx.m11())));
        variantE.appendChild(element);

        element = doc.createElement("M12");
        element.appendChild(doc.createTextNode(QString::number(tx.m12())));
        variantE.appendChild(element);

        element = doc.createElement("M13");
        element.appendChild(doc.createTextNode(QString::number(tx.m13())));
        variantE.appendChild(element);

        element = doc.createElement("M21");
        element.appendChild(doc.createTextNode(QString::number(tx.m21())));
        variantE.appendChild(element);

        element = doc.createElement("M22");
        element.appendChild(doc.createTextNode(QString::number(tx.m22())));
        variantE.appendChild(element);

        element = doc.createElement("M23");
        element.appendChild(doc.createTextNode(QString::number(tx.m23())));
        variantE.appendChild(element);

        element = doc.createElement("M31");
        element.appendChild(doc.createTextNode(QString::number(tx.m31())));
        variantE.appendChild(element);

        element = doc.createElement("M32");
        element.appendChild(doc.createTextNode(QString::number(tx.m32())));
        variantE.appendChild(element);

        element = doc.createElement("M33");
        element.appendChild(doc.createTextNode(QString::number(tx.m33())));
        variantE.appendChild(element);

        } break;

    case QVariant::List: {
        QList<QVariant> list = variant.toList();
        Q_FOREACH(QVariant item, list)
        {
            QDomElement itemE = AbstractMessageAdapter::encodeVariant(item, doc);
            variantE.appendChild(itemE);
        }
        } break;

    case QVariant::Map: {
        QMap<QString,QVariant> variantMap = variant.toMap();
        QMap<QString,QVariant>::const_iterator it = variantMap.constBegin();
        QMap<QString,QVariant>::const_iterator end = variantMap.constEnd();
        while(it != end) {
            QString key = it.key();
            QVariant value = it.value();

            QDomElement itemE = doc.createElement("Item");
            variantE.appendChild(itemE);

            QDomElement keyE = doc.createElement("Key");
            keyE.appendChild(doc.createTextNode(key));
            itemE.appendChild(keyE);

            QDomElement valueE = doc.createElement("Value");
            valueE.appendChild( AbstractMessageAdapter::encodeVariant(value, doc) );
            itemE.appendChild(valueE);

            ++it;
        }
        } break;

    default: {
            IXmlDataSerializer* serializer = XMLGCFMessageHandler::instance().findSerializer( variant.typeName() );
            bool success = false;
            if(serializer)
                success = serializer->toXml(variant, variantE, doc);
            if(!success)
                variantE.setAttribute("Type", "Unsupported");
        } break;
    }

    return variantE;
}

QVariant AbstractMessageAdapter::extractVariant(QDomElement element)
{
    QVariant variant;
    QString typeStr = element.attribute("Type");

    if(typeStr == "Unsupported")
        return variant;

    // Serialize value on to the byte array
    QVariant::Type type = QVariant::nameToType(qPrintable(typeStr));
    switch(type)
    {
    // These types are readily convertible to strings from QVariant
    // class itself!
    case QVariant::Bool:
        variant = (element.text() == "true") ? true : false;
        break;

    case QVariant::Char:
        variant = element.text().at(0);
        break;

    case QVariant::Date:
        variant = QDate::fromString(element.text());
        break;

    case QVariant::DateTime:
        variant = QDateTime::fromString(element.text());
        break;

    case QVariant::String:
        variant = element.text();
        break;

    case QVariant::Double:
        variant = element.text().toDouble();
        break;

    case QVariant::Int:
        variant = element.text().toInt();
        break;

    case QVariant::LongLong:
        variant = element.text().toLongLong();
        break;

    case QVariant::Time:
        variant = QTime::fromString(element.text());
        break;

    case QVariant::UInt:
        variant = element.text().toUInt();
        break;

    case QVariant::ULongLong:
        variant = element.text().toULongLong();
        break;

    case QVariant::Color:
        variant = QColor(element.text());
        break;

    case QVariant::Pixmap: {
        QString imgSrc = element.text();

        QByteArray format= "PNG";
        // imgSrc will have 'data:image/png;base64;charset=utf-8,' as the starting
        // sub-string. So we simply eliminate it.
        int sPos = imgSrc.indexOf(',')+1;
        QString imgType = imgSrc.mid(0, sPos);
        if(imgType.toLower().contains("jpg"))
            format = "JPG";
        else if(imgType.toLower().contains("jpeg"))
            format = "JPEG";
        else if(imgType.toLower().contains("gif"))
            format = "GIF";
        else if(imgType.toLower().contains("bmp"))
            format = "BMP";
        imgSrc = imgSrc.mid(sPos);

        // The remaining string is base64.
        QByteArray base64 = imgSrc.toLatin1();

        // Extract the image from it.
        QByteArray ba = QByteArray::fromBase64(base64);

        // Load the image from it.
        QBuffer buffer(&ba);
        buffer.open(QIODevice::ReadOnly);
        QImage image;
        image.load(&buffer, format.constData());

        // Convert it to a QPixmap
        QPixmap pm = QPixmap::fromImage(image);
        variant = pm;
        } break;

    case QVariant::ByteArray: {
        QByteArray ba;
        LoadByteArray(element, ba);

        variant = ba;
        } break;

     case QVariant::BitArray: {
         QByteArray ba;
         LoadByteArray(element, ba);

         QDataStream ds(&ba, QIODevice::ReadOnly);
         QBitArray bits;
         ds >> bits;

         variant = bits;
         } break;

    case QVariant::Image: {
        QString imgSrc = element.text();
        QByteArray format= "PNG";
        // imgSrc will have 'data:image/png;base64;charset=utf-8,' as the starting
        // sub-string. So we simply eliminate it.
        int sPos = imgSrc.indexOf(',')+1;
        QString imgType = imgSrc.mid(0, sPos);
        if(imgType.toLower().contains("jpg"))
            format = "JPG";
        else if(imgType.toLower().contains("jpeg"))
            format = "JPEG";
        else if(imgType.toLower().contains("gif"))
            format = "GIF";
        else if(imgType.toLower().contains("bmp"))
            format = "BMP";
        imgSrc = imgSrc.mid(sPos);

        // The remaining string is base64.
        QByteArray base64 = imgSrc.toLatin1();

        // Extract the image from it.
        QByteArray ba = QByteArray::fromBase64(base64);

        // Load the image from it.
        QBuffer buffer(&ba);
        buffer.open(QIODevice::ReadOnly);
        QImage image;
        image.load(&buffer, format.constData());
        variant = image;
        } break;

    case QVariant::Url:
        variant =  QUrl(element.text());
        break;

    case QVariant::StringList: {
        QStringList list;
        QDomElement itemE = element.firstChildElement("Item");

        while(!itemE.isNull())
        {
            list << itemE.text();
            itemE = itemE.nextSiblingElement("Item");
        }

        variant = list;
        } break;

    case QVariant::SizePolicy: {
        QSizePolicy sp;

        sp.setControlType(QSizePolicy::ControlType(
                element.firstChildElement("ControlType").text().toInt()));
        sp.setHeightForWidth((element.firstChildElement("HeightForWidth").text() == "true") ?
                                                        true : false);
        sp.setHorizontalPolicy(QSizePolicy::Policy(
                element.firstChildElement("HorizontalPolicy").text().toInt()));
        sp.setHorizontalStretch(element.firstChildElement("HorizontalStretch").text().toInt());
        sp.setVerticalPolicy(QSizePolicy::Policy(
                element.firstChildElement("VerticalPolicy").text().toInt()));
        sp.setVerticalStretch(element.firstChildElement("VerticalStretch").text().toInt());

        variant = sp;
        } break;

    case QVariant::SizeF: {
        QSizeF sizeF;

        sizeF.setWidth(element.firstChildElement("Width").text().toDouble());
        sizeF.setHeight(element.firstChildElement("Height").text().toDouble());

        variant = sizeF;
        } break;

    case QVariant::Size: {
        QSize size;

        size.setWidth(element.firstChildElement("Width").text().toInt());
        size.setHeight(element.firstChildElement("Height").text().toInt());

        variant = size;
        } break;

    case QVariant::RegExp:
        variant = QRegExp(element.text());
        break;

    case QVariant::RectF: {
        QRectF rectF;

        rectF.setX(element.firstChildElement("X").text().toDouble());
        rectF.setY(element.firstChildElement("X").text().toDouble());
        rectF.setX(element.firstChildElement("Width").text().toDouble());
        rectF.setY(element.firstChildElement("Height").text().toDouble());

        variant = rectF;
        } break;

    case QVariant::Rect: {
        QRect rectF;

        rectF.setX(element.firstChildElement("X").text().toInt());
        rectF.setY(element.firstChildElement("X").text().toInt());
        rectF.setX(element.firstChildElement("Width").text().toInt());
        rectF.setY(element.firstChildElement("Height").text().toInt());

        variant = rectF;
        } break;

    case QVariant::Polygon: {
        QPolygon poly;
        QDomElement itemE = element.firstChildElement("Element");

        while(!itemE.isNull())
        {
            QPoint point;
            point.setX(itemE.firstChildElement("X").text().toInt());
            point.setY(itemE.firstChildElement("Y").text().toInt());

            poly << point;
            itemE = element.nextSiblingElement("Element");
        }

        variant = poly;
        } break;

    case QVariant::PointF: {
        QPointF pt;

        pt.setX(element.firstChildElement("X").text().toDouble());
        pt.setY(element.firstChildElement("Y").text().toDouble());

        variant = pt;
        } break;

    case QVariant::Point: {
        QPoint pt ;

        pt.setX(element.firstChildElement("X").text().toInt());
        pt.setY(element.firstChildElement("Y").text().toInt());

        variant = pt;
        } break;

    case QVariant::Matrix: {
        qreal m11 = element.firstChildElement("M11").text().toDouble();
        qreal m12 = element.firstChildElement("M12").text().toDouble();
        qreal m21 = element.firstChildElement("M21").text().toDouble();
        qreal m22 = element.firstChildElement("M22").text().toDouble();
        qreal dx = element.firstChildElement("DX").text().toDouble();
        qreal dy = element.firstChildElement("DY").text().toDouble();

        QMatrix mat(m11, m12, m21, m22, dx, dy);

        variant = mat;
        } break;

    case QVariant::LineF: {
        QLineF line;

        QPointF p1(element.firstChildElement("X1").text().toDouble(),
                   element.firstChildElement("Y1").text().toDouble());
        QPointF p2(element.firstChildElement("X2").text().toDouble(),
                   element.firstChildElement("Y2").text().toDouble());
        line.setP1(p1);
        line.setP2(p2);

        variant = line;
        } break;

    case QVariant::Line: {
        QLine line;

        QPoint p1(element.firstChildElement("X1").text().toInt(),
                   element.firstChildElement("Y1").text().toInt());
        QPoint p2(element.firstChildElement("X2").text().toInt(),
                   element.firstChildElement("Y2").text().toInt());
        line.setP1(p1);
        line.setP2(p2);

        variant = line;
        } break;

    case QVariant::Pen: {
        QPen pen;

        pen.setColor(QColor(element.firstChildElement("Color").text()));
        pen.setWidthF(element.firstChildElement("Color").text().toDouble());
        pen.setStyle(Qt::PenStyle(element.firstChildElement("Color").text().toInt()));
        pen.setCapStyle(Qt::PenCapStyle(element.firstChildElement("Color").text().toInt()));
        pen.setJoinStyle(Qt::PenJoinStyle(element.firstChildElement("Color").text().toInt()));

        variant = pen;
        } break;

    case QVariant::Brush: {
        QBrush brush;

        if(element.firstChildElement("Type").text() == "Pixmap")
        {
            QPixmap pm;
            QByteArray ba;
            LoadByteArray(element, ba);
            QDataStream ds(&ba, QIODevice::ReadOnly);
            ds >> pm;

            brush.setTexture( pm );
        }
        else if(element.firstChildElement("Type").text() == "Image")
        {
            QPixmap pm;
            QByteArray ba;
            LoadByteArray(element, ba);
            QDataStream ds(&ba, QIODevice::ReadOnly);
            ds >> pm;

            QImage im = pm.toImage();
            brush.setTextureImage( im );
        }
        else if(element.firstChildElement("Type").text() == "Gradient")
        {
            QDomElement gradE = element.firstChildElement("Gradient");
            QGradient* grad;

            if(gradE.firstChildElement("Type").text().toInt() == 0)
                grad = new QLinearGradient;
            else if(gradE.firstChildElement("Type").text().toInt() == 1)
                grad = new QRadialGradient;
            else if(gradE.firstChildElement("Type").text().toInt() == 2)
                grad = new QConicalGradient;

            grad->setCoordinateMode(QGradient::CoordinateMode(gradE.firstChildElement("CoordinateMode").text().toInt()));
            grad->setSpread(QGradient::Spread(gradE.firstChildElement("Spread").text().toInt()));

            QDomElement itemE = gradE.firstChildElement("Stops");
            QVector<QGradientStop> gradStops;

            while(!itemE.isNull())
            {
                QPair<qreal, QColor> gradStop;
                gradStop.first = itemE.firstChildElement("Position").text().toDouble();
                gradStop.second = itemE.firstChildElement("Color").text().toDouble();

                gradStops << gradStop;
                itemE = gradE.nextSiblingElement("Item");
            }

            grad->setStops(gradStops);
            brush = QBrush(*grad);

            delete grad;
        }
        else
            brush = QBrush();

        variant = brush;
        }break;

    case QVariant::Bitmap: {
        QString imgSrc = element.text();

        // imgSrc will have 'data:image/png;base64;charset=utf-8,' as the starting
        // sub-string. So we simply eliminate it.
        imgSrc = imgSrc.remove(0, 36);

        // The remaining string is base64.
        QByteArray base64 = imgSrc.toLatin1();

        // Extract the image from it.
        QByteArray ba = QByteArray::fromBase64(base64);

        // Load the image from it.
        QBuffer buffer(&ba);
        buffer.open(QIODevice::ReadOnly);
        QImage image;
        image.load(&buffer, "PNG");

        // Convert it to a QBitmap
        QPixmap pm = QPixmap::fromImage(image);
        variant = QBitmap(pm);
        } break;

    case QVariant::Transform: {
        QTransform tx;

        qreal m11 = element.firstChildElement("M11").text().toDouble();
        qreal m12 = element.firstChildElement("M12").text().toDouble();
        qreal m13 = element.firstChildElement("M13").text().toDouble();
        qreal m21 = element.firstChildElement("M21").text().toDouble();
        qreal m22 = element.firstChildElement("M22").text().toDouble();
        qreal m23 = element.firstChildElement("M23").text().toDouble();
        qreal m31 = element.firstChildElement("M31").text().toDouble();
        qreal m32 = element.firstChildElement("M32").text().toDouble();
        qreal m33 = element.firstChildElement("M33").text().toDouble();

        tx.setMatrix(m11, m12, m13, m21, m22, m23, m31, m32, m33);

        variant = tx;
        } break;

    case QVariant::List: {
        QDomElement itemE = element.firstChildElement("Variant");
        QList<QVariant> variantList;
        while(!itemE.isNull())
        {
            QVariant variant = AbstractMessageAdapter::extractVariant(itemE);
            variantList.append(variant);

            itemE = itemE.nextSiblingElement("Variant");
        }

        variant = variantList;
        } break;

    case QVariant::Map: {
        QDomElement itemE = element.firstChildElement("Item");
        QMap<QString,QVariant> variantMap;
        while( !itemE.isNull() )
        {
            QDomElement keyE = itemE.firstChildElement("Key");
            QDomElement valueE = itemE.firstChildElement("Value").firstChildElement("Variant");

            variantMap[keyE.text()] = AbstractMessageAdapter::extractVariant(valueE);

            itemE = itemE.nextSiblingElement("Item");
        }

        variant = variantMap;

        } break;

    default: {
            IXmlDataSerializer* serializer
                = XMLGCFMessageHandler::instance().findSerializer( typeStr.toLatin1() );
            bool success = false;
            if(serializer)
                success = serializer->fromXml(element, variant);
            if(!success)
                variant = QVariant();
        } break;
    }

    return variant;
}

GCF::Message* AbstractMessageAdapter::fromDOM(QDomDocument doc)
{
    GCF::Message* msg = this->createMessage();
    if(msg->messageType() != m_type)
    {
        delete msg;
        return 0;
    }

    readHeader(doc, msg);
    readBody(doc, msg);
    readFooter(doc, msg);

    return msg;
}

QDomDocument AbstractMessageAdapter::toDOM(const GCF::Message* message)
{
    if(message->messageType() != m_type)
        return QDomDocument();

    QDomDocument doc;
    QDomElement rootE = doc.createElement("GCFMessage");
    rootE.setAttribute("Type", (int)message->messageType());
    rootE.setAttribute("Timestamp", QDateTime::currentDateTime().toString());
    doc.appendChild(rootE);

    writeHeader(message, doc);
    writeBody(message, doc);
    writeFooter(message, doc);

    return doc;
}

void AbstractMessageAdapter::readHeader(QDomDocument doc, GCF::Message* message)
{
    QDomElement rootE = doc.documentElement();
    QDomElement msgIdE = rootE.firstChildElement("MessageID");
    qint32 id = (qint32)msgIdE.text().toInt();

    message->setId(id);

    if(rootE.hasAttribute("GUID"))
    {
        QString guid = rootE.attribute("GUID");
        message->customData().addKey("ClientGUID", guid);
    }
}

void AbstractMessageAdapter::readFooter(QDomDocument doc, GCF::Message* message)
{
    GCF::DataStore customData = message->customData();

    QDomElement rootE = doc.documentElement();
    QDomElement customDataE = rootE.firstChildElement("CustomData");

    QDomElement itemE = customDataE.firstChildElement("Item");
    while(!itemE.isNull())
    {
        QDomElement keyE = itemE.firstChildElement("Key");
        QDomElement valueE = itemE.firstChildElement("Value");

        customData[keyE.text()] = valueE.text();

        itemE = itemE.nextSiblingElement("Item");
    }

    QDomElement sessionIdE = rootE.firstChildElement("SessionID");
    qint32 id = (qint32)sessionIdE.text().toInt();
    message->setSessionId(id);
}

void AbstractMessageAdapter::writeHeader(const GCF::Message* message, QDomDocument doc)
{
    QDomElement rootE = doc.documentElement();

    QDomElement msgIdE = doc.createElement("MessageID");
    msgIdE.appendChild( doc.createTextNode( QString::number(message->id()) ) );
    rootE.appendChild(msgIdE);
}

void AbstractMessageAdapter::writeFooter(const GCF::Message* message, QDomDocument doc)
{
    GCF::DataStore customData = message->customData();

    QDomElement rootE = doc.documentElement();

    QDomElement customDataE = doc.createElement("CustomData");
    rootE.appendChild(customDataE);

    QStringList keys = customData.keys();
    Q_FOREACH(QString key, keys)
    {
        QVariant value = customData[key];
        if(!value.canConvert(QVariant::String))
            continue;

        QDomElement itemE = doc.createElement("Item");
        customDataE.appendChild(itemE);

        QDomElement keyE = doc.createElement("Key");
        keyE.appendChild( doc.createTextNode(key) );
        itemE.appendChild(keyE);

        QDomElement valueE = doc.createElement("Value");
        valueE.appendChild( doc.createTextNode(value.toString()) );
        itemE.appendChild(valueE);
    }

    QDomElement sessionIdE = doc.createElement("SessionID");
    sessionIdE.appendChild( doc.createTextNode( QString::number(message->sessionId()) ) );
    rootE.appendChild(sessionIdE);
}

///////////////////////////////////////////////////////////////////////////////
// RequestAccessMessageAdapter
///////////////////////////////////////////////////////////////////////////////

RequestAccessMessageAdapter::RequestAccessMessageAdapter()
: AbstractMessageAdapter(GCF::Message::RequestAccess)
{

}

RequestAccessMessageAdapter::~RequestAccessMessageAdapter()
{

}

void RequestAccessMessageAdapter::readBody(QDomDocument doc, GCF::Message* message)
{
    GCF::RequestAccessMessage* msg = (GCF::RequestAccessMessage*)message;

    QDomElement rootE = doc.documentElement();
    QDomElement element = rootE.firstChildElement("ApplicationName");
    msg->setApplicationName( element.text() );
}

void RequestAccessMessageAdapter::writeBody(const GCF::Message* message, QDomDocument doc)
{
    const GCF::RequestAccessMessage* msg = (const GCF::RequestAccessMessage*)message;

    QDomElement rootE = doc.documentElement();

    QDomElement element = doc.createElement("ApplicationName");
    element.appendChild(doc.createTextNode(msg->applicationName()));
    rootE.appendChild(element);
}

GCF::Message* RequestAccessMessageAdapter::createMessage()
{
    return new GCF::RequestAccessMessage;
}

///////////////////////////////////////////////////////////////////////////////
// RequestAccessResMessageAdapter
///////////////////////////////////////////////////////////////////////////////

RequestAccessResMessageAdapter::RequestAccessResMessageAdapter()
: AbstractMessageAdapter(GCF::Message::RequestAccessRes)
{

}

RequestAccessResMessageAdapter::~RequestAccessResMessageAdapter()
{

}

void RequestAccessResMessageAdapter::readBody(QDomDocument doc, GCF::Message* message)
{
    GCF::RequestAccessResMessage* msg = (GCF::RequestAccessResMessage*)message;

    QDomElement rootE = doc.documentElement();
    QDomElement element = rootE.firstChildElement("AccessGranted");
    msg->setAccessGranted( QVariant(element.text()).toBool() );
}

void RequestAccessResMessageAdapter::writeBody(const GCF::Message* message, QDomDocument doc)
{
    const GCF::RequestAccessResMessage* msg = (const GCF::RequestAccessResMessage*)message;

    QDomElement rootE = doc.documentElement();

    QVariant val( msg->isAccessGranted());

    QDomElement element = doc.createElement("AccessGranted");
    element.appendChild(doc.createTextNode(val.toString()));
    rootE.appendChild(element);
}

GCF::Message* RequestAccessResMessageAdapter::createMessage()
{
    return new GCF::RequestAccessResMessage;
}

///////////////////////////////////////////////////////////////////////////////
// RequestObjectMessageAdapter
///////////////////////////////////////////////////////////////////////////////

RequestObjectMessageAdapter::RequestObjectMessageAdapter()
: AbstractMessageAdapter(GCF::Message::RequestObject)
{

}

RequestObjectMessageAdapter::~RequestObjectMessageAdapter()
{

}

void RequestObjectMessageAdapter::readBody(QDomDocument doc, GCF::Message* message)
{
    GCF::RequestObjectMessage* msg = (GCF::RequestObjectMessage*)message;

    QDomElement rootE = doc.documentElement();
    QDomElement element = rootE.firstChildElement("ObjectName");
    msg->setObjectName( element.text() );
}

void RequestObjectMessageAdapter::writeBody(const GCF::Message* message, QDomDocument doc)
{
    const GCF::RequestObjectMessage* msg = (const GCF::RequestObjectMessage*)message;

    QDomElement rootE = doc.documentElement();

    QDomElement element = doc.createElement("ObjectName");
    element.appendChild(doc.createTextNode(msg->objectName()));
    rootE.appendChild(element);
}

GCF::Message* RequestObjectMessageAdapter::createMessage()
{
    return new GCF::RequestObjectMessage;
}

///////////////////////////////////////////////////////////////////////////////
// RequestObjectResMessageAdapter
///////////////////////////////////////////////////////////////////////////////

RequestObjectResMessageAdapter::RequestObjectResMessageAdapter()
: AbstractMessageAdapter(GCF::Message::RequestObjectRes)
{

}

RequestObjectResMessageAdapter::~RequestObjectResMessageAdapter()
{

}

void RequestObjectResMessageAdapter::readBody(QDomDocument doc, GCF::Message* message)
{
    GCF::RequestObjectResMessage* msg = (GCF::RequestObjectResMessage*)message;

    QDomElement rootE = doc.documentElement();
    QDomElement element = rootE.firstChildElement("ObjectId");
    msg->setObjectId( (qint32)element.text().toInt() );
}

void RequestObjectResMessageAdapter::writeBody(const GCF::Message* message, QDomDocument doc)
{
    const GCF::RequestObjectResMessage* msg = (const GCF::RequestObjectResMessage*)message;

    QDomElement rootE = doc.documentElement();

    QDomElement element = doc.createElement("ObjectId");
    element.appendChild(doc.createTextNode(QString::number(msg->objectId())));
    rootE.appendChild(element);
}

GCF::Message* RequestObjectResMessageAdapter::createMessage()
{
    return new GCF::RequestObjectResMessage;
}

///////////////////////////////////////////////////////////////////////////////
// GetPropertyNamesMessageAdapter
///////////////////////////////////////////////////////////////////////////////

GetPropertyNamesMessageAdapter::GetPropertyNamesMessageAdapter()
: AbstractMessageAdapter(GCF::Message::GetPropertyNames)
{

}

GetPropertyNamesMessageAdapter::~GetPropertyNamesMessageAdapter()
{

}

void GetPropertyNamesMessageAdapter::readBody(QDomDocument doc, GCF::Message* message)
{
    GCF::GetPropertyNamesMessage* msg = (GCF::GetPropertyNamesMessage*)message;

    QDomElement rootE = doc.documentElement();
    QDomElement element = rootE.firstChildElement("ObjectId");
    msg->setObjectId( (qint32)element.text().toInt() );
}

void GetPropertyNamesMessageAdapter::writeBody(const GCF::Message* message, QDomDocument doc)
{
    const GCF::GetPropertyNamesMessage* msg = (const GCF::GetPropertyNamesMessage*)message;

    QDomElement rootE = doc.documentElement();

    QDomElement element = doc.createElement("ObjectId");
    element.appendChild(doc.createTextNode(QString::number(msg->objectId())));
    rootE.appendChild(element);
}

GCF::Message* GetPropertyNamesMessageAdapter::createMessage()
{
    return new GCF::GetPropertyNamesMessage;
}

///////////////////////////////////////////////////////////////////////////////
// GetPropertyNamesResMessageAdapter
///////////////////////////////////////////////////////////////////////////////

GetPropertyNamesResMessageAdapter::GetPropertyNamesResMessageAdapter()
: AbstractMessageAdapter(GCF::Message::GetPropertyNamesRes)
{

}

GetPropertyNamesResMessageAdapter::~GetPropertyNamesResMessageAdapter()
{

}

void GetPropertyNamesResMessageAdapter::readBody(QDomDocument doc, GCF::Message* message)
{
    GCF::GetPropertyNamesResMessage* msg = (GCF::GetPropertyNamesResMessage*)message;

    QDomElement rootE = doc.documentElement();
    QDomElement element = rootE.firstChildElement("ObjectId");
    msg->setObjectId( (qint32)element.text().toInt() );

    QStringList propNames;
    QDomElement propNamesE = rootE.firstChildElement("PropertyNames");
    QDomElement propNameE = propNamesE.firstChildElement("Item");
    while(!propNameE.isNull())
    {
        propNames.append( propNameE.text() );
        propNameE = propNameE.nextSiblingElement("Item");
    }

    msg->setPropertyNames(propNames);
}

void GetPropertyNamesResMessageAdapter::writeBody(const GCF::Message* message, QDomDocument doc)
{
    const GCF::GetPropertyNamesResMessage* msg = (const GCF::GetPropertyNamesResMessage*)message;

    QDomElement rootE = doc.documentElement();

    QDomElement element = doc.createElement("ObjectId");
    element.appendChild(doc.createTextNode(QString::number(msg->objectId())));
    rootE.appendChild(element);

    QDomElement propNamesE = doc.createElement("PropertyNames");
    rootE.appendChild(propNamesE);

    Q_FOREACH(QString propName, msg->propertyNames())
    {
        QDomElement propE = doc.createElement("Item");
        propE.appendChild(doc.createTextNode(propName));
        propNamesE.appendChild(propE);
    }
}

GCF::Message* GetPropertyNamesResMessageAdapter::createMessage()
{
    return new GCF::GetPropertyNamesResMessage;
}

///////////////////////////////////////////////////////////////////////////////
// InvokeMethodMessageAdapter
///////////////////////////////////////////////////////////////////////////////

InvokeMethodMessageAdapter::InvokeMethodMessageAdapter()
: AbstractMessageAdapter(GCF::Message::InvokeMethod)
{

}

InvokeMethodMessageAdapter::~InvokeMethodMessageAdapter()
{

}

void InvokeMethodMessageAdapter::readBody(QDomDocument doc, GCF::Message* message)
{
    GCF::InvokeMethodMessage* msg = (GCF::InvokeMethodMessage*)message;

    QDomElement rootE = doc.documentElement();

    QDomElement element = rootE.firstChildElement("ObjectId");
    msg->setObjectId( (qint32)element.text().toInt() );

    element = rootE.firstChildElement("MethodName");
    msg->setMethodName(element.text());

    element = rootE.firstChildElement("Args");
    element = element.firstChildElement("Variant");
    QVariantList args;
    while(!element.isNull())
    {
        QVariant arg = extractVariant(element);
        args << arg;

        element = element.nextSiblingElement("Variant");
    }
    msg->setArgList(args);
}

void InvokeMethodMessageAdapter::writeBody(const GCF::Message* message, QDomDocument doc)
{
    const GCF::InvokeMethodMessage* msg = (const GCF::InvokeMethodMessage*)message;

    QDomElement rootE = doc.documentElement();

    QDomElement element = doc.createElement("ObjectId");
    element.appendChild(doc.createTextNode(QString::number(msg->objectId())));
    rootE.appendChild(element);

    element = doc.createElement("MethodName");
    element.appendChild(doc.createTextNode(msg->methodName()));
    rootE.appendChild(element);

    element = doc.createElement("Args");
    rootE.appendChild(element);

    QVariantList args = msg->argList();
    Q_FOREACH(QVariant arg, args)
    {
        QDomElement argE = encodeVariant(arg, doc);
        element.appendChild(argE);
    }
}

GCF::Message* InvokeMethodMessageAdapter::createMessage()
{
    return new GCF::InvokeMethodMessage;
}

///////////////////////////////////////////////////////////////////////////////
// InvokeMethodResMessageAdapter
///////////////////////////////////////////////////////////////////////////////

InvokeMethodResMessageAdapter::InvokeMethodResMessageAdapter()
: AbstractMessageAdapter(GCF::Message::InvokeMethodRes)
{

}

InvokeMethodResMessageAdapter::~InvokeMethodResMessageAdapter()
{

}

void InvokeMethodResMessageAdapter::readBody(QDomDocument doc, GCF::Message* message)
{
    GCF::InvokeMethodResMessage* msg = (GCF::InvokeMethodResMessage*)message;

    QDomElement rootE = doc.documentElement();

    QDomElement element = rootE.firstChildElement("ObjectId");
    msg->setObjectId( (qint32)element.text().toInt() );

    element = rootE.firstChildElement("MethodName");
    msg->setMethodName(element.text());

    element = rootE.firstChildElement("Success");
    msg->setSuccess( QVariant(element.text()).toBool() );

    element = rootE.firstChildElement("Result");
    element = element.firstChildElement("Variant");
    QVariant result = extractVariant(element);
    msg->setRetValue(result);
}

void InvokeMethodResMessageAdapter::writeBody(const GCF::Message* message, QDomDocument doc)
{
    const GCF::InvokeMethodResMessage* msg = (const GCF::InvokeMethodResMessage*)message;

    QDomElement rootE = doc.documentElement();

    QDomElement element = doc.createElement("ObjectId");
    element.appendChild(doc.createTextNode(QString::number(msg->objectId())));
    rootE.appendChild(element);

    element = doc.createElement("MethodName");
    element.appendChild(doc.createTextNode(msg->methodName()));
    rootE.appendChild(element);

    element = doc.createElement("Success");
    element.appendChild(doc.createTextNode(msg->isSuccess() ? "true" : "false"));
    rootE.appendChild(element);

    element = doc.createElement("Result");
    rootE.appendChild(element);

    QDomElement resultE = encodeVariant(msg->retValue(), doc);
    element.appendChild(resultE);
}

GCF::Message* InvokeMethodResMessageAdapter::createMessage()
{
    return new GCF::InvokeMethodResMessage;
}

///////////////////////////////////////////////////////////////////////////////
// RequestConnectionMessageAdapter
///////////////////////////////////////////////////////////////////////////////

RequestConnectionMessageAdapter::RequestConnectionMessageAdapter()
: AbstractMessageAdapter(GCF::Message::RequestConnection)
{

}

RequestConnectionMessageAdapter::~RequestConnectionMessageAdapter()
{

}

void RequestConnectionMessageAdapter::readBody(QDomDocument doc, GCF::Message* message)
{
    GCF::RequestConnectionMessage* msg = (GCF::RequestConnectionMessage*)message;

    QDomElement rootE = doc.documentElement();

    QDomElement element = rootE.firstChildElement("ObjectId");
    msg->setObjectId( (qint32)element.text().toInt() );

    element = rootE.firstChildElement("Signal");
    msg->setSignal(element.text());
}

void RequestConnectionMessageAdapter::writeBody(const GCF::Message* message, QDomDocument doc)
{
    const GCF::RequestConnectionMessage* msg = (const GCF::RequestConnectionMessage*)message;

    QDomElement rootE = doc.documentElement();

    QDomElement element = doc.createElement("ObjectId");
    element.appendChild(doc.createTextNode(QString::number(msg->objectId())));
    rootE.appendChild(element);

    element = doc.createElement("Signal");
    element.appendChild(doc.createTextNode(msg->signal()));
    rootE.appendChild(element);
}

GCF::Message* RequestConnectionMessageAdapter::createMessage()
{
    return new GCF::RequestConnectionMessage;
}


///////////////////////////////////////////////////////////////////////////////
// RequestConnectionResMessageAdapter
///////////////////////////////////////////////////////////////////////////////

RequestConnectionResMessageAdapter::RequestConnectionResMessageAdapter()
: AbstractMessageAdapter(GCF::Message::RequestConnectionRes)
{

}

RequestConnectionResMessageAdapter::~RequestConnectionResMessageAdapter()
{

}

void RequestConnectionResMessageAdapter::readBody(QDomDocument doc, GCF::Message* message)
{
    GCF::RequestConnectionResMessage* msg = (GCF::RequestConnectionResMessage*)message;

    QDomElement rootE = doc.documentElement();

    QDomElement element = rootE.firstChildElement("ObjectId");
    msg->setObjectId( (qint32)element.text().toInt() );

    element = rootE.firstChildElement("Signal");
    msg->setSignal(element.text());

    element = rootE.firstChildElement("Success");
    msg->setSuccess( QVariant(element.text()).toBool() );
}

void RequestConnectionResMessageAdapter::writeBody(const GCF::Message* message, QDomDocument doc)
{
    const GCF::RequestConnectionResMessage* msg = (const GCF::RequestConnectionResMessage*)message;

    QDomElement rootE = doc.documentElement();

    QDomElement element = doc.createElement("ObjectId");
    element.appendChild(doc.createTextNode(QString::number(msg->objectId())));
    rootE.appendChild(element);

    element = doc.createElement("Signal");
    element.appendChild(doc.createTextNode(msg->signal()));
    rootE.appendChild(element);

    element = doc.createElement("Success");
    element.appendChild(doc.createTextNode(msg->isSuccess() ? "true" : "false"));
    rootE.appendChild(element);
}

GCF::Message* RequestConnectionResMessageAdapter::createMessage()
{
    return new GCF::RequestConnectionResMessage;
}

///////////////////////////////////////////////////////////////////////////////
// SignalDeliveryMessageAdapter
///////////////////////////////////////////////////////////////////////////////

SignalDeliveryMessageAdapter::SignalDeliveryMessageAdapter()
: AbstractMessageAdapter(GCF::Message::SignalDelivery)
{

}

SignalDeliveryMessageAdapter::~SignalDeliveryMessageAdapter()
{

}

void SignalDeliveryMessageAdapter::readBody(QDomDocument doc, GCF::Message* message)
{
    GCF::SignalDeliveryMessage* msg = (GCF::SignalDeliveryMessage*)message;

    QDomElement rootE = doc.documentElement();

    QDomElement element = rootE.firstChildElement("ObjectId");
    msg->setObjectId( (qint32)element.text().toInt() );

    element = rootE.firstChildElement("Signal");
    msg->setSignal(element.text());

    element = rootE.firstChildElement("Args");
    element = element.firstChildElement("Variant");
    QVariantList args;
    while(!element.isNull())
    {
        QVariant arg = extractVariant(element);
        args << arg;

        element = element.nextSiblingElement("Variant");
    }
    msg->setArgList(args);
}

void SignalDeliveryMessageAdapter::writeBody(const GCF::Message* message, QDomDocument doc)
{
    const GCF::SignalDeliveryMessage* msg = (const GCF::SignalDeliveryMessage*)message;

    QDomElement rootE = doc.documentElement();

    QDomElement element = doc.createElement("ObjectId");
    element.appendChild(doc.createTextNode(QString::number(msg->objectId())));
    rootE.appendChild(element);

    element = doc.createElement("Signal");
    element.appendChild(doc.createTextNode(msg->signal()));
    rootE.appendChild(element);

    element = doc.createElement("Args");
    rootE.appendChild(element);

    QVariantList args = msg->argList();
    Q_FOREACH(QVariant arg, args)
    {
        QDomElement argE = encodeVariant(arg, doc);
        element.appendChild(argE);
    }
}

GCF::Message* SignalDeliveryMessageAdapter::createMessage()
{
    return new GCF::SignalDeliveryMessage;
}

///////////////////////////////////////////////////////////////////////////////
// ReleaseObjectMessageAdapter
///////////////////////////////////////////////////////////////////////////////

ReleaseObjectMessageAdapter::ReleaseObjectMessageAdapter()
: AbstractMessageAdapter(GCF::Message::ReleaseObject)
{

}

ReleaseObjectMessageAdapter::~ReleaseObjectMessageAdapter()
{

}

void ReleaseObjectMessageAdapter::readBody(QDomDocument doc, GCF::Message* message)
{
    GCF::ReleaseObjectMessage* msg = (GCF::ReleaseObjectMessage*)message;

    QDomElement rootE = doc.documentElement();

    QDomElement element = rootE.firstChildElement("ObjectId");
    msg->setObjectId( (qint32)element.text().toInt() );
}

void ReleaseObjectMessageAdapter::writeBody(const GCF::Message* message, QDomDocument doc)
{
    const GCF::ReleaseObjectMessage* msg = (const GCF::ReleaseObjectMessage*)message;

    QDomElement rootE = doc.documentElement();

    QDomElement element = doc.createElement("ObjectId");
    element.appendChild(doc.createTextNode(QString::number(msg->objectId())));
    rootE.appendChild(element);
}

GCF::Message* ReleaseObjectMessageAdapter::createMessage()
{
    return new GCF::ReleaseObjectMessage;
}

///////////////////////////////////////////////////////////////////////////////
// ReleaseObjectResMessageAdapter
///////////////////////////////////////////////////////////////////////////////

ReleaseObjectResMessageAdapter::ReleaseObjectResMessageAdapter()
: AbstractMessageAdapter(GCF::Message::ReleaseObjectRes)
{

}

ReleaseObjectResMessageAdapter::~ReleaseObjectResMessageAdapter()
{

}

void ReleaseObjectResMessageAdapter::readBody(QDomDocument doc, GCF::Message* message)
{
    GCF::ReleaseObjectResMessage* msg = (GCF::ReleaseObjectResMessage*)message;

    QDomElement rootE = doc.documentElement();

    QDomElement element = rootE.firstChildElement("ObjectId");
    msg->setObjectId( (qint32)element.text().toInt() );

    element = rootE.firstChildElement("Success");
    msg->setSuccess( QVariant(element.text()).toBool() );
}

void ReleaseObjectResMessageAdapter::writeBody(const GCF::Message* message, QDomDocument doc)
{
    const GCF::ReleaseObjectResMessage* msg = (const GCF::ReleaseObjectResMessage*)message;

    QDomElement rootE = doc.documentElement();

    QDomElement element = doc.createElement("ObjectId");
    element.appendChild(doc.createTextNode(QString::number(msg->objectId())));
    rootE.appendChild(element);

    element = doc.createElement("Success");
    element.appendChild(doc.createTextNode(msg->isSuccess() ? "true" : "false"));
    rootE.appendChild(element);
}

GCF::Message* ReleaseObjectResMessageAdapter::createMessage()
{
    return new GCF::ReleaseObjectResMessage;
}

///////////////////////////////////////////////////////////////////////////////
// ReleaseAccessMessageAdapter
///////////////////////////////////////////////////////////////////////////////

ReleaseAccessMessageAdapter::ReleaseAccessMessageAdapter()
: AbstractMessageAdapter(GCF::Message::ReleaseAccess)
{

}

ReleaseAccessMessageAdapter::~ReleaseAccessMessageAdapter()
{

}

void ReleaseAccessMessageAdapter::readBody(QDomDocument doc, GCF::Message* message)
{
    Q_UNUSED(doc);
    Q_UNUSED(message);
}

void ReleaseAccessMessageAdapter::writeBody(const GCF::Message* message, QDomDocument doc)
{
    Q_UNUSED(doc);
    Q_UNUSED(message);
}

GCF::Message* ReleaseAccessMessageAdapter::createMessage()
{
    return new GCF::ReleaseAccessMessage;
}

///////////////////////////////////////////////////////////////////////////////
// ReleaseAccessResMessageAdapter
///////////////////////////////////////////////////////////////////////////////

ReleaseAccessResMessageAdapter::ReleaseAccessResMessageAdapter()
: AbstractMessageAdapter(GCF::Message::ReleaseAccessRes)
{

}

ReleaseAccessResMessageAdapter::~ReleaseAccessResMessageAdapter()
{

}

void ReleaseAccessResMessageAdapter::readBody(QDomDocument doc, GCF::Message* message)
{
    GCF::ReleaseAccessResMessage* msg = (GCF::ReleaseAccessResMessage*)message;

    QDomElement rootE = doc.documentElement();

    QDomElement element = rootE.firstChildElement("Success");
    msg->setSuccess( QVariant(element.text()).toBool() );
}

void ReleaseAccessResMessageAdapter::writeBody(const GCF::Message* message, QDomDocument doc)
{
    const GCF::ReleaseAccessResMessage* msg = (const GCF::ReleaseAccessResMessage*)message;

    QDomElement rootE = doc.documentElement();

    QDomElement element = doc.createElement("Success");
    element.appendChild(doc.createTextNode(msg->isSuccess() ? "true" : "false"));
    rootE.appendChild(element);
}

GCF::Message* ReleaseAccessResMessageAdapter::createMessage()
{
    return new GCF::ReleaseAccessResMessage;
}

///////////////////////////////////////////////////////////////////////////////
// ReleaseConnectionMessageAdapter
///////////////////////////////////////////////////////////////////////////////

ReleaseConnectionMessageAdapter::ReleaseConnectionMessageAdapter()
: AbstractMessageAdapter(GCF::Message::ReleaseConnection)
{

}

ReleaseConnectionMessageAdapter::~ReleaseConnectionMessageAdapter()
{

}

void ReleaseConnectionMessageAdapter::readBody(QDomDocument doc, GCF::Message* message)
{
    GCF::ReleaseConnectionMessage* msg = (GCF::ReleaseConnectionMessage*)message;

    QDomElement rootE = doc.documentElement();

    QDomElement element = rootE.firstChildElement("ObjectId");
    msg->setObjectId( (qint32)element.text().toInt() );

    element = rootE.firstChildElement("Signal");
    msg->setSignal(element.text());
}

void ReleaseConnectionMessageAdapter::writeBody(const GCF::Message* message, QDomDocument doc)
{
    const GCF::ReleaseConnectionMessage* msg = (const GCF::ReleaseConnectionMessage*)message;

    QDomElement rootE = doc.documentElement();

    QDomElement element = doc.createElement("ObjectId");
    element.appendChild(doc.createTextNode(QString::number(msg->objectId())));
    rootE.appendChild(element);

    element = doc.createElement("Signal");
    element.appendChild(doc.createTextNode(msg->signal()));
    rootE.appendChild(element);
}

GCF::Message* ReleaseConnectionMessageAdapter::createMessage()
{
    return new GCF::ReleaseConnectionMessage;
}

///////////////////////////////////////////////////////////////////////////////
// ReleaseConnectionResMessageAdapter
///////////////////////////////////////////////////////////////////////////////

ReleaseConnectionResMessageAdapter::ReleaseConnectionResMessageAdapter()
: AbstractMessageAdapter(GCF::Message::ReleaseConnectionRes)
{

}

ReleaseConnectionResMessageAdapter::~ReleaseConnectionResMessageAdapter()
{

}

void ReleaseConnectionResMessageAdapter::readBody(QDomDocument doc, GCF::Message* message)
{
    GCF::ReleaseConnectionResMessage* msg = (GCF::ReleaseConnectionResMessage*)message;

    QDomElement rootE = doc.documentElement();

    QDomElement element = rootE.firstChildElement("ObjectId");
    msg->setObjectId( (qint32)element.text().toInt() );

    element = rootE.firstChildElement("Signal");
    msg->setSignal(element.text());

    element = rootE.firstChildElement("Success");
    msg->setSuccess( QVariant(element.text()).toBool() );
}

void ReleaseConnectionResMessageAdapter::writeBody(const GCF::Message* message, QDomDocument doc)
{
    const GCF::ReleaseConnectionResMessage* msg = (const GCF::ReleaseConnectionResMessage*)message;

    QDomElement rootE = doc.documentElement();

    QDomElement element = doc.createElement("ObjectId");
    element.appendChild(doc.createTextNode(QString::number(msg->objectId())));
    rootE.appendChild(element);

    element = doc.createElement("Signal");
    element.appendChild(doc.createTextNode(msg->signal()));
    rootE.appendChild(element);

    element = doc.createElement("Success");
    element.appendChild(doc.createTextNode(msg->isSuccess() ? "true" : "false"));
    rootE.appendChild(element);
}

GCF::Message* ReleaseConnectionResMessageAdapter::createMessage()
{
    return new GCF::ReleaseConnectionResMessage;
}

///////////////////////////////////////////////////////////////////////////////
// AsyncInvokeMethodMessageAdapter
///////////////////////////////////////////////////////////////////////////////

AsyncInvokeMethodMessageAdapter::AsyncInvokeMethodMessageAdapter()
: AbstractMessageAdapter(GCF::Message::AsyncInvokeMethod)
{

}

AsyncInvokeMethodMessageAdapter::~AsyncInvokeMethodMessageAdapter()
{

}

void AsyncInvokeMethodMessageAdapter::readBody(QDomDocument doc, GCF::Message* message)
{
    GCF::AsyncInvokeMethodMessage* msg = (GCF::AsyncInvokeMethodMessage*)message;

    QDomElement rootE = doc.documentElement();

    QDomElement element = rootE.firstChildElement("ObjectName");
    msg->setObjectName( element.text() );

    element = rootE.firstChildElement("MethodName");
    msg->setMethodName(element.text());

    element = rootE.firstChildElement("Args");
    element = element.firstChildElement("Variant");
    QVariantList args;
    while(!element.isNull())
    {
        QVariant arg = extractVariant(element);
        args << arg;

        element = element.nextSiblingElement("Variant");
    }
    msg->setArgList(args);
}

void AsyncInvokeMethodMessageAdapter::writeBody(const GCF::Message* message, QDomDocument doc)
{
    const GCF::AsyncInvokeMethodMessage* msg = (const GCF::AsyncInvokeMethodMessage*)message;

    QDomElement rootE = doc.documentElement();

    QDomElement element = doc.createElement("ObjectName");
    element.appendChild(doc.createTextNode(msg->objectName()));
    rootE.appendChild(element);

    element = doc.createElement("MethodName");
    element.appendChild(doc.createTextNode(msg->methodName()));
    rootE.appendChild(element);

    element = doc.createElement("Args");
    rootE.appendChild(element);

    QVariantList args = msg->argList();
    Q_FOREACH(QVariant arg, args)
    {
        QDomElement argE = encodeVariant(arg, doc);
        element.appendChild(argE);
    }
}

GCF::Message* AsyncInvokeMethodMessageAdapter::createMessage()
{
    return new GCF::AsyncInvokeMethodMessage;
}

///////////////////////////////////////////////////////////////////////////////
// AsyncInvokeMethodResMessageAdapter
///////////////////////////////////////////////////////////////////////////////

AsyncInvokeMethodResMessageAdapter::AsyncInvokeMethodResMessageAdapter()
: AbstractMessageAdapter(GCF::Message::AsyncInvokeMethodRes)
{

}

AsyncInvokeMethodResMessageAdapter::~AsyncInvokeMethodResMessageAdapter()
{

}

void AsyncInvokeMethodResMessageAdapter::readBody(QDomDocument doc, GCF::Message* message)
{
    GCF::AsyncInvokeMethodResMessage* msg = (GCF::AsyncInvokeMethodResMessage*)message;

    QDomElement rootE = doc.documentElement();

    QDomElement element = rootE.firstChildElement("ObjectName");
    msg->setObjectName( element.text() );

    element = rootE.firstChildElement("MethodName");
    msg->setMethodName(element.text());

    element = rootE.firstChildElement("Success");
    msg->setSuccess( QVariant(element.text()).toBool() );

    element = rootE.firstChildElement("Result");
    element = element.firstChildElement("Variant");
    QVariant result = extractVariant(element);
    msg->setRetValue(result);
}

void AsyncInvokeMethodResMessageAdapter::writeBody(const GCF::Message* message, QDomDocument doc)
{
    const GCF::AsyncInvokeMethodResMessage* msg = (const GCF::AsyncInvokeMethodResMessage*)message;

    QDomElement rootE = doc.documentElement();

    QDomElement element = doc.createElement("ObjectName");
    element.appendChild(doc.createTextNode(msg->objectName()));
    rootE.appendChild(element);

    element = doc.createElement("MethodName");
    element.appendChild(doc.createTextNode(msg->methodName()));
    rootE.appendChild(element);

    element = doc.createElement("Success");
    element.appendChild(doc.createTextNode(msg->isSuccess() ? "true" : "false"));
    rootE.appendChild(element);

    element = doc.createElement("Result");
    rootE.appendChild(element);

    QDomElement resultE = encodeVariant(msg->retValue(), doc);
    element.appendChild(resultE);
}

GCF::Message* AsyncInvokeMethodResMessageAdapter::createMessage()
{
    return new GCF::AsyncInvokeMethodResMessage;
}

////////////
// 123456
////////////

///////////////////////////////////////////////////////////////////////////////
// AsyncInvokeMethodsMessageAdapter
///////////////////////////////////////////////////////////////////////////////

AsyncInvokeMethodsMessageAdapter::AsyncInvokeMethodsMessageAdapter()
: AbstractMessageAdapter(GCF::Message::AsyncInvokeMethods)
{

}

AsyncInvokeMethodsMessageAdapter::~AsyncInvokeMethodsMessageAdapter()
{

}

void AsyncInvokeMethodsMessageAdapter::readBody(QDomDocument doc, GCF::Message* message)
{
    GCF::AsyncInvokeMethodsMessage* msg = (GCF::AsyncInvokeMethodsMessage*)message;
    QList<GCF::Method> methodList;
    QDomElement rootE = doc.documentElement();

    QDomElement methodsE = rootE.firstChildElement("InvokeMethods");
    QDomElement methodE = methodsE.firstChildElement("InvokeMethod");
    while( !methodE.isNull() )
    {
        GCF::Method method;
        method.ObjectName = methodE.firstChildElement("ObjectName").text();
        method.MethodName = methodE.firstChildElement("MethodName").text();

        QDomElement argsE = methodE.firstChildElement("Args");
        QDomElement argE = argsE.firstChildElement("Variant");
        while(!argE.isNull())
        {
            QVariant arg = extractVariant(argE);
            method.Args << arg;

            argE = argE.nextSiblingElement("Variant");
        }

        methodList.append(method);

        methodE = methodE.nextSiblingElement("InvokeMethod");
    }

    msg->setInvokeMethodList(methodList);
}

void AsyncInvokeMethodsMessageAdapter::writeBody(const GCF::Message* message, QDomDocument doc)
{
    const GCF::AsyncInvokeMethodsMessage* msg = (const GCF::AsyncInvokeMethodsMessage*)message;
    const QList<GCF::Method> methodList = msg->invokeMethodList();

    QDomElement rootE = doc.documentElement();

    QDomElement methodsE = doc.createElement("InvokeMethods");
    rootE.appendChild(methodsE);

    Q_FOREACH(GCF::Method method, methodList)
    {
        QDomElement methodE = doc.createElement("InvokeMethod");
        methodsE.appendChild(methodE);

        QDomElement objectNameE = doc.createElement("ObjectName");
        objectNameE.appendChild(doc.createTextNode(method.ObjectName));
        methodE.appendChild(objectNameE);

        QDomElement methodNameE = doc.createElement("MethodName");
        methodNameE.appendChild(doc.createTextNode(method.MethodName));
        methodE.appendChild(methodNameE);

        QDomElement argsE = doc.createElement("Args");
        methodE.appendChild(argsE);
        Q_FOREACH(QVariant arg, method.Args)
        {
            QDomElement argE = encodeVariant(arg, doc);
            argsE.appendChild(argE);
        }
    }
}

GCF::Message* AsyncInvokeMethodsMessageAdapter::createMessage()
{
    return new GCF::AsyncInvokeMethodsMessage;
}

///////////////////////////////////////////////////////////////////////////////
// AsyncInvokeMethodsResMessageAdapter
///////////////////////////////////////////////////////////////////////////////

AsyncInvokeMethodsResMessageAdapter::AsyncInvokeMethodsResMessageAdapter()
: AbstractMessageAdapter(GCF::Message::AsyncInvokeMethodsRes)
{

}

AsyncInvokeMethodsResMessageAdapter::~AsyncInvokeMethodsResMessageAdapter()
{

}

void AsyncInvokeMethodsResMessageAdapter::readBody(QDomDocument doc, GCF::Message* message)
{
    GCF::AsyncInvokeMethodsResMessage* msg = (GCF::AsyncInvokeMethodsResMessage*)message;
    QList<GCF::Method> methodList;
    QDomElement rootE = doc.documentElement();

    QDomElement methodsE = rootE.firstChildElement("InvokedMethods");
    QDomElement methodE = methodsE.firstChildElement("InvokedMethod");
    while( !methodE.isNull() )
    {
        GCF::Method method;
        method.ObjectName = methodE.firstChildElement("ObjectName").text();
        method.MethodName = methodE.firstChildElement("MethodName").text();
        method.Success = methodE.firstChildElement("Success").text() == "true";
        method.Result = extractVariant(methodE.firstChildElement("Result").firstChildElement("Variant"));

        methodList.append(method);

        methodE = methodE.nextSiblingElement("InvokedMethod");
    }

    msg->setInvokedMethodList(methodList);
}

void AsyncInvokeMethodsResMessageAdapter::writeBody(const GCF::Message* message, QDomDocument doc)
{
    const GCF::AsyncInvokeMethodsResMessage* msg = (const GCF::AsyncInvokeMethodsResMessage*)message;
    const QList<GCF::Method> methodList = msg->invokedMethodList();
    QDomElement rootE = doc.documentElement();

    QDomElement methodsE = doc.createElement("InvokedMethods");
    rootE.appendChild(methodsE);

    Q_FOREACH(GCF::Method method, methodList)
    {
        QDomElement methodE = doc.createElement("InvokedMethod");
        methodsE.appendChild(methodE);

        QDomElement objectNameE = doc.createElement("ObjectName");
        objectNameE.appendChild(doc.createTextNode(method.ObjectName));
        methodE.appendChild(objectNameE);

        QDomElement methodNameE = doc.createElement("MethodName");
        methodNameE.appendChild(doc.createTextNode(method.MethodName));
        methodE.appendChild(methodNameE);

        QDomElement successE = doc.createElement("Success");
        successE.appendChild(doc.createTextNode(method.Success ? "true" : "false"));
        methodE.appendChild(successE);

        QDomElement resultE = doc.createElement("Result");
        resultE.appendChild(encodeVariant(method.Result, doc));
        methodE.appendChild(resultE);
    }
}

GCF::Message* AsyncInvokeMethodsResMessageAdapter::createMessage()
{
    return new GCF::AsyncInvokeMethodsResMessage;
}
