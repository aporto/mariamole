#ifndef BUILDWINDOW_H
#define BUILDWINDOW_H

#include <QWidget>
#include <QProgressDialog>
#include <QDialog>
#include <QTimer>
#include <QMouseEvent>

#include "ui_buildwindow.h"

namespace BuildWindowTypes {
	enum phaseType {cleaning, compiling, linking, linkingCore, uploading, bootloader};
}

class BuildWindow : public QProgressDialog 
{
	Q_OBJECT

public:
	BuildWindow(QWidget *parent = 0);
	~BuildWindow();
	
	void SetPhase(BuildWindowTypes::phaseType phase);
	void SetProgress(int progress);

private:
	Ui::BuildWindow ui;
};

#endif // BUILDWINDOW_H
