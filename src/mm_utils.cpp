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
	//QMessageBox::StandardButton reply;
	mb.warning(NULL, "MariaMole", msg);
}

void LoadStyleSheet(QWidget * widget, QString file)
{
	// Load stylesheet
	QString cssFileName =  qApp->applicationDirPath() + "/styles/" + file;
	QFile cssFile(cssFileName);
	cssFile.open(QFile::ReadOnly | QFile::Text);
    QTextStream css(&cssFile);
	QString styleText = css.readAll();
	widget->setStyleSheet(styleText);
}

bool PrepareSerialPort(QString portId, QString portSpeed)
{
#ifdef Q_OS_WIN
	// This is (probably) only necessary on Windows:
	// serial port timeouts shall be configured before using Qt serial port
	// this is not supported by QSerialPort
	DCB dcbSerialParams = {0};
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
	QString name = "\\\\.\\" + portId;
	HANDLE hSerial = CreateFileA(portId.toLocal8Bit(), GENERIC_READ | GENERIC_WRITE, 0, NULL,
						OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (!GetCommState(hSerial, &dcbSerialParams)) {
		return false;
	}
	dcbSerialParams.BaudRate=portSpeed.toInt();
	dcbSerialParams.ByteSize=8;
	dcbSerialParams.StopBits=ONESTOPBIT;
	dcbSerialParams.Parity=NOPARITY;
	if (!SetCommState(hSerial, &dcbSerialParams)){
		CloseHandle(hSerial);
		return false;
	}

	COMMTIMEOUTS timeouts={0};
	timeouts.ReadIntervalTimeout=MAXDWORD;
	timeouts.ReadTotalTimeoutConstant=1;
	timeouts.ReadTotalTimeoutMultiplier=1;

	timeouts.WriteTotalTimeoutConstant=0;
	timeouts.WriteTotalTimeoutMultiplier=0;
	SetCommTimeouts(hSerial, &timeouts);
	CloseHandle(hSerial);
#endif 

	return true;
}
