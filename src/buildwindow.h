#ifndef BUILDWINDOW_H
#define BUILDWINDOW_H

#include <QWidget>
#include <QDialog>
#include <QTimer>
#include <QMouseEvent>
//#include <QtConcurrent>

#include "builderthread.h"
#include "ui_buildwindow.h"

#include "builder.h"

class BuildWindow : public QDialog
{
	Q_OBJECT

public:
	BuildWindow(QWidget *parent = 0);
	~BuildWindow();
	void Build(bool upload);
	void BurnBootloader(void);

public slots:
	void update(void);   
	void completed(void);   
	void cancelBuild(void);

signals:
    void buildComplete(void);

private:
	Ui::BuildWindow ui;
	BuilderThread * thread;	
	QFuture <int> buildResult;
	QTimer * timer;
	bool building;
	int oldProgressBarType;
};

#endif // BUILDWINDOW_H
