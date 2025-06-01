#include "mongodb/mongoservice.h"
#include "mongodb/movement_rule/movement_rule.h"

#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/types.hpp>
#include <QDebug>

// Конструктор из BSON-документа
MovementRuleEntry::MovementRuleEntry(const bsoncxx::document::view& doc) {
    using namespace bsoncxx;

    if (auto elem = doc["_id"]; elem) {
        bsoncxx::oid id_tmp = elem.get_oid().value;
        id = idToQString(id_tmp);  // предполагается, что функция уже реализована
        qDebug() << "Rule ID:" << id;
    }

    // group_id (строка)
    if (auto elem = doc["group_id"]; elem) {
        qDebug() << "group_id: we are in";

        bsoncxx::oid id_tmp = elem.get_oid().value;
        group_id = idToQString(id_tmp);  // предполагается, что функция уже реализована
        // types::b_string str = elem.get_string();
        // qDebug() << "group_id: we are in 2 ";
        // group_id = QString::fromStdString(std::string(str.value));
        
        qDebug() << "group_id:" << group_id;
    }

    // sector_id (строка)
    if (auto elem = doc["sector_id"]; elem) {
        // types::b_string str = elem.get_string();
        // sector_id = QString::fromStdString(std::string(str.value));
        bsoncxx::oid id_tmp = elem.get_oid().value;
        sector_id = idToQString(id_tmp);  // предполагается, что функция уже реализована
        qDebug() << "sector_id:" << id;
    }

    if (auto elem = doc["access_type"]; elem) {
        access_type = elem.get_int32().value;
        qDebug() << "access_type:" << id;
    }

    // if (auto elem = doc["time_from"]; elem) {
    //     auto millis = elem.get_date().value.count();
    //     time_from = QDateTime::fromMSecsSinceEpoch(millis);
    //     qDebug() << "time_from:" << id;
    // }

    if (auto elem = doc["time_from"]; elem) {
        QString timeStr = QString::fromStdString(std::string(elem.get_string().value));
        time_from = QTime::fromString(timeStr, "HH:mm");
        if (!time_from.isValid()) {
            qWarning() << "Invalid time format for id:" << id << "time:" << timeStr;
            time_from = QTime(0, 0); // fallback to midnight
        }
        qDebug() << "time_from:" << time_from.toString("HH:mm") << "for id:" << id;
    }

    if (auto elem = doc["time_to"]; elem) {
        QString timeStr = QString::fromStdString(std::string(elem.get_string().value));
        time_to = QTime::fromString(timeStr, "HH:mm");
        if (!time_to.isValid()) {
            qWarning() << "Invalid time format for id:" << id << "time:" << timeStr;
            time_to = QTime(0, 0); // fallback to midnight
        }
        qDebug() << "time_to:" << time_to.toString("HH:mm") << "for id:" << id;
    }

    // if (auto elem = doc["time_to"]; elem) {
    //     auto millis = elem.get_date().value.count();
    //     time_to = QDateTime::fromMSecsSinceEpoch(millis);
    //     qDebug() << "time_to:" << id;
    // }

    if (auto elem = doc["reason"]; elem) {
        reason = QString::fromStdString(std::string(elem.get_string().value));
        qDebug() << "reason:" << id;
    }
}

// BSON-сериализация
bsoncxx::document::value MovementRuleEntry::toBson() const {
    using bsoncxx::builder::basic::kvp;
    using bsoncxx::builder::basic::make_document;

    return make_document(
        kvp("group_id", group_id.toStdString()),
        kvp("sector_id", sector_id.toStdString()),
        kvp("access_type", access_type),
        kvp("time_from", time_from.toString().toStdString()),
        kvp("time_to", time_to.toString().toStdString()),
        kvp("reason", reason.toStdString())
    );
}

// Загрузка из MongoDB
QList<MovementRuleEntry> loadMovementRuleEntryFromMongo() {
    QList<MovementRuleEntry> rules;

    mongocxx::client client{mongocxx::uri{}};
    auto db = client["lps"];
    auto collection = db["movement_rule"];

    auto cursor = collection.find({});
    for (const auto& doc : cursor) {
        MovementRuleEntry entry(doc);
        rules.append(entry);

        qDebug() << "[MovementRuleEntry]"
                 << "id =" << entry.id
                 << "group_id =" << entry.group_id
                 << "sector_id =" << entry.sector_id
                 << "access_type =" << entry.access_type
                 << "time_from =" << entry.time_from.toString()
                 << "time_to =" << entry.time_to.toString()
                 << "reason =" << entry.reason;
    }

    return rules;
}
