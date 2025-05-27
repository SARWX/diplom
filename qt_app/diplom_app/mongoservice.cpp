#include "mongoservice.h"
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/types.hpp>
#include <chrono>
#include <QDebug>

MongoService::MongoService(QObject *parent)
    : QObject(parent), client(nullptr)
{
    static mongocxx::instance instance{}; // Required once per application
}

bool MongoService::connectToDatabase(const QString &uri, const QString &dbName)
{
    try {
        client = new mongocxx::client(mongocxx::uri{uri.toStdString()});
        db = client->database(dbName.toStdString());
        emit connectionSucceeded();
        return true;
    } catch (const std::exception &e) {
        emit connectionFailed(e.what());
        return false;
    }
}

std::vector<bsoncxx::document::value> MongoService::getViolations()
{
    std::vector<bsoncxx::document::value> result;
    auto collection = db["violations"];
    auto cursor = collection.find({});
    for (auto&& doc : cursor) {
        result.push_back(bsoncxx::document::value(doc));
    }
    return result;
}

bool MongoService::insertViolation(const QString &type, int severity)
{
    try {
        auto collection = db["violations"];

        // Создание BSON-документа через basic::document
        bsoncxx::builder::basic::document doc_builder;
        doc_builder.append(
            bsoncxx::builder::basic::kvp("type", type.toStdString()),
            bsoncxx::builder::basic::kvp("severity", severity),
            bsoncxx::builder::basic::kvp("timestamp", bsoncxx::types::b_date(std::chrono::system_clock::now()))
            );

        // Вставка
        collection.insert_one(doc_builder.view());

        return true;
    } catch (const std::exception &e) {
        qDebug() << "Insert failed:" << e.what();
        return false;
    }
}

