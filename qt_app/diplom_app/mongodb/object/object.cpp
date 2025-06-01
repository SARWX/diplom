#include "mongodb/object/object.h"
#include "mongodb/mongoservice.h"
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/types.hpp>

// Конструктор из BSON-документа
ObjectEntry::ObjectEntry(const bsoncxx::document::view& doc) {
    using namespace bsoncxx;

    // _id (ObjectId) → строка
    if (auto elem = doc["_id"]; elem) {
        bsoncxx::oid id_tmp = elem.get_oid().value;
        id = idToQString(id_tmp);  // предполагается, что у тебя есть idToQString()
        qDebug() << "Object ID is:" << id;
    }

    // group_id (строка)
    if (auto elem = doc["group_id"]; elem) {
        types::b_string str = elem.get_string();
        group_id = QString::fromStdString(std::string(str.value));
    }

    // type (строка)
    if (auto elem = doc["type"]; elem) {
        types::b_string str = elem.get_string();
        type = QString::fromStdString(std::string(str.value));
    }

    // owner_name (строка)
    if (auto elem = doc["owner_name"]; elem) {
        types::b_string str = elem.get_string();
        owner_name = QString::fromStdString(std::string(str.value));
    }

    // device_uid (строка)
    if (auto elem = doc["device_uid"]; elem) {
        types::b_string str = elem.get_string();
        device_uid = QString::fromStdString(std::string(str.value));
    }
}

// Функция загрузки всех объектов из MongoDB
QList<ObjectEntry> loadObjectEntryFromMongo()
{
    QList<ObjectEntry> objects;

    mongocxx::client client{mongocxx::uri{}};
    auto db = client["lps"];
    auto collection = db["object"];

    auto cursor = collection.find({});
    for (const auto& doc : cursor) {
        ObjectEntry entry(doc);
        objects.append(entry);

        qDebug() << "[ObjectEntry]"
                 << "id =" << entry.id
                 << "group_id =" << entry.group_id
                 << "type =" << entry.type
                 << "owner_name =" << entry.owner_name
                 << "device_uid =" << entry.device_uid;
    }

    return objects;
}

// BSON-сериализация
bsoncxx::document::value ObjectEntry::toBson() const
{
    using bsoncxx::builder::basic::kvp;
    using bsoncxx::builder::basic::make_document;

    return make_document(
        kvp("group_id", group_id.toStdString()),
        kvp("type", type.toStdString()),
        kvp("owner_name", owner_name.toStdString()),
        kvp("device_uid", device_uid.toStdString())
    );
}
