#include "movementrulelistdialog.h"
#include "ui_movementrulelistdialog.h"

#include "mongodb/mongoservice.h"

#include <QListWidgetItem>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QDateTimeEdit>
#include <QDebug>

MovementRuleListDialog::MovementRuleListDialog(const QList<MovementRuleEntry> &rules, QWidget *parent)
    : QDialog(parent), ui(new Ui::MovementRuleListDialog), movement_ruleList(rules)
{
    ui->setupUi(this);
    groups_map_name_to_id = MongoService::getMongoFieldMap("group", "name", "_id");
    sectors_map_name_to_id = MongoService::getMongoFieldMap("sector", "name", "_id");

    populateMovementRuleList();

    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &MovementRuleListDialog::accept);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &MovementRuleListDialog::reject);
    connect(ui->addNewMovementRule, &QPushButton::clicked, this, &MovementRuleListDialog::onAddNewMovementRule);
}

MovementRuleListDialog::~MovementRuleListDialog()
{
    delete ui;
}

void MovementRuleListDialog::populateMovementRuleList()
{
    ui->listWidget->clear();

    for (const MovementRuleEntry &entry : movement_ruleList) {
        QWidget *editor = new QWidget;
        editor->setProperty("movementRuleId", entry.id);
        QVBoxLayout *layout = new QVBoxLayout(editor);

        QComboBox *groupComboBox = new QComboBox;
        groupComboBox->setObjectName("groupComboBox");
        QString currentGroup;
        for (const QString &name : groups_map_name_to_id.keys()) {
            groupComboBox->addItem(name);
            if (groups_map_name_to_id[name] == entry.group_id) {
                currentGroup = name;
            }
        }
        if (!currentGroup.isEmpty()) {
            groupComboBox->setCurrentText(currentGroup);
        }

        QComboBox *sectorComboBox = new QComboBox;
        sectorComboBox->setObjectName("sectorComboBox");
        QString currentSector;
        for (const QString &name : sectors_map_name_to_id.keys()) {
            sectorComboBox->addItem(name);
            if (sectors_map_name_to_id[name] == entry.sector_id) {
                currentSector = name;
            }
        }
        if (!currentSector.isEmpty()) {
            sectorComboBox->setCurrentText(currentSector);
        }

        QLineEdit *accessTypeEdit = new QLineEdit(QString::number(entry.access_type));
        accessTypeEdit->setObjectName("accessTypeEdit");

        // Создаем редактор времени (без даты)
        QTimeEdit *timeFromEdit = new QTimeEdit(entry.time_from); // entry.time_from должен быть типа QTime
        timeFromEdit->setObjectName("timeFromEdit");
        timeFromEdit->setDisplayFormat("HH:mm"); // Устанавливаем формат отображения

        // Создаем редактор времени (без даты)
        QTimeEdit *timeToEdit = new QTimeEdit(entry.time_from); // entry.time_from должен быть типа QTime
        timeToEdit->setObjectName("timeToEdit");
        timeToEdit->setDisplayFormat("HH:mm"); // Устанавливаем формат отображения

        QLineEdit *reasonEdit = new QLineEdit(entry.reason);
        reasonEdit->setObjectName("reasonEdit");

        layout->addWidget(new QLabel("Group:"));
        layout->addWidget(groupComboBox);
        layout->addWidget(new QLabel("Sector:"));
        layout->addWidget(sectorComboBox);
        layout->addWidget(new QLabel("Access Type:"));
        layout->addWidget(accessTypeEdit);
        layout->addWidget(new QLabel("Time From:"));
        layout->addWidget(timeFromEdit);
        layout->addWidget(new QLabel("Time To:"));
        layout->addWidget(timeToEdit);
        layout->addWidget(new QLabel("Reason:"));
        layout->addWidget(reasonEdit);

        QListWidgetItem *item = new QListWidgetItem(ui->listWidget);
        item->setSizeHint(editor->sizeHint());
        ui->listWidget->addItem(item);
        ui->listWidget->setItemWidget(item, editor);
    }
}

QList<MovementRuleEntry> MovementRuleListDialog::getUpdatedMovementRules() const
{
    QList<MovementRuleEntry> updated;

    for (int i = 0; i < ui->listWidget->count(); ++i) {
        QWidget *editor = ui->listWidget->itemWidget(ui->listWidget->item(i));
        if (editor) {
            updated.append(getMovementRuleFromEditor(editor));
        }
    }

    return updated;
}

MovementRuleEntry MovementRuleListDialog::getMovementRuleFromEditor(QWidget *editor) const
{
    MovementRuleEntry rule;

    QVariant idProp = editor->property("movementRuleId");
    if (idProp.isValid() && idProp.canConvert<QString>()) {
        QString ruleId = idProp.toString();
        if (!ruleId.isEmpty()) {
            rule.id = ruleId;
        }
    }

    QComboBox *groupComboBox = editor->findChild<QComboBox*>("groupComboBox");
    if (groupComboBox) {
        QString groupName = groupComboBox->currentText();
        if (groups_map_name_to_id.contains(groupName)) {
            rule.group_id = groups_map_name_to_id[groupName];
        }
    }

    QComboBox *sectorComboBox = editor->findChild<QComboBox*>("sectorComboBox");
    if (sectorComboBox) {
        QString sectorName = sectorComboBox->currentText();
        if (sectors_map_name_to_id.contains(sectorName)) {
            rule.sector_id = sectors_map_name_to_id[sectorName];
        }
    }

    QLineEdit *accessTypeEdit = editor->findChild<QLineEdit*>("accessTypeEdit");
    if (accessTypeEdit) {
        rule.access_type = accessTypeEdit->text().toInt();
    }

    QDateTimeEdit *timeFromEdit = editor->findChild<QTimeEdit*>("timeFromEdit");
    if (timeFromEdit) {
        rule.time_from = timeFromEdit->time(); 
    }

    QDateTimeEdit *timeToEdit = editor->findChild<QTimeEdit*>("timeToEdit");
    if (timeToEdit) {
        rule.time_to = timeToEdit->time(); 
    }

    QLineEdit *reasonEdit = editor->findChild<QLineEdit*>("reasonEdit");
    if (reasonEdit) {
        rule.reason = reasonEdit->text();
    }

    return rule;
}


QWidget* MovementRuleListDialog::createMovementRuleEditor(const MovementRuleEntry& entry)
{
    QWidget *editor = new QWidget;
    editor->setProperty("movementRuleId", entry.id);
    QVBoxLayout *layout = new QVBoxLayout(editor);

    QComboBox *groupComboBox = new QComboBox;
    groupComboBox->setObjectName("groupComboBox");
    QString currentGroup;
    for (const QString &name : groups_map_name_to_id.keys()) {
        groupComboBox->addItem(name);
        if (groups_map_name_to_id[name] == entry.group_id) {
            currentGroup = name;
        }
    }
    if (!currentGroup.isEmpty()) {
        groupComboBox->setCurrentText(currentGroup);
    }

    QComboBox *sectorComboBox = new QComboBox;
    sectorComboBox->setObjectName("sectorComboBox");
    QString currentSector;
    for (const QString &name : sectors_map_name_to_id.keys()) {
        sectorComboBox->addItem(name);
        if (sectors_map_name_to_id[name] == entry.sector_id) {
            currentSector = name;
        }
    }
    if (!currentSector.isEmpty()) {
        sectorComboBox->setCurrentText(currentSector);
    }

    QLineEdit *accessTypeEdit = new QLineEdit(QString::number(entry.access_type));
    accessTypeEdit->setObjectName("accessTypeEdit");

    QDateTimeEdit *timeFromEdit = new QDateTimeEdit(entry.time_from);
    timeFromEdit->setObjectName("timeFromEdit");
    timeFromEdit->setCalendarPopup(true);

    QDateTimeEdit *timeToEdit = new QDateTimeEdit(entry.time_to);
    timeToEdit->setObjectName("timeToEdit");
    timeToEdit->setCalendarPopup(true);

    QLineEdit *reasonEdit = new QLineEdit(entry.reason);
    reasonEdit->setObjectName("reasonEdit");

    layout->addWidget(new QLabel("Group:"));
    layout->addWidget(groupComboBox);
    layout->addWidget(new QLabel("Sector:"));
    layout->addWidget(sectorComboBox);
    layout->addWidget(new QLabel("Access Type:"));
    layout->addWidget(accessTypeEdit);
    layout->addWidget(new QLabel("Time From:"));
    layout->addWidget(timeFromEdit);
    layout->addWidget(new QLabel("Time To:"));
    layout->addWidget(timeToEdit);
    layout->addWidget(new QLabel("Reason:"));
    layout->addWidget(reasonEdit);

    return editor;
}

QWidget* MovementRuleListDialog::createMovementRuleEditor()
{
    return createMovementRuleEditor(MovementRuleEntry());
}
