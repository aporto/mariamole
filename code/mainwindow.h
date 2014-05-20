#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//-----------------------------------------------------------------------------

#include <QtWidgets/QMainWindow>
#include "ui_mainwindow.h"

//#include "config.h"
#include "workspace.h"

//-----------------------------------------------------------------------------

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();

public slots:
	void SetWorkspace(void);

protected:
	void resizeEvent(QResizeEvent *event);

private:
	Ui::MainWindowClass ui;

	void AdjustWorkspaceTree(void);
	void AdjustProjectFilesOnTree(int pwi, QTreeWidgetItem * projNode);
};

#endif // MAINWINDOW_H
