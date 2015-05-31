#ifndef BURNBOOTLOADER_H
#define BURNBOOTLOADER_H

#include <QDialog>
#include "ui_burnbootloader.h"

#include "builder.h"

class BurnBootloader : public QDialog
{
	Q_OBJECT

public:
	BurnBootloader(QWidget *parent = 0);
	~BurnBootloader();
	bool Configure(void);

public slots:
	void Ok(void);
	void Cancel(void);

private:
	Ui::BurnBootloader ui;
	bool ok;
};

#endif // BURNBOOTLOADER_H
