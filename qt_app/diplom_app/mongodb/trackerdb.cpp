#include "trackerdb.h"
#include "coordinates.h"
#include <QDateTime>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/types.hpp>

#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>
using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;

TrackerDB::TrackerDB(const QString& object_id, QObject* parent)
    : QObject(parent),
      m_objectId(object_id),
      m_client(mongocxx::uri{}),
      m_collection(m_client["lps"]["coordinates"])  // работа с БД "lps" и коллекцией "coordinates"
{}

void TrackerDB::coordinateChanged(coordinates point, float r1, float r2, float r3, int point_number)
{
    auto now = QDateTime::currentDateTimeUtc().toMSecsSinceEpoch();

    auto coordinates_doc = make_document(
        kvp("x", point.x),
        kvp("y", point.y),
        kvp("z", point.z)
    );

    auto doc = make_document(
        kvp("object_id", m_objectId.toStdString()),
        kvp("timestamp", bsoncxx::types::b_date{std::chrono::milliseconds{now}}),
        kvp("coordinates", coordinates_doc.view())  // <-- ВАЖНО: view!
    );

    try {
        auto result = m_collection.insert_one(doc.view());
        if (result && result->result().inserted_count() == 1) {
            qDebug() << "Inserted coordinates for" << m_objectId
                     << "at" << now
                     << "-> (" << point.x << "," << point.y << "," << point.z << ")";
        } else {
            qWarning() << "Failed to insert coordinates for" << m_objectId;
        }
    } catch (const std::exception &e) {
        qCritical() << "Exception while inserting coordinates for" << m_objectId
                    << ":" << e.what();
    }
}

void TrackerDB::showObjectRoute(const QDateTime &startTime, const QDateTime &endTime)
{
    using bsoncxx::builder::basic::kvp;
    using bsoncxx::builder::basic::make_document;

    // Переводим QDateTime в UTC миллисекунды и потом в bsoncxx::b_date
    auto start_ms = startTime.toUTC().toMSecsSinceEpoch();
    auto end_ms = endTime.toUTC().toMSecsSinceEpoch();

    auto start = bsoncxx::types::b_date{std::chrono::milliseconds{start_ms}};
    auto end = bsoncxx::types::b_date{std::chrono::milliseconds{end_ms}};

    auto filter = make_document(
        kvp("timestamp", make_document(
            kvp("$gte", start),
            kvp("$lte", end)
        ))
    );

    mongocxx::cursor cursor = m_collection.find(filter.view());

    for (auto&& doc : cursor)
    {
        auto coords_doc = doc["coordinates"].get_document().view();

        coordinates point;
        point.x = coords_doc["x"].get_double();
        point.y = coords_doc["y"].get_double();
        point.z = coords_doc["z"].get_double();

        emit routeCoordinateReady(point);
    }
}

