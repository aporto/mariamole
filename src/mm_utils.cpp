#include "mm_utils.h"
#include "mm_utils.h"


bool GetUserConfirmation(QString title, QString msg)
{
	QMessageBox mb;
	mb.setStyleSheet("");
	QMessageBox::StandardButton reply;
	reply = mb.question(NULL, title, msg, QMessageBox::Yes|QMessageBox::No);

	return (reply == QMessageBox::Yes);
}

void ErrorMessage(QString title, QString msg) {
	QMessageBox mb;
	mb.setStyleSheet("");
	QMessageBox::StandardButton reply;
	mb.warning(NULL, title, msg);
}
