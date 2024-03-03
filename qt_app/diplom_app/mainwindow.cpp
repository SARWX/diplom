#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Создание QGraphicsView и QVBoxLayout
    graphicsView = new QGraphicsView(this);
    mainLayout = new QVBoxLayout;

    // Установка основного виджета для размещения элементов интерфейса
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // Установка главного слоя в центральный виджет
    centralWidget->setLayout(mainLayout);

    // Создание и добавление первой сцены
    QGraphicsScene *scene1 = new QGraphicsScene(this);
    scene1->setBackgroundBrush(Qt::yellow);
    graphicsView->setScene(scene1);
    mainLayout->addWidget(graphicsView);

    // Создание и добавление второй сцены
    QGraphicsScene *scene2 = new QGraphicsScene(this);
    scene2->setBackgroundBrush(Qt::green);
    QGraphicsView *graphicsView2 = new QGraphicsView(this);
    graphicsView2->setScene(scene2);
    mainLayout->addWidget(graphicsView2);
}
