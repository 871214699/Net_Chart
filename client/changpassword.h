#ifndef CHANGPASSWORD_H
#define CHANGPASSWORD_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>

namespace Ui {
class ChangPassword;
}

class ChangPassword : public QDialog
{
    Q_OBJECT

public:
    explicit ChangPassword(QWidget *parent = 0);
    ~ChangPassword();

private:
    Ui::ChangPassword *ui;
    QSqlDatabase db;
};

#endif // CHANGPASSWORD_H
