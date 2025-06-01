#ifndef MONGO_SERVICE_H
#define MONGO_SERVICE_H

#include <QObject>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/database.hpp>
#include <mongocxx/collection.hpp>

#include "coordinates.h"
#include "mongodb/sector/sector.h"
#include "mongodb/object/object.h"
#include "mongodb/movement_rule/movement_rule.h"

class MongoService : public QObject
{
    Q_OBJECT

public:
    explicit MongoService(QObject *parent = nullptr);

    bool connectToDatabase(const QString &uri, const QString &dbName);
    std::vector<bsoncxx::document::value> getViolations();
    // void logEvent(const QString &event);
    bool insertViolation(const QString &type, int severity);
    static  QMap<QString, QString> getMongoFieldMap(
        const QString& collectionName, 
        const QString& keyField, 
        const QString& valueField);
    bool saveToMongo(const SectorEntry &entry);
    bool saveToMongo(const ObjectEntry &entry);
    bool saveToMongo(const MovementRuleEntry &entry);


signals:
    void connectionSucceeded();
    void connectionFailed(const QString &reason);
    void violationsLoaded(const QStringList &violations);

private:
    mongocxx::client *client;
    mongocxx::database db;
};

// Non-class, common functions
void parse_coords_from_document(const bsoncxx::document::view& doc, coordinates* coords);

QString idToQString(const bsoncxx::oid &id);

bsoncxx::oid stringToOid(const QString &str);


#endif // MONGO_SERVICE_H
