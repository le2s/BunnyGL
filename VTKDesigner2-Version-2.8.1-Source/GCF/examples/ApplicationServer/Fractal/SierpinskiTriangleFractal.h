#ifndef SIERPINSKITRIANGLEFRACTAL_H
#define SIERPINSKITRIANGLEFRACTAL_H

#include <QObject>
#include <QImage>

class SierpinskiTriangleFractal : public QObject
{
    Q_OBJECT

public:
    Q_INVOKABLE QImage generateFractal(int iterations, int imageSize);
};

#endif // SIERPINSKITRIANGLEFRACTAL_H
