#ifndef MENGER_SQUARE_FRACTAL_H
#define MENGER_SQUARE_FRACTAL_H

#include <QObject>
#include <QImage>

class MengerSquareFractal : public QObject
{
    Q_OBJECT

public:
    Q_INVOKABLE QImage generateFractal(int iterations, int imageSize);
};

#endif
