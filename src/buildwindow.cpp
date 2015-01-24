#include "buildwindow.h"

//-----------------------------------------------------------------------------

BuildWindow::BuildWindow(QWidget *parent)
	: QProgressDialog("Building...", "Cancel", 0, 101, parent)
{
	ui.setupUi(this);

	this->setBar(ui.progressBar);
	this->setLabel(ui.label);
	this->setCancelButton(ui.btnCancel);	

	this->setMinimum(0);
	this->setMaximum(101);
	this->setAutoClose(false);
	setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::FramelessWindowHint);
	
	/*QRect r = rect();
	r.adjust(-2, -2, +2, +4);
	setMask( QRegion(r) );*/
	
	ui.progressBar->setMinimum(0);
	ui.progressBar->setMaximum(100);
	

	//connect(ui.btnCancel, SIGNAL(clicked()), this, SLOT(cancelBuild()));
}

//-----------------------------------------------------------------------------

BuildWindow::~BuildWindow()
{
	//delete timer;
}

//-----------------------------------------------------------------------------

void BuildWindow::SetPhase(BuildWindowTypes::phaseType phase)
{
	//QString css = "QProgressBar:horizontal { border: 1px solid rgb(68, 85, 89); border-radius: 10px; background: rgb(32,40,42); padding: 1px; }";
	//css += "QProgressBar::chunk:horizontal { border-radius: 10px; border-bottom-right-radius: 10px; border-bottom-left-radius: 10px;border-top-right-radius: 10px;border-top-left-radius: 10px;}";
	QString css;
	switch (phase) {
		case BuildWindowTypes::cleaning:
			ui.label->setText("Cleaning the project...");
			setValue(0);
		break;
		
		case BuildWindowTypes::compiling:
		case BuildWindowTypes::linking:
			ui.label->setText("Building the project...");
			css += "QProgressBar::chunk:horizontal {background: rgb(69, 143, 208); }";			
		break;

		case BuildWindowTypes::linkingCore:
			ui.label->setText("Building core libraries...");
			css += "QProgressBar::chunk:horizontal {background: rgb(255, 255, 127); }";		
		break;

		case BuildWindowTypes::uploading:
			ui.label->setText("Uploading your project...");
			css += "QProgressBar::chunk:horizontal {background: rgb(156, 90, 90); }";			
		break;		

		case BuildWindowTypes::bootloader:
			ui.label->setText("Burning bootloader...");
			css += "QProgressBar::chunk:horizontal {background: rgb(90, 160, 90); }";			
		break;		

		case BuildWindowTypes::detectingLeonardo:
			ui.label->setText("Detecting Leonardo port...");
			css += "QProgressBar::chunk:horizontal {background: rgb(90, 160, 160); }";			
		break;		

	}
	ui.progressBar->setStyleSheet(css);
	
}

//-----------------------------------------------------------------------------

void BuildWindow::SetProgress(int progress)
{
	if (progress != value()) {
		setValue(progress);
		//ui.progressBar->setValue(progress);
	}
}

//-----------------------------------------------------------------------------
