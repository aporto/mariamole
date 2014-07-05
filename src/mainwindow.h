#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//-----------------------------------------------------------------------------

#include <QtWidgets/QMainWindow>
#include <QFileDialog>
#include <QIcon>
#include <QMessageBox>
#include <QFileInfo>
#include <QDesktopServices>

#include "ui_mainwindow.h"

#include "buildwindow.h"
#include "config.h"
#include "workspace.h"
#include "setworkspace.h"
#include "build_message_item.h"
#include "editor.h"
#include "editortab.h"
#include "wizard.h"
#include "projectproperties.h"
#include "about.h"
#include "preferences.h"
#include "burnbootloader.h"

//-----------------------------------------------------------------------------

namespace WorskspaceTree {
	enum Types {Workspace, Project,ExternalTree, ExternalFile, File};
}

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();
    Editor *textEdit;

	void closeEvent ( QCloseEvent * event );

public slots:
	void SetWorkspacePath(void);
    void open(void);
    void loadFile(const QString &);
	void update(void);
	void BuildProject(void);
	void CleanProject(void);
	void UploadProgram(void);
	void OnBuildComplete(void);
	void SetDefaultProject(void);
	void EditProjectProperties(void);
	void OpenWorkspaceFolder(void);
	void ShowTreeMenu(const QPoint point);
	void ImportLibrary(void);
	void AddNewProject(void);	
	void ConfigureCurrentProject(void);
	void SaveWorkspace(void);
	void OpenSerialPort(void);
	void OnTreeDoubleClick (QTreeWidgetItem * item, int column);
	void OnBuildMessagesDoubleClick(QListWidgetItem* item);
	void AddNewFileToProject(void);
	void OnProjectModified(void);
	void ShowAboutWindow(void);
	void ShowMainMenu(void);
	void ExitSoftware(void);
	void ReportABug(void);
	void VisitMariaMoleWebsite(void);
	void VisitArduinoHelp(void);
	void RenameFile(void);
	void RemoveFile(void);
	void RenameProject(void);
	void RemoveProject(void);
	void ExportToSketch(void);
	void EditPreferences(void);
	void StartBurnBootloader(void);

	void OnSearchKeyPress(const QString&);
	void OnSearchGO(void);
	
protected:
	void resizeEvent(QResizeEvent *event);

private:
	Ui::MainWindowClass ui;
	BuildWindow * buildWindow;
	Wizard * wizard;
	SetWorkspace * setWorkspace;
	QMenu * projectContext, * fileContext, * wsContext;
	QMenu * mainMenu;
	
	EditorTab *tabsEditor;

	void CreateMainMenuContext(void);
	void CreateTreeContextMenu(void);
	void OpenWorkspace(void);
	void AdjustWorkspaceTree(void);
    void AdjustProjectFilesOnTree(int pwi, QTreeWidgetItem *);
    void setupActions(void);
	void SetProjectModified(void);
};

#endif // MAINWINDOW_H
