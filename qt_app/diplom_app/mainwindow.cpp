#include "mainwindow.h"
#include <QPushButton>
#include <QSizePolicy>
#include <QLineEdit>
#include <QLabel>
#include "qcustomplot.h"
//#include "datadisplayer.h"
//#include "testgenerator.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

    // Это наш приватный член, ему не нужен родитель
    // поскольку он просто за разметку отвечает
    mainLayout = new QVBoxLayout;


    // Установка основного виджета для размещения элементов интерфейса
    QWidget *centralWidget = new QWidget(this);
    // Центральный виджет занимает основное пространство внутри главного окна
    // и обычно используется для размещения основного содержимого приложения.
    setCentralWidget(centralWidget);

    // Графики
//    QCustomPlot *plot1 = new QCustomPlot(centralWidget);
//    QCustomPlot *plot2 = new QCustomPlot(centralWidget);
//    QCustomPlot *plot3 = new QCustomPlot(centralWidget);
    // QCustomPlot *plot4 = new QCustomPlot(centralWidget);

    plot1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    plot2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    plot3->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
 //   plot4->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Настроим графики
    plot1->xAxis->setRange(0, 5);
    plot1->yAxis->setRange(0, 4);

    // Получите объект оси y
    QCPAxisRect *axisRect = plot2->axisRect();
    QCPAxis *yAxis = axisRect->axis(QCPAxis::atLeft);
    // Переверните ось y
    yAxis->setRangeReversed(true);
    yAxis->setRange(0, 10);
    plot2->xAxis2->setRange(0, 5);
    plot2->xAxis->setTicks(false);
    plot2->xAxis->setTicks(false);
    plot2->xAxis->setTickLabels(false);
    plot2->xAxis->setBasePen(QPen(Qt::NoPen));
    plot2->xAxis2->setVisible(true);

//QCPLineEnding
//        enum EndingStyle { esNone          ///< No ending decoration
//                           ,esFlatArrow    ///< A filled arrow head with a straight/flat back (a triangle)
//                           ,esSpikeArrow   ///< A filled arrow head with an indented back
//                           ,esLineArrow    ///< A non-filled arrow head with open back
//                           ,esDisc         ///< A filled circle
//                           ,esSquare       ///< A filled square
//                           ,esDiamond      ///< A filled diamond (45 degrees rotated square)
//                           ,esBar          ///< A bar perpendicular to the line
//                           ,esHalfBar      ///< A bar perpendicular to the line, pointing out to only one side (to which side can be changed with \ref setInverted)
//                           ,esSkewedBar    ///< A bar that is skewed (skew controllable via \ref setLength)





    plot3->xAxis->setRange(0, 10);
    plot3->yAxis->setRange(0, 4);

    // Добавим данные
    // Создайте график
    QCPGraph *graph1 = plot1->addGraph();
    QCPGraph *graph2 = plot2->addGraph();
    QCPGraph *graph3 = plot3->addGraph();

    // Настройки стиля графика
    graph1->setLineStyle(QCPGraph::lsNone); // Отключить линии между точками
    graph2->setLineStyle(QCPGraph::lsNone); // Отключить линии между точками
    graph3->setLineStyle(QCPGraph::lsNone); // Отключить линии между точками

    // Создайте объект QCPScatterStyle
    QCPScatterStyle scatterStyle;
    scatterStyle.setShape(QCPScatterStyle::ssCircle); // Установите форму точек (круги)
    scatterStyle.setSize(30); // Установите размер точек

    // Установите стиль точек для графика
    graph1->setScatterStyle(scatterStyle);
    graph2->setScatterStyle(scatterStyle);
    graph3->setScatterStyle(scatterStyle);

    // Добавьте точку на график
    double x = 1.0; // Значение по оси X
    double y = 1.0; // Значение по оси Y

    (plot1->graph(0))->addData(x, y);
    plot1->xAxis->setLabel("Ось X");
    plot1->yAxis->setLabel("Ось Y");

    (plot2->graph(0))->addData(x, y);
    plot2->xAxis2->setLabel("Ось X");
    plot2->yAxis->setLabel("Ось Z");

    (plot3->graph(0))->addData(x, y);
    plot3->xAxis->setLabel("Ось Z");
    plot3->yAxis->setLabel("Ось Y");




//    // Перерисуйте график
//    plot1->replot();




    // Создание боковых элементов управления
    QPushButton *button1 = new QPushButton("Нажми\nменя", centralWidget);
    QPushButton *button2 = new QPushButton("Нажми\nменя", centralWidget);
    QPushButton *button3 = new QPushButton("Нажми\nменя", centralWidget);
    QPushButton *button4 = new QPushButton("Нажми\nменя", centralWidget);
    QPushButton *button5 = new QPushButton("Нажми\nменя", centralWidget);
    QPushButton *button6 = new QPushButton("Нажми\nменя", centralWidget);

    // Установите вертикальное растягивание для кнопок
    button1->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    button2->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    button3->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    button4->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    button5->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    button6->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

    // Командная строка
    QLineEdit *lineEdit = new QLineEdit(centralWidget);
//    lineEdit->setFixedHeight(30);       // зададим фиксированную высоту
    // Текстовое поле вывода
    QLabel *output_line = new QLabel("Устройство не подключено", centralWidget);
//    output_line->setFixedHeight(30);       // зададим фиксированную высоту
    output_line->setStyleSheet("background-color: lightblue;");


    // Создайте компоновщик сетки
    QGridLayout *gridLayout = new QGridLayout;

    // Добавьте QGraphicsView в сетку
    gridLayout->addWidget(plot1, 0, 0, 3, 1);  // Первая строка, первый столбец
    gridLayout->addWidget(plot2, 3, 0, 3, 1);  // Вторая строка, первый столбец
    gridLayout->addWidget(plot3, 0, 1, 3, 1);  // Первая строка, второй столбец
//    gridLayout->addWidget(plot4, 3, 1, 3, 1);  // Вторая строка, второй столбец

    gridLayout->addWidget(output_line, 6, 0, 1, 2);  // Третья строка, первый и второй столбец
    gridLayout->addWidget(lineEdit, 7, 0, 1, 2);  // Третья строка, первый и второй столбец

    // Создайте главный компоновщик
    QGridLayout *mainLayout = new QGridLayout(centralWidget);

    // Создайте внутренний компоновщик
    QGridLayout *innerLayout = new QGridLayout;
    innerLayout->addWidget(button1, 0, 0);
    innerLayout->addWidget(button2, 1, 0);
    innerLayout->addWidget(button3, 2, 0);
    innerLayout->addWidget(button4, 3, 0);
    innerLayout->addWidget(button5, 4, 0);
    innerLayout->addWidget(button6, 5, 0);

    // Скомпануем все элементы
    mainLayout->addLayout(gridLayout, 0, 1);
    mainLayout->addLayout(innerLayout, 0, 0);

    centralWidget->setLayout(mainLayout);


    // Задаем связи с backend

    // Создаем объект dysplayer
//    DataDisplayer dysplayer(plot1, plot2, plot3);

    // Создаем тестовый генератор
//    TestGenerator generator;

    // Соединяем сигнал генератора со слотом дисплеера
    // Подключаемся к сигналу для обработки сгенерированных чисел
//    QObject::connect(&generator, SIGNAL(numbersGenerated(double, double, double)), &dysplayer, SLOT(coordinateChanged(double, double, double)));


//    QObject::connect(&generator, &TestGenerator::numbersGenerated, &dysplayer, &DataDisplayer::coordinateChanged);
//    QObject::connect(&generator, &TestGenerator::numbersGenerated, [&](double x, double y, double z) {
//        qDebug() << "Generated numbers: x =" << x << ", y =" << y << ", z =" << z;
//    });


}
