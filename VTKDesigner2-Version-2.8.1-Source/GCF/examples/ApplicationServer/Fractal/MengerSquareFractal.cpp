#include "MengerSquareFractal.h"
#include <QPainter>

// This structure makes it easy to represent a square in terms of
// its center and edge size. Starting from there, we can have the
// square compute and return its rect for us
struct Square
{
    Square() : x(0), y(0), size(1000) { }
    Square(double _x, double _y, double _size)
        : x(_x), y(_y), size(_size) { }

    double x;
    double y;
    double size;

    QRectF rect() const {
        return QRectF(x-size/2, y-size/2, size, size);
    }
};

// This function (implemented later) recursively divides a single
// square \c nr_iterations number of times.
QList<Square> DivideSquare(const Square& s, int nr_iterations);

// This function first iterates a square of size \c imageSize, \c iterations
// number of times. It then paints the squares on to a QImage and
// returns the QImage. This function is a web-service methods!
QImage MengerSquareFractal::generateFractal(int iterations, int imageSize)
{
    if( imageSize%2 == 0)
        imageSize = imageSize+1;

    if(iterations < 0)
        imageSize = 0;
    else if(iterations >= 5)
        iterations = 5;

    Square sq( imageSize>>1, imageSize>>1, imageSize );
    QList<Square> sqList = DivideSquare(sq, iterations);

    QImage retImage(QSize(imageSize, imageSize), QImage::Format_ARGB32);

    QPainter paint(&retImage);
    paint.setRenderHint(QPainter::Antialiasing);
    paint.fillRect(retImage.rect(), Qt::white);
    Q_FOREACH(Square square, sqList)
        paint.fillRect(square.rect(), Qt::blue);
    paint.end();

    return retImage;
}

// Implementation of DivideSquare
QList<Square> DivideSquare(const Square& s, int iterations)
{
    QList<Square> ret;

    if(iterations <= 0)
    {
        // No more iterations to perform. Return
        // Square S as the divided square!
        ret.append( s );
        return ret;
    }

    // Perform division.
    double s2 = s.size/3;

    double x1 = s.x - s2;
    double x2 = s.x;
    double x3 = s.x + s2;

    double y1 = s.y - s2;
    double y2 = s.y;
    double y3 = s.y + s2;

    ret+= DivideSquare( Square( x1, y1, s2 ), iterations-1 );
    ret+= DivideSquare( Square( x2, y1, s2 ), iterations-1 );
    ret+= DivideSquare( Square( x3, y1, s2 ), iterations-1 );
    ret+= DivideSquare( Square( x1, y2, s2 ), iterations-1 );
    ret+= DivideSquare( Square( x3, y2, s2 ), iterations-1 );
    ret+= DivideSquare( Square( x1, y3, s2 ), iterations-1 );
    ret+= DivideSquare( Square( x2, y3, s2 ), iterations-1 );
    ret+= DivideSquare( Square( x3, y3, s2 ), iterations-1 );

    // Return the grand total list
    return ret;
}

