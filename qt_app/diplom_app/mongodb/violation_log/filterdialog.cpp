#include "mongodb/violation_log/filterdialog.h"
#include "ui_filterdialog.h"
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

FilterDialog::FilterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FilterDialog)
{
    ui->setupUi(this);
    sectors_map = getSectorIdMapFromMongo();

    ui->startDateTimeEdit->setDateTime(QDateTime::currentDateTime().addDays(-1));
    ui->endDateTimeEdit->setDateTime(QDateTime::currentDateTime());

    // List-box для того, чтобы использовать name, а не id
    ui->sectorComboBox->clear();
    for (const QString &name : sectors_map.keys()) {
        ui->sectorComboBox->addItem(name);
    }

    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &FilterDialog::accept);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &FilterDialog::reject);
}

FilterDialog::~FilterDialog()
{
    delete ui;
}

FilterSettings FilterDialog::getFilterSettings() const
{
    FilterSettings settings;
    settings.startTime = ui->startDateTimeEdit->dateTime();
    settings.endTime = ui->endDateTimeEdit->dateTime();
    // settings.sectorId = ui->sectorLineEdit->text();
    QString selectedName = ui->sectorComboBox->currentText();
    settings.sectorId = sectors_map.value(selectedName);  // _id по имен
    settings.severity = ui->severityCheckBox->checkState();
    return settings;
}

QMap<QString, QString> FilterDialog::getSectorIdMapFromMongo()
{
    QMap<QString, QString> sectorMap;

    mongocxx::client client{mongocxx::uri{}};
    auto db = client["lps"];
    auto collection = db["sector"];

    auto cursor = collection.find({});  // Найдём все документы в коллекции

    for (const auto& doc : cursor) {
        // Проверим, что есть нужные поля
        if (doc["name"] && doc["_id"]) {
            // auto name = QString::fromStdString(doc["name"].get_string());    // OLD VARIANT
            bsoncxx::types::b_string str = doc["name"].get_string();
            QString name = QString::fromStdString(std::string(str.value));
        
            // auto id = QString::fromStdString(bsoncxx::to_json(doc["_id"]));  // _id как JSON-строка
            str = doc["_id"].get_string();
            QString id = QString::fromStdString(std::string(str.value));


            // // Но лучше привести _id к строке без {...} если это ObjectId
            // if (doc["_id"].type() == bsoncxx::type::k_oid) {
            //     id = QString::fromStdString(doc["_id"].get_oid().value.to_string());
            // }

            sectorMap.insert(name, id);
        }
    }

    return sectorMap;
}
