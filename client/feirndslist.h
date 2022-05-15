#ifndef FEIRNDSLIST_H
#define FEIRNDSLIST_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QListWidgetItem>
#include <QTcpSocket>
namespace Ui {
class FeirndsList;
}

class FeirndsList : public QDialog
{
    Q_OBJECT

public:
    explicit FeirndsList(QWidget *parent = 0);
    ~FeirndsList();



private slots:
    void on_change_password_clicked();

private:
    Ui::FeirndsList *ui;
    QSqlDatabase db;
};

#endif // FEIRNDSLIST_H
