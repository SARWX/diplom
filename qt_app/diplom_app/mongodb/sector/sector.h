#ifndef SECTOR_H
#define SECTOR_H

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


class SectorEntry {
public:
    QString id;
    QString name;
    QString type;
    coordinates geometry[2];    // 0 -start, 1 - size
    int floor_number;
    QString building_name;
    bsoncxx::document::value toBson() const;


    // Конструктор по умолчанию
    SectorEntry() = default;

    // Конструктор из BSON-документа
    SectorEntry(const bsoncxx::document::view &doc);

};

QList<SectorEntry> loadSectorEntryFromMongo();

#endif // SECTOR_H
