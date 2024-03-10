#ifndef TRILATERATOR_H
#define TRILATERATOR_H

#include <QObject>
#include<cmath>
#include "coordinates.h"
#include "avg_array.h"

#define POS_Y 1.0           // Задает координату якоря Y
#define POS_Z 1.0           // Задает координату якоря Z

class Trilaterator : public QObject
{
    Q_OBJECT

private:
    coordinates cur_point;
    int iterator;
    AveragingArray avg_array{};

public:
    Trilaterator() : cur_point{0.0, 0.0, 0.0}
    {

    };

    // Метод трилатерации
    coordinates trilaterate(double r1, double r2, double r3)
    {
        cur_point.y = ((pow(r1, 2) - pow(r2, 2) + pow(POS_Y, 2)) / (2*POS_Y));
        cur_point.z = ((pow(r1, 2) - pow(r3, 2) + pow(POS_Z, 2)) / (2*POS_Z));
        cur_point.x = sqrt(abs(pow(r1, 2) - pow(cur_point.y, 2) - pow(cur_point.z, 2)));
        // занесем полученные координаты в массив для усреднения
//        coordinates delta = avg_array.GetAvg() / (-1.0);
        avg_array.AddPoint(cur_point);
        avg_array.AddPredictVector(cur_point);
//        delta += avg_array.GetAvg();
        // возвращаем усредненное значение
        return (avg_array.GetAvg() /*+ avg_array.GetPredictVector()*/);
    };

signals:
    void coordinateChanged(coordinates point, int point_nimber = 0);

public slots:
    // Слот, который генерирует числа и отправляет сигнал
    void convertDistance(double r1, double r2, double r3, int point_nimber = 0) {
        emit coordinateChanged(trilaterate(r1, r2, r3), point_nimber);
    }

};

#endif // TRILATERATOR_H
