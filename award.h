#ifndef AWARD_H
#define AWARD_H

#include <QWidget>

namespace Ui {
class award;
}

class award : public QWidget
{
    Q_OBJECT

public:
    explicit award(QWidget *parent = nullptr);
    ~award();

private:
    Ui::award *ui;
};

#endif // AWARD_H
