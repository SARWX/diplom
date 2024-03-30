#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QVBoxLayout>
#include <QGraphicsScene>
//#include "datadisplayer.h"
#include "testgenerator.h"
#include "qcustomplot.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    QCustomPlot *plot1 = new QCustomPlot();
    QCustomPlot *plot2 = new QCustomPlot();
    QCustomPlot *plot3 = new QCustomPlot();
    // Командная строка
    QLineEdit *lineEdit;
    // Текст для окна вывода
    QLabel *output_line;
    // Скролл для окна вывода
    QScrollArea *scrollArea;

private:
    QVBoxLayout *mainLayout;            // Указатель на вертикальный компоновщик QVBoxLayout (расположение частей)
};

#endif // MAINWINDOW_H
