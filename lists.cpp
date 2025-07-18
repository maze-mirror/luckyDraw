#include "lists.h"
#include "ui_lists.h"
#include<QWidget>
#include<QPushButton>

lists::lists(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::lists)
{
    ui->setupUi(this);
       setupUI();
    setupMenus();
    //setupToolbars();
    setupStatusBar();
    setWindowTitle("名单管理");
    setMinimumSize(800, 600); // 设置最小窗口大小

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
}
//添加按钮，输入框
void lists::setupCentralWidget()
{
    / 创建主标签页控件
        mainTabWidget = new QTabWidget(this);

        // 初始化两个标签页
        initListTab();
        initBgTab();
        // 添加标签页到主控件
        mainTabWidget->addTab(listTab, "名单管理");
        mainTabWidget->addTab(bgTab, "背景设置");

        // 主布局
        setCentralWidget(mainTabWidget);
}
// 初始化名单管理标签页
void lists::initListTab()
{
    listTab = new QWidget(mainTabWidget);

    // 表格视图
    personTableView = new QTableView(listTab);
    personTableView->setModel(personModel);
    personTableView->horizontalHeader()->setStretchLastSection(true);

    // 输入框和按钮
    nameLineEdit = new QLineEdit(listTab);
    nameLineEdit->setPlaceholderText("请输入姓名");

    addButton = new QPushButton("添加", listTab);
    editButton = new QPushButton("编辑", listTab);
    deleteButton = new QPushButton("删除", listTab);

    // 按钮布局
    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->addWidget(nameLineEdit);
    btnLayout->addWidget(addButton);
    btnLayout->addWidget(editButton);
    btnLayout->addWidget(deleteButton);

    // 标签页布局
    QVBoxLayout *tabLayout = new QVBoxLayout(listTab);
    tabLayout->addWidget(personTableView);
    tabLayout->addLayout(btnLayout);

    // 连接信号槽
    connect(addButton, &QPushButton::clicked, this, &lists::addPerson);
    connect(editButton, &QPushButton::clicked, this, &lists::editPerson);
    connect(deleteButton, &QPushButton::clicked, this, &lists::deletePerson);
}
// 初始化背景设置标签页
void lists::initBgTab()
{
    bgTab = new QWidget(mainTabWidget);

    // 背景预览区
    bgPreviewLabel = new QLabel("背景预览", bgTab);
    bgPreviewLabel->setAlignment(Qt::AlignCenter);
    bgPreviewLabel->setMinimumHeight(300);
    bgPreviewLabel->setStyleSheet("border: 1px solid #ccc; background-color: #f5f5f5;");

    // 背景样式选择下拉框（拉伸方式）
    bgStyleCombo = new QComboBox(bgTab);
    bgStyleCombo->addItem("拉伸填充（忽略比例）", Qt::IgnoreAspectRatio);
    bgStyleCombo->addItem("保持比例（可能留空）", Qt::KeepAspectRatio);
    bgStyleCombo->addItem("保持比例并填满（可能裁剪）", Qt::KeepAspectRatioByExpanding);
    bgStyleCombo->setCurrentIndex(1); // 默认保持比例

    // 按钮
    selectBgButton = new QPushButton("选择背景图片", bgTab);
    removeBgButton = new QPushButton("移除背景", bgTab);
    removeBgButton->setEnabled(false); // 初始禁用（无背景时）

    // 按钮布局
    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->addWidget(bgStyleCombo);
    btnLayout->addWidget(selectBgButton);
    btnLayout->addWidget(removeBgButton);

    // 标签页布局
    QVBoxLayout *tabLayout = new QVBoxLayout(bgTab);
    tabLayout->addWidget(new QLabel("预览效果：", bgTab));
    tabLayout->addWidget(bgPreviewLabel);
    tabLayout->addLayout(btnLayout);
    tabLayout->addStretch(); // 底部留白

    // 连接信号槽
    connect(selectBgButton, &QPushButton::clicked, this, &lists::selectBackground);
    connect(removeBgButton, &QPushButton::clicked, this, &lists::removeBackground);
    connect(bgStyleCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &lists::onBgStyleChanged);
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
// 背景设置：选择背景图片
void lists::selectBackground()
{
    QString fileName = QFileDialog::getOpenFileName(this, "选择图片", "","图片文件 (*.png *.jpg *.jpeg *.bmp *.gif);;所有文件 (*)");
    if (!fileName.isEmpty()) {
        currentBgPath = fileName;
        removeBgButton->setEnabled(true);
        updateBgPreview(); // 更新预览
        applyBackground(); // 应用到窗口
        statusLabel->setText(QString("已选择背景图片: %1").arg(fileName));
    }
}
// 背景设置：移除背景
void lists::removeBackground()
{
    currentBgPath.clear();
    removeBgButton->setEnabled(false);
    bgPreviewLabel->setPixmap(QPixmap());
    bgPreviewLabel->setText("背景预览");

    // 恢复默认背景
    QPalette pal = palette();
    pal.setBrush(QPalette::Window, QBrush());
    setPalette(pal);
    personTableView->setStyleSheet(""); // 恢复表格背景

    statusLabel->setText("已移除背景");
}
// 背景设置：更新预览
void lists::updateBgPreview()
{
    if (!currentBgPath.isEmpty()) {
        QPixmap pixmap(currentBgPath);
        if (!pixmap.isNull()) {
            // 按照选择的缩放模式缩放图片，适应预览区域
            QPixmap scaledPixmap = pixmap.scaled(
                bgPreviewLabel->size(),
                static_cast<Qt::AspectRatioMode>(bgStyleCombo->currentData().toInt()), // 从下拉框获取缩放模式
                Qt::SmoothTransformation // 平滑缩放
            );
            bgPreviewLabel->setPixmap(scaledPixmap);
            bgPreviewLabel->setText(""); // 清空预览区文本
        } else {
            bgPreviewLabel->setText("图片无法加载");
            QMessageBox::warning(this, "错误", "选中的文件不是有效的图片");
        }
    }
}

// 背景设置：应用背景到窗口
void lists::applyBackground()
{
    if (!currentBgPath.isEmpty()) {
        QPalette palette = this->palette();
        QImage image(currentBgPath);

        if (!image.isNull()) {
            // 根据选择的缩放模式调整图片大小
            QImage scaledImage = image.scaled(
                this->size(),
                static_cast<Qt::AspectRatioMode>(bgStyleCombo->currentData().toInt()),
                Qt::SmoothTransformation
            );

            palette.setBrush(QPalette::Window, QBrush(scaledImage));
            this->setPalette(palette);

            // 让表格视图透明，显示背景
            personTableView->setStyleSheet("background-color: transparent;");
            personTableView->setAlternatingRowColors(false); // 禁用交替行颜色（避免遮挡背景）
        }
    }
}

// 背景设置：缩放模式改变时触发
void lists::onBgStyleChanged(int index)
{
    Q_UNUSED(index);
    if (!currentBgPath.isEmpty()) {
        updateBgPreview(); // 更新预览
        applyBackground(); // 重新应用到窗口
    }
}

// 窗口大小改变时，重新调整背景
void lists::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    if (!currentBgPath.isEmpty()) {
        applyBackground(); // 窗口大小变化时重新适配背景
    }
}

