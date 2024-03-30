#ifndef TRILATERATOR_H
#define TRILATERATOR_H

#include <QObject>
#include<cmath>
#include "coordinates.h"
#include "avg_array.h"

#define POS_Y 1.0           // Задает координату якоря Y
#define POS_Z 1.0           // Задает координату якоря Z

#define SPEED_DELAY 0.1     // Задержка в измененении скорости

class Trilaterator : public QObject
{
    Q_OBJECT

private:
    coordinates cur_point;          // текущие координаты точки
    coordinates prev_point;         // координаты предыдущей точки
    AveragingArray avg_array{};     // специальный объект для усреднения измеренийы
    coordinates cur_dr;             // текущее перемещение
    double cur_speed;   // текущий вектор скорости


public:
    Trilaterator() : cur_point{0.0, 0.0, 0.0}
    {

    };

    // Метод трилатерации
    coordinates trilaterate(double r1, double r2, double r3)
    {
        prev_point = cur_point;     // Запишем координаты предыдущей точки
        cur_point.y = ((pow(r1, 2) - pow(r2, 2) + pow(POS_Y, 2)) / (2*POS_Y));
        cur_point.z = ((pow(r1, 2) - pow(r3, 2) + pow(POS_Z, 2)) / (2*POS_Z));
        cur_point.x = sqrt(abs(pow(r1, 2) - pow(cur_point.y, 2) - pow(cur_point.z, 2)));
        // занесем полученные координаты в массив для усреднения
//        coordinates delta = avg_array.GetAvg() / (-1.0);
        avg_array.AddPoint(cur_point);
        avg_array.AddPredictVector(cur_point);
//        delta += avg_array.GetAvg();
        // возвращаем усредненное значение
        cur_point = avg_array.GetAvg();
        return (cur_point);
    };

signals:
    void coordinateChanged(coordinates point, int point_nimber = 0);

public slots:
    // Слот, который генерирует числа и отправляет сигнал
    void convertDistance(double r1, double r2, double r3, int point_nimber = 0) {
//        trilaterate(r1, r2, r3);                        // производим трилатерацию с усреднением
//        coordinates dr = cur_point - prev_point;        // получаем перемещение dr
//        coordinates dr_real = dr                        // сохраним реальное dr
//        dr = project_vector(cur_dr, dr);                // спроецировать вектор dr на cur_dr
//        normalize_vector(&dr);                          // нормируем вектор
//        dr = cur_speed * dr;                            // зададим итоговое перемещение
//        double speed_dif = to_dist(dr_real - dr);       // длина ветора С УЧЕТОМ ЗНАКА + не успевает - опережает
//        speed += SPEED_DELAY * speed_dif;



//        coordinates speed_vector = get_speed_vecor();
        emit coordinateChanged(trilaterate(r1, r2, r3), point_nimber);
    }

};

#endif // TRILATERATOR_H
