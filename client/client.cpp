#include "client.h"
#include "ui_client.h"
#include <QLineEdit>
#include <QDebug>
#include <QMessageBox>
#include "feirndslist.h"
#include "register.h"

QString ID = NULL;
QString PASSWORD = NULL;
QTcpSocket* socket_permanent = new QTcpSocket;
//建立该用户与服务器连接的永久套接字
//指定服务器IP
QString IP = "127.0.0.1";
//指定服务器端口号port
int PORT = 30000;

Client::Client(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Client)
{
    ui->setupUi(this);
    //设置密码不可见
    ui->password->setEchoMode(QLineEdit::Password);
}

Client::~Client()
{
    delete ui;

}

void Client::on_login_clicked()
{
    //数据库连接
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");//主机地址
    db.setPort(3306);//端口 默认3306
    db.setDatabaseName("users");//数据库名称
    db.setUserName("root");//数据库的用户名
    db.setPassword("root");//数据库密码
    if(!db.open())
        qDebug()<<"failed";
    else
        qDebug()<<"success";
    //登录检测
    ID = ui->id->text();
    PASSWORD = ui->password->text();
    //查询数据库语句
    QSqlQuery query(db);
    QString sql = "select *from users where id = '"+ID+"'";
    qDebug()<<sql<<endl;
    query.exec(sql);
    if(!query.next()){
        qDebug()<<"不存在 "+ID+" 用户"<<endl;
        QMessageBox::warning(this,"警告","不存在该用户");
    }
    else{
         sql = "select *from users where id = '"+ID+"' "+"and password='"+PASSWORD+"'";
         qDebug()<<sql<<endl;
         query.exec(sql);
         if(!query.next()){
            QMessageBox::warning(this,"警告","用户名与密码不匹配");
        }
         //登陆成功
         else{
             //设置在线状态
             sql = "update users set online = '1' where id='"+ID+"'";
             query.exec(sql);
             qDebug()<<sql<<endl;
             db.close();

             //套接字和服务器连接
             socket_permanent->connectToHost(QHostAddress(IP),PORT);

             FeirndsList *friendlist = new FeirndsList(this);
             friendlist->show();
             this->hide();

             //向服务器发送该用户的ID
             QString sendID = "000"+ID+"000" ;
             socket_permanent->write(sendID.toUtf8());
         }
    }
}

void Client::on_register_2_clicked()
{
    Register *register_user = new Register(this);
    register_user->show();
}
