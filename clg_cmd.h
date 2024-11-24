#ifndef CLG_CMD_H
#define CLG_CMD_H

#include <QWidget>
#include "python_enty.h"
#include <QVector>

namespace Ui {
class clg_cmd;
}

class clg_cmd : public QWidget
{
    Q_OBJECT

public:
    explicit clg_cmd(QWidget *parent = nullptr, QVector <python_enty>  *  aenty=nullptr);
    ~clg_cmd();

private slots:
    void on_pushButton_clicked();

private:
    Ui::clg_cmd *ui;
    QVector <python_enty>  * entys = nullptr;
       QWidget *aparent =nullptr;
};

#endif // CLG_CMD_H
