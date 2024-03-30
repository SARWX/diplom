#include <QApplication>
#include "mainwindow.h"
#include "datadisplayer.h"
#include "testgenerator.h"
#include "trilaterator.h"
#include "comander.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    // Создаем объект dysplayer
    DataDisplayer dysplayer(w.plot1, w.plot2, w.plot3);
    // Создадим объект trilaterator
    Trilaterator trilaterator{};
    // Создадим объект Comander
    Commander commander(w.lineEdit, w.output_line, w.scrollArea); // Инициализируем объект Commander, передавая указатель на lineEdit







    // ТЕСТИРОВАНИЕ


    // Создаем тестовый генератор
    TestGenerator generator(CoordinatesGenerator);

    // Зададим сигнал-слотовые связи
    // отображать сгенерированные КООРДИНАТЫ
    QObject::connect(&generator, &TestGenerator::coordinatesGenerated, &dysplayer, &DataDisplayer::coordinateChanged);
    // переводить координаты в РАССТОЯНИЯ
    QObject::connect(&generator, &TestGenerator::coordinatesGenerated, &generator, &TestGenerator::coordinatesToDistances);
    // переводить расстояния в КООРДИАНТЫ через ТРИЛАТЕРАТОР
    QObject::connect(&generator, &TestGenerator::distancesGenerated, &trilaterator, &Trilaterator::convertDistance);
    // отображать ТРИЛАТЕРИРОВАННЫЕ КООРДИНАТЫ
    QObject::connect(&trilaterator, &Trilaterator::coordinateChanged, &dysplayer, &DataDisplayer::coordinateChanged);


    w.show();
    return a.exec();
}
