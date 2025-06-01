#include "objectlistdialog.h"
#include "ui_objectlistdialog.h"

#include "mongodb/mongoservice.h"

#include <QListWidgetItem>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QDebug>

ObjectListDialog::ObjectListDialog(const QList<ObjectEntry>& objects, QWidget *parent)
    : QDialog(parent), ui(new Ui::ObjectListDialog), objectList(objects)
{
    ui->setupUi(this);
    groups_map_name_to_id = MongoService::getMongoFieldMap("group", "name", "_id");

    populateObjectList();

    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &ObjectListDialog::accept);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &ObjectListDialog::reject);
    connect(ui->addNewObject, &QPushButton::clicked, this, &ObjectListDialog::onAddNewObject);
}

ObjectListDialog::~ObjectListDialog()
{
    delete ui;
}

void ObjectListDialog::populateObjectList()
{
    ui->listWidget->clear();

    for (const ObjectEntry &entry : objectList) {
        QWidget* editor = new QWidget;
        editor->setProperty("objectId", entry.id);
        QVBoxLayout* layout = new QVBoxLayout(editor);

        QLineEdit* owner_nameEdit = new QLineEdit(entry.owner_name);
        owner_nameEdit->setObjectName("owner_nameEdit");

        QLineEdit* typeEdit = new QLineEdit(entry.type);
        typeEdit->setObjectName("typeEdit");

        QLineEdit* device_uidEdit = new QLineEdit(entry.device_uid);
        device_uidEdit->setObjectName("device_uidEdit");

        QComboBox* groupComboBox = new QComboBox;
        groupComboBox->setObjectName("groupComboBox");
        QString currentGroupName;
        for (const QString& name : groups_map_name_to_id.keys()) {
            groupComboBox->addItem(name);
            if (groups_map_name_to_id[name] == entry.group_id) {
                currentGroupName = name;
            }
        }
        if (!currentGroupName.isEmpty()) {
            groupComboBox->setCurrentText(currentGroupName);
        }

        layout->addWidget(new QLabel("Owner name:"));
        layout->addWidget(owner_nameEdit);
        layout->addWidget(new QLabel("Type:"));
        layout->addWidget(typeEdit);
        layout->addWidget(new QLabel("Device UID:"));
        layout->addWidget(device_uidEdit);
        layout->addWidget(new QLabel("Group:"));
        layout->addWidget(groupComboBox);

        QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
        item->setSizeHint(editor->sizeHint());
        ui->listWidget->addItem(item);
        ui->listWidget->setItemWidget(item, editor);
    }
}

QList<ObjectEntry> ObjectListDialog::getUpdatedObjects() const
{
    QList<ObjectEntry> updated;

    for (int i = 0; i < ui->listWidget->count(); ++i) {
        QWidget* editor = ui->listWidget->itemWidget(ui->listWidget->item(i));
        if (editor) {
            updated.append(getObjectFromEditor(editor));
        }
    }

    return updated;
}

ObjectEntry ObjectListDialog::getObjectFromEditor(QWidget* editor) const
{
    ObjectEntry result;

    QVariant idProp = editor->property("objectId");
    if (idProp.isValid() && idProp.canConvert<QString>()) {
        QString objectId = idProp.toString();
        if (!objectId.isEmpty()) {
            result.id = objectId;
        }
    }

    result.owner_name = editor->findChild<QLineEdit*>("owner_nameEdit")->text();
    result.type = editor->findChild<QLineEdit*>("typeEdit")->text();
    result.device_uid = editor->findChild<QLineEdit*>("device_uidEdit")->text();

    QComboBox* groupComboBox = editor->findChild<QComboBox*>("groupComboBox");
    if (groupComboBox) {
        QString groupName = groupComboBox->currentText();
        if (groups_map_name_to_id.contains(groupName)) {
            result.group_id = groups_map_name_to_id[groupName];
        }
    }

    return result;
}

QWidget* ObjectListDialog::createObjectEditor(const ObjectEntry& entry)
{
    QWidget *widget = new QWidget;
    QFormLayout *formLayout = new QFormLayout(widget);

    QLineEdit *owner_nameEdit = new QLineEdit(entry.owner_name);
    owner_nameEdit->setObjectName("owner_nameEdit");
    formLayout->addRow("Имя", owner_nameEdit);

    QLineEdit *typeEdit = new QLineEdit(entry.type);
    typeEdit->setObjectName("typeEdit");
    formLayout->addRow("Тип", typeEdit);

    QLineEdit *device_uidEdit = new QLineEdit(entry.device_uid);
    device_uidEdit->setObjectName("device_uidEdit");
    formLayout->addRow("UID", device_uidEdit);

    QComboBox *groupComboBox = new QComboBox;
    groupComboBox->setObjectName("groupComboBox");
    QString currentGroupName;
    for (const QString& name : groups_map_name_to_id.keys()) {
        groupComboBox->addItem(name);
        if (groups_map_name_to_id[name] == entry.group_id) {
            currentGroupName = name;
        }
    }
    if (!currentGroupName.isEmpty()) {
        groupComboBox->setCurrentText(currentGroupName);
    }
    formLayout->addRow("Группа", groupComboBox);

    return widget;
}

QWidget* ObjectListDialog::createObjectEditor()
{
    return createObjectEditor(ObjectEntry());
}
