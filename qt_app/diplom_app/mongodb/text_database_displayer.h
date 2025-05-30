#ifndef TEXT_DATABASE_DISPLAYER_H
#define TEXT_DATABASE_DISPLAYER_H

#include <QObject>
#include <QTextEdit>
#include <QList>
#include <QDialog>
#include "mongodb/violation_log/violation_log.h" // Заголовок с определением ViolationLogEntry

class TextDataBaseDisplayer : public QObject
{
    Q_OBJECT
public:
    explicit TextDataBaseDisplayer(QObject* parent = nullptr);
    void displayData(const QString& formattedData, const QString& title = "");
    
    // Методы подготовки данных
    QString prepareViolationsData(const QList<ViolationLogEntry>& violations);
    // Здесь можно добавить другие prepare... методы для разных типов данных
    
private:
    QTextEdit* createOutputWidget();
    
    QString formatViolation(const ViolationLogEntry& violation);
};

#endif // TEXT_DATABASE_DISPLAYER_H
