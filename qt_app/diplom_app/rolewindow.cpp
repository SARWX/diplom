#include "rolewindow.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

RoleWindow::RoleWindow(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Выбор роли");

    auto *layout = new QVBoxLayout(this);
    layout->addWidget(new QLabel("Выберите свою роль:"));

    // Список ролей
    struct RoleInfo {
        QString label;
        QString key;
    };

    QVector<RoleInfo> roles = {
        {"Администратор системы", "admin"},
        {"Специалист по безопасности", "security_specialist"},
        {"Оперативный дежурный", "duty_officer"},
        {"Руководитель подразделения", "unit_leader"},
        {"Гость / наблюдатель", "guest"}
    };

    // Добавляем кнопки по ролям
    for (const auto &r : roles) {
        QPushButton *button = new QPushButton(r.label);
        layout->addWidget(button);

        // Лямбда захватывает нужное значение роли
        connect(button, &QPushButton::clicked, this, [this, r]() {
            role = r.key;
            accept();
        });
    }
}

QString RoleWindow::selectedRole() const
{
    return role;
}
