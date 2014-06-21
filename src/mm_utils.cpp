#include "mm_utils.h"
#include "mm_utils.h"


bool GetUserConfirmation(QString msg)
{
	QMessageBox mb;
	mb.setStyleSheet("");
	QMessageBox::StandardButton reply;
	reply = mb.question(NULL, "MariaMole", msg, QMessageBox::Yes|QMessageBox::No);

	return (reply == QMessageBox::Yes);
}

void ErrorMessage(QString msg) {
	QMessageBox mb;
	mb.setStyleSheet("");
	QMessageBox::StandardButton reply;
	mb.warning(NULL, "MariaMole", msg);
}
