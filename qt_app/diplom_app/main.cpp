#include <QApplication>
#include "mainwindow.h"
#include "datadisplayer.h"
#include "testgenerator.h"
#include "trilaterator.h"
#include "comander.h"
#include "SerialPortReader.h"
#include "rolewindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    RoleWindow roleWindow;
    if (roleWindow.exec() != QDialog::Accepted) {
        // Если пользователь не выбрал роль — выходим
        return 0;
    }

    // Если роль выбрана — продолжаем запуск приложения
    QString role = roleWindow.selectedRole();

    MainWindow w;

    // ВАЖНО: остальной код должен быть ТОЛЬКО здесь
    // Создаем объект dysplayer
    DataDisplayer dysplayer(w.plot1, w.plot2, w.plot3, w.coordinatesLabel);
    Trilaterator trilaterator{};
    Commander commander(w.lineEdit, w.output_line, w.scrollArea);
    SerialPortReader port(&commander);
    TestGenerator generator(CoordinatesGenerator);

    // Задаем сигнал-слотовые связи
    QObject::connect(&generator, &TestGenerator::coordinatesGenerated, &generator, &TestGenerator::coordinatesToDistances);
    QObject::connect(&generator, &TestGenerator::distancesGenerated, &trilaterator, &Trilaterator::convertDistance);
    QObject::connect(&port, &SerialPortReader::newDataReceived, &trilaterator, &Trilaterator::averageDistance);
    QObject::connect(&trilaterator, &Trilaterator::avgDistanceReady, &trilaterator, &Trilaterator::convertDistance);
    QObject::connect(&trilaterator, &Trilaterator::coordinateChanged, &dysplayer, &DataDisplayer::coordinateChanged);
    QObject::connect(&port, &SerialPortReader::connectionResult, &commander, &Commander::displayConnectionStatus);
    QObject::connect(w.button1, &QPushButton::clicked, &dysplayer, &DataDisplayer::startTesting);
    QObject::connect(w.button2, &QPushButton::clicked, &dysplayer, &DataDisplayer::stopTesting);
    QObject::connect(w.button4, &QPushButton::clicked, &generator, &TestGenerator::stopGenerating);

    w.show();
    return a.exec();
}
