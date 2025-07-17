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
    ~persons();

private:
    Ui::persons *ui;
};

#endif // PERSONS_H
