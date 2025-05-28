#ifndef VIOLATION_LOG_H
#define VIOLATION_LOG_H
// Qt
#include <QString>
#include <QDateTime>
#include <QList>

// MongoDB BSON
#include <bsoncxx/document/view.hpp>
#include <bsoncxx/types.hpp>
#include <bsoncxx/types/bson_value/view.hpp>

// MongoDB C++ Driver
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/database.hpp>
#include <mongocxx/collection.hpp>
#include <mongocxx/cursor.hpp>

#include <bsoncxx/document/view.hpp>
#include <bsoncxx/document/element.hpp>
#include <bsoncxx/types.hpp>
#include <QString>

// структура координат
#include "coordinates.h"


class ViolationLogEntry {
public:
    QString id;
    QString object_id;
    QString sector_id;
    QString movement_rule_id;
    int severity;
    QDateTime timestamp;
    coordinates coords;

    // Конструктор из BSON-документа
    ViolationLogEntry(const bsoncxx::document::view &doc);
};

ViolationLogEntry::ViolationLogEntry(const bsoncxx::document::view& doc) {
    using namespace bsoncxx;

    // Строка "id"
    if (auto elem = doc["id"]; elem) {
        types::b_string str = elem.get_string();
        id = QString::fromStdString(std::string(str.value));
    }

    // Строка "object_id"
    if (auto elem = doc["object_id"]; elem) {
        types::b_string str = elem.get_string();
        object_id = QString::fromStdString(std::string(str.value));
    }

    // Строка "sector_id"
    if (auto elem = doc["sector_id"]; elem) {
        types::b_string str = elem.get_string();
        sector_id = QString::fromStdString(std::string(str.value));
    }

    // Строка "movement_rule_id"
    if (auto elem = doc["movement_rule_id"]; elem) {
        types::b_string str = elem.get_string();
        movement_rule_id = QString::fromStdString(std::string(str.value));
    }

    // Число "severity"
    if (auto elem = doc["severity"]; elem) {
        types::b_int32 val = elem.get_int32();
        severity = val.value;
    }

    // Дата/время "timestamp"
    if (auto elem = doc["timestamp"]; elem) {
        types::b_date dt = elem.get_date();
        timestamp = QDateTime::fromMSecsSinceEpoch(dt.value.count());  // <-- исправлено
    }

    // Координаты: "coords" должен быть вложенным документом с "x", "y", "z"
    if (auto elem = doc["coords"]; elem && elem.type() == bsoncxx::type::k_document) {
        auto coord_doc = elem.get_document().view();

        if (auto x_elem = coord_doc["x"]; x_elem && x_elem.type() == bsoncxx::type::k_double) {
            coords.x = x_elem.get_double().value;
        }

        if (auto y_elem = coord_doc["y"]; y_elem && y_elem.type() == bsoncxx::type::k_double) {
            coords.y = y_elem.get_double().value;
        }

        if (auto z_elem = coord_doc["z"]; z_elem && z_elem.type() == bsoncxx::type::k_double) {
            coords.z = z_elem.get_double().value;
        }
    }
}

QList<ViolationLogEntry> loadViolationsFromMongo()
{
    QList<ViolationLogEntry> violations;

    mongocxx::client client{mongocxx::uri{}};
    auto db = client["lps"];    // lps = local positioning system
    auto collection = db["violation_log"];

    auto cursor = collection.find({});
    for (const auto &doc : cursor) {
        violations.append(ViolationLogEntry(doc));
    }

    return violations;
}

#endif // VIOLATION_LOG_H
