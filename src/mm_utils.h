#ifndef MM_UTILS_H_
#define MM_UTILS_H_

#include <QMessageBox>
#include <QApplication>
#include <QString>
#include <QFile>
#include <QTextStream>

#ifdef Q_OS_WIN
#include <Windows.h>
#endif


bool GetUserConfirmation(QString msg);

void ErrorMessage(QString msg);

void LoadStyleSheet(QWidget * widget, QString file);

bool PrepareSerialPort(QString portId, QString portSpeed);


#endif
