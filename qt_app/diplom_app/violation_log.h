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

QList<ViolationLogEntry> loadViolationsFromMongo();


#endif // VIOLATION_LOG_H
