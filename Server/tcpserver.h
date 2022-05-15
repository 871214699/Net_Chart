#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMap>
#include <QSqlDatabase>
#include <QSqlQuery>
#include "mythread.h"
#include <QList>
namespace Ui {
class TcpServer;
}

class TcpServer : public QWidget
{
    Q_OBJECT

public:
    explicit TcpServer(QWidget *parent = 0);
    ~TcpServer();
    //socket连接列表
    QList<MyThread*> socket_connection;
    QString temp_id;
    QString temp_message;
private slots:
    void on_startServe_clicked();
    void on_pushButton_clicked();
    void transferMessage(QString,QString);
private:
    Ui::TcpServer *ui;
    QMap<QString,QTcpSocket*> socket_list;//当前在线用户信息<id,socket>
    QSqlDatabase db;
    QTcpSocket* socket;
    QTcpServer* server;
    int thread_num = 0;//初始线程数量为0
    int online_num = 0;//在线人数




};

#endif // TCPSERVER_H
