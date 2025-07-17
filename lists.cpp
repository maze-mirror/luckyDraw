#include "lists.h"
#include "ui_lists.h"
#include<QWidget>
#include<QPushButton>

lists::lists(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::lists)
{
    ui->setupUi(this);
    this->setWindowTitle("名单");

    //设置按键
    QPushButton*btn1=new QPushButton(this);
    btn1->setText("导入名单");
    btn1->move(10,10);
    //connect
    QPushButton*btn2=new QPushButton(this);
    btn2->setText("返回");
    btn2->move(120,140);
    //connect

}

lists::~lists()
{
    delete ui;
}
