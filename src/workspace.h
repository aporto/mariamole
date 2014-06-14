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
	bool AddProject(QString name, QString importExample);
	bool IsModified(void);
	Project * GetCurrentProject(void);

	vector <Project> projects;

private:
	//QString path;	
	bool modified;
	QString currentProject;
	bool CopyFileToProject(QString input, QString outoput, Project &project);
};

//-----------------------------------------------------------------------------

extern Workspace workspace;

//-----------------------------------------------------------------------------

