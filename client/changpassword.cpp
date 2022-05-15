#include "changpassword.h"
#include "ui_changpassword.h"
#include <QDebug>
ChangPassword::ChangPassword(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChangPassword)
{
    ui->setupUi(this);
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


}

ChangPassword::~ChangPassword()
{
    delete ui;
}
