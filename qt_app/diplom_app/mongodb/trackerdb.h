#ifndef TRACKERDB_H
#define TRACKERDB_H

#include <QObject>
#include <QVector3D>
#include <mongocxx/client.hpp>
#include <mongocxx/collection.hpp>
#include "coordinates.h"

class TrackerDB : public QObject
{
    Q_OBJECT

public:
    explicit TrackerDB(const QString& object_id, QObject* parent = nullptr);
    void showObjectRoute(const QDateTime &startTime, const QDateTime &endTime);


signals:
    void routeCoordinateReady(coordinates point);

public slots:
    void coordinateChanged (coordinates point, float r1, float r2, float r3, int point_number = 0);

private:
    QString m_objectId;
    mongocxx::client m_client;
    mongocxx::collection m_collection;
};


#endif // TRACKERDB_H
