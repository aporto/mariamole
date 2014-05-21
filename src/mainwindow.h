#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//-----------------------------------------------------------------------------

#include <QtWidgets/QMainWindow>
#include <QFileDialog>
#include <QIcon>
#include <QMessageBox>
#include <QFileInfo>
#include <Qsci/qsciscintilla.h>
#include <Qsci/qscilexercpp.h>
#include <Qsci/qsciapis.h>

#include "ui_mainwindow.h"

#include "config.h"
#include "workspace.h"

//-----------------------------------------------------------------------------

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();
    QsciScintilla *textEdit;

public slots:
	void SetWorkspace(void);
    void open(void);
    void loadFile(const QString &);

protected:
	void resizeEvent(QResizeEvent *event);

private:
	Ui::MainWindowClass ui;

	void AdjustWorkspaceTree(void);
	void AdjustProjectFilesOnTree(int pwi, QTreeWidgetItem * projNode);
};

#endif // MAINWINDOW_H
