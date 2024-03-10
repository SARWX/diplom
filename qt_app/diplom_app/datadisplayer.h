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

public:
    DataDisplayer(QCustomPlot *plt1, QCustomPlot *plt2, QCustomPlot *plt3)
        : plot1(plt1), plot2(plt2), plot3(plt3) {
    };

public slots:
    // Установить отображение новых координат
    void coordinateChanged (coordinates point, int point_number = 0) {
        (plot1->graph(point_number))->setData({point.x}, {point.y});
        plot1->replot();
        (plot2->graph(point_number))->setData({point.x}, {point.z});
        plot2->replot();
        (plot3->graph(point_number))->setData({point.z}, {point.y});
        plot3->replot();
    }

};

#endif // DATADISPLAYER_H
