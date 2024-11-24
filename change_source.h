#ifndef CHANGE_SOURCE_H
#define CHANGE_SOURCE_H

#include <QWidget>
#include "python_enty.h"
#include <QVector>

namespace Ui {
class change_source;
}

class change_source : public QWidget
{
    Q_OBJECT

public:
    explicit change_source(QWidget *parent = nullptr, QVector <python_enty>  *  aenty=nullptr);
    ~change_source();
    void showEnv();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

private:
    Ui::change_source *ui;
    QVector <python_enty>  * entys = nullptr;
    QWidget *aparent =nullptr;
};

#endif // CHANGE_SOURCE_H
