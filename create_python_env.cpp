#include "create_python_env.h"
#include "ui_create_python_env.h"
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
create_python_env::create_python_env(QWidget *parent, QVector <python_enty>  *  aenty)
    : QWidget(nullptr)
    , ui(new Ui::create_python_env),
    entys(aenty)
{
    ui->setupUi(this);
      this->setFixedSize(1600 ,1600);
    this->aparent=parent;
}

create_python_env::~create_python_env()
{
    delete ui;
}

void create_python_env::on_pushButton_clicked()
{
    this->hide();
    this->aparent->show();
}


bool isDirectoryEmpty(const QString &path) {
    QDir dir(path);

    // 检查路径是否存在且为目录
    if (!dir.exists()) {
        qDebug() << "Path does not exist or is not a directory:" << path;
        return false;
    }

    // 获取目录内容，排除 "." 和 ".."
    QStringList entries = dir.entryList(QDir::NoDotAndDotDot | QDir::AllEntries);

    // 如果目录中没有其他文件或子目录，则返回 true
    return entries.isEmpty();
}
//创建
void create_python_env::on_pushButton_3_clicked()
{

    auto py_name =this->ui->lineEdit->text();
    // 检查是否为空
    if (py_name.isEmpty()) {
        QMessageBox::warning(nullptr, "输入不合法", "名字不能为空！");
        return;
    }

    // 检查是否包含中文字符
    QRegularExpression regex("[\u4e00-\u9fa5]");
    if (regex.match(py_name).hasMatch()) {
        QMessageBox::warning(nullptr, "输入不合法", "名字不能包含中文字符！");
        return;
    }
    for(const auto & py_ent:*(this->entys))
    {
        if(py_ent.name==py_name)
        {
            QMessageBox::warning(nullptr, "输入不合法", "当前python已经存在");
            return;
        }
    }

    auto py_version = this->ui->comboBox->currentText().toInt();


    auto py_path =this->ui->textBrowser->toPlainText();
    // 检查是否为空
    if (py_path.isEmpty()) {
        QMessageBox::warning(nullptr, "输入不合法", "路径不能为空！");
        return;
    }
    // 检查是否包含中文字符
    if (regex.match(py_path).hasMatch()) {
        QMessageBox::warning(nullptr, "输入不合法", "路径中文字符！");
        return;
    }
    if(!isDirectoryEmpty(py_path))
    {
        QMessageBox::warning(nullptr, "输入不合法", "路径不为空！");
        return;
    }

    python_enty aenty ;
    aenty.name=py_name;
    aenty.type=py_version;
    aenty.pypath=py_path;
    if( !aenty.install())
    {
         QMessageBox::warning(nullptr, "输入不合法", "安装失败！");
        return ;
    }


    //向数据库中注册
    if(!aenty.regeistToDb())
    {
        aenty.uninstall();
        QMessageBox::warning(nullptr, "输入不合法", "向数据库注册失败");
        return ;
    }

    this->entys->push_back(aenty);
    QMessageBox::information(nullptr, "操作成功", "创建成功");
    this->ui->lineEdit->clear();
    this->ui->textBrowser->clearHistory();
}

//选择位置
void create_python_env::on_pushButton_2_clicked()
{
    QString folderPath = QFileDialog::getExistingDirectory(this, "选择文件夹", "");

    // 如果用户选择了文件夹，则显示路径
    if (!folderPath.isEmpty()) {
        this->ui->textBrowser->setText(folderPath);
    }
}

