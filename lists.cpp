#include "lists.h"
#include "ui_lists.h"
#include<QWidget>
#include<QPushButton>

lists::lists(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::lists)
{
    ui->setupUi(this);
    /*this->setWindowTitle("名单");

    //设置按键
    QPushButton*btn1=new QPushButton(this);
    btn1->setText("导入名单");
    btn1->move(10,10);
    //connect
    QPushButton*btn2=new QPushButton(this);
    btn2->setText("返回");
    btn2->move(120,140);
    //connect
    */
    setupUI();
    setupMenus();
    //setupToolbars();
    setupStatusBar();
    setWindowTitle("名单管理");

}

lists::~lists()
{
    delete ui;
}
//第一行
void lists::setupUI()
{
    // 创建模型
    personModel = new QStandardItemModel(0, 1, this);
    personModel->setHorizontalHeaderItem(0, new QStandardItem("姓名"));

    // 设置中央部件
    setupCentralWidget();
}
//添加按钮，输入框
void lists::setupCentralWidget()
{
    // 创建表格视图
    personTableView = new QTableView(this);
    personTableView->setModel(personModel);
    personTableView->horizontalHeader()->setStretchLastSection(true);

    // 创建输入框和按钮
    nameLineEdit = new QLineEdit(this);
    nameLineEdit->setPlaceholderText("姓名");

    addButton = new QPushButton("添加", this);
    editButton = new QPushButton("编辑", this);
    deleteButton = new QPushButton("删除", this);

    // 按钮布局
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(nameLineEdit);
    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(editButton);
    buttonLayout->addWidget(deleteButton);

    // 主布局
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(personTableView);
    mainLayout->addLayout(buttonLayout);

    // 中央部件
    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    // 连接信号和槽
    connect(addButton, &QPushButton::clicked, this, &lists::addPerson);
    connect(editButton, &QPushButton::clicked, this, &lists::editPerson);
    connect(deleteButton, &QPushButton::clicked, this, &lists::deletePerson);
}
//菜单栏
void lists::setupMenus()
{
    // 创建菜单栏
    QMenuBar *menuBar = new QMenuBar(this);
    setMenuBar(menuBar);

    // 文件菜单
    QMenu *fileMenu = menuBar->addMenu("文件");

    QAction *importTextAction = new QAction("导入文本文件", this);
    QAction *importCSVAction = new QAction("导入CSV文件", this);
    QAction *saveProjectAction = new QAction("保存项目", this);
    QAction *loadProjectAction = new QAction("加载项目", this);

    fileMenu->addAction(importTextAction);
    fileMenu->addAction(importCSVAction);
    fileMenu->addSeparator();
    fileMenu->addAction(saveProjectAction);
    fileMenu->addAction(loadProjectAction);

    // 连接信号和槽
    connect(importTextAction, &QAction::triggered, this, &lists::importFromText);
    connect(importCSVAction, &QAction::triggered, this, &lists::importFromCSV);
    connect(saveProjectAction, &QAction::triggered, this, &lists::saveProject);
    connect(loadProjectAction, &QAction::triggered, this, &lists::loadProject);
    //connect(fontAction, &QAction::triggered, this, &lists::showFontDialog);
}
//时间
void lists::setupStatusBar()
{
    // 创建状态栏
    statusLabel = new QLabel("就绪", this);
    statusBar()->addWidget(statusLabel);

    // 设置定时器更新状态栏时间
    statusTimer = new QTimer(this);
    connect(statusTimer, &QTimer::timeout, this, &lists::updateStatus);
    statusTimer->start(1000); // 每秒更新一次

    // 初始更新状态栏
    updateStatus();
}
//添加
void lists::addPerson()
{
    QString name = nameLineEdit->text().trimmed();
    if (!name.isEmpty()) {
        QList<QStandardItem*> items;
        items.append(new QStandardItem(name));
        personModel->appendRow(items);
        nameLineEdit->clear();
        statusLabel->setText(QString("添加了: %1").arg(name));
    }
}
//编辑
void lists::editPerson()
{
    QModelIndexList selectedRows = personTableView->selectionModel()->selectedRows();
    if (selectedRows.size() == 1) {
        int row = selectedRows.first().row();
        QString currentName = personModel->data(personModel->index(row, 0)).toString();
        bool ok;
        QString newName = QInputDialog::getText(this, "编辑姓名", "姓名:", QLineEdit::Normal, currentName, &ok);
        if (ok && !newName.isEmpty()) {
            personModel->setData(personModel->index(row, 0), newName);
            statusLabel->setText(QString("修改为: %1").arg(newName));
        }
    } else {
        QMessageBox::information(this, "提示", "请选择一个要编辑的行");
    }
}
//删除
void lists::deletePerson()
{
    QModelIndexList selectedRows = personTableView->selectionModel()->selectedRows();
    if (!selectedRows.isEmpty()) {
        // 按行号从大到小删除，避免索引变化
        std::sort(selectedRows.begin(), selectedRows.end(), [](const QModelIndex &a, const QModelIndex &b) {
            return a.row() > b.row();
        });

        for (const QModelIndex &index : selectedRows) {
            personModel->removeRow(index.row());
        }

        statusLabel->setText(QString("删除了 %1 行").arg(selectedRows.size()));
    } else {
        QMessageBox::information(this, "提示", "请选择要删除的行");
    }
}
//导入文本
void lists::importFromText()
{
    QString fileName = QFileDialog::getOpenFileName(this, "选择文本文件", "", "文本文件 (*.txt)");
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);//逐行读取文件内容
            in.setCodec("UTF-8");
            personModel->removeRows(0, personModel->rowCount());

            while (!in.atEnd()) {
                QString line = in.readLine().trimmed();
                if (!line.isEmpty()) {
                    QList<QStandardItem*> items;
                    items.append(new QStandardItem(line));
                    personModel->appendRow(items);
                }
            }

            file.close();
            statusLabel->setText(QString("从 %1 导入了 %2 条记录").arg(fileName).arg(personModel->rowCount()));
        } else {
            QMessageBox::warning(this, "错误", "无法打开文件");
        }
    }
}
//导入CSV
void lists::importFromCSV()
{
    QString fileName = QFileDialog::getOpenFileName(this, "选择CSV文件", "", "CSV文件 (*.csv);;所有文件 (*)");
    if (!fileName.isEmpty()) {
        // 询问用户姓名列
        bool ok;
        int nameColumn = QInputDialog::getInt(this, "列选择", "请输入姓名所在的列号 (1-based):", 1, 1, 100, 1, &ok);
        if (!ok) return;

        // 调整为0-based索引
        nameColumn--;

        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            personModel->removeRows(0, personModel->rowCount());

            int lineCount = 0;
            while (!in.atEnd()) {
                QString line = in.readLine();
                QStringList fields = line.split(',');
                if (fields.size() > nameColumn) {
                    QString name = fields[nameColumn].trimmed();
                    if (!name.isEmpty()) {
                        QList<QStandardItem*> items;
                        items.append(new QStandardItem(name));
                        personModel->appendRow(items);
                    }
                }
                lineCount++;
            }

            file.close();
            statusLabel->setText(QString("从 %1 导入了 %2 条记录").arg(fileName).arg(personModel->rowCount()));
        } else {
            QMessageBox::warning(this, "错误", "无法打开文件");
        }
    }
}
//保存项目
void lists::saveProject()
{
    QString fileName = QFileDialog::getSaveFileName(this, "保存项目", "", "名单项目 (*.lmp)");
    if (!fileName.isEmpty()) {
        QJsonObject projectObject;
        QJsonArray personArray;

        for (int i = 0; i < personModel->rowCount(); i++) {
            QJsonObject personObject;
            personObject["name"] = personModel->data(personModel->index(i, 0)).toString();
            personArray.append(personObject);
        }

        projectObject["persons"] = personArray;

        QJsonDocument doc(projectObject);
        QFile file(fileName);

        if (file.open(QIODevice::WriteOnly)) {
            file.write(doc.toJson());
            file.close();
            statusLabel->setText(QString("项目已保存到 %1").arg(fileName));
        } else {
            QMessageBox::warning(this, "错误", "无法保存项目");
        }
    }
}
//加载项目
void lists::loadProject()
{
    QString fileName = QFileDialog::getOpenFileName(this, "加载项目", "", "名单项目 (*.lmp)");
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly)) {
            QByteArray data = file.readAll();
            QJsonDocument doc = QJsonDocument::fromJson(data);
            QJsonObject projectObject = doc.object();

            personModel->removeRows(0, personModel->rowCount());

            QJsonArray personArray = projectObject["persons"].toArray();
            for (const auto &personValue : personArray) {
                QJsonObject personObject = personValue.toObject();
                QString name = personObject["name"].toString();

                QList<QStandardItem*> items;
                items.append(new QStandardItem(name));
                personModel->appendRow(items);
            }

            file.close();
            statusLabel->setText(QString("从 %1 加载了 %2 条记录").arg(fileName).arg(personModel->rowCount()));
        } else {
            QMessageBox::warning(this, "错误", "无法打开项目文件");
        }
    }
}
//时间
void lists::updateStatus()
{
    // 更新时间显示
    QDateTime currentTime = QDateTime::currentDateTime();
    QString timeStr = currentTime.toString("yyyy-MM-dd hh:mm:ss");
    statusLabel->setText(QString("就绪 - %1 | 总人数: %2").arg(timeStr).arg(personModel->rowCount()));
}

