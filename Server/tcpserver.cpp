#include "tcpserver.h"
#include "ui_tcpserver.h"
#include <QDebug>
#include <QRegExp>
#include <QMapIterator>


TcpServer::TcpServer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TcpServer)
{

    ui->setupUi(this);
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




    connect(server,&QTcpServer::newConnection,[=](){
        socket = server->nextPendingConnection();
        online_num++;//在线人数加一
        //获取套接字中的IP
        int ipv4 =  socket->peerAddress().toIPv4Address();
        QString IP_NOW = QString("%1.%2.%3.%4").arg((ipv4 >> 24) & 0xFF).arg((ipv4 >> 16) & 0xFF).arg((ipv4 >> 8) & 0xFF) .arg(ipv4 & 0xFF);
        int port = socket->peerPort();
//多线程连接
//socket_connection.append(new MyThread(socket));
//socket_connection[thread_num]->start();
//thread_num++;·

        ui->chartView->addItem ("与客户端建立连接");
        ui->chartView->scrollToBottom();
        //收消息
        connect(socket,&QTcpSocket::readyRead,[=](){
            QString  recMeg =socket ->readAll();
            //第一次登陆设置数据库中的ip就和port
            //如果该信息为“###xxxxxx###”模式则为初次登录
            QString pattern_login("000.*000");
            QRegExp rx_login(pattern_login);
            if(rx_login.exactMatch(recMeg)){
                //客户端登录时获取用户的ID
                recMeg.remove(QChar('0'),Qt::CaseInsensitive);
                ui->chartView->addItem("客户端"+recMeg+"已连接");
                ui->chartView->scrollToBottom();
                //将用户信息存入map中
                socket_list.insert(recMeg,socket);
                //并设置数据库中该用户的ip，port字段
                QString sql = "update users set ip ='"+IP_NOW+"', port="+QString::number(port,10)+ " where id='"+recMeg+"'";
                //操作数据库的对象
                QSqlQuery query(db);
                query.exec(sql);
                recMeg = "";
                qDebug()<<"现存连接"<<endl;
                qDebug()<<socket_list.keys()<<endl;
                qDebug()<<socket_list.values()<<endl;
            }





            //遍历已登录列表,判断是哪个客户端发送的数据
//            QMapIterator<QString,QTcpSocket*> iterator(socket_list);
//            while(iterator.hasNext()){
//                iterator.next();
//                QString recMeg_temp = iterator.value()->readAll();
//                qDebug()<<recMeg_temp;
//                if(recMeg_temp!=NULL){//不为空说明是该套接字发送的信息
//                    //将套接字中的内容赋值给recMeg
//                    recMeg = recMeg_temp;
//                }
//            }



            //获取到正确的IO信息recMeg
            if(recMeg!= NULL){
                //判断为发送文件
                //协议为 id；文件内容
                QString pattern_recFile("###.*###");
                QRegExp rx_recFile(pattern_recFile);
                if(rx_recFile.exactMatch(recMeg)){//匹配成功则为发送文件
                    //解析出需要接受文件的对象id以及文件内容
                    recMeg.remove(QChar('#'),Qt::CaseInsensitive);
                    QString file_id = recMeg.split(";")[0];
                    QString file_content = recMeg.section(';',1,-1);
                    // 加上协议 转发给正确的对象
                    file_content = "###"+file_content+"###";
                    socket = socket_list.value(file_id);
                    socket->write(file_content.toUtf8());
                }
                //判断为聊天信息
                //协议为name；ip；port；聊天信息
                else{
                    QString recMeg_name = recMeg.split(";")[0];
                    QString recMeg_ip = recMeg.split(";")[1];
                    QString recMeg_port = recMeg.split(";")[2];
                    QString recMeg_content = recMeg.section(';',3,-1);
                    // 转发给正确的对象
                    socket = socket_list.value(recMeg_name);
                    socket->write(recMeg_content.toUtf8());
                    ui->chartView->addItem("客户端"+QString::number(port,10)+"输出："+recMeg);
                    ui->chartView->scrollToBottom();
                }
            }
        });
        connect(socket,&QTcpSocket::disconnected,[=](){
            ui->chartView->addItem("客户端断开了连接");
            //获取关闭的套接字信息
            QString id_logout = socket_list.key(socket);
            socket_list.remove(id_logout);
            qDebug()<<"剩余连接"<<endl;
            qDebug()<<socket_list.keys()<<endl;
            qDebug()<<socket_list.values()<<endl;
            socket->deleteLater();
        });
    });

}









TcpServer::~TcpServer()
{
    delete ui;
    db.close();//关闭数据库
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

void TcpServer::transferMessage(QString id, QString messages)
{
    temp_id = id;
    temp_message = messages;

}
