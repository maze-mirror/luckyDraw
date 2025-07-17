#include "persons.h"
#include "ui_persons.h"

persons::persons(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::persons)
{
    ui->setupUi(this);
}

persons::~persons()
{
    delete ui;
}
