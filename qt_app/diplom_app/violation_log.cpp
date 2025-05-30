#include "violation_log.h"


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

// Функция 1: Только загрузка данных из MongoDB
// QList<ViolationLogEntry> loadViolationsFromMongo()
// {
//     QList<ViolationLogEntry> violations;

//     mongocxx::client client{mongocxx::uri{}};
//     auto db = client["lps"];    // lps = local positioning system
//     auto collection = db["violation_log"];

//     auto cursor = collection.find({});
//     for (const auto &doc : cursor) {
//         violations.append(ViolationLogEntry(doc));
//     }

//     return violations;
// }

QList<ViolationLogEntry> loadViolationsFromMongo(
    const QDateTime& startTime,
    const QDateTime& endTime,
    const QString& sectorId)
{
    QList<ViolationLogEntry> violations;

    mongocxx::client client{mongocxx::uri{}};
    auto db = client["lps"];
    auto collection = db["violation_log"];

    bsoncxx::builder::basic::array condition_array;
    bool hasConditions = false;

    if (startTime.isValid() && endTime.isValid()) {
        condition_array.append(bsoncxx::builder::basic::make_document(
            bsoncxx::builder::basic::kvp("timestamp",
                                         bsoncxx::builder::basic::make_document(
                                             bsoncxx::builder::basic::kvp("$gte", bsoncxx::types::b_date{std::chrono::milliseconds{startTime.toMSecsSinceEpoch()}}),
                                             bsoncxx::builder::basic::kvp("$lte", bsoncxx::types::b_date{std::chrono::milliseconds{endTime.toMSecsSinceEpoch()}})
                                             )
                                         )
            ));
        hasConditions = true;
    }

    if (!sectorId.isEmpty()) {
        condition_array.append(bsoncxx::builder::basic::make_document(
            bsoncxx::builder::basic::kvp("sector_id", sectorId.toStdString())
            ));
        hasConditions = true;
    }

    mongocxx::options::find opts{};
    opts.sort(bsoncxx::builder::basic::make_document(
        bsoncxx::builder::basic::kvp("timestamp", -1)));

    // Теперь создаём курсор сразу, без предварительного объявления
    auto cursor = hasConditions
                      ? collection.find(
                            bsoncxx::builder::basic::make_document(
                                bsoncxx::builder::basic::kvp("$and", condition_array)
                                ).view(),
                            opts
                            )
                      : collection.find({}, opts);

    for (const auto& doc : cursor) {
        violations.append(ViolationLogEntry(doc));
    }

    return violations;
}

