#ifndef CHATBOX_H
#define CHATBOX_H

#include <QWidget>

namespace Ui {
class ChatBox;
}

class ChatBox : public QWidget
{
    Q_OBJECT

public:
    explicit ChatBox(QWidget *parent = 0);
    ~ChatBox();
    QString name;
    QString id;
private:
    Ui::ChatBox *ui;
};

#endif // CHATBOX_H
