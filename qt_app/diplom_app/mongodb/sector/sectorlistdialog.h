#ifndef SECTORLISTDIALOG_H
#define SECTORLISTDIALOG_H

#include <QDialog>
#include <QList>
#include <QFormLayout>

#include "sector.h"
#include "ui_sectorlistdialog.h"

namespace Ui {
class SectorListDialog;
}

class SectorListDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SectorListDialog(const QList<SectorEntry>& sectors, QWidget *parent = nullptr);
    ~SectorListDialog();

    QList<SectorEntry> getUpdatedSectors() const;

private:
    Ui::SectorListDialog *ui;
    QList<SectorEntry> sectorList;
    QWidget* createSectorEditor(const SectorEntry& entry);
    QWidget* createSectorEditor();

    void populateSectorList();
    SectorEntry getSectorFromEditor(QWidget* editor) const;

public slots:
    void onAddNewSector()
    {
        // Создаём редактор записи
        QWidget *editor = createSectorEditor();

        // Создаём QListWidgetItem (он нужен, чтобы вставить редактор в список)
        QListWidgetItem *item = new QListWidgetItem(ui->listWidget);

        // Задаём фиксированную высоту под редактор (можно уточнить)
        item->setSizeHint(editor->sizeHint());

        // Добавляем элемент и вставляем редактор внутрь него
        ui->listWidget->addItem(item);
        ui->listWidget->setItemWidget(item, editor);
    }

};

#endif // SECTORLISTDIALOG_H
