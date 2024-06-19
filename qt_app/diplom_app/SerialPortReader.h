#ifndef SERIALPORTREADER_H
#define SERIALPORTREADER_H
#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QDataStream>
#include <QTimer>
#include <QDebug>

#include "comander.h"


class SerialPortReader : public QObject
{
    Q_OBJECT

//private:
//    Commander m_commander;
private:
    QTimer readTimer; // Таймер для чтения данных из порта



public:
    SerialPortReader(Commander *commander, QObject *parent = nullptr) : QObject(parent)
    {
        // Инициализация объекта для работы с COM-портом
        serialPort.setBaudRate(QSerialPort::Baud115200);
        serialPort.setDataBits(QSerialPort::Data8);
        serialPort.setParity(QSerialPort::NoParity);
        serialPort.setStopBits(QSerialPort::OneStop);
        serialPort.setFlowControl(QSerialPort::NoFlowControl);

        // Поиск доступных COM-портов
        QStringList portNames;
        QList<QSerialPortInfo> portList = QSerialPortInfo::availablePorts();
        for (const auto &portInfo : portList) {
            portNames << portInfo.portName();
        }
        connect(commander, &Commander::setCOMport, this, &SerialPortReader::updateCOMsettings);
        // ГЕНЕРАЦИЯ
        connect(&readTimer, &QTimer::timeout, this, &SerialPortReader::readDataFromPort);
//        connect(commander, &Commander::portListAvailable, this, &SerialPortReader::startReadingFromPort);
//        connect(commander, &Commander::portSelected, this, &SerialPortReader::stopReadingFromPort);

    }

signals:
    // Сигнал, который отправляется при получении новых данных
//    void newDataReceived(float x, float y, float z);
    void newDataReceived(double r1, double r2, double r3, int point_number = 0);
    void connectionResult(int status);


private slots:
    void updateCOMsettings(const QString &portName)
    {
        // СТОП ТАЙМЕР
        readTimer.stop();
        // Закрываем предыдущее соединение, если оно было открыто
        if (serialPort.isOpen()) {
            serialPort.close();
        }

        // Подключаемся к новому COM-порту
        serialPort.setPortName(portName);
        if (serialPort.open(QIODevice::ReadWrite)) {
            emit connectionResult(1);

            qDebug() << "Connected to" << portName;
            // Запускаем таймер
            readTimer.start(100); // Читаем данные из порта каждую секунду
        } else {
            emit connectionResult(0);

            qDebug() << "Failed to connect to" << portName;
        }
    }

    void readDataFromPort()
    {
        // Читаем данные из порта
        while (serialPort.bytesAvailable() >= 6) {
            // Чтение байт для целой части X, Y и Z
            QByteArray data = serialPort.read(6);

            // Извлечение целых частей X, Y и Z
//            qint16 integerX = (static_cast<quint8>(data[0]))
            float x = (float)(data[0] + data[1] / 100.0);
            float y = (float)(data[2] + data[3] / 100.0);
            float z = (float)(data[4] + data[5] / 100.0);
//            qint16 integerY = (static_cast<quint8>(data[2]))
//                    | static_cast<quint8>(data[3]);
//            qint16 integerZ = (static_cast<quint8>(data[4]))
//                    | static_cast<quint8>(data[5]);

            // Преобразование целых частей в дробные значения float
//            float x = static_cast<float>(integerX) / 100.0f; // Предполагаем, что дробная часть состоит из двух знаков
//            float y = static_cast<float>(integerY) / 100.0f;
//            float z = static_cast<float>(integerZ) / 100.0f;

            // Вывод значений в консоль для проверки
            qDebug() << "x:" << x << ", y:" << y << ", z:" << z;

            // Отправка сигнала с новыми данными
            emit newDataReceived(x, y, z);
        }
    }



private:
    QSerialPort serialPort; // Объект для работы с COM-портом
};

#endif // SERIALPORTREADER_H
