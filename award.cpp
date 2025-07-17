#include "award.h"
#include "ui_award.h"
#include<QWidget>
#include<QPushButton>
award::award(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::award)
{
    ui->setupUi(this);
    this->setWindowTitle("抽奖");
    QPushButton*btn1=new QPushButton(this);
    btn1->setText("返回");
}

award::~award()
{
    delete ui;
}
