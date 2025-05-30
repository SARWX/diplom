#include "mongodb/text_database_displayer.h"
#include <QDateTime>
#include <QVBoxLayout>

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
    
    // Настраиваем layout
    QVBoxLayout* layout = new QVBoxLayout(dialog);
    layout->addWidget(outputWidget);
    dialog->setLayout(layout);
    
    // Форматируем и выводим данные
    QString output;
    if (!title.isEmpty()) {
        output += QString("<h2>%1</h2>").arg(title);
    }
    output += formattedData;
    outputWidget->setHtml(output);
    
    // Показываем немодальное окно (можно закрыть независимо от основного)
    dialog->show();
    
    // Автоматическое удаление при закрытии
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
    return QString("<tr>"
                  "<td>%1</td>"
                  "<td>%2</td>"
                  "<td>%3</td>"
                  "<td>%4</td>"
                  "<td>(%5, %6, %7)</td>"
                  "<td>%8</td>"
                  "</tr>")
            .arg(violation.id)
            .arg(violation.object_id)
            .arg(violation.sector_id)
            .arg(violation.timestamp.toString("yyyy-MM-dd HH:mm:ss"))
            .arg(violation.coords.x).arg(violation.coords.y).arg(violation.coords.z)
            .arg(violation.severity);
}
