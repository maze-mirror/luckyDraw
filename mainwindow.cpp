#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include<QWidget>
#include<Qdebug>
#include<QPushButton>
#include<QLabel>
#include"lists.h"
#include"settings.h"
#include"award.h"
#include"random.h"

mainWindow::mainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::mainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("抽选助手");

    //名单
    QPushButton *list=new QPushButton(this);
    list->setText("名单");
    list->move(90,210);
    connect(list,&QPushButton::clicked,this,&mainWindow::pushBtnList);

    //设置
    QPushButton*setting=new QPushButton(this);
    setting->setText("设置");
    setting->move(160,250);
    connect(setting,&QPushButton::clicked,this,&mainWindow::pushBtnSetting);

    //抽奖
    QPushButton*awardBtn=new QPushButton(this);
    awardBtn->setText("进行抽奖");
    awardBtn->move(50,50);
    connect(awardBtn,&QPushButton::clicked,this,&mainWindow::pushBtnAward);

    //点人
    QPushButton*randomBtn=new QPushButton(this);
    randomBtn->setText("进行随机点名");
    randomBtn->move(70,70);
    connect(randomBtn,&QPushButton::clicked,this,&mainWindow::pushBtnRandom);

    //退出
    QPushButton*btn1=new QPushButton(this);
    btn1->setText("退出");
    btn1->move(90,90);
    connect(btn1,&QPushButton::clicked,this,&mainWindow::pushBtn1);

}

mainWindow::~mainWindow()
{
    delete ui;
}

//按下“导入名单”后跳转
void mainWindow::pushBtnList(){
    ui->mainWindow::list;
    this->hide();
    lists*newPage1=new lists(this);
    newPage1->show();

}
//跳转到设置
void mainWindow::pushBtnSetting(){
    this->hide();
    settings*newPage2=new settings(this);
    newPage2->show();
}
//跳转到抽奖
void mainWindow::pushBtnAward(){
    this->hide();
    award*newPage3=new award(this);
    newPage3->show();
}
//跳转到随机点人
void mainWindow::pushBtnRandom(){
    this->hide();
    random*newPage4=new random(this);
    newPage4->show();
}
//退出
void mainWindow::pushBtn1(){
    this->close();
}
