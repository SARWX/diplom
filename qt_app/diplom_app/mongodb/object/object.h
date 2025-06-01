#ifndef OBJECT_H
#define OBJECT_H

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

class ObjectEntry {
public:
    QString id;         //
    QString group_id;
    QString type;       //
    QString owner_name; //
    QString device_uid; //
    bsoncxx::document::value toBson() const;

    // Конструктор по умолчанию
    ObjectEntry() = default;

    // Конструктор из BSON-документа
    ObjectEntry(const bsoncxx::document::view &doc);
};

QList<ObjectEntry> loadObjectEntryFromMongo();

#endif // OBJECT_H
