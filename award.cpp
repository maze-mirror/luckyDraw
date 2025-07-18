#include "award.h"
#include "ui_award.h"
#include<QWidget>
#include<QPushButton>
#include<QMenu>
#include<QMenuBar>
#include<QMainWindow>
#include<QCheckBox>
#include<QAction>
#include<QMessageBox>
#include<QFileDialog>
#include<QRandomGenerator>
#include<QLabel>
#include<QMovie>
#include<QSound>
#include"lists.h"

award::award(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::award)
    ,nAllow(false)
    ,alreadyChosen()
{
    ui->setupUi(this);
    this->setWindowTitle("抽奖");

    timer=new QTimer(this);
    connect(timer, &QTimer::timeout, this, &award::updateDisplay);
    currentIndex=0;
    isDrawing=false;

    //按键设置
    //QPushButton*startBtn=new QPushButton(this);
    ui->startBtn->setGeometry(30,120,75,24);
    //QPushButton*stopBtn=new QPushButton(this);
    ui->stopBtn->setGeometry(30, 160, 75, 24);
    connect(ui->startBtn, &QPushButton::clicked, this, &award::onStartClicked);
    connect(ui->stopBtn, &QPushButton::clicked, this, &award::onStopClicked);

    //菜单栏
    QMenuBar*menuBar =new QMenuBar(this);
    setMenuBar(menuBar);
    //创建菜单
    QMenu*fileMenu=menuBar->addMenu("中奖名单");
    //查看以往中奖名单
    QAction*hstAction=fileMenu->addAction("历史记录");
    connect(hstAction, &QAction::triggered, this, &award::showHistory);
   //导出历史中奖名单
    QAction *exportAction = fileMenu->addAction("导出中奖记录");
    connect(exportAction, &QAction::triggered, this, &award::exportResults);

    //是否重复
    QCheckBox*repeatCheckBox=new QCheckBox("允许重复",this);
    repeatCheckBox->setGeometry(70,80,61,20);
    repeatCheckBox->setChecked(allowRepeat());
    repeatCheckBox->show();
    connect(repeatCheckBox,&QCheckBox::toggled,this,&award::setAllowRepeat);
    //中奖显示区域
    ui->chosenList->setGeometry(260,40, 101, 192);
    //导入候选人名单
    lists*listWidget=new lists(this);
    connect(listWidget,&lists::listImported,this,&award::importList);
    QPushButton*importBtn=new QPushButton("设置名单",this);
    importBtn->setGeometry(30,40,61,20);
    connect(importBtn,&QPushButton::clicked,listWidget,&lists::show);
}

award::~award()
{
    delete ui;
}
void award::onStartClicked()
{
    QString countText=ui->countEdit->text();
    bool ok;
    int count=countText.toInt(&ok);
    if(!ok||count<=0){
        QMessageBox::warning(this, "警告", "请输入有效的抽奖人数");
        return;
    }

    if (candidates.isEmpty()) {
        QMessageBox::warning(this, "警告", "候选人列表为空");
        return;
    }

    isDrawing = true;
    ui->startBtn->setEnabled(false);
    ui->stopBtn->setEnabled(true);
    // 播放抽奖音效（需要提供实际的音频文件路径）
    QSound::play(".\\crushMusic.WAV");

    QLabel *gifLabel = new QLabel(this);
    gifLabel->setGeometry(20, 20, 400, 200);
    gifLabel->setAlignment(Qt::AlignCenter);
    QMovie*movie=new QMovie(".\\worm.gif");
    gifLabel->setMovie(movie);
    movie->start();

    // 启动定时器，快速切换显示的候选人
    timer->start(50); // 每50毫秒更新一次
}

void award::onStopClicked(){
    timer->stop();
    isDrawing = false;

    QString countText=ui->countEdit->text();
    bool ok;
    int count=countText.toInt(&ok);
    if(!ok||count<=0){
        QMessageBox::warning(this, "警告", "请输入有效的抽奖人数");
        return;
    }

    if(!allowRepeat()&&candidates.size()<count){
        QMessageBox::warning(this,"警告","参与抽奖人数不足");
        return;
    }

    QRandomGenerator*generator=QRandomGenerator::global();
    for(int i=0;i<countText;++i){
        if(candidates.isEmpty()){
            QMessageBox::warning(this, "警告", "候选人列表已空，无法继续抽奖");
            break;
        }

        int chosenIndex=generator->bounded(candidates.size());
        QString chosen=candidates[chosenIndex];

        //添加到中奖列表
        chosens.append(chosen);
        ui->chosenList->addItem(chosen);
        alreadyChosen.insert(chosen);
        //如果不允许重复，从候选人列表中移除
        if(!allowRepeat()){
        candidates.removeAt(chosenIndex);
        }
        //更新历史纪录
        QString historyEntry = QString("%1: %2").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")).arg(chosen);
        history.append(historyEntry);
    }

    ui->startBtn->setEnabled(true);
    ui->stopBtn->setEnabled(false);

}

//关于是否允许重复抽奖
void award::setAllowRepeat(bool allow){
    if(nAllow==allow)
        return;
    nAllow=allow;
    emit repeatChange(allow);
}
bool award::allowRepeat(){
    return nAllow;
}
void award::addChosen(QString&name){
    alreadyChosen.insert(name);
}
void award::clearRecord(){
    alreadyChosen.clear();
}
bool award::isChosed(QString&name){
    return alreadyChosen.contains(name);
}

void award::updateDisplay(){
    QRandomGenerator*generator=QRandomGenerator::global();
    currentIndex = generator->bounded(candidates.size());
    QString chosen = candidates[currentIndex];
    ui->currentName->setText(chosen);
}
//历史记录
void award::showHistory(){
    if(history.isEmpty()){
    QMessageBox::information(this, "历史记录", "暂无历史记录！");
    return;
}

    QString historyText;
    for (const QString &entry : history) {
    historyText += entry + "\n";
}

    QMessageBox::information(this, "历史记录", historyText);
}
void award::exportResults(){
    if (chosens.isEmpty()) {
        QMessageBox::warning(this, "警告", "中奖名单为空！");
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(this, "导出中奖记录", "", "文本文件 (*.txt)");
    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "错误", "无法打开文件！");
        qDebug()<<"fail to open";
        return;
    }

    QTextStream out(&file);
    out << "=== 中奖记录 ===" << Qt::endl;
    out << "抽奖时间: " << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << Qt::endl;

    for (const QString &chosen : chosens) {
        out << chosen <<Qt::endl;
    }

    file.close();
    QMessageBox::information(this, "成功", "中奖记录已导出！");
}

void award::importList(const QStringList&list){
    candidates.clear();
    if (!allowRepeat()) {
        // 将历史中奖者添加到已选择集合
        alreadyChosen.clear();
        for (const QString& chosen : chosens) {
            alreadyChosen.insert(chosen);
        }
    } else {
        alreadyChosen.clear();
    }

    // 添加新候选人
    for (const QString& candidate : list) {
        // 如果不允许重复，过滤掉已中奖的人
        if (!allowRepeat() && alreadyChosen.contains(candidate)) {
            continue;
        }
        candidates.append(candidate);
    }

    // 更新界面显示
    if (candidates.isEmpty()) {
        QMessageBox::information(this, "提示", "导入的名单中没有有效候选人");
    } else {
        QMessageBox::information(this, "成功", QString("已导入 %1 名候选人").arg(candidates.size()));
    }
}
