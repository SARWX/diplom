#ifndef OBJECTLISTDIALOG_H
#define OBJECTLISTDIALOG_H

#include <QDialog>
#include <QList>
#include <QFormLayout>

#include "object.h"
#include "ui_objectlistdialog.h"

namespace Ui {
class ObjectListDialog;
}

class ObjectListDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ObjectListDialog(const QList<ObjectEntry>& objects, QWidget *parent = nullptr);
    ~ObjectListDialog();

    QList<ObjectEntry> getUpdatedObjects() const;
    QMap<QString, QString> groups_map_name_to_id;

private:
    Ui::ObjectListDialog *ui;
    QList<ObjectEntry> objectList;
    QWidget* createObjectEditor(const ObjectEntry& entry);
    QWidget* createObjectEditor();

    void populateObjectList();
    ObjectEntry getObjectFromEditor(QWidget* editor) const;

public slots:
    void onAddNewObject()
    {
        // Создаём редактор записи
        QWidget *editor = createObjectEditor();

        // Создаём QListWidgetItem (он нужен, чтобы вставить редактор в список)
        QListWidgetItem *item = new QListWidgetItem(ui->listWidget);

        // Задаём фиксированную высоту под редактор (можно уточнить)
        item->setSizeHint(editor->sizeHint());

        // Добавляем элемент и вставляем редактор внутрь него
        ui->listWidget->addItem(item);
        ui->listWidget->setItemWidget(item, editor);
    }

};

#endif // OBJECTLISTDIALOG_H
