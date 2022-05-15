#ifndef REGISTER_H
#define REGISTER_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>

namespace Ui {
class Register;
}

class Register : public QDialog
{
    Q_OBJECT

public:
    explicit Register(QWidget *parent = 0);
    ~Register();

private slots:
    void on_cancel_clicked();

    void on_ok_clicked();

private:
    Ui::Register *ui;
     QSqlDatabase db;
};

#endif // REGISTER_H
