#include "preferences.h"

//-----------------------------------------------------------------------------

Preferences::Preferences(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	ui.editorFontName->setFontFilters(QFontComboBox::ScalableFonts | QFontComboBox::NonScalableFonts | QFontComboBox::MonospacedFonts);

    connect(ui.btnOk, SIGNAL(clicked()), this, SLOT(OnOk()));
    connect(ui.btnApply, SIGNAL(clicked()), this, SLOT(OnApply()));
    connect(ui.btnColorPicker  , SIGNAL(clicked()), this, SLOT(ColorPick()));
    connect(ui.btnLoadCore, SIGNAL(clicked()), this, SLOT(OnLoadCore()));
    connect(ui.btnLoadUserLibs, SIGNAL(clicked()), this, SLOT(OnLoadUserLibs()));
	connect(ui.btnCancel, SIGNAL(clicked()), this, SLOT(OnCancel()));
	connect(ui.menuList, SIGNAL(currentItemChanged( QListWidgetItem * , QListWidgetItem * )), 
        this, SLOT(PageChange( QListWidgetItem * , QListWidgetItem * )));


    color = QColor(config.editorColorName);

	ui.colorPicked->setVisible(false);
	ui.btnColorPicker->setVisible(false);
    QPalette palette = ui.colorPicked->palette();
    palette.setColor(QPalette::Background, color);
    ui.colorPicked->setAutoFillBackground(true); // IMPORTANT!
    ui.colorPicked->setPalette(palette);

	ui.cbBraceHighlight->setChecked(config.highlightBraces);

	//editor->setText("#include <Arduino.h>\n\n// single line comment \n/*\nMultiple line comment\n*/\n\n void function (int index) {\n\tchar * value = \"Test\";\n\tchar c='65';\n}");
	//editor->setReadOnly(true);
	ui.cbThemes->clear();
	map <QString, ColorTheme>::iterator theme;
	for (theme = config.colorThemes.begin(); theme != config.colorThemes.end(); theme++) {
		ui.cbThemes->addItem(theme->first);
	}

	if (ui.cbThemes->count() >= 0) {
		ui.cbThemes->setCurrentIndex(0);
		for (int i=0; i < ui.cbThemes->count(); i++) {
			if (ui.cbThemes->itemText(i) == config.themeName) {
				ui.cbThemes->setCurrentIndex(i);
				break;
			}
		}
	}
    //qDebug() << "Color: " << color;
}

//-----------------------------------------------------------------------------

Preferences::~Preferences()
{

}

//-----------------------------------------------------------------------------


void Preferences::ColorPick(void)
{
    color = QColorDialog::getColor(color.isValid() ? color : Qt::transparent, this);

    if (color.isValid())
    {
        //qDebug() << color;
        QPalette palette = ui.colorPicked->palette();
        palette.setColor(QPalette::Background, color);
        ui.colorPicked->setAutoFillBackground(true); // IMPORTANT!
        ui.colorPicked->setPalette(palette);
    }
}




void Preferences::OnOk(void)
{
    OnApply();
	close();
}



void Preferences::OnApply(void)
{
    ok = true;

    config.uploadTimeout = ui.uploadTimeout->value();
    //config.arduinoCoreOpt = ui.arduinoCore->text();
    if(ui.arduinoInstall->text().length() > 0)
	   config.arduinoInstall = ui.arduinoInstall->text();
    else
        config.arduinoInstall =  qApp->applicationDirPath() + "/arduino";
    config.extraArduinoLibsSearchPaths = ui.userLibraries->text();
    config.editorFontName  = ui.editorFontName->currentFont().family();
    config.editorFontSize  = ui.editorFontSize->value();
    config.editorColorName = ui.colorPicked->palette().color(ui.colorPicked->backgroundRole()).name();
    config.useMenuButton   = ui.embedMenu->isChecked();
    config.hideCompilerWarnings = ui.hideWarnings->isChecked();
	config.themeName = ui.cbThemes->currentText();
	config.highlightBraces = ui.cbBraceHighlight->isChecked();

    config.Save();

	emit apply();
}

//-----------------------------------------------------------------------------

void Preferences::OnCancel(void)
{
	close();
}



void Preferences::OnLoadCore(void)
{
    QString path = "";
    if (QDir(config.arduinoInstall).exists()) {
        path = config.arduinoInstall;
    }
    path = QFileDialog::getExistingDirectory(this, tr("Arduino install path"),
                                                path,
                                                QFileDialog::ShowDirsOnly
                                                | QFileDialog::DontResolveSymlinks);

    if (QDir(path).exists()) {
        ui.arduinoInstall->setText(path);
    }
}



void Preferences::OnLoadUserLibs(void)
{
    QString path = "";

    path = QFileDialog::getExistingDirectory(this, tr("User libraries paths"),
                                                QDir::homePath(),
                                                QFileDialog::ShowDirsOnly
                                                | QFileDialog::DontResolveSymlinks);

    qDebug() << path.length();

    if (path.length() > 0)
    {
        if (config.extraArduinoLibsSearchPaths.length() > 0)
            config.extraArduinoLibsSearchPaths += ";" + path;
        else
            config.extraArduinoLibsSearchPaths = path;
    }

    ui.userLibraries->setText(config.extraArduinoLibsSearchPaths);
}


//-----------------------------------------------------------------------------

void Preferences::PageChange ( QListWidgetItem * current, QListWidgetItem * previous )
{
	int index = ui.menuList->currentRow();

    if (index < 0)
		index = 0;

	ui.stackedWidget->setCurrentIndex(index);
}

//-----------------------------------------------------------------------------

bool Preferences::Edit(void)
{
	ui.menuList->setCurrentRow(0);
	ui.stackedWidget->setCurrentIndex(0);

	ui.uploadTimeout->setValue(config.uploadTimeout);
    ui.arduinoInstall->setText(config.arduinoInstall);
	ui.userLibraries->setText(config.extraArduinoLibsSearchPaths);	
	ui.editorFontSize->setValue(config.editorFontSize);
	ui.embedMenu->setChecked(config.useMenuButton);
	ui.hideWarnings->setChecked(config.hideCompilerWarnings);


	//ui.editorFontName->currentFont().setFamily("");
	QFont font(config.editorFontName);
	ui.editorFontName->setCurrentFont(font);

	ok = false;
	exec();

	return ok;
}

//-----------------------------------------------------------------------------
