#pragma once

//-----------------------------------------------------------------------------

#include <QString>
#include <QDir>
#include <QFileDialog>

#include <vector>

#include "project.h"
#include "message_handler.h"
#include "config.h" 
#include "getuserstring.h" 
#include "mm_utils.h" 
#include "getuserstring.h"

//-----------------------------------------------------------------------------

using namespace std;

//-----------------------------------------------------------------------------

class Workspace
{
public:
	Workspace(void);
	~Workspace(void);

	bool Open(QString workPath);
	bool Save(void);
	bool SetCurrentProject(QString projectName);
	bool AddProject(QString name, QString importSketch);
	bool AddNewFile(QString projectName, QString fullPath);
	bool IsModified(void);
	Project * GetCurrentProject(void);
	QString GetFullFilePath(QString projectName, QString filename);
	bool ImportLibrary(Project * project, QString libPath, QString prefixPath = "");
	bool ImportSketch(Project * project, QString sketchFullPath) ;
	void RemoveFile(QString projectName, QString file, bool isExternal);
	void RenameFile(QString projectName, QString file);
	void RemoveProject(QString projectName);
	void RenameProject(QString projectName);
	void ExportProjectToSketch(QString projectName, QString path);
	Project * FindProject(QString name);	

	vector <Project> projects;

private:
	//QString path;	
	bool modified;
	//QString currentProject;
	bool CopyFileToProject(QString input, QString outoput, Project * project);	
	void ImportLibraryFilesRecursively(Project * project, QString path, QString libPath);
	void ImportFilesFromSketchDirectory(Project * project, QString sketchPath);
	
	
};

//-----------------------------------------------------------------------------

extern Workspace workspace;

//-----------------------------------------------------------------------------

