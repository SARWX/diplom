#ifndef TESTGENERATOR_H
#define TESTGENERATOR_H

#include <QObject>
#include <QTimer>
#include <QDebug>
#include<cmath>

#define PLOT_MAX_X 5.0
#define PLOT_MAX_Y 2.0
#define PLOT_MAX_Z 5.0

#define PLOT_LEFT_X 0.1
#define PLOT_RIGHT_X 4.9
#define PLOT_LEFT_Y 0.1
#define PLOT_RIGHT_Y 2.0
#define PLOT_LEFT_Z 0.1
#define PLOT_RIGHT_Z 4.9

#define INHIBITOR 0.01

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

        timer.start(33); // Запускаем таймер, генерация каждые 33 миллисекунд примерно 30 кадров в секунду
    }

    int gen_sign_x = 1;
    int gen_sign_y = 1;
    int gen_sign_z = 1;
    double x = 1.0;
    double y = 1.0;
    double z = 1.0;

signals:
    // Сигнал для передачи сгенерированных чисел
    void coordinatesGenerated(double x, double y, double z);
    void distancesGenerated(double r1, double r2, double r3);

private slots:
    // Слот, который генерирует числа и отправляет сигнал
    void generateCoordinates() {

        // Сгенерировать X
        double dx = INHIBITOR * (double)qrand() / RAND_MAX;
        if (x < PLOT_LEFT_X)
            gen_sign_x = 1;
        else if (x > PLOT_RIGHT_X)
            gen_sign_x = -1;
        x = x + gen_sign_x * dx;

        // Сгенерировать Y
        double dy = INHIBITOR * (double)qrand() / RAND_MAX;
        if (y < PLOT_LEFT_Y)
            gen_sign_y = 1;
        else if (y > PLOT_RIGHT_Y)
            gen_sign_y = -1;
        y = y + gen_sign_y * dy;

        // Сгенерировать Z
        double dz = INHIBITOR * (double)qrand() / RAND_MAX;
        if (z < PLOT_LEFT_Z)
            gen_sign_z = 1;
        else if (z > PLOT_RIGHT_Z)
            gen_sign_z = -1;
        z = z + gen_sign_z * dz;
//        qDebug() << "new coordinate: " << x << " ; " << y << " ; " << z;

        emit coordinatesGenerated(x, y, z);
    }

    void generateDistances() {
        double r1 = sqrt(3.0);          // X
        double r2 = sqrt(2.0);          // Y
        double r3 = sqrt(2.0);          // Z
        emit distancesGenerated(r1, r2, r3);
    }
};

#endif // TESTGENERATOR_H
