#include "mongodb/violation_log/filterdialog.h"
#include "mongodb/mongoservice.h"
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
    qDebug() << "Filter 1.1";
    sectors_map_name_to_id = MongoService::getMongoFieldMap("sector", "name", "_id");

    qDebug() << "Filter 1.2";
    ui->startDateTimeEdit->setDateTime(QDateTime::currentDateTime().addDays(-1));
    ui->endDateTimeEdit->setDateTime(QDateTime::currentDateTime());

    // List-box для того, чтобы использовать name, а не id
    ui->sectorComboBox->clear();
    qDebug() << "Filter 1.3";
    for (const QString &name : sectors_map_name_to_id.keys()) {
        ui->sectorComboBox->addItem(name);
    }
    qDebug() << "Filter 1.4";
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
    QString selectedName = ui->sectorComboBox->currentText();
    // qDebug() << "=============== selected NAME: " << selectedName;
    // qDebug() << "=============== selected ID: " << sectors_map_name_to_id.value(selectedName);
    // qDebug() << "=============== available NAMES: ";
    // for (const QString &name : sectors_map_name_to_id.keys()) {
    //     qDebug() << name;
    // }
    // qDebug() << "=============== available ID: ";
    // for (const QString &name : sectors_map_name_to_id.values()) {
    //     qDebug() << name;
    // }
    settings.sectorId = sectors_map_name_to_id.value(selectedName);  // _id по имен
    settings.severity = ui->severityCheckBox->checkState();
    return settings;
}
