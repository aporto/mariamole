#include "buildwindow.h"

//-----------------------------------------------------------------------------

BuildWindow::BuildWindow(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	setWindowFlags( Qt::FramelessWindowHint);//Qt::CustomizeWindowHint );
	
	ui.progressBar->setMinimum(0);
	ui.progressBar->setMaximum(100);

	connect(ui.btnCancel, SIGNAL(clicked()), this, SLOT(cancelBuild()));

	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(update()));
	timer->setSingleShot(false);
}

//-----------------------------------------------------------------------------

BuildWindow::~BuildWindow()
{
	delete timer;
}

//-----------------------------------------------------------------------------

void BuildWindow::Build(bool program)
{
	ui.progressBar->setValue(0);
	ui.label->setText("Building project");
	
	this->setModal(true);

	oldProgressBarType = -1;
	
	show();
	
	timer->start(50);
	thread = new BuilderThread(this);
	thread->program = program;
	thread->burnBootloader = false;
	connect(thread, SIGNAL(finished ()), this, SLOT(completed()));
	thread->start();	
}

//-----------------------------------------------------------------------------

void BuildWindow::BurnBootloader(void)
{
	ui.progressBar->setValue(0);
	ui.label->setText("Burning bootloader");
	
	this->setModal(true);

	oldProgressBarType = -1;
	
	show();
	
	timer->start(50);
	thread = new BuilderThread(this);
	thread->burnBootloader = true;
	connect(thread, SIGNAL(finished ()), this, SLOT(completed()));
	thread->start();	
}

//-----------------------------------------------------------------------------

void BuildWindow::update(void)
{	
	ui.progressBar->setValue(builder.GetPercentage());
		
	int type = builder.GetBuildType();
	if (type != oldProgressBarType) {
		QString css = "QProgressBar:horizontal { border: 1px solid rgb(68, 85, 89); border-radius: 10px; background: rgb(32,40,42); padding: 1px; }";
		css += "QProgressBar::chunk:horizontal { border-radius: 10px; border-bottom-right-radius: 10px; border-bottom-left-radius: 10px;border-top-right-radius: 10px;border-top-left-radius: 10px;}";

		if (type == 1) { // build core lib
			ui.label->setText("Building core libraries...");
			css += "QProgressBar::chunk:horizontal {background: rgb(255, 255, 127); }";			
		} else if (type == 2) { // uploading
			ui.label->setText("Uploading program...");
			css += "QProgressBar::chunk:horizontal {background: rgb(156, 90, 90); }";			
		} else { // compiling project
			ui.label->setText("Compiling project files...");
			css += "QProgressBar::chunk:horizontal {background: rgb(69, 143, 208); }";			
		}
		ui.progressBar->setStyleSheet(css);
		oldProgressBarType = type;
	}
	
	qApp->processEvents();
}

//-----------------------------------------------------------------------------

void BuildWindow::completed(void)
{
	timer->stop();
	emit buildComplete();
	close();
}

//-----------------------------------------------------------------------------

void BuildWindow::cancelBuild(void)
{
	emit buildComplete();
	builder.Cancel();
}