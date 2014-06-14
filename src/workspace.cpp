#include "workspace.h"

//-----------------------------------------------------------------------------

Workspace workspace;

//-----------------------------------------------------------------------------s

Workspace::Workspace(void)
{
	//path = "";
	modified = false;
}


//-----------------------------------------------------------------------------

Workspace::~Workspace(void)
{
}

//-----------------------------------------------------------------------------

bool Workspace::IsModified(void)
{
	return modified;
}

bool Workspace::Open(QString workPath)
{
	QDir dir;
	dir.setPath(workPath);

	if (dir.exists() == false) {
		return false;
	}

	config.workspace = workPath;

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
			 QString projectPath = QDir::cleanPath(config.workspace + QDir::separator());
			 Project proj;
			 if (proj.Load(projectPath, projectName)) {
				 projects.push_back(proj);
			 } else {
				 msg.Add("Could not load project '" + projectPath + "/" + projectName + "'", mtError);
				 //ok = false;
			 }
		}        
	}

	currentProject = "";
	if (projects.size() > 0) {
		SetCurrentProject(projects.at(0).name);
	}

	return ok;
}

//-----------------------------------------------------------------------------

bool Workspace::SetCurrentProject(QString projectName)
{
	//vector <Project> projects;
	for (int i=0; i < projects.size(); i++) {
		if (projects.at(i).name == projectName) {
			currentProject = projectName;
			modified = true;
			return true;
		}
	}

	currentProject = "";
	return false;
}

//-----------------------------------------------------------------------------

Project * Workspace::GetCurrentProject(void)
{
	if (currentProject == "") {
		return NULL;
	}

	for (int i=0; i < projects.size(); i++) {
		if (projects.at(i).name == currentProject) {
			return &(projects.at(i));
		}
	}

	return NULL;
}

//-----------------------------------------------------------------------------

bool Workspace::AddProject(QString name, QString importExample) 
{
	QString path = config.workspace;
	//QString name = ui.projectName.text();
	path = path + "/" + name;
	int aux = 0;	
	QString auxPath = path;
	bool ok = true;
	do {		
		ok = QDir().mkdir(path);
		if (ok == false) {
			aux++;
			auxPath = path + "_" + QString::number(aux);
		}
	} while ((ok == false) && (aux < 50));
	
	if (aux >= 50) {
		return false;
	}

	QDir().mkdir(path + "/source");
	Project project;
	project.name = name;	

	if (importExample == "") {		
		CopyFileToProject(qApp->applicationDirPath() + "/templates/main.cpp", "main.cpp", project);
		CopyFileToProject(qApp->applicationDirPath() + "/templates/main.h", "main.h", project);		
	} else {
		QString exampleName = QFileInfo(importExample).fileName();
		QString mainFile = importExample + "/" + exampleName + ".ino";		
		CopyFileToProject(mainFile, "main.cpp", project);
		CopyFileToProject(qApp->applicationDirPath() + "/templates/main.h", "main.h", project);		
		//QFile::copy(mainFile, path + "/main.cpp");
		//QFile::copy(qApp->applicationDirPath() + "/templates/main.h", path + "/main.h");

		//if example comes from a lib, add the lib files as external
		QString libPath = importExample;
		if (libPath.toUpper().indexOf("/EXAMPLES/") > 0) {
			libPath = libPath.left(libPath.toUpper().indexOf("/EXAMPLES/"));
			QDir dir(libPath);                            
			QFileInfoList files = dir.entryInfoList(QDir::NoDotAndDotDot|QDir::AllEntries); 
			for (int f=0; f < files.size(); f++) {	
				if (files.at(f).isDir() == false) {
					QString fileName = QFileInfo(files.at(f).absoluteFilePath()).fileName();
					QString ext = QFileInfo(files.at(f).absoluteFilePath()).suffix().toUpper();
					if ( ( (ext == "CPP") || (ext == "C") ) || (ext == "H") ) {
						ProjectFile file;
						file.name = "$(LIBRARIES)/" + fileName;
						file.type = ptExternal;
						project.files.push_back(file);
					}
				}
			}		
		}
	}
	projects.push_back(project);

	modified = true;

	return true;
}

//-----------------------------------------------------------------------------

bool Workspace::CopyFileToProject(QString input, QString output, Project &project)
{
	QString path = config.workspace + "/" + project.name + "/source/";
	bool ok = QFile::copy(input, path + output);
	
	if (ok) {
		ProjectFile projectFile;
		projectFile.name = output;
		projectFile.open = false;
		projectFile.type = ptSource;
		project.files.push_back(projectFile);
	}

	return ok;
}

//-----------------------------------------------------------------------------

bool Workspace::Save(void)
{
	bool ok = true;
	for (int i=0; i < projects.size(); i++) {
		if (projects.at(i).Save(config.workspace) == false) {
			ok = false;
			msg.Add("Could not save project '" + projects.at(i).name + "'", mtError);
		}
	}

	if (ok) {
		modified = false;
	}
	return ok;
}
