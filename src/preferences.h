#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QDialog>
#include "ui_preferences.h"

#include "config.h"

class Preferences : public QDialog
{
	Q_OBJECT

public:
	Preferences(QWidget *parent = 0);
	~Preferences();
	bool Edit(void);

public slots:
	void OnOk(void);
	void OnCancel(void);
	void PageChange (QListWidgetItem * current, QListWidgetItem * previous);

private:
	Ui::Preferences ui;
	bool ok;
};

#endif // PREFERENCES_H
