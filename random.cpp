#include "random.h"
#include "ui_random.h"

random::random(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::random)
{
    ui->setupUi(this);
    this->setWindowTitle("随机选人");

}

random::~random()
{
    delete ui;
}
