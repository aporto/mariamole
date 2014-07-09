#include "projectproperties.h"

ProjectProperties::ProjectProperties(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	connect(ui.btnOk, SIGNAL(clicked()), this, SLOT(Ok()));
	connect(ui.btnCancel, SIGNAL(clicked()), this, SLOT(Cancel()));

	connect(ui.rbNoExternalProgrammer, SIGNAL(clicked()), this, SLOT(DisableProgrammer()));
	connect(ui.rbYesExternalProgrammer, SIGNAL(clicked()), this, SLOT(EnableProgrammer()));
	
	connect(ui.menuList, SIGNAL(currentItemChanged( QListWidgetItem * , QListWidgetItem * )), 
		this, SLOT(PageChange( QListWidgetItem * , QListWidgetItem * )));

	connect(ui.cbSerialPort, SIGNAL(activated(int)), this, SLOT(OnSerialPortSelected(int))); 	
}

//-----------------------------------------------------------------------------

ProjectProperties::~ProjectProperties()
{

}

//-----------------------------------------------------------------------------

void ProjectProperties::PageChange ( QListWidgetItem * current, QListWidgetItem * previous )
{
	int index = ui.menuList->currentRow();
	if (index < 0) {
		index = 0;
	}

	//if (current->text() == "Directories") {
	ui.stackedWidget->setCurrentIndex(index);
}

//-----------------------------------------------------------------------------

void ProjectProperties::Ok(void)
{
	ok = true; 
	close();
}

//-----------------------------------------------------------------------------

void ProjectProperties::Cancel(void)
{
	close();
}

//-----------------------------------------------------------------------------

void ProjectProperties::DisableProgrammer(void)
{
	ui.cbExternalProgrammer->setEnabled(false);
}

//-----------------------------------------------------------------------------

void ProjectProperties::EnableProgrammer(void)
{
	ui.cbExternalProgrammer->setEnabled(true);
}

//-----------------------------------------------------------------------------

bool ProjectProperties::Edit(Project * project)
{
	if (project == NULL) {
		return false;
	}
	ok = false;	

	ui.stackedWidget->setCurrentIndex(0);
	ui.menuList->setCurrentRow(0);

	ui.lblName->setText("General options for project '" + project->name + "':");
	
	// set board names combobox
	ui.cbBoardName->clear();
	map <QString, BoardDef>::iterator board;
    for (board = config.boards.begin(); board != config.boards.end(); board++) {
		ui.cbBoardName->addItem(board->first);
		if (board->first == project->boardName) {
			ui.cbBoardName->setCurrentIndex(ui.cbBoardName->count()-1);
		}
	}
	if (ui.cbBoardName->currentIndex() < 0) {
		if (ui.cbBoardName->count() > 0) {
			ui.cbBoardName->setCurrentIndex(0);
		}
	}	

	// get list of serial ports
	ui.cbSerialPort->clear();	
	ui.cbSerialPort->addItem("N/A");
	OnSerialPortSelected(0);
	ui.cbSerialPort->setCurrentIndex(0);
	for (int i=0; i < ui.cbSerialPort->count(); i++) {
		if (project->serialPort == ui.cbSerialPort->itemText(i)) {
			ui.cbSerialPort->setCurrentIndex(i);
		}
	}

	// populate programmers list
	ui.cbExternalProgrammer->clear();
	map <QString, ProgrammerDef>::iterator prog;
	for (prog = config.programmers.begin(); prog != config.programmers.end(); prog++) {
		ui.cbExternalProgrammer->addItem(prog->first);
		if (prog->first == project->programmer) {
			ui.cbExternalProgrammer->setCurrentIndex(ui.cbExternalProgrammer->count()-1);
		}
	}
	if (ui.cbExternalProgrammer->currentIndex() < 0) {
		if (ui.cbExternalProgrammer->count() > 0) {
			ui.cbExternalProgrammer->setCurrentIndex(0);
		}
	}	

	if (project->programmer == "") {
		ui.rbNoExternalProgrammer->setChecked(true);
		ui.cbExternalProgrammer->setEnabled(false);
	} else {
		ui.rbYesExternalProgrammer->setChecked(true);
		ui.cbExternalProgrammer->setEnabled(true);
	}

	exec();

	if (ok == false) {
		return false;
	}

	project->serialPort = ui.cbSerialPort->itemText(ui.cbSerialPort->currentIndex());
	if (project->serialPort == "N/A") {
		project->serialPort = "";
	}

	if (ui.rbNoExternalProgrammer->isChecked()) {
		project->programmer = "";
	} else {
		project->programmer = ui.cbExternalProgrammer->itemText(ui.cbExternalProgrammer->currentIndex());
	}

	project->boardName = ui.cbBoardName->itemText(ui.cbBoardName->currentIndex());
	
	return true;
}

//-----------------------------------------------------------------------------

void ProjectProperties::OnSerialPortSelected(int index)
{
	QString curr = "N/A";
	if (ui.cbSerialPort->currentIndex() >= 0) {
		curr = ui.cbBoardName->itemText(ui.cbBoardName->currentIndex());
	}

	QSerialPortInfo serial;

	// add all new serial ports
	for (int i=0; i < serial.availablePorts().count();i++) {
		bool exists = false;
		for (int j=0; j < ui.cbSerialPort->count(); j++) {
			if (ui.cbSerialPort->itemText(j) == serial.availablePorts().at(i).portName()) {
				exists = true;
				break;
			}
		}
		if (exists == false) {
			ui.cbSerialPort->addItem(serial.availablePorts().at(i).portName());
		}
	}
	
	// remove all serial ports that no longer exist 
	int j = 0;
	while (j < ui.cbSerialPort->count()) {
		bool exists = false;
		for (int i=0; i < serial.availablePorts().count();i++) {
			QString pName = serial.availablePorts().at(i).portName();
			QString cName = ui.cbSerialPort->itemText(j);
			if ( (cName == pName) || (cName == "N/A") ){
				exists = true;
				break;
			}
		}
		if (exists == false) {
			ui.cbSerialPort->removeItem(j);
		} else {
			j++;
		}
	}

	// retorna a escolher o item que ja existia
	/*ui.cbSerialPort->setCurrentIndex(0);
	for (int i=0; i < ui.cbSerialPort->count(); i++) {
		if (curr == ui.cbSerialPort->itemText(i)) {
			ui.cbSerialPort->setCurrentIndex(i);
		}
	}*/
}

