#include "mongodb/text_database_displayer.h"
#include "mongodb/mongoservice.h"
#include <QDateTime>
#include <QVBoxLayout>
#include <QPushButton>
#include <QPrinter>
#include <QFileDialog>
#include <QTextDocument>
#include <QStandardPaths>  // Добавлено для QStandardPaths
#include <QMessageBox>     // Добавлено для QMessageBox
#include <QHBoxLayout>     // Добавлено для QHBoxLayout
TextDataBaseDisplayer::TextDataBaseDisplayer(QObject* parent)
    : QObject(parent)
{
}

void TextDataBaseDisplayer::displayData(const QString& formattedData, const QString& title)
{
    // Создаем диалоговое окно
    QDialog* dialog = new QDialog();
    dialog->setWindowTitle(title.isEmpty() ? "Database Output" : title);
    dialog->setMinimumSize(600, 400);
    
    // Создаем текстовое поле
    QTextEdit* outputWidget = createOutputWidget();
    outputWidget->setHtml((!title.isEmpty() ? QString("<h2>%1</h2>").arg(title) : "") + formattedData);
    
    // Кнопка экспорта в PDF
    QPushButton* exportBtn = new QPushButton("Export to PDF", dialog);
    exportBtn->setFixedWidth(120);
    
    // Настраиваем layout
    QVBoxLayout* mainLayout = new QVBoxLayout(dialog);
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(exportBtn);
    
    mainLayout->addLayout(buttonLayout);
    mainLayout->addWidget(outputWidget);
    
    // Обработчик кнопки экспорта
    QObject::connect(exportBtn, &QPushButton::clicked, [outputWidget, title]() {
        QString fileName = QFileDialog::getSaveFileName(
            nullptr,
            "Export to PDF",
            QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + 
            "/" + (title.isEmpty() ? "violation_report" : title.simplified()) + ".pdf",
            "PDF Files (*.pdf)");
        
        if (!fileName.isEmpty()) {
            QPrinter printer(QPrinter::HighResolution);
            printer.setOutputFormat(QPrinter::PdfFormat);
            printer.setOutputFileName(fileName);
            printer.setPageMargins(QMarginsF(15, 15, 15, 15));
            printer.setPageOrientation(QPageLayout::Landscape); // Альбомная ориентация
            
            QTextDocument doc;
            doc.setHtml(outputWidget->toHtml());
            doc.print(&printer);
            
            QMessageBox::information(nullptr, "Export Successful", 
                                   QString("Report saved to:\n%1").arg(fileName));
        }
    });
    
    dialog->show();
    dialog->setAttribute(Qt::WA_DeleteOnClose);
}

QTextEdit* TextDataBaseDisplayer::createOutputWidget()
{
    QTextEdit* widget = new QTextEdit();
    widget->setReadOnly(true);
    widget->setLineWrapMode(QTextEdit::NoWrap);
    widget->setFont(QFont("Courier New", 10));
    return widget;
}

QString TextDataBaseDisplayer::prepareViolationsData(const QList<ViolationLogEntry>& violations)
{
    if (violations.isEmpty()) {
        return "<p>No violations found</p>";
    }

    QString result;
    result += QString("<p>Found %1 violations:</p>").arg(violations.size());
    result += "<table border='1' cellpadding='4' cellspacing='0'>";
    result += "<tr><th>ID</th><th>Object</th><th>Sector</th><th>Time</th><th>Coordinates</th><th>Severity</th></tr>";
    
    for (const auto& violation : violations) {
        result += formatViolation(violation);
    }
    
    result += "</table>";
    return result;
}

QString TextDataBaseDisplayer::formatViolation(const ViolationLogEntry& violation)
{
    QMap<QString, QString> objects_map = MongoService::getMongoFieldMap("object", "_id", "owner_name");
    qDebug() << "keys: " << objects_map.keys() << "\nvalues: " << objects_map.values();
    QMap<QString, QString> sectors_map_id_to_name = MongoService::getMongoFieldMap("sector", "_id", "name");
    QMap<QString, QString> sectors_map_name_to_id = MongoService::getMongoFieldMap("sector", "name", "_id");

    // Безопасное получение значений с fallback
    QString object_name = objects_map.value(violation.object_id, "Unknown object");
    qDebug() << "Ours object_id: " << violation.object_id;
    QString sector_name = sectors_map_id_to_name.value(violation.sector_id, "Unknown sector");

    return QString("<tr>"
                  "<td>%1</td>"
                  "<td>%2</td>"
                  "<td>%3</td>"
                  "<td>%4</td>"
                  "<td>(%5, %6, %7)</td>"
                  "<td>%8</td>"
                  "</tr>")
            .arg(violation.id)
            .arg(object_name)
            .arg(sector_name)
            .arg(violation.timestamp.toString("yyyy-MM-dd HH:mm:ss"))
            .arg(violation.coords.x).arg(violation.coords.y).arg(violation.coords.z)
            .arg(violation.severity);
}
