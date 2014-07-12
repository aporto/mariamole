#include "qserialselector.h"

QSerialSelector::QSerialSelector(QWidget *parent)
	: QComboBox(parent)
{
	ui.setupUi(this);
}

QSerialSelector::~QSerialSelector()
{

}


void QSerialSelector::focusInEvent(QFocusEvent * e)
{
#ifndef Q_OS_WIN
	// this is not working on windows, but takes a long time to run only on linux, so...
	if (e->reason() != Qt::MouseFocusReason) {
		QComboBox::focusInEvent(e);
		return;
	}	
#endif	
    QString curr = "N/A";
	if (currentIndex() >= 0) {
		curr = itemText(currentIndex());
	}

	QSerialPortInfo serial;
	QStringList info;
	for (int i=0; i < serial.availablePorts().count();i++) {
		info << serial.availablePorts().at(i).portName();
	}

	// add all new serial ports
	for (int i=0; i < info.count();i++) {
		bool exists = false;
		for (int j=0; j < count(); j++) {
			if (itemText(j) == info.at(i)) {
				exists = true;
				break;
			}
		}
		if (exists == false) {
			addItem(info.at(i));
		}
	}

	// remove all serial ports that no longer exist 
	int j = 0;
	while (j < count()) {
		bool exists = false;
		for (int i=0; i < info.count();i++) {
			QString pName = info.at(i);
			QString cName = itemText(j);
			if ( (cName == pName) || (cName == "N/A") ){
				exists = true;
				break;
			}
		}
		if (exists == false) {
			removeItem(j);
		} else {
			j++;
		}
	}

	QComboBox::focusInEvent(e);
}