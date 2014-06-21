#ifndef PROJECTPROPERTIES_H
#define PROJECTPROPERTIES_H

#include <QDialog>
#include <QtSerialPort/QSerialPortInfo>

#include "ui_projectproperties.h"

#include "workspace.h"

class ProjectProperties : public QDialog
{
	Q_OBJECT

public:
	ProjectProperties(QWidget *parent = 0);
	~ProjectProperties();
	bool Edit(Project * project);
	
	public slots:
		void PageChange ( QListWidgetItem * current, QListWidgetItem * previous );
		void Ok(void);
		void Cancel(void);
		void DisableProgrammer(void);
		void EnableProgrammer(void);
		

private:
	Ui::ProjectProperties ui;
	bool ok;
};

#endif // PROJECTPROPERTIES_H
