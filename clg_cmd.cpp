#include "clg_cmd.h"
#include "ui_clg_cmd.h"

clg_cmd::clg_cmd(QWidget *parent, QVector <python_enty>  *  aenty)
    : QWidget(nullptr)
    , ui(new Ui::clg_cmd),
    entys(aenty)
{
    ui->setupUi(this);
    this->aparent=parent;
      this->setFixedSize(1600 ,1600);
}

clg_cmd::~clg_cmd()
{
    delete ui;
}

void clg_cmd::on_pushButton_clicked()
{
    this->hide();
    this->aparent->show();
}

