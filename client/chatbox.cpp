#include "chatbox.h"
#include "ui_chatbox.h"
#include <QDebug>
#include <QDateTime>
#include <QHostAddress>
#include "client.h"
#include <QDebug>
#include <QListWidgetItem>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QTextStream>
ChatBox::ChatBox(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChatBox)
{

    ui->setupUi(this);

    //数据库连接
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");//主机地址
    db.setPort(3306);//端口 默认3306
    db.setDatabaseName("users");//数据库名称
    db.setUserName("root");//数据库的用户名
    db.setPassword("root");//数据库密码
    if(!db.open())//打开数据库并判断数据是否打开成功
        qDebug()<<"failed";
    else
        qDebug()<<"success";

    //与服务器连接的相关信号
    connect(socket_permanent,&QTcpSocket::connected,[=](){
        ui->output->addItem("与服务器连接成功");
    });
    connect(socket_permanent,&QTcpSocket::readyRead,[=](){
        QString recMsg = socket_permanent->readAll();
        QDateTime current_date_time =QDateTime::currentDateTime();
        QString current_Msg =current_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz ddd");
        //接收文件
        //获取当前时间作为文件名
        QString pattern_recFile("###.*###");
        QRegExp rx_recFile(pattern_recFile);
        if(rx_recFile.exactMatch(recMsg)){//匹配成功则为发送文件
            QDateTime current_date_time =QDateTime::currentDateTime();
            QString current_Msg =current_date_time.toString("yyyy.MM.dd hh.mm.ss.zzz ddd");
            recMsg.remove(QChar('#'),Qt::CaseInsensitive);
            QString file_address ="D:/test/" + current_Msg+ ".txt";
            QFile *file = new QFile(file_address);
            file->open(QFile::WriteOnly);
            QTextStream stream(file);
            stream<<recMsg;
            file->close();
            ui->output->addItem(file_address);
            QMessageBox::information(this, "提示", "文件已成功接收",
                                     QMessageBox::Ok,QMessageBox::NoButton);
        }

        //接收聊天消息
        current_Msg+="\n"+recMsg;
        ui->output->addItem(current_Msg);
        ui->output->scrollToBottom();

    });
    connect(socket_permanent,&QTcpSocket::disconnected,[=](){
        ui->output->addItem("服务器断开连接");
    });
}

ChatBox::~ChatBox()
{
    db.close();
    delete ui;
    delete this;
}
//设置聊天框名称函数
void ChatBox::changeGroupName(QString temp){
    ui->groupBox->setTitle(temp);
}
//发送消息按钮
void ChatBox::on_send_clicked()
{

    QSqlQuery query(db);
    //发送消息样式设置
    QString sendMsg = ui->input->toPlainText();//文本框中的输入
    QListWidgetItem *item =new QListWidgetItem();
    item->setTextAlignment(Qt::AlignRight);
    ui->input->clear();
    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_Msg =current_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz ddd");
    current_Msg+="\n"+sendMsg;
    item->setText(current_Msg);
    ui->output->addItem(item);
    ui->output->scrollToBottom();
    //将消息发送给服务器标注聊天对象的套接字
    QString sql = "select name,ip,port from users where id = '"+id+"'";
    query.exec(sql);
    if(query.next()){
        QString ip_friend = query.value("ip").toString();
        QString port_friend = query.value("port").toString();
        sendMsg = id +";" + ip_friend+";"+port_friend+";"+sendMsg;


        qDebug()<<sendMsg<<endl;
    }

    socket_permanent->write(sendMsg.toUtf8());



}
//发送文件

void ChatBox::on_send_file_clicked()
{
    //获取文件路径
    QString path = ui->send_file_address->text();
//    QString path = "D:/test_flie.txt";


    if(path.isEmpty()){
        return;
    }
    //打开文件
    QFile file(path);
    file.open(QFile::ReadOnly);

    //显示文件内容
    QString file_contents = "###"+id+";";
    while(!file.atEnd()){
        QString line = file.readLine();
        file_contents+=line;
    }
    file_contents += "###";
    socket_permanent->write(file_contents.toUtf8());
    qDebug()<<"文件中的内容"<<file_contents<<endl;

}
