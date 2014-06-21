#include "serialmonitor.h"

//-----------------------------------------------------------------------------

SerialMonitor::SerialMonitor(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	setWindowIconText("serial");
	ui.plainTextEdit->setReadOnly(true);

	ui.btnAutoScroll->setVisible(false); // disable autoscroll not yet implemented

	connect (ui.btnReOpen, SIGNAL(clicked()), this, SLOT(ReOpenPort()));
	connect (ui.btnSend, SIGNAL(clicked()), this, SLOT(SendData()));
	connect (ui.lineEdit, SIGNAL(returnPressed()), this, SLOT(SendData()));

	
	//QTimer *timer = new QTimer();
	//connect(timer, SIGNAL(timeout()), this, SLOT(ReadSerialPort()));
    //timer->start(100);
	//QComboBox::currentIndexChanged ( int index ) 

	portId = "";
	portSpeed = "9600";
}

//-----------------------------------------------------------------------------

SerialMonitor::~SerialMonitor()
{
	ClosePort();
}

//-----------------------------------------------------------------------------

void SerialMonitor::CRT(QString text)
{	
	ui.plainTextEdit->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
	ui.plainTextEdit->moveCursor(QTextCursor::EndOfLine, QTextCursor::MoveAnchor);
	ui.plainTextEdit->insertPlainText(text);		
	//ui.plainTextEdit->appendPlainText(text);
	if (ui.plainTextEdit->blockCount() > 200) {
		// ho-ho-ho... This is how I'm supposed to delete a single line from this Qt text editor... ???
		//QTextCursor tc = ui.plainTextEdit->textCursor();
		QTextCursor tcaux = ui.plainTextEdit->textCursor();
		//ui.plainTextEdit->textCursor().movePosition( QTextCursor::Start );
		ui.plainTextEdit->moveCursor(QTextCursor::Start, QTextCursor::MoveAnchor);
		ui.plainTextEdit->moveCursor(QTextCursor::StartOfLine, QTextCursor::MoveAnchor);
		ui.plainTextEdit->moveCursor(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);
		//ui.plainTextEdit->textCursor().select( QTextCursor::LineUnderCursor );
		ui.plainTextEdit->textCursor().removeSelectedText();
		ui.plainTextEdit->moveCursor(QTextCursor::NextBlock, QTextCursor::MoveAnchor);
		ui.plainTextEdit->textCursor().deletePreviousChar();
		ui.plainTextEdit->textCursor().deletePreviousChar();
		ui.plainTextEdit->textCursor().deletePreviousChar();
		ui.plainTextEdit->textCursor().deletePreviousChar();
		ui.plainTextEdit->textCursor().deletePreviousChar();
		ui.plainTextEdit->setTextCursor( tcaux ); 
	}
}

//-----------------------------------------------------------------------------

QString SerialMonitor::GetPort(void)
{
	return portId;
}

//-----------------------------------------------------------------------------

bool SerialMonitor::OpenPort(void)
{
	return OpenPort(portId, portSpeed);
}

//-----------------------------------------------------------------------------

bool SerialMonitor::OpenPort(QString port, QString speed)
{
	if (serialPort.isOpen()) {
		return true;
	} 
		
	portId = port;
	portSpeed = speed;

	int index = ui.cbSpeed->findText(portSpeed);
	if (index < 0) {
		portSpeed = "9600";
	}

	serialPort.setPortName(portId);
	ui.cbSpeed->setCurrentText(portSpeed);

	if (PrepareSerialPort() == false) {
		CRT(">> Failed to open Serial port [" + portId + "] at " + portSpeed + "!\n\r");		
		return false;
	}

	serialPort.setFlowControl(QSerialPort::NoFlowControl);
	serialPort.setBaudRate(portSpeed.toInt());
	serialPort.setParity(QSerialPort::NoParity);
	serialPort.setStopBits(QSerialPort::OneStop);
	serialPort.setDataBits(QSerialPort::Data8);
	if (serialPort.open(QIODevice::ReadWrite)) {		
		//serialPort.setDataTerminalReady(true);
		//serialPort.setRequestToSend(true);
		bool ok = connect (&serialPort, SIGNAL(readyRead()), this, SLOT(ReadSerialPort()));
		CRT(">> Serial port [" + portId + "] open at " + portSpeed + "!\n\r");		
		return true;
	} else {
		CRT(">> Failed to open Serial port [" + portId + "] at " + portSpeed + "!\n\r");		
		return false;
	}
}

//-----------------------------------------------------------------------------

bool SerialMonitor::PrepareSerialPort(void)
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
	timeouts.ReadIntervalTimeout=50;
	timeouts.ReadTotalTimeoutConstant=1;
	timeouts.ReadTotalTimeoutMultiplier=1;

	timeouts.WriteTotalTimeoutConstant=50;
	timeouts.WriteTotalTimeoutMultiplier=1;
	SetCommTimeouts(hSerial, &timeouts);
	CloseHandle(hSerial);
#endif 

	return true;
}

//-----------------------------------------------------------------------------

bool SerialMonitor::ClosePort(void)
{
	if (serialPort.isOpen() == false) {
		return true;
	} 

	bool ok = disconnect (&serialPort, SIGNAL(readyRead()), this, SLOT(ReadSerialPort()));

	serialPort.clear();	

	QThread::msleep(100);

	serialPort.close();	
	if (serialPort.isOpen()) {
		CRT(">> Failed: Could not close the serial port [" + portId + "]\n\r");
		return false;
	} else {
		
		CRT(">> Serial port [" + portId + "] closed!\n\r");
		return true;
	}	
}

//-----------------------------------------------------------------------------

void SerialMonitor::ReOpenPort(void)
{
	if (serialPort.isOpen()) {
		if (ClosePort()) {
			OpenPort();
		}
	} 	
}

//-----------------------------------------------------------------------------

void SerialMonitor::SendData(void)
{
	QString text = ui.lineEdit->text();	
	ui.lineEdit->setFocus();
	ui.lineEdit->selectAll();

	int lineEnding = ui.cbLineEnding->currentIndex();
	
	switch (lineEnding) {
		case 1: text += "\n"; break;
		case 2: text += "\r"; break;
		case 3: text += "\n\r"; break;
	}
	if (serialPort.isOpen()) {
		QByteArray bytes (text.toLocal8Bit());		
		serialPort.write (bytes);//(text.toLocal8Bit(), text.length());
		//serialPort.flush();
	}
}

//-----------------------------------------------------------------------------

void SerialMonitor::ReadSerialPort(void)
{
	if (serialPort.isOpen()) {
		QByteArray bytes = serialPort.readAll();
		if (ui.btnPause->isChecked() == false) {
			CRT(QString(bytes));
		}
		//ui.plainTextEdit->insertPlainText(QString(bytes));		
		/*QByteArray bytes;
		int size = serialPort.bytesAvailable();		
		bytes.resize(size);
		serialPort.read(bytes.data(), bytes.size());
		//bytes = serialPort.readAll();
		int count = bytes.count();
		QString text = "";
		for (int i=0; i < count; i++) {
			unsigned char byte = bytes[i];
			if ( (byte == '\n') || (byte == '\r')) {
				CRT(text);
				text = "";
			} else {
				text += byte;
			}
		}
		if (text.length() > 0) {
			CRT(text);
		}*/
	}
}