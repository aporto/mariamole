#include "wizard.h"

//-----------------------------------------------------------------------------

Wizard::Wizard(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	connect(ui.examplesTree, SIGNAL(itemClicked(QTreeWidgetItem*, int)),
		this,    SLOT(examplesTreeClicked(QTreeWidgetItem*, int)));
	//void QTreeWidget::itemClicked ( QTreeWidgetItem * item, int column ) [signal]

	connect(ui.btnNext, SIGNAL(clicked()), this, SLOT(btnNextClicked()));
	connect(ui.btnPrevious, SIGNAL(clicked()), this, SLOT(btnPreviousClicked()));
	connect(ui.btnFinish, SIGNAL(clicked()), this, SLOT(btnFinishClicked()));
	connect(ui.btnCancel, SIGNAL(clicked()), this, SLOT(btnCancelClicked()));	

	connect(ui.rbEmptyProject, SIGNAL(clicked()), this, SLOT(rbEmptyProject()));	
    connect(ui.rbImportSketch, SIGNAL(clicked()), this, SLOT(rbEmptyProject()));
	connect(ui.rbImportExample, SIGNAL(clicked()), this, SLOT(rbImportExample()));	

	connect(ui.listLibs, SIGNAL(itemClicked (QListWidgetItem *)), this, SLOT(listLibsClicked(QListWidgetItem *)));		

	connect (ui.pbSketchFile, SIGNAL(clicked()), this, SLOT(OnSelectSketchFile()));	
}

//-----------------------------------------------------------------------------

Wizard::~Wizard()
{

}

//-----------------------------------------------------------------------------

void Wizard::OnSelectSketchFile(void)
{
	QString path = "";
	path = QFileDialog::getOpenFileName(this, tr("Select sketch file"), 
		ui.ebSketchName->text(), tr("Arduino sketches(*.ino)"));

	if (QFileInfo(path).exists()) {
		ui.ebSketchName->setText(path);
	}
}

//-----------------------------------------------------------------------------

void Wizard::listLibsClicked (QListWidgetItem * item)
{
	ui.btnFinish->setEnabled(true);
}

//-----------------------------------------------------------------------------

void Wizard::btnNextClicked(void)
{
	if (ui.stackedWidget->currentIndex() == 0) {
		if (ui.rbImportSketch->isChecked()) {
			ui.stackedWidget->setCurrentIndex(4);			
			ui.btnFinish->setEnabled(false);
			ui.btnNext->setEnabled(true);
			ui.btnPrevious->setEnabled(true);
		} else {
			ui.stackedWidget->setCurrentIndex(2);
			ui.btnFinish->setEnabled(true);
			ui.btnNext->setEnabled(false);
			ui.btnPrevious->setEnabled(true);
			if (ui.rbImportExample->isChecked()) {
				QList<QTreeWidgetItem *> selection = ui.examplesTree->selectedItems();
				if (selection.size() != 0) {
					ui.projectName->setText(selection.at(0)->text(0));
				}
			} else if (ui.rbImportSketch->isChecked()) {
				QFileInfo finfo (ui.ebSketchName->text());
				ui.projectName->setText(finfo.baseName());
			} else { // new empty project 
				if (ui.projectName->text() == "") {
					ui.projectName->setText("New project");
				}
			}
		}
	} else if (ui.stackedWidget->currentIndex() == 4) {
		if (QFileInfo(ui.ebSketchName->text()).exists() == false) {
			ErrorMessage("Sketch file doesn't exists!\n" + ui.ebSketchName->text());
		} else {
			ui.stackedWidget->setCurrentIndex(2);
			ui.btnFinish->setEnabled(true);
			ui.btnNext->setEnabled(false);
			ui.btnPrevious->setEnabled(true);
			if (ui.rbImportSketch->isChecked()) {
				QFileInfo finfo (ui.ebSketchName->text());
				ui.projectName->setText(finfo.baseName());
			} else { // new empty project 
				if (ui.projectName->text() == "") {
					ui.projectName->setText("New project");
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------

void Wizard::btnPreviousClicked(void)
{
	if (ui.stackedWidget->currentIndex() == 2) {
		if (ui.rbImportSketch->isChecked()) {
			ui.stackedWidget->setCurrentIndex(4);
			ui.btnFinish->setEnabled(false);
			ui.btnNext->setEnabled(true);
			ui.btnPrevious->setEnabled(true);		
		} else {
			ui.stackedWidget->setCurrentIndex(0);
			ui.btnFinish->setEnabled(false);
			ui.btnNext->setEnabled(true);
			ui.btnPrevious->setEnabled(false);		
		}
	} else if (ui.stackedWidget->currentIndex() == 4) {
		ui.stackedWidget->setCurrentIndex(0);
		ui.btnFinish->setEnabled(false);
		ui.btnNext->setEnabled(true);
		ui.btnPrevious->setEnabled(false);		
	}
}

//-----------------------------------------------------------------------------

void Wizard::rbImportExample(void)
{
	ui.examplesTree->setVisible(true);
	QList<QTreeWidgetItem *> selection = ui.examplesTree->selectedItems();
	
	bool btn = false;
	if (selection.size() != 0) {
		btn = (selection.at(0)->childCount() == 0);		
	}
	ui.btnNext->setEnabled(btn);	
}

//-----------------------------------------------------------------------------

void Wizard::rbEmptyProject(void)
{
	ui.btnNext->setEnabled(true);
	ui.examplesTree->setVisible(false);
}

//-----------------------------------------------------------------------------

void Wizard::btnFinishClicked(void)
{
	if (ui.stackedWidget->currentIndex() == 1) { // import lib
		ok = true;
		close();
	} else if (ui.stackedWidget->currentIndex() == 2) {
		ok = ui.projectName->text() != "";
		close();
	} else if (ui.stackedWidget->currentIndex() == 5) { // add file
		if (ui.ebNewFile->text().trimmed() == "") {
			return;
		}
		if (ui.ebNewFile->text().trimmed().at(0) == '.') {
			return;
		}
		newFile = config.workspace + "/" + currentProject->name + "/source/" + ui.ebNewFile->text();
		if (ui.rbCppFile->isChecked() ) {
			newFile += ".cpp";
		} else if (ui.rbCFile->isChecked() ) {
			newFile += ".c";
		} else {
			newFile += ".h";
		}
		if (workspace.AddNewFile(currentProject->name, newFile)) {
			ok = true;
			close();			
		}
	}	
}

//-----------------------------------------------------------------------------

QString Wizard::GetNewFileName(void)
{
	return newFile;
}

//-----------------------------------------------------------------------------

void Wizard::btnCancelClicked(void)
{
	ok = false;
	close();
}

//-----------------------------------------------------------------------------

bool Wizard::Display(void)
{
	//int top=(QDesktopWidget().screenGeometry().height()/2)-(size().height()/2);
	//int left=(QDesktopWidget().screenGeometry().width()/2)-(size().width()/2);	
	//move(left, top);

	ok = false;
	exec();	
	return ok;
}

//-----------------------------------------------------------------------------

bool Wizard::NewFile(Project * project)
{
	currentProject = project;
	ui.lblTitle->setText("Add a new file to '" + project->name +"':");
	
	ui.stackedWidget->setCurrentIndex(5);
	ui.btnCancel->setEnabled(true);
	ui.btnFinish->setEnabled(true);
	ui.btnPrevious->setEnabled(false);
	ui.btnNext->setEnabled(false);

	ui.rbCppFile->setChecked(true);

	ok = Display(); 

	return ok;
	/*if (ok) {
		
	}*/
}
//-----------------------------------------------------------------------------


bool Wizard::NewProject(void)
{
	ui.lblTitle->setText("Create a new project:");
	ui.stackedWidget->setCurrentIndex(0);
	ui.btnCancel->setEnabled(true);
	ui.btnFinish->setEnabled(false);
	ui.btnPrevious->setEnabled(false);
	ui.btnNext->setEnabled(true);

	ui.projectName->setText("");

	ui.rbEmptyProject->setChecked(true);
	ui.examplesTree->setVisible(false);
	
	PopulateExamplesTree();
		
	//ok = false;
	ok = Display(); 
	//exec();

	if (ok) {
		QString path = "";
		if (ui.rbImportExample->isChecked()) {
			path = GetSelectedExamplePath();
		} else if (ui.rbImportSketch->isChecked()) {
			path = ui.ebSketchName->text();
		}
		ok = workspace.AddProject(ui.projectName->text(), path);
	}
	
	return ok;
}

//-----------------------------------------------------------------------------

bool Wizard::ImportLibrary(QString projectName)
{
	ui.stackedWidget->setCurrentIndex(1);
	ui.btnCancel->setEnabled(true);
	ui.btnFinish->setEnabled(false);
	ui.btnPrevious->setEnabled(false);
	ui.btnNext->setEnabled(false);
	ui.lblTitle->setText("Import library to '" + projectName + "':");

	PopulateLibrariesList();

	return Display();	
}

//-----------------------------------------------------------------------------

QString Wizard::GetLibraryName(void)
{
	QList <QListWidgetItem *> selection = ui.listLibs->selectedItems();
	if (selection.count() == 0) {
		return "";
	}
	QString libName = selection.at(0)->text();
	
	return libName;
}

//-----------------------------------------------------------------------------

void Wizard::PopulateLibrariesList(void)
{
	QStringList libraries;
	QStringList paths;

	QString allPaths = config.DecodeMacros("$(ARDUINO_LIBS)", NULL);
	allPaths +=";" + config.extraArduinoLibsSearchPaths;
	paths = allPaths.split(";");
	//paths.append("C:/docs/lixo2/libraries");
	//paths.append("C:/docs/lixo2/libraries1");
	//paths.append("C:/docs/lixo2/libraries2");
	
	ui.listLibs->clear();
	for (int i=0; i < paths.count(); i++) {
		paths[i] = config.DecodeMacros(paths[i], NULL);
		QDir dir(paths[i]);                            
		QFileInfoList files = dir.entryInfoList(QDir::NoDotAndDotDot|QDir::AllEntries); 
		for (int f=0; f < files.size(); f++) {			
            if (files.at(f).isDir()) {
				QString dirName = QFileInfo(files.at(f).absoluteFilePath()).fileName();
                qDebug() << dirName;
				ui.listLibs->addItem(dirName);
			}
		}
	}	
	ui.listLibs->sortItems();
}

//-----------------------------------------------------------------------------

void Wizard::PopulateExamplesTree(void)
{
	QStringList examples;
	ListExamples(examples);

	ui.examplesTree->clear();
	QTreeWidgetItem *parent;// = new QTreeWidgetItem();
	//parent->setText(0, "examples");
	//ui.examplesTree->addTopLevelItem(parent);	
	for (int i=0; i < examples.count(); i++) {
		parent = NULL; //ui.examplesTree->topLevelItem(0);
		QStringList levels = examples[i].split("/");
		bool foundParent = false;
		for (int l=0; l < ui.examplesTree->topLevelItemCount(); l++) {
			if (ui.examplesTree->topLevelItem(l)->text(0) == levels[0]) {
				foundParent = true;
				parent = ui.examplesTree->topLevelItem(l);
			}
		}
		if (foundParent == false) {
			parent = new QTreeWidgetItem();
			parent->setText(0,levels[0]);
			ui.examplesTree->addTopLevelItem(parent);	
		}
		
		for (int l=1; l < levels.count(); l++) {
			bool found = false;			
			for( int tl = 0; tl < parent->childCount(); tl++ ) {
				QTreeWidgetItem *item = parent->child(tl);
				if (item->text(0) == levels[l]) {
					found = true;
					parent = item;
				}
			}
			if (found == false) {
				QTreeWidgetItem *item = new QTreeWidgetItem();
				parent->addChild(item);
				item->setText(0, levels[l]);
				parent = item;
			}
		}
	}

	//ui.examplesTree->expandAll();
	ui.examplesTree->setRootIsDecorated(true);

	//for (int i=0; i < ui.examplesTree->items().count(); i++) {		
	//}

}

//-----------------------------------------------------------------------------

QString Wizard::GetSelectedExamplePath(void)
{
	if (ui.rbEmptyProject->isChecked() ){
		return  "";
	}

	QList<QTreeWidgetItem *> selection = ui.examplesTree->selectedItems();
	
	if (selection.size() == 0) {
		return "";
	}
	QTreeWidgetItem * selected = selection.at(0);

	QString name = selected->text(0);

	if (selected->childCount() > 0) {
		return "";
	}

	QString path = selected->text(0);
	selected = selected->parent();
	if (selected == NULL) {
		return "";
	}
	while (selected->parent() != NULL) {
		path = selected->text(0) + "/" + path;
		selected = selected->parent();		
	}

	if (selected->text(0) == "arduino") {
		path = config.arduinoInstall + "/examples/" + path;
		path = path + "/" + QFileInfo(path).fileName() + ".ino";
	} else {
		path = selected->text(0) + "/examples/" + path;// + "/" + path + ".ino";
		path += "/" + QFileInfo(path).fileName() + ".ino";
		path = config.LocateFileUsingSearchPaths(path, "$(LIBRARIES)", false);
		if (path == "") {
			ErrorMessage("Could not locate library path... Please report this error to MariaMole developers"); 
		}
	}

	return path;
}

//-----------------------------------------------------------------------------	

void Wizard::ListExamples(QStringList &examples)
{
	QString allPaths = config.arduinoInstall;//  + "/examples";
	allPaths +=";" + config.extraArduinoLibsSearchPaths;
	
	QStringList paths;
	paths = allPaths.split(";");

	examples.clear();	
	for (int i=0; i < paths.size(); i++) {		
        GetExamplesDirectoriesRecursivelly(0, config.DecodeMacros(paths[i], NULL), examples);
	}
}

//-----------------------------------------------------------------------------

void Wizard::GetExamplesDirectoriesRecursivelly(int level, QString path, QStringList &examples)
{
	QDir dir(path);    
	QFileInfoList files = dir.entryInfoList(QDir::NoDotAndDotDot|QDir::AllEntries); 
	for (int f=0; f < files.size(); f++) {
		QString currentPath = files.at(f).absoluteFilePath();
		if (files.at(f).isDir()) {
			QString dirName = QFileInfo(currentPath).fileName();
			if (dirName.toUpper() == "EXAMPLES") {
				//GetAllExamplesFromPath(currentPath, examples);
				QString baseName = currentPath.left(currentPath.length() - QString("/examples").length());
				baseName = QFileInfo(baseName).fileName();
				GetExamplesNamesRecursivelly(baseName, currentPath, examples);
			} else {
                if (level < 2) { // limit the maximum depth for seaching on directories tree. Arduino examples shall be up to the 2nd level. Any 'examples' folder found beyond that is something else, and shall be ignored
                    GetExamplesDirectoriesRecursivelly(level + 1, path + "/" + dirName, examples);
                }
			}
		}
	}
}

//-----------------------------------------------------------------------------

void Wizard::examplesTreeClicked(QTreeWidgetItem *item, int column)
{
	int c = item->childCount();
	ui.btnNext->setEnabled(c == 0);
}

//-----------------------------------------------------------------------------

void Wizard::GetExamplesNamesRecursivelly(QString hierarchy, QString path, QStringList &examples)
{
	QDir dir(path);                            
	QFileInfoList files = dir.entryInfoList(QDir::NoDotAndDotDot|QDir::AllEntries); 
	// Check if this directory is the last directory in the hierarchy. That would mean that it is an example item
	bool isLeaf = true; 
	for (int f=0; f < files.size(); f++) {
		if (files.at(f).fileName() == "examples") {
			continue;
		}
		if (files.at(f).isDir()) {
			isLeaf = false;
			QString dirName = files.at(f).fileName();
			QString newHierarchy = hierarchy + "/" + dirName; 
			GetExamplesNamesRecursivelly(newHierarchy, path + "/" + dirName, examples);
		}
	}

	if (isLeaf) {
		//QString example = hierarchy + "/" + QFileInfo(path).fileName();
		examples.push_back(hierarchy);
	}	
}

//-----------------------------------------------------------------------------



