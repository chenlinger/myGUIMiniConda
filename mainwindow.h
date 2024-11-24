#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "create_python_env.h"
#include "clg_cmd.h"
#include "change_source.h"
#include "pip_alter.h"
#include "python_enty.h"
#include <QVector>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void init();


private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();


private:
    Ui::MainWindow *ui;
    QVector <python_enty>  * entys = nullptr;
    create_python_env *create_view;
    change_source * change_source_view;
    pip_alter * pip_view;
    clg_cmd * cmd_view;


};
#endif // MAINWINDOW_H
