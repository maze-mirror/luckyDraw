#include "settings.h"
#include "ui_settings.h"

settings::settings(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::settings)
{
    ui->setupUi(this);
    this->setWindowTitle("设置");

}

settings::~settings()
{
    delete ui;
}
