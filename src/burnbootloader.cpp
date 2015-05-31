#include "burnbootloader.h"

BurnBootloader::BurnBootloader(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	connect(ui.btnOk, SIGNAL(clicked()), this, SLOT(Ok()));
	connect(ui.btnCancel, SIGNAL(clicked()), this, SLOT(Cancel()));
}
//-----------------------------------------------------------------------------

BurnBootloader::~BurnBootloader()
{

}

//-----------------------------------------------------------------------------

void BurnBootloader::Ok(void)
{
	ok = true; 
	close();
}

//-----------------------------------------------------------------------------

void BurnBootloader::Cancel(void)
{
	close();
}

//-----------------------------------------------------------------------------

bool BurnBootloader::Configure(void)
{
	ok = false;

	// set board names combobox
	ui.boards->clear();
	map <QString, BoardDef>::iterator board;
	for (board = config.boards.begin(); board != config.boards.end() ;board++) {
		ui.boards->addItem(board->first);		
	}
	
	// get list of serial ports
	// to do: use QSerialPort

	ui.ports->clear();
	ui.ports->addItem("N/A");

	QSerialPortInfo serial;
	for (int i=0; i < serial.availablePorts().count();i++) {
		ui.ports->addItem(serial.availablePorts().at(i).portName());
	}
	
	// populate programmers list
	ui.programmers->clear();
	map <QString, ProgrammerDef>::iterator prog;
	for (prog = config.programmers.begin(); prog != config.programmers.end(); prog++) {
		ui.programmers->addItem(prog->first);
	}

	exec();

	if (ok) {
		QString board = ui.boards->itemText(ui.boards->currentIndex());
		QString programmer = ui.programmers->itemText(ui.programmers->currentIndex());
		QString port = ui.ports->itemText(ui.ports->currentIndex());
//		builder.ConfigureBootloaderBurner(programmer, board, port);
	}

	return ok;
}
		
