#include "workspace.h"

//-----------------------------------------------------------------------------

Workspace workspace;

//-----------------------------------------------------------------------------s

Workspace::Workspace(void)
{
	path = "";
}


//-----------------------------------------------------------------------------

Workspace::~Workspace(void)
{
}

//-----------------------------------------------------------------------------

bool Workspace::Open(QString workPath)
{
	QDir dir;
	dir.setPath(workPath);

	if (dir.exists() == false) {
		return false;
	}

	path = workPath;

	bool ok = true;

	dir.setFilter(QDir::Dirs | QDir::Hidden | QDir::NoSymLinks);
	QFileInfoList list = dir.entryInfoList();
	for (int i = 0; i < list.size(); ++i) {
		QFileInfo fileInfo = list.at(i);
		QString projectName = fileInfo.fileName();

		if ((projectName == ".") || (projectName == "..")) {
			continue;
		}

		if (projectName.contains(".ignore") == false) {
			 QString projectPath = QDir::cleanPath(path + QDir::separator());
			 Project proj;
			 if (proj.Load(projectPath, projectName)) {
				 projects.push_back(proj);
			 } else {
				 msg.Add("Could not load project '" + projectPath + "'", mtError);
				 ok = false;
			 }
		}        
	}

	return ok;
}


//-----------------------------------------------------------------------------
