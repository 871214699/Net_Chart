#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QSqlDatabase>
#include <QSqlQuery>

class MyThread : public QThread
{
    Q_OBJECT
public:
    explicit MyThread(QTcpSocket* tcp,QObject *parent = nullptr);
    QTcpSocket* m_tcp;
    QString id_user; //对应数据库users表中id字段，每个进程对应一个
    QString tempid;//聊天对象id
    QString temptexts;//聊天内容
protected:
    void run() override;
private:
    QSqlDatabase db;
signals:
   // void sendMessage(QString,QString);//聊天对象的id和聊天内容
public slots:
};

#endif // MYTHREAD_H
