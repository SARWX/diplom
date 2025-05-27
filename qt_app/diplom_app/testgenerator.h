#ifndef TESTGENERATOR_H
#define TESTGENERATOR_H

#include <QObject>
#include <QTimer>
#include <QDebug>
#include<cmath>
#include "coordinates.h"

#define PLOT_MAX_X 5.0
#define PLOT_MAX_Y 2.0
#define PLOT_MAX_Z 5.0

#define PLOT_LEFT_X 0.1
#define PLOT_RIGHT_X 4.9
#define PLOT_LEFT_Y 0.1
#define PLOT_RIGHT_Y 2.0
#define PLOT_LEFT_Z 0.1
#define PLOT_RIGHT_Z 4.9

#define INHIBITOR 0.1
#define ERROR 0.2

#define POS_Y 1.0       // Задает координату якоря Y
#define POS_Z 1.0       // Задает координату якоря Z

enum GeneratorType
{
    CoordinatesGenerator,
    DistancesGenerator
};


class TestGenerator : public QObject {
    Q_OBJECT

private:
    QTimer timer;

public:
    TestGenerator(GeneratorType type, QObject *parent = nullptr) : QObject(parent) {
        if (type == CoordinatesGenerator)
        {
            connect(&timer, &QTimer::timeout, this, &TestGenerator::generateCoordinates);
        }
        else if (type == DistancesGenerator)
        {
            connect(&timer, &QTimer::timeout, this, &TestGenerator::generateDistances);
        }

//        timer.start(33); // Запускаем таймер, генерация каждые 33 миллисекунд примерно 30 кадров в секунду
    }

    int gen_sign_x = 1;
    int gen_sign_y = 1;
    int gen_sign_z = 1;
//    double x = 1.0;
//    double y = 1.0;
//    double z = 1.0;
    coordinates gp{1.0, 1.0, 1.0};       // generated point

signals:
    // Сигнал для передачи сгенерированных чисел
    void coordinatesGenerated(coordinates point, int point_number = 0);
    void distancesGenerated(double r1, double r2, double r3, int point_number = 0);

public slots:
    // Слот, который генерирует числа и отправляет сигнал
    void generateCoordinates() {

        // Сгенерировать X
        double dx = INHIBITOR * (double)std::rand() / RAND_MAX;
        if (gp.x < PLOT_LEFT_X)
            gen_sign_x = 1;
        else if (gp.x > PLOT_RIGHT_X)
            gen_sign_x = -1;
        gp.x = gp.x + gen_sign_x * dx;

        // Сгенерировать Y
        double dy = INHIBITOR * (double)std::rand() / RAND_MAX;
        if (gp.y < PLOT_LEFT_Y)
            gen_sign_y = 1;
        else if (gp.y > PLOT_RIGHT_Y)
            gen_sign_y = -1;
        gp.y = gp.y + gen_sign_y * dy;

        // Сгенерировать Z
        double dz = INHIBITOR * (double)std::rand() / RAND_MAX;
        if (gp.z < PLOT_LEFT_Z)
            gen_sign_z = 1;
        else if (gp.z > PLOT_RIGHT_Z)
            gen_sign_z = -1;
        gp.z = gp.z + gen_sign_z * dz;
//        qDebug() << "new coordinate: " << x << " ; " << y << " ; " << z;
        // Координаты точки
        gp.x = 0.5;
        gp.y = 0.4;
        gp.z = 0.5;
        emit coordinatesGenerated(gp);
    }

    void generateDistances() {
        double r1 = sqrt(3.0);          // X
        double r2 = sqrt(2.0);          // Y
        double r3 = sqrt(2.0);          // Z
        emit distancesGenerated(r1, r2, r3);
    }

     void coordinatesToDistances(coordinates point) {
        double r1 = sqrt(pow(point.x, 2) + pow(point.y, 2) + pow(point.z, 2));
        double r2 = sqrt(pow(point.x, 2) + pow(point.y - POS_Y, 2) + pow(point.z, 2));
        double r3 = sqrt(pow(point.x, 2) + pow(point.y, 2) + pow(point.z - POS_Z, 2));
        // добавим погрешности
        r1 += (((double)std::rand() / RAND_MAX) * (ERROR * 2) - ERROR) * 1;     // +/- 30 см. * 0.5
        r2 += (((double)std::rand() / RAND_MAX) * (ERROR * 2) - ERROR) * 1;
        r3 += (((double)std::rand() / RAND_MAX) * (ERROR * 2) - ERROR) * 1;
        emit distancesGenerated(r1, r2, r3, 1);
    }

     void stopGenerating() {
         if (timer.isActive()) {
             timer.stop();
             qDebug() << "Generation stopped.";
         } else {
             timer.start(33); // Запускаем таймер, генерация каждые 33 миллисекунды
             qDebug() << "Generation started.";
         }
     }

};

#endif // TESTGENERATOR_H
