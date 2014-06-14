#ifndef SETWORKSPACE_H
#define SETWORKSPACE_H

#include <QDialog>
#include <QFiledialog>

#include "ui_setworkspace.h"

#include "config.h" 

class SetWorkspace : public QDialog
{
	Q_OBJECT

public:
	SetWorkspace(QWidget *parent = 0);
	~SetWorkspace();
	bool Select(void);

	public slots:
		void SelectPath(void);
		void Cancel(void);
		void Ok(void);
private:
	Ui::SetWorkspace ui;
	bool ok;
};

#endif // SETWORKSPACE_H
