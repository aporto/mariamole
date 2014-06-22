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

void LoadStyleSheet(QWidget * widget, QString file)
{
	// Load stylesheet
	QString cssFileName =  qApp->applicationDirPath() + "/config/" + file;
	QFile cssFile(cssFileName);
	cssFile.open(QFile::ReadOnly | QFile::Text);
    QTextStream css(&cssFile);
	QString styleText = css.readAll();
	widget->setStyleSheet(styleText);
}