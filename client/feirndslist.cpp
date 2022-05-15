#include "feirndslist.h"
#include "ui_feirndslist.h"
#include <QDebug>
#include "client.h"
#include "chatbox.h"
#include "changpassword.h"

FeirndsList::FeirndsList(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FeirndsList)
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
    //设置好友名称
    QSqlQuery query(db);
    //从登录界面获取ID
    QString sql = "select name from users where id = '"+ID+"'";
    query.exec(sql);
    //获取用户姓名
    query.next();
    QString name = query.value("name").toString();
    //设置好友列表名称
    ui->groupBox->setTitle(name);
//设置好友列表中的好友
    QString friends_id[20];//该用户的好友列表
    sql = "select friends_id from friends where user_id = '"+ID+"'";
    query.exec(sql);
    //把表中的friends_id数据赋值给数组
    int friends_num = 0;//好友数量
    while(query.next()){
        friends_id[friends_num] = query.value("friends_id").toString();
        qDebug()<<friends_id[friends_num]<<endl;
        friends_num++;
    }
//在好友列表中显示好友信息
    for(int i =0;i<friends_num;i++){
        sql = "select name,online from users where id = '"+friends_id[i]+"'";
        query.exec(sql);
        query.next();
        if(query.value("online") == "1")
            ui->list->addItem(query.value("name").toString()+"  (在线)");
        if(query.value("online") == "0")
            ui->list->addItem(query.value("name").toString()+"  (离线)");
    }
    //双击打开对应好友聊天框
    connect(ui->list,&QListWidget::itemDoubleClicked,[=](){
        QString sql = "select name from users where id = '"+friends_id[ui->list->currentIndex().row()]+"'";
        qDebug()<<sql;
        QSqlQuery query(db);
        query.exec(sql);
        qDebug()<<query.next();
        QString friend_name = query.value("name").toString();
        ChatBox *chatbox = new ChatBox(this);
        //设置聊天对象id
        chatbox->id = friends_id[ui->list->currentIndex().row()];
        qDebug()<<"聊天对象id"<<chatbox->id;
        //设置聊天框的信息
        chatbox->setWindowTitle(friend_name);
        chatbox->changeGroupName(friend_name);
        chatbox->show();

    });



}

FeirndsList::~FeirndsList()
{
    delete ui;
    qDebug()<<"下线"<<endl;
    QString sql = "update users set online = '0',ip =null,port=null where id='"+ID+"'";
    QSqlQuery query(db);
    query.exec(sql);
    db.close();

}


void FeirndsList::on_change_password_clicked()
{
    ChangPassword* changepassword = new ChangPassword(this);
    changepassword->show();
}
