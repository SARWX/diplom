#ifndef TRILATERATOR_H
#define TRILATERATOR_H

#include <QObject>
#include<cmath>

#define POS_Y 1.0       // Задает координату якоря Y
#define POS_Z 1.0       // Задает координату якоря Z

class Trilaterator : public QObject
{
    Q_OBJECT

private:
    double x;
    double y;
    double z;

public:
    Trilaterator() : x(0), y(0), z(0)
    {

    };

    // Метод трилатерации
    void trilaterate(double r1, double r2, double r3)
    {
        y = ((pow(r1, 2) - pow(r2, 2) + pow(POS_Y, 2)) / (2*POS_Y));
        z = ((pow(r1, 2) - pow(r3, 2) + pow(POS_Z, 2)) / (2*POS_Z));
        x = sqrt(pow(r1, 2) - pow(y, 2) - pow(z, 2));
    };

signals:
    void coordinateChanged(double tril_x, double tril_y, double tril_z);

public slots:
    // Слот, который генерирует числа и отправляет сигнал
    void convertDistance(double r1, double r2, double r3) {
        trilaterate(r1, r2, r3);
        emit coordinateChanged(x, y, z);
    }

};

#endif // TRILATERATOR_H
