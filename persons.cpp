#include "persons.h"
#include "ui_persons.h"
#include <QInputDialog>
#include <QMessageBox>
persons::persons(QWidget *parent)
    : QWidget(parent)
    , m_lotteryNumber(1) // 默认抽奖人数为1
    , ui(new Ui::persons)
{
    ui->setupUi(this);
    m_person = new Person(this);
        // 连接信号与槽，当抽奖人数改变时触发 onLotteryNumberChanged 槽函数
        connect(m_person, &persons::lotteryNumberChanged,this, &persons::onLotteryNumberChanged);


        // 动态创建setLotteryNumberBtn按钮
            QPushButton *setLotteryNumberBtn = new QPushButton("设置抽奖人数", this);
            setLotteryNumberBtn->setGeometry(10, 50, 120, 30);

            // 连接按钮点击事件
            connect(setLotteryNumberBtn, &QPushButton::clicked,this, &persons::onSetLotteryNumberBtnClicked);
}

persons::~persons()
{
    delete ui;
}
void persons::setLotteryNumber(int number)
{
    if (number > 0) { // 简单校验，抽奖人数应大于0
        m_lotteryNumber = number;
        emit lotteryNumberChanged(number);
    } else {
        QMessageBox::warning(nullptr, "错误", "抽奖人数应大于0，请重新设置");
    }
}

int persons::getLotteryNumber() const
{
    return m_lotteryNumber;
}

int persons::showSettingDialog()
{
    bool ok;
    int number = QInputDialog::getInt(nullptr, "设置抽奖人数","请输入抽奖人数（大于0）：",m_lotteryNumber, 1, 9999, 1, &ok);
    if (ok) {
        setLotteryNumber(number);
        return number;
    }
    return m_lotteryNumber; // 若取消对话框，返回当前已设置的人数
}
void persons::onLotteryNumberChanged(int newNumber)
{
    // 这里处理抽奖人数改变后的逻辑，比如更新界面显示、准备抽奖数据等
    QMessageBox::information(this, "提示", "抽奖人数已设置为：" + QString::number(newNumber));
}

void persons::onSetLotteryNumberBtnClicked()
{
    m_person->showSettingDialog();
}
