#include "violation_log.h"
#include "datadisplayer.h"
#include "globals.h"

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
        ViolationLogEntry violation = ViolationLogEntry(doc);
        violations.append(violation);
        if (g_dataDisplayer) {
            // Генерация point_number из любой строки id
            uint point_number = qHash(violation.id) % 1000 + 1000;
            qDebug() << "point_number: " << point_number;

            // Передаем в дисплей
            g_dataDisplayer->coordinateChanged(violation.coords, 0, 0, 0, point_number);
        }
    }

    return violations;
}
