#include "mythread.h"
#include<QDebug>
#include<QHostAddress>
#include <QRegExp>
MyThread::MyThread(QTcpSocket* tcp,QObject *parent) : QThread(parent)
{
    m_tcp = tcp;
}

void MyThread::run()
{
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
    //获取套接字中的IP
    int ipv4 =  m_tcp->peerAddress().toIPv4Address();
    QString IP_NOW = QString("%1.%2.%3.%4").arg((ipv4 >> 24) & 0xFF).arg((ipv4 >> 16) & 0xFF).arg((ipv4 >> 8) & 0xFF) .arg(ipv4 & 0xFF);
    int port = m_tcp->peerPort();


    connect(m_tcp,&QTcpSocket::readyRead,[=](){
        QString recMeg = m_tcp->readAll();
        //如果该信息为“###xxxxxx###”模式则为初次登录
        QString pattern_login("000.*000");
        QRegExp rx_login(pattern_login);
        if(rx_login.exactMatch(recMeg)){
            //客户端登录时获取用户的ID
            recMeg.remove(QChar('0'),Qt::CaseInsensitive);
            qDebug()<<"客户端"+recMeg+"已连接";
            //设置该进程的用户id
            id_user = recMeg;
            //并设置数据库中该用户的ip，port字段
            QString sql = "update users set ip ='"+IP_NOW+"', port="+QString::number(port,10)+ " where id='"+recMeg+"'";
            //操作数据库的对象
            QSqlQuery query(db);
            query.exec(sql);
        }
        else{//将套接字传到主线程转发
            m_tcp->write(recMeg.toUtf8());
            qDebug()<<recMeg.split(";")[0];
            qDebug()<<recMeg.split(";")[3];
            tempid = recMeg.split(";")[0];
            temptexts = recMeg.split(";")[3];
            //聊天对象id , 聊天内容
            //emit sendMessage(recMeg.split(";")[0],recMeg.split(";")[3]);
        }
        qDebug()<<"客户端"+QString::number(port,10)+"输出："+recMeg;
    });
    connect(m_tcp,&QTcpSocket::disconnected,[=](){
        qDebug()<<"客户端断开了连接";
         m_tcp->deleteLater();
    });
    exec();
}
