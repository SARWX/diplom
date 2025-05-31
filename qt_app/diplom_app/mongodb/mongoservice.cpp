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

QMap<QString, QString> MongoService::getMongoFieldMap(
    const QString& collectionName, 
    const QString& keyField, 
    const QString& valueField)
{
    QMap<QString, QString> resultMap;

    mongocxx::client client{mongocxx::uri{}};
    auto db = client["lps"];
    auto collection = db[collectionName.toStdString()];

    auto cursor = collection.find({});

    for (const auto& doc : cursor) {
        if (doc[keyField.toStdString()] && doc[valueField.toStdString()]) {
            bsoncxx::types::b_string keyStr = doc[keyField.toStdString()].get_string();
            bsoncxx::types::b_string valueStr = doc[valueField.toStdString()].get_string();
            
            resultMap.insert(
                QString::fromStdString(std::string(keyStr.value)),
                QString::fromStdString(std::string(valueStr.value))
            );
        }
    }

    return resultMap;
}

void parse_coords_from_document(const bsoncxx::document::view& doc, coordinates* coords) {
    if (auto x_elem = doc["x"]; x_elem && x_elem.type() == bsoncxx::type::k_double) {
        coords->x = x_elem.get_double().value;
    }

    if (auto y_elem = doc["y"]; y_elem && y_elem.type() == bsoncxx::type::k_double) {
        coords->y = y_elem.get_double().value;
    }

    if (auto z_elem = doc["z"]; z_elem && z_elem.type() == bsoncxx::type::k_double) {
        coords->z = z_elem.get_double().value;
    }
}