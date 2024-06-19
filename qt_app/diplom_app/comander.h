#ifndef COMMANDER_H
#define COMMANDER_H

#include <QObject>
#include <QLineEdit>
#include <QLabel>
#include <QScrollArea>
#include <QScrollBar>
#include <QSerialPortInfo>
#include <QRegularExpression>


class Commander : public QObject
{
    Q_OBJECT

public:
    explicit Commander(QLineEdit *commandLineEdit, QLabel *outputLabel, QScrollArea *scrollArea, QObject *parent = nullptr)
        : QObject(parent), m_commandLineEdit(commandLineEdit), m_outputLabel(outputLabel), m_scrollArea(scrollArea)
    {
        connect(m_commandLineEdit, &QLineEdit::returnPressed, this, &Commander::processCommand);
    }

signals:
    void commandProcessed(const QString &command);
    void setCOMport(const QString &portName);
    void startTimer();
    void stopTimer();

public slots:
    void processCommand()
    {
        QString command = m_commandLineEdit->text().trimmed();
        if (!command.isEmpty())
        {
            QRegularExpression re("^set\\s+COM(\\d+)$"); // Регулярное выражение для поиска "set COMX"
            QRegularExpressionMatch match = re.match(command);
            if (match.hasMatch())
            {
                QString comPort = "COM" + match.captured(1); // Захватываем номер COM-порта и добавляем префикс "COM"
                emit setCOMport(comPort); // Отправляем сигнал с номером COM-порта
            }
            else
            {
                if (command == "COM")
                {
                    QStringList portNames;
                    QList<QSerialPortInfo> availablePorts = QSerialPortInfo::availablePorts();
                    for (const auto &port : availablePorts)
                    {
                        portNames << port.portName();
                    }
                    appendOutput("Available COM ports: " + portNames.join(", "));
                }
                else
                {
                    emit commandProcessed(command);
                    appendOutput(command);
                }
            }
            m_commandLineEdit->clear();
            scrollToBottom();
        }
    }

    void displayConnectionStatus(int status)
    {
        if (status == 1)
        {
            appendOutput("Устройство подключено");
        }
        else
        {
            appendOutput("Ошибка при подключении устройства");
        }
    }

private:
    QLineEdit *m_commandLineEdit; // Указатель на объект командной строки
    QLabel *m_outputLabel; // Указатель на объект метки вывода
    QScrollArea *m_scrollArea; // Указатель на объект области прокрутки

    void appendOutput(const QString &text)
    {
        QString currentText = m_outputLabel->text();
        if (!currentText.isEmpty())
            currentText += "\n";
        currentText += text;
        m_outputLabel->setText(currentText);
    }

    void scrollToBottom()
    {
        m_scrollArea->verticalScrollBar()->setValue(m_scrollArea->verticalScrollBar()->maximum());
    }
};

#endif // COMMANDER_H
