#pragma once

#include <QDialog>
#include <QDateTime>

namespace Ui {
class FilterDialog;
}

struct FilterSettings {
    QDateTime startTime;
    QDateTime endTime;
    QString sectorId;
    int severity;
};

class FilterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FilterDialog(QWidget *parent = nullptr);
    ~FilterDialog();

    FilterSettings getFilterSettings() const;

private:
    Ui::FilterDialog *ui;
    QMap<QString, QString> sectors_map_name_to_id;
    QMap<QString, QString> sectors_map_new_id_to_name;
    
    QMap<QString, QString> getSectorIdMapFromMongo();
};
