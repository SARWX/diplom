#include "mainwindow.h"
#include <QPushButton>
#include <QSizePolicy>
#include <QLineEdit>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Создание QGraphicsView и QVBoxLayout

    // Поскольку это виджет, то зададим его родителем MainWindow (тогда они уничтожатся вместе)
    // Вообще можно этого и не делать, потому, что родителем будет назначен вызвавший его виджет
    // т.е. MainWindow в нашем случае, но все же.
    graphicsView1 = new QGraphicsView(this);
    graphicsView2 = new QGraphicsView(this);
    graphicsView3 = new QGraphicsView(this);
    graphicsView4 = new QGraphicsView(this);

    // Это наш приватный член, ему не нужен родитель
    // поскольку он просто за разметку отвечает
    mainLayout = new QVBoxLayout;


    // Установка основного виджета для размещения элементов интерфейса
    QWidget *centralWidget = new QWidget(this);
    // Центральный виджет занимает основное пространство внутри главного окна
    // и обычно используется для размещения основного содержимого приложения.
    setCentralWidget(centralWidget);


    // Создание и добавление первой сцены
    QGraphicsScene *scene1 = new QGraphicsScene(this);
    scene1->setBackgroundBrush(Qt::yellow);
    graphicsView1->setScene(scene1);

    // Создание и добавление второй сцены
    QGraphicsScene *scene2 = new QGraphicsScene(this);
    scene2->setBackgroundBrush(Qt::green);
    graphicsView2->setScene(scene2);


    // Создание и добавление третьей сцены
    QGraphicsScene *scene3 = new QGraphicsScene(this);
    scene3->setBackgroundBrush(Qt::blue);
    graphicsView3->setScene(scene3);

    // Создание и добавление четвертой сцены
    QGraphicsScene *scene4 = new QGraphicsScene(this);
    scene4->setBackgroundBrush(Qt::cyan);
    graphicsView4->setScene(scene4);

    // Создание боковых элементов управления
    QPushButton *button1 = new QPushButton("Нажми\nменя", centralWidget);
    QPushButton *button2 = new QPushButton("Нажми\nменя", centralWidget);
    QPushButton *button3 = new QPushButton("Нажми\nменя", centralWidget);
    QPushButton *button4 = new QPushButton("Нажми\nменя", centralWidget);
    QPushButton *button5 = new QPushButton("Нажми\nменя", centralWidget);
    QPushButton *button6 = new QPushButton("Нажми\nменя", centralWidget);

    // Командная строка
    QLineEdit *lineEdit = new QLineEdit(centralWidget);
    // Текстовое поле вывода
    QLabel *output_line = new QLabel("Устройство не подключено", centralWidget);
    output_line->setStyleSheet("background-color: lightblue;");


    // Создайте компоновщик сетки
    QGridLayout *gridLayout = new QGridLayout(centralWidget);

    // Добавьте QGraphicsView в сетку
    gridLayout->addWidget(graphicsView1, 0, 1, 3, 1);  // Первая строка, первый столбец
    gridLayout->addWidget(graphicsView2, 3, 1, 3, 1);  // Вторая строка, первый столбец
    gridLayout->addWidget(graphicsView3, 0, 2, 3, 1);  // Первая строка, второй столбец
    gridLayout->addWidget(graphicsView4, 3, 2, 3, 1);  // Вторая строка, второй столбец
    gridLayout->addWidget(button1, 0, 0, 1, 1);  // Третья строка, первый и второй столбец
    gridLayout->addWidget(button2, 1, 0, 1, 1);  // Третья строка, первый и второй столбец
    gridLayout->addWidget(button3, 2, 0, 1, 1);  // Третья строка, первый и второй столбец
    gridLayout->addWidget(button4, 3, 0, 1, 1);  // Третья строка, первый и второй столбец
    gridLayout->addWidget(button5, 4, 0, 1, 1);  // Третья строка, первый и второй столбец
    gridLayout->addWidget(button6, 5, 0, 1, 1);  // Третья строка, первый и второй столбец
    gridLayout->addWidget(output_line, 6, 1, 1, 2);  // Третья строка, первый и второй столбец
    gridLayout->addWidget(lineEdit, 7, 1, 1, 2);  // Третья строка, первый и второй столбец
    // Установите растяжение всех строк
    int numRows = gridLayout->rowCount();
    for (int row = 0; row < numRows; row++) {
        gridLayout->setRowStretch(row, 1);
    }
}
