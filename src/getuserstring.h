#ifndef GETUSERSTRING_H
#define GETUSERSTRING_H

#include <QDialog>
#include "ui_getuserstring.h"

class GetUserString : public QDialog
{
	Q_OBJECT

public:
	GetUserString(QWidget *parent = 0);
	~GetUserString();

	QString GetNewName(QString oldName, bool isFile);

public slots:
	void OnOk(void);
	void OnCancel(void);

private:
	Ui::GetUserString ui;
	bool ok;
};

#endif // GETUSERSTRING_H
