#include "setworkspace.h"

SetWorkspace::SetWorkspace(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	//|setWindowFlags( Qt::FramelessWindowHint);//Qt::CustomizeWindowHint );
	
	connect (ui.pbSelect, SIGNAL(clicked()), this, SLOT(SelectPath()));
	connect (ui.cancelButton, SIGNAL(clicked()), this, SLOT(Cancel()));
	connect (ui.okButton, SIGNAL(clicked()), this, SLOT(Ok()));

}

SetWorkspace::~SetWorkspace()
{

}

void SetWorkspace::SelectPath(void)
{
	QString path = "";
	if (QDir(config.workspace).exists()) {
		path = config.workspace;
	} 

	/*QFileDialog dialog;
	dialog.setFileMode(QFileDialog::Directory);
	if (dialog.exec() != QFileDialog::Accepted) {
		return;
	}*/

	path = QFileDialog::getExistingDirectory(this, tr("Select workspace path"),
                                                path,
                                                QFileDialog::ShowDirsOnly
                                                | QFileDialog::DontResolveSymlinks);
	
	//path = dialog.Directory();
	if (QDir(path).exists()) {
		ui.ebPath->setText(path);
	}
}


bool SetWorkspace::Select(void)
{
	ui.ebPath->setText(config.workspace);

	ok = false;
	setModal(true);
	exec();
	if (ok == false) {
		return false;
	}

	QDir dir(ui.ebPath->text());
	if (dir.exists() == false) {
		return false;
	}

	config.workspace = dir.absolutePath();

	return true;
}

void SetWorkspace::Cancel(void)
{
	close();
}

void SetWorkspace::Ok(void)
{
	if (QDir(ui.ebPath->text()).exists()) {
		ok = true;
		close();
	}
}