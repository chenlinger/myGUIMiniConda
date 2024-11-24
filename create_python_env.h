#ifndef CREATE_PYTHON_ENV_H
#define CREATE_PYTHON_ENV_H

#include <QWidget>
#include "python_enty.h"
#include <QVector>

namespace Ui {
class create_python_env;
}

class create_python_env : public QWidget
{
    Q_OBJECT

public:
    explicit create_python_env(QWidget *parent = nullptr, QVector <python_enty>  *  aenty=nullptr);
    ~create_python_env();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::create_python_env *ui;
    QVector <python_enty>  * entys = nullptr;
    QWidget * aparent=nullptr;
};

#endif // CREATE_PYTHON_ENV_H
