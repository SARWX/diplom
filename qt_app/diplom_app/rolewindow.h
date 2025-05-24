#ifndef ROLEWINDOW_H
#define ROLEWINDOW_H

#include <QDialog>
#include <QString>

class RoleWindow : public QDialog
{
    Q_OBJECT

public:
    explicit RoleWindow(QWidget *parent = nullptr);

    QString selectedRole() const;

private:
    QString role;
};

#endif // ROLEWINDOW_H
