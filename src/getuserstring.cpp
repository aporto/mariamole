#include "getuserstring.h"

GetUserString::GetUserString(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	connect(ui.btnOk, SIGNAL(clicked()), this, SLOT(OnOk()));
	connect(ui.btnCancel, SIGNAL(clicked()), this, SLOT(OnCancel()));
}

GetUserString::~GetUserString()
{

}

void GetUserString::OnOk(void)
{
	ok = true;
	close();
}

void GetUserString::OnCancel(void)
{
	ok = false;
	close();
}



QString GetUserString::GetNewName(QString oldName, bool isFile)
{
	if (isFile) {
		this->setWindowTitle("Rename file");
		ui.label->setText("Type the new file name:");
	} else {
		this->setWindowTitle("Rename project");
		ui.label->setText("Type the new project name:");
	}
	ui.userText->setText(oldName);
	ui.userText->selectAll();

	ok = false;
	exec();
	if (ok) {
		return ui.userText->text();
	} else {
		return "";
	}
}


