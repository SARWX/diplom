#include "mongodb/violation_log/violation_log.h"
#include "mongodb/mongoservice.h"

ViolationLogEntry::ViolationLogEntry(const bsoncxx::document::view& doc) {
    using namespace bsoncxx;

    // Строка "id"
    if (auto elem = doc["_id"]; elem) {
        bsoncxx::oid id_tmp = elem.get_oid().value;
        id = idToQString(id_tmp);
        qDebug() << "Document ID is:" << id;
    }

    // Строка "object_id"
    if (auto elem = doc["object_id"]; elem) {
        bsoncxx::oid id_tmp = elem.get_oid().value;
        object_id = idToQString(id_tmp);
        qDebug() << "Document ID is:" << object_id;
    }

    // Строка "sector_id"
    if (auto elem = doc["sector_id"]; elem) {
        bsoncxx::oid id_tmp = elem.get_oid().value;
        sector_id = idToQString(id_tmp);
        qDebug() << "Document ID is:" << sector_id;
    }

    // Строка "movement_rule_id"
    if (auto elem = doc["movement_rule_id"]; elem) {
        bsoncxx::oid id_tmp = elem.get_oid().value;
        movement_rule_id = idToQString(id_tmp);
        qDebug() << "Document ID is:" << movement_rule_id;
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
        parse_coords_from_document(elem.get_document().view(), &coords);
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
                                         ))
        ));
        hasConditions = true;
    }

    if (!sectorId.isEmpty()) {
        try {
            bsoncxx::oid realOid = stringToOid(sectorId);
            condition_array.append(bsoncxx::builder::basic::make_document(
                bsoncxx::builder::basic::kvp("sector_id", realOid)
            ));
            hasConditions = true;
        } catch (const std::exception& e) {
            qDebug() << "Invalid sectorId format for OID:" << sectorId << "Error:" << e.what();
            return violations;  // можно также продолжить без сектора, если нужно
        }
    }

    mongocxx::options::find opts{};
    opts.sort(bsoncxx::builder::basic::make_document(
        bsoncxx::builder::basic::kvp("timestamp", -1)));

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


