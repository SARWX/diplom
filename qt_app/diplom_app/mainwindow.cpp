#include "mainwindow.h"
#include <QPushButton>
#include <QSizePolicy>
#include <QLineEdit>
#include <QLabel>
#include <QScrollArea>
#include <QApplication>
#include <QScreen>
#include "qcustomplot.h"
#include "mongodb/text_database_displayer.h"
//#include "datadisplayer.h"
//#include "testgenerator.h"

#define ADD_ROLE_BUTTON(label, slot)                          \
do {                                                      \
        QPushButton *btn = new QPushButton(label);            \
        connect(btn, &QPushButton::clicked, this, slot);      \
        role_buttons.append(btn);                             \
} while (0)



MainWindow::MainWindow(const QString &role, QWidget *parent)
    : QMainWindow(parent)
{
    // Определим разрешение ээкрана
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int screenWidth = screenGeometry.width();
    int screenHeight = screenGeometry.height();

    // Это наш приватный член, ему не нужен родитель
    // поскольку он просто за разметку отвечает
    mainLayout = new QVBoxLayout;


    // Установка основного виджета для размещения элементов интерфейса
    QWidget *centralWidget = new QWidget(this);
    // Центральный виджет занимает основное пространство внутри главного окна
    // и обычно используется для размещения основного содержимого приложения.
    setCentralWidget(centralWidget);

    plot1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    plot2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    plot3->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
 //   plot4->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Настроим графики
    // график 1
    plot1->xAxis->setRange(0, 10);
    plot1->yAxis->setRange(0, 4);
    // график 2
    // Получите объект оси y
    QCPAxisRect *axisRect = plot2->axisRect();
    QCPAxis *yAxis = axisRect->axis(QCPAxis::atLeft);
    // Переверните ось y
    yAxis->setRangeReversed(true);
    yAxis->setRange(0, 4);
    plot2->xAxis->setRange(0, 10);
    plot2->xAxis2->setRange(0, 10);
    plot2->xAxis->setTicks(false);
    plot2->xAxis->setTicks(false);
    plot2->xAxis->setTickLabels(false);
    plot2->xAxis->setBasePen(QPen(Qt::NoPen));
    plot2->xAxis2->setVisible(true);
    // график 3
    plot3->xAxis->setRange(0, 10);
    plot3->yAxis->setRange(0, 4);

    // Добавим данные
    // Создаем графики (основные)
    QCPGraph *graph1 = plot1->addGraph();
    QCPGraph *graph2 = plot2->addGraph();
    QCPGraph *graph3 = plot3->addGraph();
    // Создаем графики (тестовые)
    QCPGraph *graph1test = plot1->addGraph();
    QCPGraph *graph2test = plot2->addGraph();
    QCPGraph *graph3test = plot3->addGraph();
    // Создаем графики (отображения нарушений)
    QCPGraph *graph1violation = plot1->addGraph();
    QCPGraph *graph2violation = plot2->addGraph();
    QCPGraph *graph3violation = plot3->addGraph();

    // Настройки стиля графика
    graph1->setLineStyle(QCPGraph::lsNone); // Отключить линии между точками
    graph2->setLineStyle(QCPGraph::lsNone); // Отключить линии между точками
    graph3->setLineStyle(QCPGraph::lsNone); // Отключить линии между точками
    // Настройки стиля отображения нарушений
    graph1violation->setLineStyle(QCPGraph::lsNone); // Отключить линии между точками
    graph2violation->setLineStyle(QCPGraph::lsNone); // Отключить линии между точками
    graph3violation->setLineStyle(QCPGraph::lsNone); // Отключить линии между точками

    // Создайте объект QCPScatterStyle - стиль для графика
    QCPScatterStyle scatterStyle;
    scatterStyle.setShape(QCPScatterStyle::ssCircle); // Установите форму точек (круги)
    scatterStyle.setSize(5); // Установите размер точек

    // Установите стиль точек для графика
    graph1->setScatterStyle(scatterStyle);
    graph2->setScatterStyle(scatterStyle);
    graph3->setScatterStyle(scatterStyle);
    // тестовые
    scatterStyle.setSize(30); // Увеличить размер точек
    graph1test->setScatterStyle(scatterStyle);
    graph2test->setScatterStyle(scatterStyle);
    graph3test->setScatterStyle(scatterStyle);
    // Нарушения (1000-1999)
    QCPScatterStyle violationStyle;
    violationStyle.setShape(QCPScatterStyle::ssDisc);  // Сплошной круг
    violationStyle.setSize(10);                        // Размер точки
    violationStyle.setPen(QPen(Qt::red, 1.5));         // Красная обводка (толщина 1.5)
    violationStyle.setBrush(Qt::red);                  // Красная заливк
    graph1violation->setScatterStyle(violationStyle);
    graph2violation->setScatterStyle(violationStyle);
    graph3violation->setScatterStyle(violationStyle);
    // Траектории движения

    // Добавьте точку на график
    double x = 1.0; // Значение по оси X
    double y = 1.0; // Значение по оси Y

    (plot1->graph(0))->addData(x, y);
    (plot1->graph(1))->addData(2.0, 2.0);
    plot1->xAxis->setLabel("Ось X");
    plot1->yAxis->setLabel("Ось Y");

    (plot2->graph(0))->addData(x, y);
    (plot2->graph(1))->addData(2.0, 2.0);
    plot2->xAxis2->setLabel("Ось X");
    plot2->yAxis->setLabel("Ось Z");

    (plot3->graph(0))->addData(x, y);
    (plot3->graph(1))->addData(2.0, 2.0);
    plot3->xAxis->setLabel("Ось Z");
    plot3->yAxis->setLabel("Ось Y");




//    // Перерисуйте график
//    plot1->replot();




    setupInterfaceForRole(role);

    // Установите вертикальное растягивание для кнопок
    button1->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    button2->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    button3->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    button4->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    for (int i = 0; i < role_buttons.size(); ++i) {
        role_buttons[i]->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    }

    // Командная строка       Перенесена в заголовочынй файл
    lineEdit = new QLineEdit(centralWidget);
    // Текстовое поле вывода
    output_line = new QLabel("Устройство не подключено\n", centralWidget);
    output_line->setStyleSheet("background-color: lightblue;");
    output_line->setWordWrap(true); // Включаем перенос текста по словам
    output_line->setTextInteractionFlags(Qt::TextSelectableByMouse); // Разрешаем выделение текста мышью
    // Сделаем текстовое поле вывода прокручиваемым (обернем его объектом типа QScrollArea)
    scrollArea = new QScrollArea(centralWidget);
    scrollArea->setWidgetResizable(true); // Разрешаем масштабирование содержимого
    scrollArea->setWidget(output_line); // Устанавливаем QLabel в качестве виджета для прокрутки
  //  scrollArea->setMaximumHeight(centralWidget->height() * 0.1); // Установка максимальной высоты QScrollArea как 0.1 высоты родительского виджета
    scrollArea->setMaximumHeight(screenHeight * 0.1);


    // Управляющий

    // Создайте компоновщик сетки
    QGridLayout *gridLayout = new QGridLayout;

    // Добавьте QGraphicsView в сетку
    gridLayout->addWidget(plot1, 0, 0, 3, 1);  // Первая строка, первый столбец
    gridLayout->addWidget(plot2, 3, 0, 3, 1);  // Вторая строка, первый столбец
    gridLayout->addWidget(plot3, 0, 1, 3, 1);  // Первая строка, второй столбец
    gridLayout->addWidget(coordinatesLabel, 3, 1, 3, 1);
    // Увеличим шрифт координат
    coordinatesLabel->setStyleSheet("font-size: 30pt;");
//    gridLayout->addWidget(plot4, 3, 1, 3, 1);  // Вторая строка, второй столбец

    gridLayout->addWidget(scrollArea, 6, 0, 1, 2);  // Третья строка, первый и второй столбец
    gridLayout->addWidget(lineEdit, 7, 0, 1, 2);  // Третья строка, первый и второй столбец

    // Создайте главный компоновщик
    QGridLayout *mainLayout = new QGridLayout(centralWidget);

    // Создайте внутренний компоновщик
    QGridLayout *innerLayout = new QGridLayout;
    innerLayout->addWidget(button1, 0, 0);
    innerLayout->addWidget(button2, 1, 0);
    innerLayout->addWidget(button3, 2, 0);
    innerLayout->addWidget(button4, 3, 0);
//    innerLayout->addWidget(buttons["generate_coords"], 4, 0);
//    innerLayout->addWidget(buttons["action_button"], 5, 0);
    for (int i = 0; i < role_buttons.size(); ++i) {
        innerLayout->addWidget(role_buttons[i], (4 + i), 0); // кнопки одна под другой
    }


    // Скомпануем все элементы
    mainLayout->addLayout(gridLayout, 0, 1);
    mainLayout->addLayout(innerLayout, 0, 0);

    centralWidget->setLayout(mainLayout);

    m_textDataBaseDisplayer = new TextDataBaseDisplayer(this);



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

    connect(button3, &QPushButton::clicked, this, &MainWindow::toggleOutputLineVisibility);

}

void MainWindow::setupInterfaceForRole(const QString &role)
{
    role_buttons.clear();

    if (role == "duty_officer" || role == "security_specialist" || role == "unit_leader") {
        ADD_ROLE_BUTTON("Индикация нарушений", &MainWindow::showViolationIndicators);
    }

    if (role == "duty_officer" || role == "security_specialist" || role == "unit_leader") {
        ADD_ROLE_BUTTON("Фильтр нарушений", &MainWindow::filterViolations);
        ADD_ROLE_BUTTON("Детали нарушения", &MainWindow::showViolationDetails);
        ADD_ROLE_BUTTON("Экспорт отчёта", &MainWindow::exportReport);
        ADD_ROLE_BUTTON("Добавить комментарий к нарушению", &MainWindow::addViolationComment);
    }

    if (role == "duty_officer" || role == "security_specialist") {
        ADD_ROLE_BUTTON("Выбрать объект", &MainWindow::selectObject);
        ADD_ROLE_BUTTON("Маршрут на карте", &MainWindow::showRouteOnMap);
    }

    if (role == "admin") {
        ADD_ROLE_BUTTON("Управление зонами", &MainWindow::manageZones);
        ADD_ROLE_BUTTON("Настройка правил", &MainWindow::configureRules);
        ADD_ROLE_BUTTON("Связь правил с объектами", &MainWindow::linkRulesToObjects);

        ADD_ROLE_BUTTON("Создать пользователя", &MainWindow::createUser);
        ADD_ROLE_BUTTON("Назначить роли", &MainWindow::assignRoles);
        ADD_ROLE_BUTTON("Управление доступом к зонам", &MainWindow::manageZoneAccess);
    }

    if (role == "unit_leader" || role == "security_specialist") {
        ADD_ROLE_BUTTON("Сводки по подразделениям", &MainWindow::showUnitReports);
        ADD_ROLE_BUTTON("Нарушения по зонам", &MainWindow::violationsByZones);
        ADD_ROLE_BUTTON("Время в зонах", &MainWindow::timeInZones);
        ADD_ROLE_BUTTON("Использование оборудования", &MainWindow::equipmentUsage);
    }
}

void MainWindow::displayViolations(const QList<ViolationLogEntry>& violations) {
    // Очищаем старые графики
    plot1->graph(2)->data()->clear();
    plot2->graph(2)->data()->clear();
    plot3->graph(2)->data()->clear();

    for (const auto& entry : violations) {
        qDebug() << "Violation:";
        qDebug() << "  ID: " << entry.id;
        qDebug() << "  Object ID: " << entry.object_id;
        qDebug() << "  Sector ID: " << entry.sector_id;
        qDebug() << "  Movement Rule ID: " << entry.movement_rule_id;
        qDebug() << "  Severity: " << entry.severity;
        qDebug() << "  Timestamp: " << entry.timestamp.toString(Qt::ISODate);
        qDebug() << "  Coordinates: (" << entry.coords.x << "," << entry.coords.y << "," << entry.coords.z << ")";

        plot1->graph(2)->addData(entry.coords.x, entry.coords.y);
        plot2->graph(2)->addData(entry.coords.x, entry.coords.z);
        plot3->graph(2)->addData(entry.coords.z, entry.coords.y);
    }

    // Обновляем графики
    plot1->replot();
    plot2->replot();
    plot3->replot();
}

// Общая функция для получения отфильтрованных нарушений
QList<ViolationLogEntry> MainWindow::getFilteredViolations()
{
    FilterDialog dialog(this);
    if (dialog.exec() != QDialog::Accepted) {
        qDebug() << "User cancelled filter dialog.";
        return QList<ViolationLogEntry>();
    }
    
    FilterSettings params = dialog.getFilterSettings();
    qDebug() << "Filter params:" << params.startTime << params.endTime 
             << params.sectorId << params.severity;
    
    return loadViolationsFromMongo(params.startTime, params.endTime, params.sectorId);
}