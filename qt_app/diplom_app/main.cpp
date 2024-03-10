#include <QApplication>
#include "mainwindow.h"
#include "datadisplayer.h"
#include "testgenerator.h"
#include "trilaterator.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    // Создаем объект dysplayer
    DataDisplayer dysplayer(w.plot1, w.plot2, w.plot3);
    // Создадим объект trilaterator
    Trilaterator trilaterator{};

    // Создаем тестовый генератор
//    TestGenerator generator(CoordinatesGenerator);
    TestGenerator generator(DistancesGenerator);

    // Зададим сигнал-слотовые связи
//    QObject::connect(&generator, &TestGenerator::coordinatesGenerated, &dysplayer, &DataDisplayer::coordinateChanged);
    QObject::connect(&generator, &TestGenerator::distancesGenerated, &trilaterator, &Trilaterator::convertDistance);
    QObject::connect(&trilaterator, &Trilaterator::coordinateChanged, &dysplayer, &DataDisplayer::coordinateChanged);


    w.show();
    return a.exec();
}
