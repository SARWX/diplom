#ifndef DATADISPLAYER_H
#define DATADISPLAYER_H

#include "qcustomplot.h"

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
    void coordinateChanged (double x, double y, double z) {
        (plot1->graph(0))->setData({x}, {y});
        plot1->replot();
        (plot2->graph(0))->setData({x}, {z});
        plot2->replot();
        (plot3->graph(0))->setData({z}, {y});
        plot3->replot();
    }

};

#endif // DATADISPLAYER_H
