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
    //背景设置
    void selectBackground();
    void removeBackground();
    void updateBgPreview();
    void applyBackground();
    void onBgStyleChanged(int index);
private:
      Ui::MainWindow *ui;
    // 名单管理相关成员
      QStandardItemModel *personModel;
      QTableView *personTableView;
      QLineEdit *nameLineEdit;
      QPushButton *addButton;
      QPushButton *editButton;
      QPushButton *deleteButton;

      // 背景设置相关成员
      QTabWidget *mainTabWidget;       // 主标签页
      QWidget *listTab;                // 名单管理标签页
      QWidget *bgTab;                  // 背景设置标签页
      QLabel *bgPreviewLabel;          // 背景预览
      QPushButton *selectBgButton;     // 选择背景按钮
      QPushButton *removeBgButton;     // 移除背景按钮
      QComboBox *bgStyleCombo;         // 背景样式选择
      QString currentBgPath;           // 当前背景路径

      // 状态栏相关
      QLabel *statusLabel;
      QTimer *statusTimer;

      // 初始化函数
      void setupUI();
      void setupMenus();
      void setupStatusBar();
      void setupCentralWidget();

      // 标签页初始化
      void initListTab();              // 初始化名单管理标签页
      void initBgTab();                // 初始化背景设置标签页
      //void updateBgPreview();          // 更新背景预览

protected:
    void resizeEvent(QResizeEvent *event) override;

};

#endif // LISTS_H
