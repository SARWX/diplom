#include <QApplication>
#include <QCoreApplication>
#include <QTimer>

#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/json.hpp>

#include <mongocxx/client.hpp>
#include <mongocxx/exception/exception.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>

#include <chrono>
#include <iostream>

#include "mainwindow.h"
#include "datadisplayer.h"
#include "testgenerator.h"
#include "trilaterator.h"
#include "comander.h"
#include "SerialPortReader.h"
#include "rolewindow.h"
#include "globals.h"
#include "mongodb/mongoservice.h"
#include "mongodb/trackerdb.h"



DataDisplayer* g_dataDisplayer = nullptr;
TrackerDB* g_trackerdb = nullptr;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 1. Подключиться к б.д.
    MongoService mongo;

    const QString uri = "mongodb://localhost:27017";  // Подставь свой URI при необходимости
    const QString dbName = "lps";

    if (!mongo.connectToDatabase(uri, dbName)) {
        qCritical() << "Не удалось подключиться к базе данных.";
        return -1;
    }

    // 1.5 ТЕСТЫ ДЛЯ БАЗЫ ДАННЫХ
    int ret = std::system("mongosh /home/tnovikov/study/diplom/qt_app/diplom_app/mongodb/test_database.js");

    // 2. Определить роль пользователя
    RoleWindow roleWindow;
    if (roleWindow.exec() != QDialog::Accepted) {
        // Если пользователь не выбрал роль — выходим
        return 0;
    }

    // Если роль выбрана — продолжаем запуск приложения
    QString role = roleWindow.selectedRole();

    MainWindow w(role, &mongo);

    // ВАЖНО: остальной код должен быть ТОЛЬКО здесь
    // Создаем объект dysplayer
    DataDisplayer dysplayer(w.plot1, w.plot2, w.plot3, w.coordinatesLabel);
    g_dataDisplayer = &dysplayer;
    Trilaterator trilaterator{};
    Commander commander(w.lineEdit, w.output_line, w.scrollArea);
    SerialPortReader port(&commander);
    TestGenerator generator(CoordinatesGenerator);
    TrackerDB trackerdb("obj_001");
    g_trackerdb = &trackerdb;

    // Задаем сигнал-слотовые связи
    QObject::connect(&generator, &TestGenerator::coordinatesGenerated, &generator, &TestGenerator::coordinatesToDistances);
    QObject::connect(&generator, &TestGenerator::distancesGenerated, &trilaterator, &Trilaterator::convertDistance);
    QObject::connect(&port, &SerialPortReader::newDataReceived, &trilaterator, &Trilaterator::averageDistance);
    QObject::connect(&trilaterator, &Trilaterator::avgDistanceReady, &trilaterator, &Trilaterator::convertDistance);
    QObject::connect(&trilaterator, &Trilaterator::coordinateChanged, &dysplayer, &DataDisplayer::coordinateChanged);
    // Сохраняем координаты в БД
    QObject::connect(&trilaterator, &Trilaterator::coordinateChanged, &trackerdb, &TrackerDB::coordinateChanged);
    // Отображать route на графиках
    QObject::connect(&trackerdb, &TrackerDB::routeCoordinateReady, &dysplayer, &DataDisplayer::displayRouteCoordinate);

    QObject::connect(&port, &SerialPortReader::connectionResult, &commander, &Commander::displayConnectionStatus);
    QObject::connect(w.button1, &QPushButton::clicked, &dysplayer, &DataDisplayer::startTesting);
    QObject::connect(w.button2, &QPushButton::clicked, &dysplayer, &DataDisplayer::stopTesting);
    QObject::connect(w.button4, &QPushButton::clicked, &generator, &TestGenerator::stopGenerating);

    w.show();
    return a.exec();
}
