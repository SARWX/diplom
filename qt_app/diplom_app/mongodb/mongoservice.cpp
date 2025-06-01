#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/types.hpp>
#include <chrono>
#include <QDebug>

#include <QObject>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/database.hpp>
#include <mongocxx/collection.hpp>

#include "mongodb/sector/sector.h"
#include "mongodb/mongoservice.h"
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/types.hpp>

#include <optional> // Что это????


#include "mongodb/mongoservice.h"
#include "mongodb/sector/sector.h"
#include "mongodb/object/object.h"
#include "mongodb/movement_rule/movement_rule.h"


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
        auto keyElement = doc[keyField.toStdString()];
        auto valueElement = doc[valueField.toStdString()];

        QString keyStr, valueStr;

        try {
            bsoncxx::types::b_string str = keyElement.get_string();
            keyStr = QString::fromStdString(std::string(str.value));
        } catch (...) {
            try {
                keyStr = idToQString(keyElement.get_oid().value);
            } catch (...) {
                qDebug() << "Unsupported keyField in doc:" << QString::fromStdString(bsoncxx::to_json(doc));
                continue;
            }
        }

        try {        
            bsoncxx::types::b_string str = valueElement.get_string();
            valueStr = QString::fromStdString(std::string(str.value));
        } catch (...) {
            try {
                valueStr = idToQString(valueElement.get_oid().value);
            } catch (...) {
                qDebug() << "Unsupported valueField in doc:" << QString::fromStdString(bsoncxx::to_json(doc));
                continue;
            }
        }

        resultMap.insert(keyStr, valueStr);
    }

    return resultMap;
}




bool MongoService::saveToMongo(const SectorEntry &entry)
{
    try {
        qDebug() << "[saveToMongo] Starting...";

        auto collection = db["sector"];
        qDebug() << "[saveToMongo] Using collection: 'sector'";

        // Преобразуем в BSON
        auto doc = entry.toBson();
        qDebug() << "[saveToMongo] BSON document:";
        qDebug().noquote() << QString::fromStdString(bsoncxx::to_json(doc.view()));

        if (!entry.id.isEmpty()) {
            // Если id есть — обновляем документ по _id
            bsoncxx::builder::basic::document filter;
            filter.append(bsoncxx::builder::basic::kvp("_id", bsoncxx::oid{entry.id.toStdString()}));

            mongocxx::options::replace opts;
            opts.upsert(false); // не вставляем, если не найден

            auto result = collection.replace_one(filter.view(), doc.view(), opts);

            if (result && result->modified_count() > 0) {
                qDebug() << "[saveToMongo] Document with _id" << entry.id << "updated.";
            } else {
                qDebug() << "[saveToMongo] No matching document found to update.";
                return false;
            }
        } else {
            // Если id нет — просто вставляем новый документ
            auto insert_result = collection.insert_one(doc.view());

            if (insert_result && insert_result->inserted_id().type() == bsoncxx::type::k_oid) {
                auto id = insert_result->inserted_id().get_oid().value.to_string();
                qDebug() << "[saveToMongo] New document inserted with _id:"
                         << QString::fromStdString(id);
            } else {
                qDebug() << "[saveToMongo] Insert failed.";
                return false;
            }
        }

        return true;

    } catch (const std::exception &e) {
        qDebug() << "[saveToMongo] Exception:" << e.what();
        return false;
    }
}

bool MongoService::saveToMongo(const ObjectEntry &entry)
{
    try {
        qDebug() << "[saveToMongo] Starting...";

        auto collection = db["object"];
        qDebug() << "[saveToMongo] Using collection: 'object'";

        // Преобразуем в BSON
        auto doc = entry.toBson();
        qDebug() << "[saveToMongo] BSON document:";
        qDebug().noquote() << QString::fromStdString(bsoncxx::to_json(doc.view()));

        if (!entry.id.isEmpty()) {
            // Если id есть — обновляем документ по _id
            bsoncxx::builder::basic::document filter;
            filter.append(bsoncxx::builder::basic::kvp("_id", bsoncxx::oid{entry.id.toStdString()}));

            mongocxx::options::replace opts;
            opts.upsert(false); // не вставляем, если не найден

            auto result = collection.replace_one(filter.view(), doc.view(), opts);

            if (result && result->modified_count() > 0) {
                qDebug() << "[saveToMongo] Document with _id" << entry.id << "updated.";
            } else {
                qDebug() << "[saveToMongo] No matching document found to update.";
                return false;
            }
        } else {
            // Если id нет — просто вставляем новый документ
            auto insert_result = collection.insert_one(doc.view());

            if (insert_result && insert_result->inserted_id().type() == bsoncxx::type::k_oid) {
                auto id = insert_result->inserted_id().get_oid().value.to_string();
                qDebug() << "[saveToMongo] New document inserted with _id:"
                         << QString::fromStdString(id);
            } else {
                qDebug() << "[saveToMongo] Insert failed.";
                return false;
            }
        }

        return true;

    } catch (const std::exception &e) {
        qDebug() << "[saveToMongo] Exception:" << e.what();
        return false;
    }
}

bool MongoService::saveToMongo(const MovementRuleEntry &entry)
{
    try {
        qDebug() << "[saveToMongo] Starting...";

        auto collection = db["movement_rule"];
        qDebug() << "[saveToMongo] Using collection: 'movement_rule'";

        // Преобразуем в BSON
        auto doc = entry.toBson();
        qDebug() << "[saveToMongo] BSON document:";
        qDebug().noquote() << QString::fromStdString(bsoncxx::to_json(doc.view()));

        if (!entry.id.isEmpty()) {
            // Если id есть — обновляем документ по _id
            bsoncxx::builder::basic::document filter;
            filter.append(bsoncxx::builder::basic::kvp("_id", bsoncxx::oid{entry.id.toStdString()}));

            mongocxx::options::replace opts;
            opts.upsert(false); // не вставляем, если не найден

            auto result = collection.replace_one(filter.view(), doc.view(), opts);

            if (result && result->modified_count() > 0) {
                qDebug() << "[saveToMongo] Document with _id" << entry.id << "updated.";
            } else {
                qDebug() << "[saveToMongo] No matching document found to update.";
                return false;
            }
        } else {
            // Если id нет — просто вставляем новый документ
            auto insert_result = collection.insert_one(doc.view());

            if (insert_result && insert_result->inserted_id().type() == bsoncxx::type::k_oid) {
                auto id = insert_result->inserted_id().get_oid().value.to_string();
                qDebug() << "[saveToMongo] New document inserted with _id:"
                         << QString::fromStdString(id);
            } else {
                qDebug() << "[saveToMongo] Insert failed.";
                return false;
            }
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


QString idToQString(const bsoncxx::oid &id) {
    return QString::fromStdString(id.to_string());
}

bsoncxx::oid stringToOid(const QString &str) {
    return bsoncxx::oid{str.toStdString()};
}
