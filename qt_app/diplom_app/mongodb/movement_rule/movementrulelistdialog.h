#ifndef MOVEMENTRULELISTDIALOG_H
#define MOVEMENTRULELISTDIALOG_H

#include <QDialog>
#include <QList>
#include <QFormLayout>

#include "movement_rule.h"
#include "ui_movementrulelistdialog.h"

namespace Ui {
class MovementRuleListDialog;
}

class MovementRuleListDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MovementRuleListDialog(const QList<MovementRuleEntry>& movement_rules, QWidget *parent = nullptr);
    ~MovementRuleListDialog();

    QList<MovementRuleEntry> getUpdatedMovementRules() const;
    QMap<QString, QString> groups_map_name_to_id;
    QMap<QString, QString> sectors_map_name_to_id;

private:
    Ui::MovementRuleListDialog *ui;
    QList<MovementRuleEntry> movement_ruleList;
    QWidget* createMovementRuleEditor(const MovementRuleEntry& entry);
    QWidget* createMovementRuleEditor();

    void populateMovementRuleList();
    MovementRuleEntry getMovementRuleFromEditor(QWidget* editor) const;

public slots:
    void onAddNewMovementRule()
    {
        // Создаём редактор записи
        QWidget *editor = createMovementRuleEditor();

        // Создаём QListWidgetItem (он нужен, чтобы вставить редактор в список)
        QListWidgetItem *item = new QListWidgetItem(ui->listWidget);

        // Задаём фиксированную высоту под редактор (можно уточнить)
        item->setSizeHint(editor->sizeHint());

        // Добавляем элемент и вставляем редактор внутрь него
        ui->listWidget->addItem(item);
        ui->listWidget->setItemWidget(item, editor);
    }

};

#endif // MOVEMENTRULELISTDIALOG_H
