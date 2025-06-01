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

QString get_test_object();

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
    TrackerDB trackerdb(get_test_object());
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


QString get_test_object()
{
    try {
        // Подключение к MongoDB
        mongocxx::client client{mongocxx::uri{}};
        mongocxx::database db = client["lps"];
        mongocxx::collection collection = db["object"];

        // Формируем фильтр
        bsoncxx::builder::basic::document filter;
        filter.append(bsoncxx::builder::basic::kvp("owner_name", "Иванов С.А."));

        // Выполняем запрос
        auto maybe_doc = collection.find_one(filter.view());

        if (maybe_doc) {
            auto doc = maybe_doc->view();

            if (doc["_id"] && doc["_id"].type() == bsoncxx::type::k_oid) {
                std::string id_str = doc["_id"].get_oid().value.to_string();
                qDebug() << "[get_test_object] Found _id:" << QString::fromStdString(id_str);
                return QString::fromStdString(id_str);
            } else {
                qDebug() << "[get_test_object] _id missing or not ObjectId";
                return QString();
            }
        } else {
            qDebug() << "[get_test_object] No document with owner_name = 'Иванов С.А.'";
            return QString();
        }

    } catch (const std::exception &e) {
        qDebug() << "[get_test_object] Exception:" << e.what();
        return QString();
    }
}

