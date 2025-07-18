#ifndef LISTS_H
#define LISTS_H

#include <QWidget>
#include <QStandardItemModel>
#include <QTableView>
#include <QFileDialog>
#include <QMessageBox>
#include <QMenuBar>
#include <QToolBar>
#include <QAction>
#include <QStatusBar>
#include <QTimer>
#include <QDateTime>
#include <QTextStream>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFontDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QComboBox>
#include <QLabel>
#include <QHeaderView>
#include<QInputDialog>
//#include<QMediaPlay>
namespace Ui {
class lists;
}

class lists : public QWidget
{
    Q_OBJECT

public:
    explicit lists(QWidget *parent = nullptr);
    ~lists();
private slots:
    void addPerson();
    void editPerson();
    void deletePerson();
    void importFromText();
    void importFromCSV();
    void saveProject();
    void loadProject();
    void updateStatus();
private:
    Ui::lists *ui;
    void setupUI();
    void setupMenus();
    void setupToolbars();
    void setupStatusBar();
    void setupCentralWidget();

    QStandardItemModel *personModel;
    QTableView *personTableView;
    QLineEdit *nameLineEdit;
    QPushButton *addButton;
    QPushButton *editButton;
    QPushButton *deleteButton;
    QLabel *statusLabel;
    QTimer *statusTimer;
};

#endif // LISTS_H
