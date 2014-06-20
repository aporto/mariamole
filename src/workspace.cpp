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

	//currentProject = "";
	//if (projects.size() > 0) {
		//SetCurrentProject(projects.at(0).name);
	//}

	if (GetCurrentProject() == NULL) {
		if (projects.size() > 0) {
			SetCurrentProject(projects.at(0).name);
		}
	}

	return ok;
}

//-----------------------------------------------------------------------------

bool Workspace::SetCurrentProject(QString projectName)
{
	//vector <Project> projects;
	bool ok = false;
	for (int i=0; i < projects.size(); i++) {
		if (projects.at(i).name == projectName) {
			projects.at(i).current = true;
			ok = true;
		} else { 
			projects.at(i).current = false;
		}

		//	currentProject = projectName;	
		//}
	}
	
	if (ok == false) {
		if (projects.size() > 0) {
			projects.at(0).current = true;
		}
	}

	modified = true;

	return ok;
}

//-----------------------------------------------------------------------------

Project * Workspace::GetCurrentProject(void)
{
	/*if (currentProject == "") {
		return NULL;
	}*/

	for (int i=0; i < projects.size(); i++) {
		if (projects.at(i).current) {
			return &(projects.at(i));
		}
	}

	return NULL;
}

//-----------------------------------------------------------------------------

bool Workspace::AddProject(QString name, QString importExample) 
{
	QString path = config.workspace;

	// Force renaming the project name if another project with the same name already exists
	int aux = 0;	
	QString auxName = name;
	bool ok = true;
	do {		
		ok = QDir().mkdir(path + "/" + auxName);
		if (ok == false) {
			aux++;
			auxName = name + "_" + QString::number(aux);
		}
	} while ((ok == false) && (aux < 50));	
	if (aux >= 50) {
		return false;
	}

	name = auxName;
	path = path + "/" + name;

	QDir().mkdir(path + "/source");
	Project project;
	project.name = name;	

	if (importExample == "") {		
		CopyFileToProject(qApp->applicationDirPath() + "/templates/main.cpp", "main.cpp", project);
		CopyFileToProject(qApp->applicationDirPath() + "/templates/main.h", "main.h", project);		
		CopyFileToProject(qApp->applicationDirPath() + "/templates/mariamole_auto_generated.h", "mariamole_auto_generated.h", project);		
	} else {
		QString examplePath = config.LocateFileUsingSearchPaths(importExample, "$(LIBRARIES)", true);
		QString exampleName = QFileInfo(importExample).fileName();
		QString mainFile = examplePath + "/" + exampleName + ".ino";		
		CopyFileToProject(mainFile, "main.cpp", project);
		CopyFileToProject(qApp->applicationDirPath() + "/templates/main.h", "main.h", project);		
		CopyFileToProject(qApp->applicationDirPath() + "/templates/mariamole_auto_generated.h", "mariamole_auto_generated.h", project);		

		//if example comes from a lib, add the lib files as external
		if (importExample.toUpper().indexOf("/EXAMPLES/") > 0) {
			QString libName = importExample;
			libName = libName.left(libName.toUpper().indexOf("/EXAMPLES/"));
			libName = QFileInfo(libName).fileName();
			ImportLibrary(&project, libName);
		}		
	}
	projects.push_back(project);

	modified = true;

	if (ok) {
		SetCurrentProject(project.name);
	}

	return true;
}

//-----------------------------------------------------------------------------

bool Workspace::ImportLibrary(Project * project, QString libPath, QString prefixPath)
{
	// first, find the library path
	if (libPath.indexOf("/") < 0) {
		QString libName = libPath;
		libPath = config.LocateFileUsingSearchPaths(libPath, "$(LIBRARIES)", true);
		if (libPath == "") {
			msg.Add("Error importing library '" + libName + "' to project. Please add it manually from toobar button", mtError);
			return false;
		}
	}

	ImportLibraryFilesRecursively(project, libPath, libPath);

	// Add the list of all .h files to the automatically generated header file	
	QString autoFileName = qApp->applicationDirPath() + "/templates/mariamole_auto_generated.h";// config.workspace + "/" + project->name + "/source/mariamole_auto_generated.h";
	QFile autoFile(autoFileName);
	autoFile.open(QFile::ReadOnly | QFile::Text);
    QTextStream stream(&autoFile);
	QString fileContent = stream.readAll();
	autoFile.close();

	// Remove the last #endif from the file
	while (fileContent.at(fileContent.length()-1) != '#') {
		fileContent.remove(fileContent.length()-1, 1);
	}
	fileContent.remove(fileContent.length()-1, 1);
	//fileContent; += "\n\n";

	// add all header files
	for (int i=0; i < project->files.size(); i++) {
		if (project->files.at(i).type == ptExternal) {
			QString name = QFileInfo(project->files.at(i).name).fileName();
			QString ext = QFileInfo(project->files.at(i).name).suffix().toUpper();
			if ( (ext == "H") || (ext == "HPPC") ) {
				fileContent += "#include <" + name + ">\n";
			}
		}
	}
	fileContent += "\n#endif\n";

	autoFileName = config.workspace + "/" + project->name + "/source/mariamole_auto_generated.h";
	QFile autoFileOutput(autoFileName);	
	autoFileOutput.open(QFile::WriteOnly);
	//autoFileOutput.setlin
	//setEolMode(QsciScintilla::EolUnix);
    QTextStream streamOutput(&autoFileOutput);	
	streamOutput << fileContent;
	autoFileOutput.close();

	modified = true;

	return true;
}

//-----------------------------------------------------------------------------

void Workspace::ImportLibraryFilesRecursively(Project * project, QString path, QString libPath)
{
	QString libName = QFileInfo(libPath).fileName();

	int pathLen = path.length() - libPath.length();				
	QString pref;
	if (pathLen > 0) {
		pref = path.right(pathLen);
	}

	project->includePaths += ";$(LIBRARIES)/" + libName;
	if (pref != "") {
		project->includePaths += pref;
	}

	QDir dir(path);                            
	QFileInfoList files = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::AllEntries); 
	for (int f=0; f < files.size(); f++) {	
		if (files.at(f).isDir()) {
			QString pref = files.at(f).fileName();
			if (pref.toUpper() != "EXAMPLES") {
				ImportLibraryFilesRecursively(project, path + "/" + pref, libPath);
				//ok = ok & ImportLibrary(project, libPath + "/" + pref, pref + "/");
			}
		} else {
			QString fileName = QFileInfo(files.at(f).absoluteFilePath()).fileName();
			QString ext = QFileInfo(files.at(f).absoluteFilePath()).suffix().toUpper();
			if ( ( (ext == "CPP") || (ext == "C") ) || (ext == "H") ) {
				ProjectFile file;				
				if (pref == "") {
					file.name = libName + "/" + fileName;
				} else {
					file.name = libName + "/" + pref + "/" + fileName;
				}
				file.type = ptExternal;

				//Only add this file if its wasn't yet preset at the project
				bool found = false;
				for (int i=0; i < project->files.size(); i++) {
					if (project->files.at(i).name == file.name) {
						found = true;
						break;
					}
				}
				if (found == false) {
					project->files.push_back(file);
				}
			}
		}
	}		
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

//-----------------------------------------------------------------------------

QString Workspace::GetFullFilePath(QString projectName, QString filename)
{
	Project * project = NULL;
	for (int i=0; i < projects.size(); i++) {
		if (projects.at(i).name == projectName) {
			project = &(projects.at(i));
			break;
		}
	}

	if (project == NULL) {
		return "";
	}

	for (int i=0; i < project->files.size(); i++) {
		//QFileInfo file(project->files.at(i).name);
		//QString nameCheck = file.fileName();
		if (project->files.at(i).name == filename) {
			if (project->files.at(i).type == ptSource) {
				return config.workspace + "/" + project->name + "/source/" + filename;
			} else {
				return config.LocateFileUsingSearchPaths(filename, "$(LIBRARIES)", false);			
			}
		}
	}
	return "";
}