#ifndef MM_UTILS_H_
#define MM_UTILS_H_

#include <QMessageBox>
#include <QApplication>
#include <QString>
#include <QFile>
#include <QTextStream>

bool GetUserConfirmation(QString msg);

void ErrorMessage(QString msg);

void LoadStyleSheet(QWidget * widget, QString file);

#endif
