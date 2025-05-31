#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QVBoxLayout>
#include <QGraphicsScene>
#include "testgenerator.h"
#include "qcustomplot.h"
#include "mongodb/violation_log/violation_log.h"
#include "datadisplayer.h"
#include "globals.h"
#include "mongodb/violation_log/filterdialog.h"
#include "mongodb/sector/sector.h"
#include "mongodb/text_database_displayer.h"
#include "mongodb/trackerdb.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(const QString &role, QWidget *parent = nullptr);
    QCustomPlot *plot1 = new QCustomPlot();
    QCustomPlot *plot2 = new QCustomPlot();
    QCustomPlot *plot3 = new QCustomPlot();
    // Метки для отображения значений X, Y, Z
    QLabel *coordinatesLabel = new QLabel();
    // Командная строка
    QLineEdit *lineEdit;
    // Текст для окна вывода
    QLabel *output_line;
    // Скролл для окна вывода
    QScrollArea *scrollArea;
    // Создание боковых элементов управления
    QPushButton *button1 = new QPushButton("Запуск\nестирования");
    QPushButton *button2 = new QPushButton("Остановка\nтестирования");
    QPushButton *button3 = new QPushButton("Отобразить\nкоординаты");
    QPushButton *button4 = new QPushButton("Генерировать\nкоординаты");
    QPushButton *button5 = new QPushButton("Нажми\nменя");
    QPushButton *button6 = new QPushButton("Нажми\nменя");
    QMap<QString, QPushButton*> buttons;

private:
    QVBoxLayout *mainLayout;            // Указатель на вертикальный компоновщик QVBoxLayout (расположение частей)
    QVector<QPushButton*> role_buttons;
    void setupInterfaceForRole(const QString &role);
    void displayViolations(const QList<ViolationLogEntry>& violations);
    QList<ViolationLogEntry> getFilteredViolations();
    TextDataBaseDisplayer* m_textDataBaseDisplayer;

public slots:
    void toggleOutputLineVisibility() {
        if (coordinatesLabel->isVisible()) {
            coordinatesLabel->hide();
        } else {
            coordinatesLabel->show();
        }
    }

    void showViolationIndicators() {
        static bool violation_displayed = false;
        qDebug() << "showViolationIndicators clicked";

        if (violation_displayed) {
            // Просто очищаем графики
            plot1->graph(2)->data()->clear();
            plot2->graph(2)->data()->clear();
            plot3->graph(2)->data()->clear();

            plot1->replot();
            plot2->replot();
            plot3->replot();

            violation_displayed = false;
        } else {
            QList<ViolationLogEntry> violations = loadViolationsFromMongo();
            displayViolations(violations);
            violation_displayed = true;
        }
    }

    // Обработка фильтрации с графическим выводом
    void filterViolations() 
    {
        QList<ViolationLogEntry> violations = getFilteredViolations();
        displayViolations(violations);
    }

    // Обработка фильтрации с текстовым выводом
    void showViolationDetails() 
    {
        QList<ViolationLogEntry> violations = getFilteredViolations();
        QString formattedData = m_textDataBaseDisplayer->prepareViolationsData(violations);
        m_textDataBaseDisplayer->displayData(formattedData, "Violation Report");
    }

    void selectObject() {
        qDebug() << "selectObject clicked";
    }

    void showRouteOnMap() {
        qDebug() << "showRouteOnMap clicked";

        FilterDialog dialog(this);
        if (dialog.exec() != QDialog::Accepted) {
            qDebug() << "User cancelled filter dialog.";
            return;
        }
        FilterSettings params = dialog.getFilterSettings(); // Тут можно специфичное окно поставить
        g_trackerdb->showObjectRoute(params.startTime, params.endTime);
    }

    void manageSectors() {
        qDebug() << "manageSectors clicked";
        QList<SectorEntry> sectors = loadSectorEntryFromMongo();
    }

    void configureRules() {
        qDebug() << "configureRules clicked";
    }

    void linkRulesToObjects() {
        qDebug() << "linkRulesToObjects clicked";
    }

    void createUser() {
        qDebug() << "createUser clicked";
    }

    void assignRoles() {
        qDebug() << "assignRoles clicked";
    }

    void manageZoneAccess() {
        qDebug() << "manageZoneAccess clicked";
    }

    void timeInZones() {
        qDebug() << "timeInZones clicked";
    }

    void equipmentUsage() {
        qDebug() << "equipmentUsage clicked";
    }

};

#endif // MAINWINDOW_H
