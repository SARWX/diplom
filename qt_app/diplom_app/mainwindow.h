#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QVBoxLayout>
#include <QGraphicsScene>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private:
    QGraphicsView *graphicsView1;        // Указатель на экземпляр QGraphicsView, ответственного за графики
    QGraphicsView *graphicsView2;
    QGraphicsView *graphicsView3;
    QGraphicsView *graphicsView4;

    QVBoxLayout *mainLayout;            // Указатель на вертикальный компоновщик QVBoxLayout (расположение частей)
};

#endif // MAINWINDOW_H
