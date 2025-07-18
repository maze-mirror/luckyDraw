#ifndef PERSONS_H
#define PERSONS_H

#include <QWidget>

namespace Ui {
class persons;
}

class persons : public QWidget
{
    Q_OBJECT

public:
    explicit persons(QWidget *parent = nullptr);
    // 设置抽奖人数
    void setLotteryNumber(int number);
    // 获取当前设置的抽奖人数
    int getLotteryNumber() const;

    // 弹出设置抽奖人数的对话框
    int showSettingDialog();
    ~persons();
signals:
    // 当抽奖人数设置改变时发出的信号
    void lotteryNumberChanged(int newNumber);

private:
    Ui::persons *ui;
    int m_lotteryNumber; // 存储抽奖人数
};

#endif // PERSONS_H
