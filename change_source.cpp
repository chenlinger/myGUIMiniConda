#include "change_source.h"
#include "ui_change_source.h"
#include <QPushButton>
#include <QTextBrowser>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QWidget>
#include <QString>
#include <QApplication>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <QMessageBox>
#include <QRegularExpression>
#include <QDebug>
#include <QProcess>
#include <QClipboard>
change_source::change_source(QWidget *parent, QVector <python_enty>  *  aenty)
    : QWidget( nullptr)
    , ui(new Ui::change_source),
    entys(aenty)
{
    ui->setupUi(this);
    this->aparent=parent;
       this->setFixedSize(1600,1600);
    this->showEnv();
}

change_source::~change_source()
{
    delete ui;
}

void change_source::showEnv()
{
    this->ui->tableWidget->clear();
    this->ui->comboBox->clear();
    this->ui->tableWidget->setRowCount(this->entys->size());

    this->ui->tableWidget->setColumnCount(3);

    this->ui->tableWidget->setHorizontalHeaderLabels({"环境名子","python 版本","python环境路径"});
    for (int i = 0; i < entys->size(); ++i) {
        const python_enty& env = entys->at(i);
        this->ui->tableWidget->setItem(i, 1, new QTableWidgetItem(QString::number(env.type)));
        this->ui->tableWidget->setItem(i, 0, new QTableWidgetItem(env.name));
        this->ui->tableWidget->setItem(i, 2, new QTableWidgetItem(env.pypath));
        this->ui->comboBox->addItem(env.name);
    }
}

void change_source::on_pushButton_clicked()
{
    this->hide();
  this->aparent->show();
}


void change_source::on_pushButton_2_clicked()
{
    auto currentIndex =this->ui->sources->currentIndex();
    QString source = QString("pip config set global.index-url ");
    qDebug()<<"index "<<currentIndex;
    switch (currentIndex) {
    case 0:
    {
        source=source +"https://pypi.org/simple";
        break;}
    case 1:
    {
         source=source +"https://pypi.tuna.tsinghua.edu.cn/simple";
        break;}
    case 2:
    {
         source=source +"https://mirrors.aliyun.com/pypi/simple";
        break;}
    case 3:
    {
         source=source +"https://pypi.mirrors.ustc.edu.cn/simple";
        break;}
    case 4:
    {
         source=source +"https://mirrors.huaweicloud.com/repository/pypi/simple";
        break;}
    case  5:
    {
         source=source +"https://mirrors.tencent.com/pypi/simple";
        break;}
    default:
         source=source +"https://pypi.org/simple";
        break;
    }

    // 使用 QProcess 来执行该命令
    QProcess *process = new QProcess(this);  // 创建QProcess对象
    process->setProcessChannelMode(QProcess::MergedChannels);  // 合并标准输出和标准错误

    // 设置命令和参数
    process->start("cmd.exe", QStringList() << "/c" << source);

    // 检查命令是否成功启动
    if (!process->waitForStarted()) {
          QMessageBox::warning(nullptr, "失败", "换源失败");
        return;
    }

    // 等待进程完成
    if (!process->waitForFinished()) {
          QMessageBox::warning(nullptr, "失败", "换源失败");
        return;
    }

    // 获取标准输出和错误输出
    qDebug() << "Standard Output: " << process->readAllStandardOutput();
    qDebug() << "Standard Error: " << process->readAllStandardError();

    QMessageBox::information(nullptr, "操作成功", "换源成功");
    // 进程执行完成
    qDebug() << "Process Finished!";
}


void change_source::on_pushButton_3_clicked()
{
  this->showEnv();
}


void change_source::on_pushButton_4_clicked()
{
    auto pyname = this->ui->comboBox->currentText();
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "确认删除", "确定要删除吗?",
                                  QMessageBox::Yes | QMessageBox::No);
    if(!reply)
    {
        return ;
    }
    for (auto it = entys->begin(); it != entys->end(); /*no increment here*/)
    {
        if (it->name == pyname) {
            it->uninstall();
            it->deletefromdb();
            it = entys->erase(it);  // 删除当前元素，并返回下一个元素的迭代器
        } else {
            ++it;  // 如果没有删除，才继续遍历下一个元素
        }
    }
    QMessageBox::information(nullptr, "操作成功", "删除成功");
}


void change_source::on_pushButton_5_clicked()
{
    auto pyname = this->ui->comboBox->currentText();
    for(const auto & py : *(this->entys))
    {
        if(py.name==pyname)
        {
            pyname=py.pypath;
        }
    }

    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(pyname);
     QMessageBox::information(nullptr, "操作成功", "复制成功");
}

