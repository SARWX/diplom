#ifndef TRILATERATOR_H
#define TRILATERATOR_H

#include <QObject>
#include<cmath>
#include "coordinates.h"
#include "avg_array.h"

#define POS_Y 1.0           // Задает координату якоря Y
#define POS_Z 1.0           // Задает координату якоря Z

#define SPEED_DELAY 0.1     // Задержка в измененении скорости

#define AVG_DIST_NUM 20
#define AVG_DIST_NUM_DIST 2

class Trilaterator : public QObject
{
    Q_OBJECT

private:
    coordinates cur_point;          // текущие координаты точки
    coordinates prev_point;         // координаты предыдущей точки
    AveragingArray avg_array{};     // специальный объект для усреднения измеренийы
    coordinates cur_dr;             // текущее перемещение
    double cur_speed;   // текущий вектор скорости
    // Для усреднения
//    double cur_r1 = 0.0;
//    double cur_r2 = 0.0;
//    double cur_r3 = 0.0;
//    int cur_num_of_mes = 0;


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
        // РАБОЧЯ ТЕМА, просто пока убрал
            avg_array.AddPoint(cur_point);
//        avg_array.AddPredictVector(cur_point);



//        delta += avg_array.GetAvg();


        // РАБОЧЯ ТЕМА, просто пока убрал
        // возвращаем усредненное значение
        cur_point = avg_array.GetAvg();
        return (cur_point);
    };

signals:
    void coordinateChanged(coordinates point, float r1, float r2, float r3, int point_nimber = 0);
    void avgDistanceReady(double r1, double r2, double r3, int point_nimber = 0);

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

        r1 += 0.04;
        r2 -= 0.13;
        r3 -= 0.18;

//        coordinates speed_vector = get_speed_vecor();
        emit coordinateChanged(trilaterate(r1, r2, r3), r1, r2, r3, point_nimber);
    }

    // Усреднение расстояний
    void averageDistance(double r1, double r2, double r3, int point_number) {
        static double cur_r1 = 0.0;
        static double cur_r2 = 0.0;
        static double cur_r3 = 0.0;
        static int cur_num_of_mes = 0;

        if ( ((r1 < 50.0) && (r2 < 50.0) && (r3 < 50.0))
                && ((r1 > 0.03) && (r2 > 0.03) && (r3 > 0.03)) ) {
            cur_r1 += r1;
            cur_r2 += r2;
            cur_r3 += r3;
            cur_num_of_mes += 1;
            if (cur_num_of_mes >= AVG_DIST_NUM_DIST) {
                emit avgDistanceReady((cur_r1 / cur_num_of_mes), (cur_r2 / cur_num_of_mes), (cur_r3 / cur_num_of_mes));
                cur_r1 = 0.0;
                cur_r2 = 0.0;
                cur_r3 = 0.0;
                cur_num_of_mes = 0;
            }
        }
    }

};

#endif // TRILATERATOR_H
