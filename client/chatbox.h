#ifndef CHATBOX_H
#define CHATBOX_H

#include <QDialog>
#include <QTcpSocket>
#include <QSqlDatabase>
#include <QSqlQuery>
namespace Ui {
class ChatBox;
}

class ChatBox : public QDialog
{
    Q_OBJECT

public:
    explicit ChatBox(QWidget *parent = 0);
    ~ChatBox();
    QString id;//聊天对象id
   // QString name;//聊天对象name，用于设置聊天框的信息

    void changeGroupName(QString temp);
private slots:
    void on_send_clicked();

    void on_send_file_clicked();

private:
    Ui::ChatBox *ui;
    QSqlDatabase db;

};

#endif // CHATBOX_H
