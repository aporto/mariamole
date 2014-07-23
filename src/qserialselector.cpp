#include "qserialselector.h"

QSerialSelector::QSerialSelector(QWidget *parent)
	: QComboBox(parent)
{
    ui.setupUi(this);
    installEventFilter(this);
}

QSerialSelector::~QSerialSelector()
{

}



bool QSerialSelector::eventFilter (QObject *object, QEvent *event)
{
    if (event->type() != QEvent::Show) {
        //QComboBox::eventFilter(object, event);
        return QObject::event(event);
    }

    QString curr = "N/A";
    if (currentIndex() >= 0) {
        curr = itemText(currentIndex());
    }

    //cout << "Vamos pra Lapa" << endl;

    QSerialPortInfo serial;
    QStringList info;

    foreach (const QSerialPortInfo &list, QSerialPortInfo::availablePorts()) {
        info << list.portName();
        //cout << list.portName().toStdString() << endl;
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


    QComboBox::eventFilter(object, event);
    return QObject::event( event);
}


/*

void QSerialSelector::focusInEvent(QFocusEvent * e)
{
    if (e->reason() != Qt::MouseFocusReason) {
        QComboBox::focusInEvent(e);
        return;
    }
    QString curr = "N/A";
    if (currentIndex() >= 0) {
        curr = itemText(currentIndex());
    }

    QSerialPortInfo serial;
    QStringList info;

    foreach (const QSerialPortInfo &list, QSerialPortInfo::availablePorts()) {
        info << list.portName();
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
*/
