#include "pip_alter.h"
#include "ui_pip_alter.h"
#include <QMessageBox>
#include <qDebug>
#include <QClipboard>
pip_alter::pip_alter(QWidget *parent, QVector <python_enty>  *  aenty)
    : QWidget(nullptr)
    , ui(new Ui::pip_alter),
    entys(aenty)
{
    ui->setupUi(this);
       this->setFixedSize(1600 ,1600);
     this->aparent=parent;
       this->process=new QProcess(this);
     this->pipprocess=new QProcess(this);

     connect(process, &QProcess::readyReadStandardOutput, this, [=](){
           QString output = process->readAllStandardOutput();
           this->ui->cmdView->append(output);
       });
     connect(process, &QProcess::readyReadStandardError, this, [=](){
         QString output = process->readAllStandardError();
         this->ui->cmdView->append(output);
     });
     connect(process,
             QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
             this,
             &pip_alter::onProcessFinished);
}

pip_alter::~pip_alter()
{
    delete ui;
}

void pip_alter::on_pushButton_clicked()
{
    this->hide();
this->aparent->show();
}

void pip_alter::onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    if (exitStatus == QProcess::NormalExit) {
        QMessageBox::information(nullptr, "成功", "Pip 包已成功安装！");
    } else {
        QMessageBox::warning(nullptr, "失败", "Pip 包安装失败，请检查！");
    }
    this->pick();
    this->ui->show_pip_lists->click();
}


void pip_alter::on_switch_python_clicked()
{

    for (int i = 0; i < entys->size(); ++i) {

        const python_enty& env = entys->at(i);
        qDebug()<<"python index : "<<i<<", python name "<<env.name;
        if(env.name==this->ui->comboBox->currentText()){
            this->currentEnty=env;
             QMessageBox::information(nullptr, "操作成功", "选择成功,当前python  ！");
                 qDebug()<<"ok : "<<i<<", python name "<<env.name;
             this->ui->show_pip_lists->click();
            return ;
        }
    }
   QMessageBox::warning(nullptr, "失败", "未知错误");

}


void pip_alter::on_flush_clicked()
{
    this->ui->comboBox->clear();
    for (int i = 0; i < entys->size(); ++i) {
        const python_enty& env = entys->at(i);
        this->ui->comboBox->addItem(env.name);
    }
    this->ui->piplistwidget->clear();
    this->ui->piplistwidget->setColumnCount(0);
}


void pip_alter::on_show_pip_lists_clicked()
{
    if(!this->currentEnty.pypath.size())
    {
            QMessageBox::warning(nullptr, "失败", "请先点击刷新选择一个python");
        return ;
    }
    this->ui->piplistwidget->clear();
     this->ui->piplistwidget->setColumnCount(0);
    this->ui->cmdView->clear();
    QString cmd =this->currentEnty.pypath;


    QString listcmd = cmd + "/python.exe -m pip list";
    qDebug()<<listcmd;

    pipprocess->start(listcmd);

    // 等待进程执行完成
    if (!pipprocess->waitForFinished()) {
        QMessageBox::warning(nullptr, "失败", "无法执行命令！");
        return;
    }

    // 读取标准输出并保存到 QString
    QByteArray output = pipprocess->readAllStandardOutput();
    QString outputStr = QString::fromUtf8(output);
    // qDebug()<<"pip list : "<<output;



      QVector<std::pair<QString, QString>> pipPackages;
   QRegularExpression regex(R"(^(\S+)\s+([\d\.]+.*)$)");

      QString splitstr = QString("\r\n");
    // 按行分割输出
       QList<QByteArray> lines  = output.split('\n');

      // 遍历每一行并应用正则表达式
      for (const QByteArray &line : lines)
      {
          QString lineStr = QString::fromUtf8(line);
          QRegularExpressionMatch match = regex.match(lineStr);

          if (match.hasMatch())
          {
              // 提取包名和版本
              QString packageName = match.captured(1);
              QString packageVersion = match.captured(2);
                packageVersion.replace("\r", "");
              // 将结果添加到 QVector 中
              pipPackages.append(std::make_pair(packageName, packageVersion));
          }
      }

    // 输出结果（仅调试用）

      this->ui->piplistwidget->setRowCount(pipPackages.size());

      this->ui->piplistwidget->setColumnCount(5);

      this->ui->piplistwidget->setHorizontalHeaderLabels({"序号","包名子","包版本","是否更新","是否删除"});
      for(int i =0;i<pipPackages.size();i++)
      {
          this->ui->piplistwidget->setItem(i, 0, new QTableWidgetItem(QString::number(i+1)));
          this->ui->piplistwidget->setItem(i, 1, new QTableWidgetItem(pipPackages.at(i).first));
          this->ui->piplistwidget->setItem(i, 2, new QTableWidgetItem(pipPackages.at(i).second));

          // “是否更新”按钮列
          QPushButton* updateButton = new QPushButton("更新");
          this->ui->piplistwidget->setCellWidget(i, 3, updateButton);

          connect(updateButton, &QPushButton::clicked, this, [=]() {
              QString updateCmd = cmd+"/python -m pip install --upgrade "+pipPackages.at(i).first;
               this->ui->cmdView->clear();
              this->ban();
              process->start(updateCmd);
              if (!process->waitForStarted()) {
                  QMessageBox::warning(nullptr, "失败", "更新pip包失败");
                  return ;
              }

          });


          QPushButton* deleteButton = new QPushButton("删除");
          this->ui->piplistwidget->setCellWidget(i, 4, deleteButton);


          connect(deleteButton, &QPushButton::clicked, this, [=]() {

              QString deletecmd = cmd+"/python -m pip uninstall -y  "+pipPackages.at(i).first;
              QMessageBox::StandardButton reply;
              reply = QMessageBox::question(this, "确认删除", "确定要删除吗?",
                                            QMessageBox::Yes | QMessageBox::No);
              if(!reply)
              {
                  return ;
              }
 this->ui->cmdView->clear();
              this->ban();
              process->start(deletecmd);
              if (!process->waitForStarted()) {
                  QMessageBox::warning(nullptr, "失败", "删除这个包失败");
                  return ;
              }

          });
      }


}


void pip_alter::on_install_clicked()
{
    this->ban();
    if(!this->ui->pipname->text().size())
    {
       QMessageBox::warning(nullptr, "失败", "请先输入一个pip包！");
        return ;
    }
    auto cmd = this->currentEnty.pypath;
    if(!cmd.size())
    {
        QMessageBox::warning(nullptr, "失败", "请先选择一个python环境，请点击刷新！");
        return ;
    }

    cmd = cmd + "/python.exe -m pip install "+this->ui->pipname->text();
    qDebug()<<cmd;
    process->start(cmd);
    if (!process->waitForStarted()) {
        QMessageBox::warning(nullptr, "失败", "无法安装这个python");
        return ;
    }


}

void pip_alter::ban()
{
    this->ui->flush->setDisabled(true);
    this->ui->pushButton->setDisabled(true);
    this->ui->install->setDisabled(true);
    this->ui->show_pip_lists->setDisabled(true);
    this->ui->switch_python->setDisabled(true);
}

void pip_alter::pick()
{

    this->ui->flush->setDisabled(false);
    this->ui->pushButton->setDisabled(false);
    this->ui->install->setDisabled(false);
    this->ui->show_pip_lists->setDisabled(false);
    this->ui->switch_python->setDisabled(false);
}


void pip_alter::on_stop_install_clicked()
{
    if (process->state() != QProcess::NotRunning) {
        process->kill();  // 强制终止进程
    }
    this->ui->flush->setDisabled(false);
    this->ui->pushButton->setDisabled(false);
    this->ui->install->setDisabled(false);
    this->ui->show_pip_lists->setDisabled(false);
    this->ui->switch_python->setDisabled(false);
}


void pip_alter::on_pushButton_5_clicked()
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

