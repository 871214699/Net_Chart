#ifndef CLIENT_H
#define CLIENT_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QTcpSocket>
#include <QHostAddress>
namespace Ui {
class Client;
}
//全局变量
extern QString ID ;
extern QString PASSWORD;
extern QTcpSocket* socket_permanent;//永久套接字
extern QString IP;
extern int PORT;


class Client : public QWidget
{
    Q_OBJECT

public:
    explicit Client(QWidget *parent = 0);
    ~Client();

private slots:
    void on_login_clicked();

    void on_register_2_clicked();

private:
    Ui::Client *ui;
    QSqlDatabase db;
};

#endif // CLIENT_H
