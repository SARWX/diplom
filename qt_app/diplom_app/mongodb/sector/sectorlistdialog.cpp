#include "sectorlistdialog.h"
#include "ui_sectorlistdialog.h"

#include <QListWidgetItem>
#include <QLineEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

SectorListDialog::SectorListDialog(const QList<SectorEntry>& sectors, QWidget *parent)
    : QDialog(parent), ui(new Ui::SectorListDialog), sectorList(sectors)
{
    ui->setupUi(this);
    populateSectorList();

    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &SectorListDialog::accept);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &SectorListDialog::reject);

    connect(ui->addNewSector, &QPushButton::clicked, this, &SectorListDialog::onAddNewSector);

}

SectorListDialog::~SectorListDialog()
{
    delete ui;
}

void SectorListDialog::populateSectorList()
{
    ui->listWidget->clear();

    for (const SectorEntry &entry : sectorList) {
        QWidget* editor = new QWidget;
        editor->setProperty("sectorId", entry.id); // сохранение id
        QVBoxLayout* layout = new QVBoxLayout(editor);

        QLineEdit* nameEdit = new QLineEdit(entry.name);
        nameEdit->setObjectName("nameEdit");

        QLineEdit* typeEdit = new QLineEdit(entry.type);
        typeEdit->setObjectName("typeEdit");

        QSpinBox* floorSpin = new QSpinBox;
        floorSpin->setRange(-5, 100);
        floorSpin->setValue(entry.floor_number);
        floorSpin->setObjectName("floorSpin");

        QLineEdit* buildingEdit = new QLineEdit(entry.building_name);
        buildingEdit->setObjectName("buildingEdit");

        QDoubleSpinBox* startX = new QDoubleSpinBox;
        startX->setValue(entry.geometry[0].x);
        startX->setObjectName("startX");

        QDoubleSpinBox* startY = new QDoubleSpinBox;
        startY->setValue(entry.geometry[0].y);
        startY->setObjectName("startY");

        QDoubleSpinBox* startZ = new QDoubleSpinBox;
        startZ->setValue(entry.geometry[0].z);
        startZ->setObjectName("startZ");

        QDoubleSpinBox* sizeX = new QDoubleSpinBox;
        sizeX->setValue(entry.geometry[1].x);
        sizeX->setObjectName("sizeX");

        QDoubleSpinBox* sizeY = new QDoubleSpinBox;
        sizeY->setValue(entry.geometry[1].y);
        sizeY->setObjectName("sizeY");

        QDoubleSpinBox* sizeZ = new QDoubleSpinBox;
        sizeZ->setValue(entry.geometry[1].z);
        sizeZ->setObjectName("sizeZ");

        layout->addWidget(new QLabel("Name:"));
        layout->addWidget(nameEdit);
        layout->addWidget(new QLabel("Type:"));
        layout->addWidget(typeEdit);
        layout->addWidget(new QLabel("Floor:"));
        layout->addWidget(floorSpin);
        layout->addWidget(new QLabel("Building:"));
        layout->addWidget(buildingEdit);
        layout->addWidget(new QLabel("Start coords (X, Y, Z):"));
        layout->addWidget(startX);
        layout->addWidget(startY);
        layout->addWidget(startZ);
        layout->addWidget(new QLabel("Size of area (X, Y, Z):"));
        layout->addWidget(sizeX);
        layout->addWidget(sizeY);
        layout->addWidget(sizeZ);

        QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
        item->setSizeHint(editor->sizeHint());
        ui->listWidget->addItem(item);
        ui->listWidget->setItemWidget(item, editor);
    }
}

QList<SectorEntry> SectorListDialog::getUpdatedSectors() const
{
    QList<SectorEntry> updated;
    qDebug() << "We are in upadting method";

    for (int i = 0; i < ui->listWidget->count(); ++i) {
        qDebug() << "We 1";
        QWidget* editor = ui->listWidget->itemWidget(ui->listWidget->item(i));
        qDebug() << "We 2";
        if (editor) {
            qDebug() << "We 3";
            updated.append(getSectorFromEditor(editor));
        }
    }

    return updated;
}

SectorEntry SectorListDialog::getSectorFromEditor(QWidget* editor) const
{
    SectorEntry result;
    QVariant idProp = editor->property("sectorId");
    if (idProp.isValid() && idProp.canConvert<QString>()) {
        QString sectorId = idProp.toString();
        if (!sectorId.isEmpty()) {
            result.id = sectorId;
        }
    }
    qDebug() << "We save id: " << result.id;
    result.name = editor->findChild<QLineEdit*>("nameEdit")->text();
    qDebug() << "We1 1";
    result.type = editor->findChild<QLineEdit*>("typeEdit")->text();
    qDebug() << "We1 2";
    result.floor_number = editor->findChild<QSpinBox*>("floorSpin")->value();
    qDebug() << "We1 3";
    result.building_name = editor->findChild<QLineEdit*>("buildingEdit")->text();
    qDebug() << "We1 4";
    auto spinBox = editor->findChild<QDoubleSpinBox*>("startX");
    if (spinBox) {
        result.geometry[0].x = spinBox->value();
    } else {
        qWarning() << "QDoubleSpinBox с objectName 'startX' не найден в editor";
    }
    // result.geometry[0].x = editor->findChild<QDoubleSpinBox*>("startX")->value();
    qDebug() << "We1 5";
    result.geometry[0].y = editor->findChild<QDoubleSpinBox*>("startY")->value();
    qDebug() << "We1 6";
    result.geometry[0].z = editor->findChild<QDoubleSpinBox*>("startZ")->value();
    qDebug() << "We1 7";
    result.geometry[1].x = editor->findChild<QDoubleSpinBox*>("sizeX")->value();
    qDebug() << "We1 8";
    result.geometry[1].y = editor->findChild<QDoubleSpinBox*>("sizeY")->value();
    qDebug() << "We1 9";
    result.geometry[1].z = editor->findChild<QDoubleSpinBox*>("sizeZ")->value();

    qDebug() << "We get real SectorEntry? ---- " << result.name;
    return result;
}

QWidget* SectorListDialog::createSectorEditor(const SectorEntry& entry)
{
    QWidget *widget = new QWidget;
    QFormLayout *formLayout = new QFormLayout(widget);

    // name
    QLineEdit *nameEdit = new QLineEdit(entry.name);
    nameEdit->setObjectName("nameEdit");
    formLayout->addRow("Имя", nameEdit);

    // type
    QLineEdit *typeEdit = new QLineEdit(entry.type);
    typeEdit->setObjectName("typeEdit");
    formLayout->addRow("Тип", typeEdit);

    // floor
    QSpinBox *floorSpin = new QSpinBox;
    floorSpin->setObjectName("floorSpin");
    floorSpin->setValue(entry.floor_number);
    formLayout->addRow("Этаж", floorSpin);

    // building
    QLineEdit *buildingEdit = new QLineEdit(entry.building_name);
    buildingEdit->setObjectName("buildingEdit");
    formLayout->addRow("Здание", buildingEdit);

    // start X/Y/Z
    QDoubleSpinBox *startX = new QDoubleSpinBox;
    startX->setObjectName("startX");
    startX->setValue(entry.geometry[0].x);
    formLayout->addRow("Start X", startX);

    QDoubleSpinBox *startY = new QDoubleSpinBox;
    startY->setObjectName("startY");
    startY->setValue(entry.geometry[0].y);
    formLayout->addRow("Start Y", startY);

    QDoubleSpinBox *startZ = new QDoubleSpinBox;
    startZ->setObjectName("startZ");
    startZ->setValue(entry.geometry[0].z);
    formLayout->addRow("Start Z", startZ);

    // size X/Y/Z
    QDoubleSpinBox *sizeX = new QDoubleSpinBox;
    sizeX->setObjectName("sizeX");
    sizeX->setValue(entry.geometry[0].x);
    formLayout->addRow("size X", sizeX);

    QDoubleSpinBox *sizeY = new QDoubleSpinBox;
    sizeY->setObjectName("sizeY");
    sizeY->setValue(entry.geometry[0].y);
    formLayout->addRow("size Y", sizeY);

    QDoubleSpinBox *sizeZ = new QDoubleSpinBox;
    sizeZ->setObjectName("sizeZ");
    sizeZ->setValue(entry.geometry[0].z);
    formLayout->addRow("size Z", sizeZ);

    return widget;
}

QWidget* SectorListDialog::createSectorEditor()
{
    return createSectorEditor(SectorEntry());  // вызов конструктора по умолчанию
}
