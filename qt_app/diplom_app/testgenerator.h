#ifndef TESTGENERATOR_H
#define TESTGENERATOR_H

#include <QObject>
#include <QTimer>
#include <QDebug>

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


class TestGenerator : public QObject {
    Q_OBJECT

private:
    QTimer timer;

public:
    TestGenerator(QObject *parent = nullptr) : QObject(parent) {
        connect(&timer, &QTimer::timeout, this, &TestGenerator::generateNumbers);
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
    void numbersGenerated(double x, double y, double z);

private slots:
    // Слот, который генерирует числа и отправляет сигнал
    void generateNumbers() {

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

        emit numbersGenerated(x, y, z);
    }
};

#endif // TESTGENERATOR_H
