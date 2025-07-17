#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QWidget>
#include<QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui {
class mainWindow;
}
QT_END_NAMESPACE

class mainWindow : public QMainWindow
{
    Q_OBJECT

public:
    mainWindow(QWidget *parent = nullptr);
    ~mainWindow();
    QPushButton*list=new QPushButton(this);

public slots:
    void pushBtnList();
    void pushBtnSetting();
    void pushBtnAward();
    void pushBtnRandom();
    void pushBtn1();
private:
    Ui::mainWindow *ui;
};
#endif // MAINWINDOW_H
