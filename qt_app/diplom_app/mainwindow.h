#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QVBoxLayout>
#include <QGraphicsScene>
#include "testgenerator.h"
#include "qcustomplot.h"
#include "violation_log.h"
#include "datadisplayer.h"
#include "globals.h"
#include "filterdialog.h"

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


    // void filterViolations() {
    //     qDebug() << "filterViolations clicked";

    //     QList<ViolationLogEntry> all = loadViolationsFromMongo();
    //     QList<ViolationLogEntry> filtered;

    //     for (const auto& entry : all) {
    //         // Пример фильтра
    //         if (entry.severity == 1) {
    //             filtered.append(entry);
    //         }
    //     }

    //     displayViolations(filtered);
    // }

    void filterViolations() {
        qDebug() << "filterViolations clicked";

        FilterDialog dialog(this);  // this — указатель на родительское окно (MainWindow)
        if (dialog.exec() == QDialog::Accepted) {
            // Получаем данные из диалога
            FilterSettings params = dialog.getFilterSettings();  // Структура, которую ты возвращаешь

            QDateTime startTime = params.startTime;
            QDateTime endTime = params.endTime;
            QString sectorId = params.sectorId;
            int severityState = params.severity;

            // При необходимости можешь логгировать:
            qDebug() << "Start:" << startTime << "End:" << endTime << "Sector:" << sectorId << "Severity:" << severityState;

            QList<ViolationLogEntry> filteredViolations = loadViolationsFromMongo(startTime, endTime, sectorId);

            displayViolations(filteredViolations);
        } else {
            qDebug() << "User cancelled filter dialog.";
        }
    }



    void showViolationDetails() {
        qDebug() << "showViolationDetails clicked";
    }

    void exportReport() {
        qDebug() << "exportReport clicked";
    }

    void addViolationComment() {
        qDebug() << "addViolationComment clicked";
    }

    void selectObject() {
        qDebug() << "selectObject clicked";
    }

    void showRouteOnMap() {
        qDebug() << "showRouteOnMap clicked";
    }

    void manageZones() {
        qDebug() << "manageZones clicked";
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

    void showUnitReports() {
        qDebug() << "showUnitReports clicked";
    }

    void violationsByZones() {
        qDebug() << "violationsByZones clicked";
    }

    void timeInZones() {
        qDebug() << "timeInZones clicked";
    }

    void equipmentUsage() {
        qDebug() << "equipmentUsage clicked";
    }

};




#endif // MAINWINDOW_H
