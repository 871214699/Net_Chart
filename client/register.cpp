#include "register.h"
#include "ui_register.h"
#include <QMessageBox>
#include <QDebug>
Register::Register(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Register)
{
    ui->setupUi(this);

}

Register::~Register()
{
    delete ui;
}

void Register::on_cancel_clicked()
{
    this->close();
}

void Register::on_ok_clicked()
{
    //连接数据库
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
    //检查是否填写完整
    if(ui->id->text().isEmpty()|ui->name->text().isEmpty()|ui->pd->text().isEmpty()|ui->pdagain->text().isEmpty()){
         QMessageBox::warning(this,"警告","请将注册信息填写完整");
    }
    //查找id是否重复
    QSqlQuery query(db);
    QString sql = "select * from users where id='"+ui->id->text()+"'";
    query.exec(sql);
    if(query.next()){
        QMessageBox::warning(this,"警告","已存在该用户");

    }
    else{//是否有重名
        sql = "select * from users where name='"+ui->name->text()+"'";
        query.exec(sql);
        if(query.next()){
             QMessageBox::warning(this,"警告","昵称以重复，请修改昵称");
        }
        else{//判断两次密码是否一致
            if(ui->pd->text()!=ui->pdagain->text()){
                QMessageBox::warning(this,"警告","两次密码不相同");
            }
            else{//正确创建用户
                sql = "insert into users set id='"+ui->id->text()+"',name='"+ui->name->text()+"',password='"+ui->pd->text()+"'";
                query.exec(sql);
                db.close();
                this->close();
            }
        }
    }

}
