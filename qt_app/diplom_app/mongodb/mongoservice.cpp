#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/types.hpp>
#include <chrono>
#include <QDebug>

#include "mongodb/mongoservice.h"
#include "mongodb/sector/sector.h"

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

bool MongoService::saveToMongo(const SectorEntry &entry)
{
    try {
        qDebug() << "[saveToMongo] Starting...";

        // Проверим имя БД
        qDebug() << "[saveToMongo] Database name:" << QString::fromStdString(db.name().data());

        // Получаем коллекцию
        auto collection = db["sector"];
        qDebug() << "[saveToMongo] Using collection: 'sector'";

        // Преобразуем в BSON
        auto doc = entry.toBson();
        qDebug() << "[saveToMongo] BSON document:";
        qDebug().noquote() << QString::fromStdString(bsoncxx::to_json(doc.view()));

        // Фильтр
        bsoncxx::builder::basic::document filter;
        filter.append(
            bsoncxx::builder::basic::kvp("name", entry.name.toStdString()),
            bsoncxx::builder::basic::kvp("building_name", entry.building_name.toStdString())
        );

        qDebug().noquote() << "[saveToMongo] Filter:"
                           << QString::fromStdString(bsoncxx::to_json(filter.view()));

        // upsert
        mongocxx::options::replace opts;
        opts.upsert(true);

        // Выполняем
        auto result = collection.replace_one(filter.view(), doc.view(), opts);

        if (result) {
            if (result->modified_count() > 0) {
                qDebug() << "[saveToMongo] Existing document modified.";
            } else if (result->upserted_id()) {
                auto id = result->upserted_id()->get_oid().value.to_string();
                qDebug() << "[saveToMongo] New document inserted with _id:"
                         << QString::fromStdString(id);
            } else {
                qDebug() << "[saveToMongo] No changes made.";
            }
        } else {
            qDebug() << "[saveToMongo] replace_one returned std::nullopt.";
        }

        return true;
    } catch (const std::exception &e) {
        qDebug() << "[saveToMongo] Exception:" << e.what();
        return false;
    }
}




void parse_coords_from_document(const bsoncxx::document::view& doc, coordinates* coords) {
    if (auto x_elem = doc["x"]) {
        if (x_elem.type() == bsoncxx::type::k_double)
            coords->x = x_elem.get_double().value;
        else if (x_elem.type() == bsoncxx::type::k_int32)
            coords->x = static_cast<double>(x_elem.get_int32().value);
        else if (x_elem.type() == bsoncxx::type::k_int64)
            coords->x = static_cast<double>(x_elem.get_int64().value);
    }

    if (auto y_elem = doc["y"]) {
        if (y_elem.type() == bsoncxx::type::k_double)
            coords->y = y_elem.get_double().value;
        else if (y_elem.type() == bsoncxx::type::k_int32)
            coords->y = static_cast<double>(y_elem.get_int32().value);
        else if (y_elem.type() == bsoncxx::type::k_int64)
            coords->y = static_cast<double>(y_elem.get_int64().value);
    }

    if (auto z_elem = doc["z"]) {
        if (z_elem.type() == bsoncxx::type::k_double)
            coords->z = z_elem.get_double().value;
        else if (z_elem.type() == bsoncxx::type::k_int32)
            coords->z = static_cast<double>(z_elem.get_int32().value);
        else if (z_elem.type() == bsoncxx::type::k_int64)
            coords->z = static_cast<double>(z_elem.get_int64().value);
    }
}

