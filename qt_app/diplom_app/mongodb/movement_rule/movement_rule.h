#ifndef MOVEMENTRULE_H
#define MOVEMENTRULE_H

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

class MovementRuleEntry {
public:
    QString id;         
    QString group_id;
    QString sector_id;
    int access_type;    
    QTime time_from;
    QTime time_to;
    QString reason; 
    bsoncxx::document::value toBson() const;

    // Конструктор по умолчанию
    MovementRuleEntry() = default;

    // Конструктор из BSON-документа
    MovementRuleEntry(const bsoncxx::document::view &doc);
};

QList<MovementRuleEntry> loadMovementRuleEntryFromMongo();

#endif // MOVEMENTRULE_H
