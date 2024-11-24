#ifndef PIP_ALTER_H
#define PIP_ALTER_H

#include <QWidget>
#include "python_enty.h"
#include <QProcess>
#include <QVector>

namespace Ui {
class pip_alter;
}

class pip_alter : public QWidget
{
    Q_OBJECT

public:
    explicit pip_alter(QWidget *parent = nullptr, QVector <python_enty>  *  aenty=nullptr);
    ~pip_alter();

private slots:
    void on_pushButton_clicked();
    void onProcessFinished (int exitCode, QProcess::ExitStatus exitStatus);
    void on_switch_python_clicked();

    void on_flush_clicked();

    void on_show_pip_lists_clicked();

    void on_install_clicked();

    //执行安装时阻塞所有其他按钮
    void ban();
    //
    void pick();
    void on_stop_install_clicked();


    void on_pushButton_5_clicked();

private:
    Ui::pip_alter *ui;
    QVector <python_enty>  * entys = nullptr;
       QWidget *aparent =nullptr;
    python_enty currentEnty;
         QProcess *process;                       // QProcess 对象
    QProcess * pipprocess;
};

#endif // PIP_ALTER_H
