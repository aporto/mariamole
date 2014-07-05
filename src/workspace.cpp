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

	projects.clear();

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

    for (unsigned int i=0; i < projects.size(); i++) {
		if (projects.at(i).current) {
			return &(projects.at(i));
		}
	}

	return NULL;
}

//-----------------------------------------------------------------------------

bool Workspace::AddProject(QString name, QString importSketch) 
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

	if (importSketch == "") {		
		CopyFileToProject(qApp->applicationDirPath() + "/templates/main.cpp", project.name + ".cpp", &project);
		//CopyFileToProject(qApp->applicationDirPath() + "/templates/main.h", "main.h", &project);		
		CopyFileToProject(qApp->applicationDirPath() + "/templates/mariamole_auto_generated.h", "mariamole_auto_generated.h", &project);		
	} else {

		if (ImportSketch(&project, importSketch) == false) {
			return false;
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

bool Workspace::ImportSketch(Project * project, QString sketchFullPath) 
{
	QString name = QFileInfo(sketchFullPath).fileName();
	QString path = QFileInfo(sketchFullPath).absolutePath();

	// Copy main sketch file to project source folder, 
	// adjusting the code as necessary

	QStringList importLibs;
	
	QFile file(sketchFullPath);
	file.open(QFile::ReadOnly | QFile::Text);
	QTextStream streamIn(&file);
	QString fileContent = streamIn.readAll();
	file.close();
	QStringList list = fileContent.split("\n");
	for (int i=0; i<list.count(); i++) {
		if (list[i].indexOf("#include") >= 0) {
			QString text = list[i];
			text.remove(0, text.indexOf("#include") +  8);
			text = text.trimmed();
			text = text.remove(0, 1); //remove first quote or "<"
			int p = text.indexOf(".h");
			if (p > 0) {
				text = text.left(p);
				importLibs.append(text);
			}
		}
	}

	fileContent = "#include \"mariamole_auto_generated.h\"\n" + fileContent;
	QFile outFile(config.workspace + "/" + project->name + "/source/" + project->name + ".cpp"); // main.cpp
	outFile.open(QFile::WriteOnly);
	QTextStream streamOut(&outFile);
	streamOut << fileContent;
	outFile.close();
	ProjectFile pfile;
	pfile.name = project->name + ".cpp"; //"main.cpp";
	pfile.open = false;
	pfile.type = ptSource;
	project->files.push_back(pfile);

	//CopyFileToProject(qApp->applicationDirPath() + "/templates/main.h", "main.h", project);		
	CopyFileToProject(qApp->applicationDirPath() + "/templates/mariamole_auto_generated.h", "mariamole_auto_generated.h", project);		

	ImportFilesFromSketchDirectory(project, path);

	for (int i=0; i < importLibs.count(); i++) {
		ImportLibrary(project, importLibs[i], "");
	}

	return true;
}

//-----------------------------------------------------------------------------

void Workspace::ImportFilesFromSketchDirectory(Project * project, QString sketchPath)
{
	QString projectPath = config.workspace + "/" + project->name + "/source";
	
	QDir dir(sketchPath);                            
	QFileInfoList files = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::AllEntries); 
	// to do: Import these files recursivelly
	for (int f=0; f < files.size(); f++) {	
		if (files.at(f).isDir() == false) {
			QString fileName = QFileInfo(files.at(f).absoluteFilePath()).fileName();		
			QString ext = QFileInfo(files.at(f)).suffix().toUpper();
			if ( (ext != "INO") && (ext != "PDE") ) {
				CopyFileToProject(files.at(f).absoluteFilePath(), fileName, project);		
			}
		} 		
	}	
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
    for (unsigned int i=0; i < project->files.size(); i++) {
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
                for (unsigned int i=0; i < project->files.size(); i++) {
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

bool Workspace::CopyFileToProject(QString input, QString output, Project * project)
{
	QString path = config.workspace + "/" + project->name + "/source/";
	bool ok = QFile::copy(input, path + output);
	
	if (ok) {
		ProjectFile projectFile;
		projectFile.name = output;
		projectFile.open = false;
		projectFile.type = ptSource;
		project->files.push_back(projectFile);
	}

	return ok;
}

//-----------------------------------------------------------------------------

bool Workspace::Save(void)
{
	bool ok = true;
    for (unsigned int i=0; i < projects.size(); i++) {
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

//-----------------------------------------------------------------------------

bool Workspace::AddNewFile(QString fullPath)
{
	Project * project = GetCurrentProject();
	if (project == NULL) {
		return false;
	}

	if (QFileInfo(fullPath).exists()) {
		ErrorMessage("File already exists: \n" + fullPath);
		return false;
	}

	QString path = QFileInfo(fullPath).absoluteDir().absolutePath();
	if (path != config.workspace + "/" + project->name + "/source") {
		ErrorMessage("Invalid file name/path: \n" + fullPath);
		return false;
	}

	QString name = QFileInfo(fullPath).fileName();
	QString ext = QFileInfo(fullPath).suffix().toUpper();
	
	// create the file
	QFile autoFileOutput(fullPath);	
	bool ok = autoFileOutput.open(QFile::WriteOnly);
	if (ok == false) {
		ErrorMessage("Error creating the file: \n" + fullPath);
		return false;
	}
	QTextStream streamOutput(&autoFileOutput);	
	streamOutput << "/* File automatically created by MariaMole */ \n";
	autoFileOutput.close();

	modified = true;

	ProjectFile pfile;
	pfile.name = name;
	pfile.open = false;
	pfile.type = ptSource;
	project->files.push_back(pfile);

	return true;
}

//-----------------------------------------------------------------------------

void Workspace::RemoveFile(QString projectName, QString file, bool isExternal)
{
	Project * project = FindProject(projectName);
	if (project == NULL) {
		return;
	}

	QString msg;
	QString fullPath = GetFullFilePath(projectName, file);
	if (isExternal) {
		msg = "Confirm removing reference to this external file?\nThe file will NOT be deleted!\n\n"
			+ file;
	} else {
		msg = "Confirm deleting this file: \n\n" + fullPath;
	}
	
	if (GetUserConfirmation(msg) == false) {
		return;
	}

	int index = -1;
	for (int i=0; i < project->files.size(); i++) {
		if (isExternal) {
			if ( (project->files.at(i).type == ptExternal) && (project->files.at(i).name == file) ) {
				index = i;
				break;
			}
		} else {
			if ( (project->files.at(i).type == ptSource) && (project->files.at(i).name == file) ) {
				index = i;
				break;
			}
		}
	}

	if (index < 0 ) {
		ErrorMessage("Could not delete the file!\n");
		return;
	}

	if (isExternal == false) {
		bool ok = QFile(fullPath).remove();
		if (ok == false) {
			ErrorMessage("Error deleting the file!\n");
			return;
		}		
	}

	project->files.erase(project->files.begin() + index);
	modified = true;
}

//-----------------------------------------------------------------------------

void Workspace::RenameFile(QString projectName, QString file)
{
	Project * project = FindProject(projectName);
	if (project == NULL) {
		return;
	}

	int index = -1;
	for (int i=0; i < project->files.size(); i++) {
		if ( (project->files.at(i).type != ptExternal) && (project->files.at(i).name == file) ) {
			index = i;
			break;
		}
	}

	if (index < 0 ) {
		ErrorMessage("Could not rename the file!\n");
		return;
	}

	GetUserString * user = new GetUserString();
	QString newName = user->GetNewName(QFileInfo(file).baseName(), true);
	if (newName == "") {
		return;
	}

	QString ext = QFileInfo(file).suffix();
	newName = QFileInfo(newName).baseName();
	newName = config.workspace + "/" + project->name + "/source/" + newName + "." + ext;

	QString oldName = GetFullFilePath(projectName, file);

	if (QFile(oldName).rename(newName) == false) {
		ErrorMessage("Error while renaming the file!\n");
		return;
	}

	project->files.at(index).name = QFileInfo(newName).fileName();
	modified = true;
}

//-----------------------------------------------------------------------------

void Workspace::RemoveProject(QString projectName)
{

	Project * project = FindProject(projectName);
	if (project == NULL) {
		return;
	}

	if (GetUserConfirmation("Confirm deleting this whole project?\n\n" + projectName) == false) {
		return;
	}

	QString path = config.workspace + "/" + projectName;
	QDir(path).removeRecursively();

	if (QDir(path).exists()) {
		ErrorMessage("Could not remove the project files. Please delete them manually.");
		return;
	}

    for (unsigned int i=0; i < projects.size(); i++) {
		if (projects.at(i).name == projectName) {
			projects.erase(projects.begin() + i);
			break;
		}
	}

	if (projects.size() > 0) {
		SetCurrentProject(projects.at(0).name);
	}
	
	modified = true;
}

//-----------------------------------------------------------------------------

void Workspace::RenameProject(QString projectName)
{
	Project * project = FindProject(projectName);
	if (project == NULL) {
		return;
	}

	GetUserString * user = new GetUserString();
	QString newName = user->GetNewName(projectName, false);
	if (newName == "") {
		return;
	}
	newName = QFileInfo(newName).baseName();

	QString newPath = QFileInfo(newName).baseName();
	QString oldPath = config.workspace + "/" + projectName;

	if (QDir().rename(oldPath, config.workspace + "/" + newPath) == false) {
		ErrorMessage("Error while renaming the project!\n");
		return;
	}

	oldPath = config.workspace + "/" + newPath + "/" + projectName + ".mmproj";
	newPath = config.workspace + "/" + newPath + "/" + newName + ".mmproj";
	

	if (QFile(oldPath).rename(newPath) == false) {
		ErrorMessage("Error while renaming the project definiton file. You'll need to do that manually.\n\nYou won't be able to load this project while this is not fixed");		
	}

	project->name = newName;
	SetCurrentProject(newName);
	modified = true;
}

//-----------------------------------------------------------------------------

Project * Workspace::FindProject(QString name)
{
	for (int i=0; i < projects.size(); i++) {
		if (projects.at(i).name == name) {
			return &(projects.at(i));
		}
	} 
	return NULL;
}
	
//-----------------------------------------------------------------------------

void Workspace::ExportProjectToSketch(QString projectName, QString path)
{
	Project * project = FindProject(projectName);
	if (project == NULL) {
		ErrorMessage("Current project not defined!");
		return;
	}

	path = path + "/" + project->name;
	QDir().mkpath(path);

	bool ok = true;
	bool main = false;
	
	for (int i=0; i < project->files.size(); i++) {
		ProjectFile *pfile = &(project->files.at(i));
		if (pfile->type != ptSource) {
			continue;
		}
		QString src = config.workspace + "/" + project->name + "/source/" + pfile->name;
		QString dst = path + "/" + pfile->name;
		if (pfile->name.toUpper() == (project->name.toUpper() + ".CPP")) { //"MAIN.CPP") {
			dst = path + "/" + project->name + ".ino"; 
			main = true;		
		}
		ok = ok & QFile::copy(src, dst);
	}
	ok = ok && main;
	if (ok == false) {
		ErrorMessage("Error while exporting Arduino sketch");
	}
}
