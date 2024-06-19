#ifndef DATADISPLAYER_H
#define DATADISPLAYER_H

#include "qcustomplot.h"
#include "coordinates.h"

class DataDisplayer : public QObject
{
    Q_OBJECT

private:
    QCustomPlot *plot1;
    QCustomPlot *plot2;
    QCustomPlot *plot3;
    QLabel *coordinatesLabel;

public:
    DataDisplayer(QCustomPlot *plt1, QCustomPlot *plt2, QCustomPlot *plt3, QLabel *coorLabel)
        : plot1(plt1), plot2(plt2), plot3(plt3), coordinatesLabel(coorLabel) {
    };

    int testFlag = 0;

public slots:
    // Установить отображение новых координат
    void coordinateChanged (coordinates point, float r1, float r2, float r3, int point_number = 0) {
        (plot1->graph(point_number))->setData({point.x}, {point.y});
        plot1->replot();
        (plot1->graph(1))->setData({point.x}, {point.y});
        plot1->replot();

        (plot2->graph(point_number))->setData({point.x}, {point.z});
        plot2->replot();
        (plot2->graph(1))->setData({point.x}, {point.z});
        plot2->replot();

        (plot3->graph(point_number))->setData({point.z}, {point.y});
        plot3->replot();
        (plot3->graph(1))->setData({point.z}, {point.y});
        plot3->replot();

       coordinatesLabel->setText(QString("X = %1, \nY = %2, \nZ = %3\nR1 = %4\nR2 = %5\nR3 = %6")
                                 .arg(point.x * 100).arg(point.y * 100).arg(point.z * 100)
                                 .arg(r1).arg(r2).arg(r3));

       // Информация для тестирования
       static QFile xFile("x_data.txt");
       static QFile yFile("y_data.txt");
       static QFile zFile("z_data.txt");

       if (this->testFlag == 1)
       {
           if (xFile.open(QIODevice::Append | QIODevice::Text) &&
               yFile.open(QIODevice::Append | QIODevice::Text) &&
               zFile.open(QIODevice::Append | QIODevice::Text)) {
               QTextStream xStream(&xFile);
               QTextStream yStream(&yFile);
               QTextStream zStream(&zFile);

               QString xString = QString::number(point.x).replace('.', ',');
               QString yString = QString::number(point.y).replace('.', ',');
               QString zString = QString::number(point.z).replace('.', ',');

               xStream << xString << '\n';
               yStream << yString << '\n';
               zStream << zString << '\n';

               xFile.close();
               yFile.close();
               zFile.close();
           } else {
               qWarning() << "Unable to open one or more files for writing.";
           }
       }


    }

    void startTesting() {
        this->testFlag = 1;
        // Дополнительный код для начала тестирования (если необходимо)
    }

    void stopTesting() {
        this->testFlag = 0;
        // Дополнительный код для остановки тестирования (если необходимо)
    }



};

#endif // DATADISPLAYER_H
