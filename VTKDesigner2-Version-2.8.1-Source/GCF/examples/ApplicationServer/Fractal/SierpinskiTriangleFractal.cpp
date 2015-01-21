#include "SierpinskiTriangleFractal.h"
#include <math.h>
#include <QPainter>

class Point : public QPointF
{
public:
    Point() : QPointF() { }
    Point(double x, double y) : QPointF(x, y) { }
    Point(const Point& other) : QPointF(other) { }

    Point midPointTo(const Point& other) const;
};

class Triangle
{
public:
    Triangle();
    Triangle(const Triangle& other);
    Triangle(const Point &_a, const Point& _b, const Point& _c);
    ~Triangle() { }

    Triangle& operator = (const Triangle& other);
    bool operator == (const Triangle& other) const;
    QPainterPath path() const;

public:
    Point a;
    Point b;
    Point c;
};

QList<Triangle> DivideSierpinski(const Triangle& tgl, int iterations=0)
{
    QList<Triangle> ret;

    if(iterations == 0)
    {
        ret.push_back(tgl);
        return ret;
    }

    // Sierpinski this triangle into three parts.
    Point d = tgl.a.midPointTo(tgl.b);
    Point e = tgl.b.midPointTo(tgl.c);
    Point f = tgl.c.midPointTo(tgl.a);

    Triangle t1(tgl.a, d, f);
    Triangle t2(d, tgl.b, e);
    Triangle t3(f, e, tgl.c);

    // Should we divide more?
    if(iterations <= 1)
    {
        // No we dont!
        ret.push_back(t1);
        ret.push_back(t2);
        ret.push_back(t3);

        return ret;
    }

    // Yes we do.
    ret += DivideSierpinski(t1, iterations-1);
    ret += DivideSierpinski(t2, iterations-1);
    ret += DivideSierpinski(t3, iterations-1);

    return ret;
}

QImage SierpinskiTriangleFractal::generateFractal(int iterations, int imageSize)
{
    if( imageSize%2 == 0)
        imageSize = imageSize+1;

    if(iterations < 0)
        imageSize = 0;
    else if(iterations >= 5)
        iterations = 5;

    Triangle tgl( Point(double(imageSize/2),0),
                  Point(0, imageSize), Point(imageSize, imageSize) );
    QList<Triangle> tglList = DivideSierpinski(tgl, iterations);
    QImage retImage(QSize(imageSize, imageSize), QImage::Format_ARGB32);

    QPainter paint(&retImage);
    paint.setRenderHint(QPainter::Antialiasing);
    paint.fillRect(retImage.rect(), Qt::white);
    paint.setBrush(Qt::blue);
    Q_FOREACH(Triangle tgl, tglList)
        paint.drawPath(tgl.path());
    paint.end();

    return retImage;
}

// Point class implementation
Point Point::midPointTo(const Point& other) const
{
    return Point( (x()+other.x())/2, (y()+other.y())/2 );
}

// Triangle class implementation
Triangle::Triangle(const Triangle& other)
{
    a = other.a;
    b = other.b;
    c = other.c;
}

Triangle::Triangle(const Point &_a, const Point& _b, const Point& _c)
{
    a = _a;
    b = _b;
    c = _c;
}

Triangle::Triangle()
{
    // We consider a simple triangle with 3 equal sides. Distance
    // from center of the triangle to any of the points is 1 units
    a = Point(0, 1);
    b = Point(cos( (90+120)*3.1415/180.0 ),
              sin( (90+120)*3.1415/180.0 ) );
    c = Point(cos( (90+240)*3.1415/180.0 ),
              sin( (90+240)*3.1415/180.0 ) );
}

Triangle& Triangle::operator = (const Triangle& other)
{
    a = other.a;
    b = other.b;
    c = other.c;

    return *this;
}

bool Triangle::operator == (const Triangle& other) const
{
    return (a==other.a) && (b==other.b) && (c==other.c);
}

QPainterPath Triangle::path() const
{
    QPainterPath path;
    path.moveTo(a);
    path.lineTo(b);
    path.lineTo(c);
    path.closeSubpath();

    return path;
}
