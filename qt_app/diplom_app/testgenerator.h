#ifndef TESTGENERATOR_H
#define TESTGENERATOR_H

#include <QObject>
#include <QTimer>
#include <QDebug>

//#define RAND_MAX 5

class TestGenerator : public QObject {
    Q_OBJECT

private:
    QTimer timer;

public:
    TestGenerator(QObject *parent = nullptr) : QObject(parent) {
        connect(&timer, &QTimer::timeout, this, &TestGenerator::generateNumbers);
        timer.start(500); // Запускаем таймер, генерация каждые 500 миллисекунд (половина секунды)
    }

signals:
    // Сигнал для передачи сгенерированных чисел
    void numbersGenerated(double x, double y, double z);

private slots:
    // Слот, который генерирует числа и отправляет сигнал
    void generateNumbers() {
        double x = (double)qrand() / RAND_MAX; // Генерация числа от 0 до 1
        double y = (double)qrand() / RAND_MAX;
        double z = (double)qrand() / RAND_MAX;
        qDebug() << "new coordinate: " << x << " ; " << y << " ; " << z;

        emit numbersGenerated(x, y, z);
    }
};

#endif // TESTGENERATOR_H
