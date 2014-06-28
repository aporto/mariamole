#include "preferences.h"

//-----------------------------------------------------------------------------

Preferences::Preferences(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	ui.editorFontName->setFontFilters(QFontComboBox::ScalableFonts | QFontComboBox::NonScalableFonts | QFontComboBox::MonospacedFonts);

	connect(ui.btnOk, SIGNAL(clicked()), this, SLOT(OnOk()));
	connect(ui.btnCancel, SIGNAL(clicked()), this, SLOT(OnCancel()));
	connect(ui.menuList, SIGNAL(currentItemChanged( QListWidgetItem * , QListWidgetItem * )), 
		this, SLOT(PageChange( QListWidgetItem * , QListWidgetItem * )));
	//connect(ui.btnCancel, SIGNAL(triggered()), this, SLOT(BuildProject()));
}

//-----------------------------------------------------------------------------

Preferences::~Preferences()
{

}

//-----------------------------------------------------------------------------

void Preferences::OnOk(void)
{
	ok = true;

	config.uploadTimeout = ui.uploadTimeout->value();
	config.extraArduinoLibsSearchPaths = ui.userLibraries->text();
	config.editorFontName = ui.editorFontName->currentFont().family();
	config.editorFontSize = ui.editorFontSize->value();
	config.useMenuButton = ui.embedMenu->isChecked();

	config.Save();

	close();
}

//-----------------------------------------------------------------------------

void Preferences::OnCancel(void)
{
	close();
}

//-----------------------------------------------------------------------------

void Preferences::PageChange ( QListWidgetItem * current, QListWidgetItem * previous )
{
	int index = ui.menuList->currentRow();
	if (index < 0) {
		index = 0;
	}

	ui.stackedWidget->setCurrentIndex(index);
}

//-----------------------------------------------------------------------------

bool Preferences::Edit(void)
{
	ui.menuList->setCurrentRow(0);
	ui.stackedWidget->setCurrentIndex(0);

	ui.uploadTimeout->setValue(config.uploadTimeout);
	ui.userLibraries->setText(config.extraArduinoLibsSearchPaths);	
	ui.editorFontSize->setValue(config.editorFontSize);
	ui.embedMenu->setChecked(config.useMenuButton);

	//ui.editorFontName->currentFont().setFamily("");
	QFont font(config.editorFontName);
	ui.editorFontName->setCurrentFont(font);

	ok = false;
	exec();

	return ok;
}

//-----------------------------------------------------------------------------
