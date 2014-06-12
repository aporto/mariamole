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
#include "editortab.h"

//-----------------------------------------------------------------------------

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();

public slots:
	void SetWorkspace(void);
    void open(void);
    void loadFile(const QString &);
    bool saveFile(const QString &);

protected:
	void resizeEvent(QResizeEvent *event);

private:
	Ui::MainWindowClass ui;
    EditorTab *tabsEditor;

	void AdjustWorkspaceTree(void);
    void AdjustProjectFilesOnTree(int pwi, QTreeWidgetItem *);
    void setupActions(void);
};

#endif // MAINWINDOW_H
