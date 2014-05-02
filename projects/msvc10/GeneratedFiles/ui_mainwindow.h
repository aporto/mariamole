/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindowClass
{
public:
    QAction *actionSelect_workspace;
    QAction *actionAdd_a_new_project;
    QAction *actionAdd_a_new_file;
    QAction *actionSave_project;
    QAction *actionProject_configurations;
    QAction *actionBuild_project;
    QAction *actionBuild_and_upload_project;
    QAction *actionOpen_serial_port;
    QAction *actionSwitch_workspace;
    QAction *actionSave_workspace;
    QAction *actionSave_workspace_as;
    QAction *actionReload_workspace;
    QAction *actionAdd_file;
    QAction *actionRename_file;
    QAction *actionRemove_file;
    QAction *actionOpen_file;
    QAction *actionOpen_workspace_folder;
    QAction *actionExit;
    QAction *actionSearch;
    QAction *actionFormat_code;
    QAction *actionPrefereces;
    QAction *actionAdd;
    QAction *actionRename;
    QAction *actionRemove;
    QAction *actionEdit_configuration;
    QAction *actionOpen_serial_port_2;
    QAction *actionClean_current_project;
    QAction *actionClean_project_and_core_libraries;
    QAction *actionBuild_current_project;
    QAction *actionUpload_to_board;
    QAction *actionAbout;
    QAction *actionVisit_MariaMole_website;
    QAction *actionView_software_licence;
    QAction *actionReport_a_bug;
    QAction *actionOpen_Arduino_online_help;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QSplitter *splitter_2;
    QSplitter *splitter;
    QTreeWidget *tree;
    QTabWidget *editorTabs;
    QWidget *tab_3;
    QWidget *tab_4;
    QTabWidget *messageTabs;
    QWidget *tab;
    QGridLayout *gridLayout_3;
    QListWidget *listWidget;
    QWidget *tab_2;
    QGridLayout *gridLayout_2;
    QTextBrowser *textBrowser;
    QWidget *tab_5;
    QComboBox *searchText;
    QLabel *label;
    QCheckBox *searchCBCase;
    QPushButton *btnSearch;
    QCheckBox *searchCBWords;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuEdit;
    QMenu *menuProject;
    QMenu *menuHelp;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindowClass)
    {
        if (MainWindowClass->objectName().isEmpty())
            MainWindowClass->setObjectName(QStringLiteral("MainWindowClass"));
        MainWindowClass->resize(650, 654);
        MainWindowClass->setAutoFillBackground(false);
        MainWindowClass->setStyleSheet(QLatin1String("color:rgb(200,200,200);\n"
"background-color: rgb(28, 36, 38);\n"
"selection-background-color: rgb(68, 85, 89);"));
        actionSelect_workspace = new QAction(MainWindowClass);
        actionSelect_workspace->setObjectName(QStringLiteral("actionSelect_workspace"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/MainWindow/resources/workspace.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSelect_workspace->setIcon(icon);
        actionAdd_a_new_project = new QAction(MainWindowClass);
        actionAdd_a_new_project->setObjectName(QStringLiteral("actionAdd_a_new_project"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/MainWindow/resources/add_project.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionAdd_a_new_project->setIcon(icon1);
        actionAdd_a_new_file = new QAction(MainWindowClass);
        actionAdd_a_new_file->setObjectName(QStringLiteral("actionAdd_a_new_file"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/MainWindow/resources/add_file.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionAdd_a_new_file->setIcon(icon2);
        actionSave_project = new QAction(MainWindowClass);
        actionSave_project->setObjectName(QStringLiteral("actionSave_project"));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/MainWindow/resources/save_all.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSave_project->setIcon(icon3);
        actionProject_configurations = new QAction(MainWindowClass);
        actionProject_configurations->setObjectName(QStringLiteral("actionProject_configurations"));
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/MainWindow/resources/project_properties.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionProject_configurations->setIcon(icon4);
        actionBuild_project = new QAction(MainWindowClass);
        actionBuild_project->setObjectName(QStringLiteral("actionBuild_project"));
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/MainWindow/resources/compile.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionBuild_project->setIcon(icon5);
        actionBuild_and_upload_project = new QAction(MainWindowClass);
        actionBuild_and_upload_project->setObjectName(QStringLiteral("actionBuild_and_upload_project"));
        QIcon icon6;
        icon6.addFile(QStringLiteral(":/MainWindow/resources/upload.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionBuild_and_upload_project->setIcon(icon6);
        actionOpen_serial_port = new QAction(MainWindowClass);
        actionOpen_serial_port->setObjectName(QStringLiteral("actionOpen_serial_port"));
        QIcon icon7;
        icon7.addFile(QStringLiteral(":/MainWindow/resources/terminal.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionOpen_serial_port->setIcon(icon7);
        actionSwitch_workspace = new QAction(MainWindowClass);
        actionSwitch_workspace->setObjectName(QStringLiteral("actionSwitch_workspace"));
        actionSave_workspace = new QAction(MainWindowClass);
        actionSave_workspace->setObjectName(QStringLiteral("actionSave_workspace"));
        actionSave_workspace_as = new QAction(MainWindowClass);
        actionSave_workspace_as->setObjectName(QStringLiteral("actionSave_workspace_as"));
        actionReload_workspace = new QAction(MainWindowClass);
        actionReload_workspace->setObjectName(QStringLiteral("actionReload_workspace"));
        actionAdd_file = new QAction(MainWindowClass);
        actionAdd_file->setObjectName(QStringLiteral("actionAdd_file"));
        actionRename_file = new QAction(MainWindowClass);
        actionRename_file->setObjectName(QStringLiteral("actionRename_file"));
        actionRemove_file = new QAction(MainWindowClass);
        actionRemove_file->setObjectName(QStringLiteral("actionRemove_file"));
        actionOpen_file = new QAction(MainWindowClass);
        actionOpen_file->setObjectName(QStringLiteral("actionOpen_file"));
        actionOpen_workspace_folder = new QAction(MainWindowClass);
        actionOpen_workspace_folder->setObjectName(QStringLiteral("actionOpen_workspace_folder"));
        actionExit = new QAction(MainWindowClass);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        actionSearch = new QAction(MainWindowClass);
        actionSearch->setObjectName(QStringLiteral("actionSearch"));
        actionFormat_code = new QAction(MainWindowClass);
        actionFormat_code->setObjectName(QStringLiteral("actionFormat_code"));
        actionPrefereces = new QAction(MainWindowClass);
        actionPrefereces->setObjectName(QStringLiteral("actionPrefereces"));
        actionAdd = new QAction(MainWindowClass);
        actionAdd->setObjectName(QStringLiteral("actionAdd"));
        actionRename = new QAction(MainWindowClass);
        actionRename->setObjectName(QStringLiteral("actionRename"));
        actionRemove = new QAction(MainWindowClass);
        actionRemove->setObjectName(QStringLiteral("actionRemove"));
        actionEdit_configuration = new QAction(MainWindowClass);
        actionEdit_configuration->setObjectName(QStringLiteral("actionEdit_configuration"));
        actionOpen_serial_port_2 = new QAction(MainWindowClass);
        actionOpen_serial_port_2->setObjectName(QStringLiteral("actionOpen_serial_port_2"));
        actionClean_current_project = new QAction(MainWindowClass);
        actionClean_current_project->setObjectName(QStringLiteral("actionClean_current_project"));
        actionClean_project_and_core_libraries = new QAction(MainWindowClass);
        actionClean_project_and_core_libraries->setObjectName(QStringLiteral("actionClean_project_and_core_libraries"));
        actionBuild_current_project = new QAction(MainWindowClass);
        actionBuild_current_project->setObjectName(QStringLiteral("actionBuild_current_project"));
        actionUpload_to_board = new QAction(MainWindowClass);
        actionUpload_to_board->setObjectName(QStringLiteral("actionUpload_to_board"));
        actionAbout = new QAction(MainWindowClass);
        actionAbout->setObjectName(QStringLiteral("actionAbout"));
        actionVisit_MariaMole_website = new QAction(MainWindowClass);
        actionVisit_MariaMole_website->setObjectName(QStringLiteral("actionVisit_MariaMole_website"));
        actionView_software_licence = new QAction(MainWindowClass);
        actionView_software_licence->setObjectName(QStringLiteral("actionView_software_licence"));
        actionReport_a_bug = new QAction(MainWindowClass);
        actionReport_a_bug->setObjectName(QStringLiteral("actionReport_a_bug"));
        actionOpen_Arduino_online_help = new QAction(MainWindowClass);
        actionOpen_Arduino_online_help->setObjectName(QStringLiteral("actionOpen_Arduino_online_help"));
        centralWidget = new QWidget(MainWindowClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        splitter_2 = new QSplitter(centralWidget);
        splitter_2->setObjectName(QStringLiteral("splitter_2"));
        splitter_2->setOrientation(Qt::Vertical);
        splitter = new QSplitter(splitter_2);
        splitter->setObjectName(QStringLiteral("splitter"));
        splitter->setOrientation(Qt::Horizontal);
        tree = new QTreeWidget(splitter);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QStringLiteral("1"));
        tree->setHeaderItem(__qtreewidgetitem);
        tree->setObjectName(QStringLiteral("tree"));
        tree->setStyleSheet(QLatin1String(" background-color: rgb(32,40,42);\n"
"outline: 0;\n"
"border-image: none;"));
        tree->setFrameShape(QFrame::WinPanel);
        tree->setLineWidth(5);
        tree->setRootIsDecorated(false);
        tree->setHeaderHidden(true);
        splitter->addWidget(tree);
        editorTabs = new QTabWidget(splitter);
        editorTabs->setObjectName(QStringLiteral("editorTabs"));
        editorTabs->setStyleSheet(QLatin1String("border-style:solid;\n"
"background-color: rgb(32, 40, 42);\n"
"\n"
"QTabBar::tab:selected, QTabBar::tab:hover {\n"
"background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #fafafa, stop: 0.4 #f4f4f4, stop: 0.5 #e7e7e7, stop: 1.0 #fafafa);\n"
"}\n"
"\n"
"QTabBar::tab:selected {\n"
"border-color: #9B9B9B;\n"
"border-bottom-color: #C2C7CB; /* same as pane color */\n"
"}\n"
"QTabBar::tab:!selected {\n"
"margin-top: 2px; /* make non-selected tabs look smaller */\n"
"}"));
        editorTabs->setTabShape(QTabWidget::Triangular);
        editorTabs->setTabsClosable(true);
        tab_3 = new QWidget();
        tab_3->setObjectName(QStringLiteral("tab_3"));
        editorTabs->addTab(tab_3, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName(QStringLiteral("tab_4"));
        editorTabs->addTab(tab_4, QString());
        splitter->addWidget(editorTabs);
        splitter_2->addWidget(splitter);
        messageTabs = new QTabWidget(splitter_2);
        messageTabs->setObjectName(QStringLiteral("messageTabs"));
        messageTabs->setMaximumSize(QSize(16777215, 200));
        messageTabs->setStyleSheet(QLatin1String("border-style:solid;\n"
"background-color: rgb(32, 40, 42);"));
        messageTabs->setTabShape(QTabWidget::Triangular);
        messageTabs->setTabsClosable(false);
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        gridLayout_3 = new QGridLayout(tab);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        gridLayout_3->setContentsMargins(0, 0, 0, 0);
        listWidget = new QListWidget(tab);
        listWidget->setObjectName(QStringLiteral("listWidget"));

        gridLayout_3->addWidget(listWidget, 0, 0, 1, 1);

        messageTabs->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        gridLayout_2 = new QGridLayout(tab_2);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        textBrowser = new QTextBrowser(tab_2);
        textBrowser->setObjectName(QStringLiteral("textBrowser"));

        gridLayout_2->addWidget(textBrowser, 0, 0, 1, 1);

        messageTabs->addTab(tab_2, QString());
        tab_5 = new QWidget();
        tab_5->setObjectName(QStringLiteral("tab_5"));
        searchText = new QComboBox(tab_5);
        searchText->setObjectName(QStringLiteral("searchText"));
        searchText->setGeometry(QRect(10, 30, 381, 22));
        searchText->setStyleSheet(QLatin1String("     border-style: outset;\n"
"     border-width: 1px;\n"
"     border-radius: 0px;\n"
"     border-color: rgb(60,60,60);\n"
""));
        label = new QLabel(tab_5);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 10, 121, 16));
        label->setStyleSheet(QStringLiteral(""));
        searchCBCase = new QCheckBox(tab_5);
        searchCBCase->setObjectName(QStringLiteral("searchCBCase"));
        searchCBCase->setGeometry(QRect(10, 60, 101, 18));
        btnSearch = new QPushButton(tab_5);
        btnSearch->setObjectName(QStringLiteral("btnSearch"));
        btnSearch->setGeometry(QRect(400, 30, 41, 21));
        btnSearch->setStyleSheet(QLatin1String("     border-style: outset;\n"
"     border-width: 1px;\n"
"     border-radius: 0px;\n"
"     border-color: rgb(60,60,60);\n"
""));
        searchCBWords = new QCheckBox(tab_5);
        searchCBWords->setObjectName(QStringLiteral("searchCBWords"));
        searchCBWords->setGeometry(QRect(130, 60, 141, 18));
        messageTabs->addTab(tab_5, QString());
        splitter_2->addWidget(messageTabs);

        gridLayout->addWidget(splitter_2, 1, 0, 1, 1);

        MainWindowClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindowClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 650, 26));
        menuBar->setStyleSheet(QStringLiteral("background-color: rgb(28, 36, 38)"));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuEdit = new QMenu(menuBar);
        menuEdit->setObjectName(QStringLiteral("menuEdit"));
        menuProject = new QMenu(menuBar);
        menuProject->setObjectName(QStringLiteral("menuProject"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QStringLiteral("menuHelp"));
        MainWindowClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindowClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        mainToolBar->setStyleSheet(QLatin1String("background-color: rgb(32, 40, 42);\n"
"border:none;"));
        mainToolBar->setIconSize(QSize(32, 32));
        MainWindowClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindowClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        statusBar->setStyleSheet(QStringLiteral("background-color: rgb(28, 36, 38)"));
        MainWindowClass->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuEdit->menuAction());
        menuBar->addAction(menuProject->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionSwitch_workspace);
        menuFile->addAction(actionSave_workspace);
        menuFile->addAction(actionSave_workspace_as);
        menuFile->addAction(actionReload_workspace);
        menuFile->addSeparator();
        menuFile->addAction(actionAdd_file);
        menuFile->addAction(actionRename_file);
        menuFile->addAction(actionRemove_file);
        menuFile->addAction(actionOpen_file);
        menuFile->addSeparator();
        menuFile->addAction(actionOpen_workspace_folder);
        menuFile->addSeparator();
        menuFile->addAction(actionExit);
        menuEdit->addAction(actionSearch);
        menuEdit->addAction(actionFormat_code);
        menuEdit->addSeparator();
        menuEdit->addAction(actionPrefereces);
        menuProject->addAction(actionAdd);
        menuProject->addAction(actionRename);
        menuProject->addAction(actionRemove);
        menuProject->addSeparator();
        menuProject->addAction(actionEdit_configuration);
        menuProject->addSeparator();
        menuProject->addAction(actionOpen_serial_port_2);
        menuProject->addAction(actionClean_current_project);
        menuProject->addAction(actionClean_project_and_core_libraries);
        menuProject->addAction(actionBuild_current_project);
        menuProject->addAction(actionUpload_to_board);
        menuHelp->addAction(actionAbout);
        menuHelp->addAction(actionVisit_MariaMole_website);
        menuHelp->addAction(actionView_software_licence);
        menuHelp->addAction(actionReport_a_bug);
        menuHelp->addSeparator();
        menuHelp->addAction(actionOpen_Arduino_online_help);
        mainToolBar->addAction(actionSelect_workspace);
        mainToolBar->addAction(actionAdd_a_new_project);
        mainToolBar->addAction(actionAdd_a_new_file);
        mainToolBar->addAction(actionSave_project);
        mainToolBar->addAction(actionProject_configurations);
        mainToolBar->addAction(actionBuild_project);
        mainToolBar->addAction(actionBuild_and_upload_project);
        mainToolBar->addAction(actionOpen_serial_port);
        mainToolBar->addSeparator();

        retranslateUi(MainWindowClass);

        editorTabs->setCurrentIndex(1);
        messageTabs->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(MainWindowClass);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindowClass)
    {
        MainWindowClass->setWindowTitle(QApplication::translate("MainWindowClass", "MariaMole", 0));
        actionSelect_workspace->setText(QApplication::translate("MainWindowClass", "Select workspace", 0));
#ifndef QT_NO_TOOLTIP
        actionSelect_workspace->setToolTip(QApplication::translate("MainWindowClass", "Select workspace", 0));
#endif // QT_NO_TOOLTIP
        actionAdd_a_new_project->setText(QApplication::translate("MainWindowClass", "Add a new project", 0));
        actionAdd_a_new_file->setText(QApplication::translate("MainWindowClass", "Add a new file", 0));
        actionSave_project->setText(QApplication::translate("MainWindowClass", "Save project", 0));
        actionProject_configurations->setText(QApplication::translate("MainWindowClass", "Project configurations", 0));
        actionBuild_project->setText(QApplication::translate("MainWindowClass", "Build project", 0));
        actionBuild_and_upload_project->setText(QApplication::translate("MainWindowClass", "Build and upload project", 0));
        actionOpen_serial_port->setText(QApplication::translate("MainWindowClass", "Open serial port", 0));
        actionSwitch_workspace->setText(QApplication::translate("MainWindowClass", "Switch workspace", 0));
        actionSave_workspace->setText(QApplication::translate("MainWindowClass", "Save workspace", 0));
        actionSave_workspace_as->setText(QApplication::translate("MainWindowClass", "Save workspace as", 0));
        actionReload_workspace->setText(QApplication::translate("MainWindowClass", "Reload workspace", 0));
        actionAdd_file->setText(QApplication::translate("MainWindowClass", "Add file", 0));
        actionRename_file->setText(QApplication::translate("MainWindowClass", "Rename file", 0));
        actionRemove_file->setText(QApplication::translate("MainWindowClass", "Remove file", 0));
        actionOpen_file->setText(QApplication::translate("MainWindowClass", "Open file", 0));
        actionOpen_workspace_folder->setText(QApplication::translate("MainWindowClass", "Open workspace folder", 0));
        actionExit->setText(QApplication::translate("MainWindowClass", "Exit", 0));
        actionSearch->setText(QApplication::translate("MainWindowClass", "Search", 0));
        actionFormat_code->setText(QApplication::translate("MainWindowClass", "Format code", 0));
        actionPrefereces->setText(QApplication::translate("MainWindowClass", "Prefereces", 0));
        actionAdd->setText(QApplication::translate("MainWindowClass", "Add", 0));
        actionRename->setText(QApplication::translate("MainWindowClass", "Rename", 0));
        actionRemove->setText(QApplication::translate("MainWindowClass", "Remove", 0));
        actionEdit_configuration->setText(QApplication::translate("MainWindowClass", "Edit configuration", 0));
        actionOpen_serial_port_2->setText(QApplication::translate("MainWindowClass", "Open serial port", 0));
        actionClean_current_project->setText(QApplication::translate("MainWindowClass", "Clean current project", 0));
        actionClean_project_and_core_libraries->setText(QApplication::translate("MainWindowClass", "Clean project and core libraries", 0));
        actionBuild_current_project->setText(QApplication::translate("MainWindowClass", "Build current project", 0));
        actionUpload_to_board->setText(QApplication::translate("MainWindowClass", "Upload to board", 0));
        actionAbout->setText(QApplication::translate("MainWindowClass", "About", 0));
        actionVisit_MariaMole_website->setText(QApplication::translate("MainWindowClass", "Visit MariaMole website", 0));
        actionView_software_licence->setText(QApplication::translate("MainWindowClass", "View software licence", 0));
        actionReport_a_bug->setText(QApplication::translate("MainWindowClass", "Report a bug", 0));
        actionOpen_Arduino_online_help->setText(QApplication::translate("MainWindowClass", "Open Arduino online help", 0));
        editorTabs->setTabText(editorTabs->indexOf(tab_3), QApplication::translate("MainWindowClass", "Tab 1", 0));
        editorTabs->setTabText(editorTabs->indexOf(tab_4), QApplication::translate("MainWindowClass", "Tab 2", 0));
        messageTabs->setTabText(messageTabs->indexOf(tab), QApplication::translate("MainWindowClass", "Build", 0));
        messageTabs->setTabText(messageTabs->indexOf(tab_2), QApplication::translate("MainWindowClass", "Output", 0));
        label->setText(QApplication::translate("MainWindowClass", "Type your search string:", 0));
        searchCBCase->setText(QApplication::translate("MainWindowClass", "Case sensitive", 0));
        btnSearch->setText(QApplication::translate("MainWindowClass", "Find", 0));
        searchCBWords->setText(QApplication::translate("MainWindowClass", "Match only whole words", 0));
        messageTabs->setTabText(messageTabs->indexOf(tab_5), QApplication::translate("MainWindowClass", "Search", 0));
        menuFile->setTitle(QApplication::translate("MainWindowClass", "File", 0));
        menuEdit->setTitle(QApplication::translate("MainWindowClass", "Edit", 0));
        menuProject->setTitle(QApplication::translate("MainWindowClass", "Project", 0));
        menuHelp->setTitle(QApplication::translate("MainWindowClass", "Help", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindowClass: public Ui_MainWindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
