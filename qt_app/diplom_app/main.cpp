#include <QApplication>
#include "mainwindow.h"
#include "datadisplayer.h"
#include "testgenerator.h"
#include "trilaterator.h"
#include "comander.h"
#include "SerialPortReader.h"
#include "rolewindow.h"
#include <QCoreApplication>
#include "mongodb/mongoservice.h"
#include <QTimer>

#include <iostream>

#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/json.hpp>

#include <mongocxx/client.hpp>
#include <mongocxx/exception/exception.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>

#include <chrono>

#include "globals.h"
DataDisplayer* g_dataDisplayer = nullptr;

int insertTestViolation();

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 1. Подключиться к б.д.
    MongoService mongo;

    const QString uri = "mongodb://localhost:27017";  // Подставь свой URI при необходимости
    const QString dbName = "testdb";

    if (!mongo.connectToDatabase(uri, dbName)) {
        qCritical() << "Не удалось подключиться к базе данных.";
        return -1;
    }

    // 1.5 ТЕСТЫ ДЛЯ БАЗЫ ДАННЫХ
    insertTestViolation();
    int ret = std::system("mongosh /home/tnovikov/study/diplom/qt_app/diplom_app/test_database.js");

    // 2. Определить роль пользователя
    RoleWindow roleWindow;
    if (roleWindow.exec() != QDialog::Accepted) {
        // Если пользователь не выбрал роль — выходим
        return 0;
    }

    // Если роль выбрана — продолжаем запуск приложения
    QString role = roleWindow.selectedRole();

    MainWindow w(role);

    // ВАЖНО: остальной код должен быть ТОЛЬКО здесь
    // Создаем объект dysplayer
    DataDisplayer dysplayer(w.plot1, w.plot2, w.plot3, w.coordinatesLabel);
    g_dataDisplayer = &dysplayer;
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


// int main(int argc, char *argv[])
// {
//     QCoreApplication a(argc, argv);

//     MongoService mongo;

//     const QString uri = "mongodb://localhost:27017";  // Подставь свой URI при необходимости
//     const QString dbName = "testdb";

//     if (!mongo.connectToDatabase(uri, dbName)) {
//         qCritical() << "Не удалось подключиться к базе данных.";
//         return -1;
//     }

//     // Таймер — чтобы попытки шли не в блокирующем цикле
//     QTimer *timer = new QTimer;
//     QObject::connect(timer, &QTimer::timeout, [&mongo, timer]() {
//         bool success = mongo.insertViolation("Тестовое нарушение", 3);
//         if (success) {
//             qInfo() << "Документ успешно вставлен.";
//             timer->stop();
//             QCoreApplication::quit();
//         } else {
//             qWarning() << "Ошибка вставки. Повтор...";
//         }
//     });

//     timer->start(1000);  // Повторять каждые 1000 мс

//     return a.exec();
// }


// ================ TEST ONLY =================== //
using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

int insertTestViolation() {
    try {
        mongocxx::instance instance{};
        mongocxx::uri uri("mongodb://localhost:27017");  // или твой URI
        mongocxx::client client(uri);

        auto database = client["lps"];
        auto collection = database["violation_log"];

        auto result = collection.insert_one(
            make_document(
                kvp("id", "test_violation_001"),
                kvp("object_id", "obj_123"),
                kvp("sector_id", "sector_A1"),
                kvp("movement_rule_id", "rule_99"),
                kvp("severity", 3),
                kvp("timestamp", bsoncxx::types::b_date(std::chrono::system_clock::now())),
                kvp("coords", make_document(
                                  kvp("x", 100.0),
                                  kvp("y", 200.0),
                                  kvp("z", 0.0)
                                  ))
                )
            );

        if (result) {
            std::cout << "Inserted document ID: "
                      << result->inserted_id().get_oid().value.to_string()
                      << std::endl;
        }

    } catch (const mongocxx::exception& e) {
        std::cout << "An exception occurred: " << e.what() << "\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
