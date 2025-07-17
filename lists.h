#ifndef LISTS_H
#define LISTS_H

#include <QWidget>

namespace Ui {
class lists;
}

class lists : public QWidget
{
    Q_OBJECT

public:
    explicit lists(QWidget *parent = nullptr);
    ~lists();

private:
    Ui::lists *ui;
};

#endif // LISTS_H
