#ifndef ABOUT_H
#define ABOUT_H

#include <QWidget>
#include <QDialog>

#include "ui_about.h"

#include "git_version.h"

class About : public QDialog
{
	Q_OBJECT

public:
	About(QWidget *parent = 0);
	~About();

public:
	void Display(void);

public slots:
	void OnButtonClose(void);

private:
	Ui::About ui;
};

#endif // ABOUT_H
