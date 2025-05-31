#include "mongodb/sector/sector.h"
#include "mongodb/mongoservice.h"

SectorEntry::SectorEntry(const bsoncxx::document::view& doc) {
    using namespace bsoncxx;

    // Строка "id"
    if (auto elem = doc["_id"]; elem) {
        types::b_string str = elem.get_string();
        id = QString::fromStdString(std::string(str.value));
    }

    // Строка "name"
    if (auto elem = doc["name"]; elem) {
        types::b_string str = elem.get_string();
        name = QString::fromStdString(std::string(str.value));
    }

    // Строка "type"
    if (auto elem = doc["type"]; elem) {
        types::b_string str = elem.get_string();
        type = QString::fromStdString(std::string(str.value));
    }

    // Число "floor_number"
    if (auto elem = doc["floor_number"]; elem) {
        types::b_int32 val = elem.get_int32();
        floor_number = val.value;
    }

    // Строка "building_name"
    if (auto elem = doc["building_name"]; elem) {
        types::b_string str = elem.get_string();
        building_name = QString::fromStdString(std::string(str.value));
    }

    // Координаты: "geometry" должен быть вложенным документом с "x", "y", "z"
    if (auto elem = doc["geometry"]; elem && elem.type() == bsoncxx::type::k_document) {
        auto geometry_doc = elem.get_document().view();

        // 1 - start point
        if (auto elem = geometry_doc["start"]; elem && elem.type() == bsoncxx::type::k_document) {
            parse_coords_from_document(elem.get_document().view(), (geometry + 0));
        }

        // 2 - size
        if (auto elem = geometry_doc["size"]; elem && elem.type() == bsoncxx::type::k_document) {
            parse_coords_from_document(elem.get_document().view(), (geometry + 1));
        }
    }
}

// Функция 1: Только загрузка данных из MongoDB
QList<SectorEntry> loadSectorEntryFromMongo()
{
    QList<SectorEntry> sectors;

    mongocxx::client client{mongocxx::uri{}};
    auto db = client["lps"];    // lps = local positioning system
    auto collection = db["sector"];

    auto cursor = collection.find({});
    for (const auto &doc : cursor) {
        SectorEntry entry(doc);
        sectors.append(entry);

        qDebug() << "SectorEntry:"
                << "id =" << entry.id
                << "name =" << entry.name
                << "geometry start =" << entry.geometry[0].x
                << "geometry size =" << entry.geometry[1].x;
    }

    return sectors;
}
