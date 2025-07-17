#ifndef RANDOM_H
#define RANDOM_H

#include <QWidget>

namespace Ui {
class random;
}

class random : public QWidget
{
    Q_OBJECT

public:
    explicit random(QWidget *parent = nullptr);
    ~random();

private:
    Ui::random *ui;
};

#endif // RANDOM_H
