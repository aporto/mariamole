#include "about.h"

About::About(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	setWindowFlags( Qt::FramelessWindowHint);
	connect(ui.btnClose, SIGNAL(clicked()), this, SLOT(OnButtonClose()));
}

About::~About()
{

}

void About::Display(void)
{
	ui.treeWidget->expandAll();
	this->setModal(true);
	show();	
}


void About::OnButtonClose(void)
{
	close();
}
