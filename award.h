#ifndef AWARD_H
#define AWARD_H

#include <QWidget>
#include<QMainWindow>
#include<QTimer>
#include<QString>
#include<QList>
#include<QSet>

namespace Ui {
class award;
}
QT_END_NAMESPACE

class award : public QMainWindow
{
    Q_OBJECT

public:
    explicit award(QWidget *parent = nullptr);
    ~award();
    void setAllowRepeat(bool allow);
    bool allowRepeat();
    void addChosen(QString&name);
    void clearRecord();
    bool isChosed(QString&name);
signals:
    void repeatChange(bool allow);
private slots:
    void onStartClicked();
    void onStopClicked();
    void exportResults();
    void updateDisplay();
    void showHistory();
    void importList(const QStringList& candidates);
private:
    Ui::award *ui;
    QTimer*timer;
    QList<QString>candidates;
    QList<QString>chosens;
    QList<QString>history;
    QString countText;
    int currentIndex;
    bool isDrawing;
    bool nAllow;
    QSet<QString> alreadyChosen;
};

#endif // AWARD_H
