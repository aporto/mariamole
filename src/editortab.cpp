#include "editortab.h"

EditorTab::EditorTab(QWidget *parent)
	: QTabWidget(parent)
{
	this->setTabsClosable(true);
	this->setTabShape(Triangular);

	LoadStyleSheet(this, "style_code_tab.css");

	connect(this, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));

	//create context menu
	context= new QMenu(this);		
	LoadStyleSheet(context, "style_menu.css");
	QAction * action = context->addAction("Close tab");
	connect(action, SIGNAL(triggered()), this, SLOT(closeThis()));
	action = context->addAction("Close all but this");
	connect(action, SIGNAL(triggered()), this, SLOT(closeAllButThis()));
	//projectContext->addAction(action);
	//projectContext->addSeparator();
}

EditorTab::~EditorTab()
{
    closeAll();
}

MM::TabType EditorTab::tabType(int index)
{
	if ( (index < 0) || (index >= count()) ) {
		return MM::undefinedTab;
	}

	//QWidget * w = widget(index);
	
	if (widget(index)->windowIconText() == "editor") {
		return MM::codeTab;
	} else {
		return MM::serialTab;
	}
}

int EditorTab::fileIndex(QString filename)
// returns the tab index that holds the requested file
// if no tab holds this file, returns -1
{
	for (int i=0;  i < count(); i++) {
		//QWidget * w = widget(i);
		// Check if the widget is a code editor
		if (tabType(i) == MM::codeTab) {		
			Editor * editor = (Editor *)(widget(i));
			if (editor->GetFileName() == filename) {
				return i;
			}
		}		
	}

	return -1;
}

int EditorTab::portIndex(QString port)
// returns the tab index that holds the requested serila port
{
	for (int i=0;  i < count(); i++) {
        //QWidget * w = widget(i); //not used
		// Check if the widget is a code editor
		if (tabType(i) == MM::serialTab) {		
			SerialMonitor * monitor = (SerialMonitor *)(widget(i));
			if (monitor->GetPort() == port) {
				return i;
			}
		}		
	}

	return -1;
}

void EditorTab::EnableAllSerialPorts(bool enable)
{
	for (int i=0; i < count(); i++) {
		if (tabType(i) == MM::serialTab) {		
			SerialMonitor * serial = (SerialMonitor *)widget(i);
			if (enable) {
				serial->OpenPort();
			} else {
				serial->ClosePort();				
			}
		}
	}
}

bool EditorTab::openSerialPort(QString port, QString speed)
{	
	int index = portIndex(port);
	if (index >= 0) {
		setCurrentIndex(index);		
	} else {		
		SerialMonitor * monitor = new SerialMonitor(this);
		addTab(monitor, port);
		monitor->OpenPort(port, speed);		
		setCurrentIndex(count() - 1);		
		QApplication::restoreOverrideCursor();
	}

	return true;
}

bool EditorTab::openFile(QString filename, int highlightLine)
{
	// First, check if the file isn't already loaded
	Editor * textEdit = NULL;
	int index = fileIndex(filename);
	if (index >= 0) {
		setCurrentIndex(index);
		textEdit = (Editor *)widget(index);
	} else {		

		if (QFileInfo(filename).exists() == false) {
			ErrorMessage("Could not load file: \n\n" +  filename);
			return false;
		}
		textEdit = new Editor(this);
		textEdit->Open(filename);
		
		connect(textEdit, SIGNAL(textChanged()), this, SLOT(onEditorTextChanged()));
	
		addTab(textEdit, QFileInfo(filename).fileName());
		setCurrentIndex(count() - 1);		

		connect(textEdit, SIGNAL(customContextMenuRequested(const QPoint &)),
					this,SLOT(ShowEditorMenu(const QPoint )));
		textEdit->setContextMenuPolicy(Qt::CustomContextMenu);

		QApplication::restoreOverrideCursor();
	}

	if (highlightLine >= 0) {		
		textEdit->setCursorPosition(highlightLine-1, 0);
		textEdit->ensureLineVisible(highlightLine-1);		
		textEdit->markerAdd (highlightLine-1, 0);
		textEdit->setCaretLineBackgroundColor(QColor(100, 20, 20));
	}

	return true;
}

void EditorTab::onEditorTextChanged(void)
{
	int index = currentIndex();
	Editor * editor = (Editor *)widget(index);
	
	setTabText(index, "*" + QFileInfo(editor->GetFileName()).fileName());
	emit codeChanged();
}

void EditorTab::closeTab(int index)
{
    switch(tabType(index)) {

        case MM::codeTab:
        {
            Editor * ed = (Editor*)widget(index);
            if (ed->isModified()) {
                if (GetUserConfirmation("This file is being closed, but has unsaved changes. Do you want to save it?\n" + ed->GetFileName()))
                  saveFile(index);

            }
        }
            break;

        case MM::serialTab:
        {
            SerialMonitor * serial = (SerialMonitor*)widget(index);
            serial->ClosePort();
        }
            break;

    }

	// removeTab doesnt delete the widget
	this->removeTab(index);
#if !defined(Q_OS_LINUX)
    QWidget * w = widget(index);
	delete w; // For some reason, this is causing a segfault on Linux. Data can't be freed for while :(
#endif
}




//check if all files are saved
bool EditorTab::allSaved(void)
{
    bool saved = true;

    for (int i = 0; i < count(); i++) {
        if (tabType(i) == MM::codeTab) {
            Editor * editor = (Editor *)widget(i);
            if(editor->isModified())
                saved = false;
        }
    }
    return saved;
}



bool EditorTab::saveFile(int index) 
{
	Editor * editor = (Editor *)widget(index);
	
	QString fileNamemod = tabText(index);
	QChar asterisk = tabText(index).at(0);
	
	if(asterisk == '*' && editor->isModified()) {
		fileNamemod.remove(0, 1);
		this->setTabText(index, fileNamemod);
  }
  
	return editor->Save();
}

bool EditorTab::saveAllFiles(void)
{
	bool ok = true;
	for (int i=0; i < count(); i++) {
		if (tabType(i) != MM::codeTab) {
			continue;
		}
		ok = ok && saveFile(i);		
	}

	return ok;
}

void EditorTab::closeAll(void)
{
	while (count() > 0) {
		closeTab(count()-1);
	}
}


void EditorTab::FormatCode(void)
{
	if (currentIndex() < 0) {
		return;
	}
	
	if (tabType(currentIndex() == MM::codeTab)) {
		Editor * editor = (Editor *)widget(currentIndex());
		editor->CodeBeautifier();
	}
}

void EditorTab::ConfigureAllTabs(void)
{
	for (int i=0; i < count(); i++) {
		if (tabType(i) == MM::codeTab) {
			Editor * editor = (Editor *)widget(i);
			editor->Configure();			
		}		
	}
}

void EditorTab::Search(QString text, bool caseSensitive, bool wholeWords)
{
	if (currentIndex() < 0) {
		return;
	}
	
	if (tabType(currentIndex() == MM::codeTab)) {
		Editor * editor = (Editor *)widget(currentIndex());
		if (editor->findFirst(text, false, caseSensitive, wholeWords, true, true, false) == false) {
			ErrorMessage("Text not found!");
		}
	}
}

void EditorTab::ShowEditorMenu(const QPoint point)
{
	if (currentIndex() < 0) {
		return;
	}
	
	context->popup(widget(currentIndex())->mapToGlobal(point));
}

void EditorTab::closeThis(void)
{
	if (currentIndex() < 0) {
		return;
	}
	
	closeTab(currentIndex());
}

void EditorTab::closeAllButThis(void)
{
	int t = currentIndex();
	if (t < 0) {
		return;
	}

	for (int i=0; i < t; i++) {
		closeTab(0);
	}
	while (count() > 1) {
		closeTab(1);
	}
}
