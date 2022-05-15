#include "tcpserver.h"
#include "ui_tcpserver.h"
#include <QDebug>
#include "mythread.h"
#include <QRegExp>

TcpServer::TcpServer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TcpServer)
{
    ui->setupUi(this);
    qDebug()<<"111";
    server = new QTcpServer(this);
    socket = new QTcpSocket(this);
    //连接数据库
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");//主机地址
    db.setPort(3306);//端口 默认3306
    db.setDatabaseName("users");//数据库名称
    db.setUserName("root");//数据库的用户名
    db.setPassword("root");//数据库密码
    if(!db.open())//打开数据库并判断是否打开成功
        qDebug()<<"failed";
    else
        qDebug()<<"success";

    //ceshidaima
    MyThread* mythread = new MyThread;
    mythread->start();




    connect(server,&QTcpServer::newConnection,[=](){
        socket = server->nextPendingConnection();

//ceshidaima
mythread->f = 0;

        //获取套接字中的IP
        int ipv4 =  socket->peerAddress().toIPv4Address();
        QString IP_NOW = QString("%1.%2.%3.%4").arg((ipv4 >> 24) & 0xFF).arg((ipv4 >> 16) & 0xFF).arg((ipv4 >> 8) & 0xFF) .arg(ipv4 & 0xFF);
        int port = socket->peerPort();

        qDebug()<<"新登录用户的ip和port"<<endl;
        qDebug()<<IP_NOW<<"\n"<<port<<endl;


        ui->chartView->addItem ("与客户端建立连接");
        ui->chartView->scrollToBottom();
        connect(socket,&QTcpSocket::readyRead,[=](){
            QString recMeg;
            //获取对应套接字发送的信息
            if(socket_list.key(socket)!= NULL){
                recMeg =socket ->readAll();
            }
            //如果该信息为“000xxxxxx000”模式则为初次登录
            QString pattern_login("000.*000");
            QRegExp rx_login(pattern_login);
            if(rx_login.exactMatch(recMeg)){
                //客户端登录时获取用户的ID
                recMeg.remove(QChar('0'),Qt::CaseInsensitive);
                ui->chartView->addItem("客户端"+recMeg+"已连接");
                ui->chartView->scrollToBottom();
                //将用户信息存入map中
                socket_list.insert(recMeg,socket);

                qDebug()<<"登录用户后全部键值对"<<endl;
                qDebug()<<socket_list.keys()<<endl;
                qDebug()<<socket_list.values()<<endl;

                //并设置数据库中该用户的ip，port字段
                QString sql = "update users set ip ='"+IP_NOW+"', port="+QString::number(port,10)+ " where id='"+recMeg+"'";
                //操作数据库的对象
                QSqlQuery query(db);
                query.exec(sql);
            }
            //用户登出时从用户列表中删除用户信息
//            QString pattern_logout(";;;.*;;;");
//            QRegExp rx_logout(pattern_logout);
//            if(rx_logout.exactMatch(recMeg)){
//                recMeg.remove(QChar(';'),Qt::CaseInsensitive);
//                ui->chartView->addItem("客户端"+recMeg+"已断开连接");
//                ui->chartView->scrollToBottom();
//            }

            qDebug()<<recMeg;
            ui->chartView->addItem("客户端"+QString::number(port,10)+"输出："+recMeg);
            ui->chartView->scrollToBottom();

        });
        connect(socket,&QTcpSocket::disconnected,[=](){
            ui->chartView->addItem("客户端断开了连接");
            //获取关闭的套接字信息
            QString id_logout = socket_list.key(socket);
            socket_list.remove(id_logout);

            socket->deleteLater();
            qDebug()<<"退出后剩余键值对"<<endl;
            qDebug()<<socket_list.keys()<<endl;
            qDebug()<<socket_list.values()<<endl;
        });
    });
}









TcpServer::~TcpServer()
{
    delete ui;
   // db.close();//关闭数据库
}

void TcpServer::on_startServe_clicked()
{
    int port = ui->port->text().toInt();
    server->listen(QHostAddress::Any,port);

}

void TcpServer::on_pushButton_clicked()
{
    QString sendMsg = ui->input->toPlainText();
    socket->write(sendMsg.toUtf8());

}
