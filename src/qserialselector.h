#ifndef QSERIALSELECTOR_H
#define QSERIALSELECTOR_H

#include <QWidget>
#include <QFocusEvent>
#include <QComboBox>
#include "ui_qserialselector.h"
#include <QtSerialPort/QSerialPortInfo>


class QSerialSelector : public QComboBox
{
	Q_OBJECT

public:
	QSerialSelector(QWidget *parent = 0);
	~QSerialSelector();

protected: 
    //void focusInEvent(QFocusEvent * e);
    bool eventFilter (QObject *object, QEvent *event);

private:
	Ui::QSerialSelector ui;
};

#endif // QSERIALSELECTOR_H
