#ifndef SERIALMONITOR_H
#define SERIALMONITOR_H

#include <QWidget>
#include <QTimer>
#include <QThread>
#include <QtSerialPort/QSerialPort>
#include "ui_serialmonitor.h"

#include "mm_utils.h"

class SerialMonitor : public QWidget
{
	Q_OBJECT

public:
	SerialMonitor(QWidget *parent = 0);
	~SerialMonitor();
	QString GetPort(void);
	bool OpenPort(void);
	bool OpenPort(QString port, QString speed);
	bool ClosePort(void);

public slots:
	void ReOpenPort(void);
	void SendData(void);
	void ReadSerialPort(void);

private:
	Ui::SerialMonitor ui;
	QString portId, portSpeed;
	QSerialPort serialPort;
	
	void CRT(QString text);
};

#endif // SERIALMONITOR_H
