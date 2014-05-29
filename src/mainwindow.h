#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//-----------------------------------------------------------------------------

#include <QtWidgets/QMainWindow>
#include <QFileDialog>
#include <QIcon>
#include <QMessageBox>
#include <QFileInfo>

#include "ui_mainwindow.h"

#include "config.h"
#include "workspace.h"

#include "editor.h"

//-----------------------------------------------------------------------------

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();
    Editor *textEdit;

public slots:
	void SetWorkspace(void);
    void open(void);
    void loadFile(const QString &);
    void closeTab(int);

protected:
	void resizeEvent(QResizeEvent *event);

private:
	Ui::MainWindowClass ui;

	void AdjustWorkspaceTree(void);
    void AdjustProjectFilesOnTree(int pwi, QTreeWidgetItem *);
    void setupActions(void);
};

#endif // MAINWINDOW_H
