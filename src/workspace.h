#pragma once

//-----------------------------------------------------------------------------

#include <QString>
#include <QDir>

#include <vector>

#include "project.h"
#include "message_handler.h"
#include "config.h" 

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
	bool IsModified(void);
	Project * GetCurrentProject(void);
	QString GetFullFilePath(QString projectName, QString filename);
	bool ImportLibrary(Project * project, QString libPath, QString prefixPath = "");
	bool ImportSketch(Project * project, QString sketchFullPath) ;

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

