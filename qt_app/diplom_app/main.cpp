#include <QApplication>
#include "mainwindow.h"
#include "datadisplayer.h"
#include "testgenerator.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    // Создаем объект dysplayer
    DataDisplayer dysplayer(w.plot1, w.plot2, w.plot3);

    // Создаем тестовый генератор
    TestGenerator generator;

    QObject::connect(&generator, &TestGenerator::numbersGenerated, &dysplayer, &DataDisplayer::coordinateChanged);


    w.show();
    return a.exec();
}
