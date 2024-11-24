#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "QDebug"
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    entys = new QVector <python_enty>  ();

    create_view=new create_python_env (this,this->entys);
     create_view->hide();
    change_source_view=new change_source  (this,this->entys);
      change_source_view->hide();
    pip_view=new pip_alter  (this,this->entys);
       pip_view->hide();
    cmd_view=new clg_cmd  (this,this->entys);
        cmd_view->hide();
    this->setFixedSize(1600 ,1600);
        this->init();


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{
     QString appDir = QCoreApplication::applicationDirPath();
    QString dbFilePath =QString("%1/pyenv.db").arg(appDir);

    // 创建与数据库的连接
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

    // 设置数据库文件路径
    db.setDatabaseName(dbFilePath);
    if (!db.open()) {
        qDebug() << "Database not open: " << db.lastError().text();
        return;  // 如果连接失败，退出函数
    }

    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS python_entries ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "type INTEGER, "
               "name TEXT, "
               "pypath TEXT)");

    query.exec("SELECT * FROM python_entries");

    while (query.next()) {
        int type = query.value(1).toInt();
        QString name = query.value(2).toString();
        QString pypath = query.value(3).toString();
        python_enty env;
        env.type=type;
        env.name=name;
        env.pypath=pypath;
        entys->append(env);
    }
}

//创建
void MainWindow::on_pushButton_clicked()
{
    this->hide();
    qDebug() <<" this->parent()""";

    this->create_view->show();
     qDebug() <<" ok ";
}


//换源
void MainWindow::on_pushButton_2_clicked()
{
    this->hide();
    this->change_source_view->show();
}


//依赖管理
void MainWindow::on_pushButton_3_clicked()
{
    this->hide();
    this->pip_view->show();
}


//命令行
void MainWindow::on_pushButton_4_clicked()
{
    this->hide();
    this->cmd_view->show();
}

