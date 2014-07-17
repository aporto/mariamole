#include "config.h"

//-----------------------------------------------------------------------------

Config config;


//-----------------------------------------------------------------------------

Config::Config(void)
{	
}

//-----------------------------------------------------------------------------

Config::~Config(void)
{
	//Save();
}

//-----------------------------------------------------------------------------

int Config::Load(void)
{
	appPath = qApp->applicationDirPath();

	QSettings settings(QSettings::IniFormat, QSettings::UserScope, 
		"MariaMole", "config");

	settings.beginGroup("main");
	workspace = settings.value("workspace", "").toString();
	useMenuButton= settings.value("useMenuButton", "1").toBool();
	settings.endGroup();

	settings.beginGroup("build");
	includePaths = settings.value("includePaths", "").toString();
	libPaths = settings.value("libPaths", "").toString();
	libs = settings.value("libs", "").toString();
	
  uploadTimeout = settings.value("uploadTimeout", 30).toInt();

#ifdef Q_OS_WIN
	arduinoInstall = settings.value("arduinoInstall", appPath +"/arduino").toString();
#endif

#ifdef Q_OS_LINUX	
	arduinoInstall = settings.value("arduinoInstall", "/usr/share/arduino").toString();
#endif

#ifdef Q_OS_MAC
	arduinoInstall = settings.value("arduinoInstall", appPath).toString();
	//arduinoInstall = settings.value("arduinoInstall", "").toString();
	//arduinoInstall = arduinoInstall.length() > 0
	//				? arduinoInstall
	//				: qApp->applicationDirPath();
#endif		

  qDebug() << "arduinoInstall" << arduinoInstall;


#ifdef Q_OS_WIN	
  configPath = appPath;
  configUserPath = QDir::homePath() + "/MariaMole";    
#endif

#ifdef Q_OS_LINUX
  configUserPath = QDir::homePath() + "/.mariamole";
  configPath = "/etc/mariamole";    
#endif

#ifdef Q_OS_MAC
  configUserPath = QDir::homePath() + "/Library/mariamole";
  configPath = "/etc/mariamole";    
#endif

  avrPath = settings.value("avrPath", arduinoInstall + "/hardware/tools/avr/bin").toString();
	//avrPath = arduinoInstall + "/hardware/tools/avr/bin";
  //qDebug() << "avrPath" << avrPath;
  settings.endGroup();
	
	settings.beginGroup("arduino");
	extraArduinoLibsSearchPaths = settings.value("extraArduinoLibsSearchPaths", "").toString();
	settings.endGroup();
	
	settings.beginGroup("ui");
	themeName  = settings.value("themeName", "MariaMole").toString();	
	editorFontName  = settings.value("editorFontName", "Consolas").toString();
	editorFontSize  = settings.value("editorFontSize", "12").toInt();
	editorColorName = settings.value("editorColorName", "#182022").toString();

	settings.endGroup();

	settings.beginGroup("compiler");
	hideCompilerWarnings = settings.value("hideCompilerWarnings", "0").toBool();
	settings.endGroup();
	
	int res = LoadHardwareDefinitions();
	if (res != 0) {
		return res;
	}

	res = LoadStyles();

	return res;
}

//-----------------------------------------------------------------------------

int Config::LoadHardwareDefinitions(void)
{
	QString filepath = configUserPath + "config/hardware.xml";
	
	if(!QFile::exists(filepath)) {
		filepath = configPath + "/config/hardware.xml";
	}
	if(!QFile::exists(filepath)) {
		filepath = appPath + "/config/hardware.xml";
	}

	qDebug() << "Loading " << filepath;

	QFile file(filepath);
	
	if (file.open(QIODevice::ReadOnly) == false) {
		return 101;
	}

	QDomDocument doc("mydocument");
	if (doc.setContent(&file) == false) {
		file.close();
		return 102;
	}

	file.close();

	QDomElement docElem = doc.documentElement();
	QDomNode xmlHw = docElem.firstChild();
	
	while(xmlHw.isNull() == false) {
		if (xmlHw.nodeName() == "boards") {
			QDomNode xmlBoard = xmlHw.firstChild();
			while (xmlBoard.isNull() == false) {
				BoardDef board;
				QDomElement elemName = xmlBoard.toElement();
				board.name = elemName.attribute("name");// xmlBoard.attributes().namedItem("name").TextNode();
				
				board.type = ReadXMLNode(xmlBoard, "type", "");				
				board.bootloader_file = ReadXMLNode(xmlBoard, "bootloader.file", "");
				board.bootloader_path = ReadXMLNode(xmlBoard, "bootloader.path", "");
				board.bootloader_unlockbits = ReadXMLNode(xmlBoard, "bootloader.lock_bits", "").toInt();
				board.bootloader_lock_bits = ReadXMLNode(xmlBoard, "bootloader.unlock_bits", "").toInt();
				
				board.build_core = ReadXMLNode(xmlBoard, "build.core", "");
				board.build_f_cpu = ReadXMLNode(xmlBoard, "build.f_cpu", "");
				board.build_mcu = ReadXMLNode(xmlBoard, "build.mcu", "");
				board.build_variant = ReadXMLNode(xmlBoard, "build.variant", "");
				board.build_pid =  ReadXMLNode(xmlBoard, "build.pid", "");
				board.build_vid =  ReadXMLNode(xmlBoard, "build.vid", "");
				
				board.fuses_extended = ReadXMLNode(xmlBoard, "bootloader.extended_fuses", "").toInt();
				board.fuses_high = ReadXMLNode(xmlBoard, "bootloader.high_fuses", "").toInt();
				board.fuses_low = ReadXMLNode(xmlBoard, "bootloader.low_fuses", "").toInt();
				
				board.upload_maxsize = ReadXMLNode(xmlBoard, "upload.maxsize", "");
				board.upload_protocol = ReadXMLNode(xmlBoard, "upload.protocol", "");
				board.upload_speed = ReadXMLNode(xmlBoard, "upload.speed", "");
				board.disable_flushing = (ReadXMLNode(xmlBoard, "upload.speed", "0").toInt() != 0);				
			
				boards.insert (pair <QString, BoardDef> (board.name, board));
				xmlBoard = xmlBoard.nextSibling();
			}
		} else if (xmlHw.nodeName() == "programmers") {
			QDomNode xmlProgrammer = xmlHw.firstChild();
			while (xmlProgrammer.isNull() == false) {			
				ProgrammerDef programmer;
				QDomElement elemName = xmlProgrammer.toElement();
				programmer.name = elemName.attribute("name");
				
				programmer.communication = ReadXMLNode(xmlProgrammer, "communication", "");				
				programmer.delay = ReadXMLNode(xmlProgrammer, "delay", "");
				programmer.force = ReadXMLNode(xmlProgrammer, "force", "");
				programmer.protocol = ReadXMLNode(xmlProgrammer, "protocol", "");
				programmer.speed = ReadXMLNode(xmlProgrammer, "speed", "");
				
				programmers.insert (pair <QString, ProgrammerDef> (programmer.name, programmer));
				xmlProgrammer = xmlProgrammer.nextSibling();
			}
		} else if (xmlHw.nodeName() == "builds") {
			QDomNode xmlBuild = xmlHw.firstChild();
			while (xmlBuild.isNull() == false) {			
				BuildDef build;
				QDomElement elemName = xmlBuild.toElement();
				build.name = elemName.attribute("name");				
				build.coreLibs = ReadXMLNode(xmlBuild, "core_files", "");								
				builds.insert (pair <QString, BuildDef> (build.name, build));
				xmlBuild = xmlBuild.nextSibling();
			}
		}
		xmlHw = xmlHw.nextSibling();		
	}		
	return 0;
}

//-----------------------------------------------------------------------------

void Config::GetThemeStyle(QString themeName, QString styleName, TextStyle &style)
{
	map <QString, ColorTheme>::iterator theme;
	map <QString, ColorTheme>::iterator themeDef;
	theme = colorThemes.find(themeName);
	
	themeDef = colorThemes.find(themeName);
	if (themeDef == colorThemes.end()) {
		return;
	}
	if (themeDef->second.styles.find("DEFAULT") == themeDef->second.styles.end()) {
		return;
	}
	TextStyle defStyle = themeDef->second.styles.find("DEFAULT")->second;
	if (defStyle.fontName == "") {		
		style.fontName = config.editorFontName;
	} else {
		style.fontName = defStyle.fontName;
	}
	if (defStyle.fontSize < 1) {		
		style.fontSize = config.editorFontSize;
	} else {
		style.fontSize = defStyle.fontSize;
	}
	style.backColor = defStyle.backColor;//QColor (120, 120, 120);
	style.foreColor = defStyle.foreColor; //QColor (24, 30, 32);
	style.bold = defStyle.bold;
	style.italic = defStyle.italic;
	style.underline = defStyle.underline;

	if (theme == colorThemes.end()) {
		theme = themeDef;
	}
	
	if (theme != colorThemes.end()) {
		map <QString, TextStyle>::iterator fStyle;
		fStyle = theme->second.styles.find(styleName);
		if (fStyle != theme->second.styles.end()) {
			if (fStyle->second.fontName != "") {
				style.fontName = fStyle->second.fontName;
			}
			if (fStyle->second.fontSize > 0) {
				style.fontSize = fStyle->second.fontSize;
			}

			style.italic = fStyle->second.italic;
			style.bold = fStyle->second.bold;
			style.underline = fStyle->second.underline;

			int alpha = fStyle->second.foreColor.alpha();
			if (alpha != 100) {
				style.foreColor = fStyle->second.foreColor;
			}			
			alpha = fStyle->second.backColor.alpha();
			if (alpha != 100) {
				style.backColor = fStyle->second.backColor;
			}
		}
	}
}

//-----------------------------------------------------------------------------


int Config::LoadStyles(void)
{
	QString filepath = QDir::cleanPath(appPath + QDir::separator() + "config" + 
											QDir::separator() + "color_themes.xml");

#if defined(Q_OS_LINUX) || defined(Q_OS_MAC)
	if(!QFile::exists(filepath))
		filepath = "/etc/mariamole/config/color_themes.xml";
#endif

	QFile file(filepath);
	
	if (file.open(QIODevice::ReadOnly) == false) {
		return 205;
	}

	QDomDocument doc("mydocument");
	if (doc.setContent(&file) == false) {
		file.close();
		return 206;
	}

	file.close();

	QDomElement docElem = doc.documentElement();
	QDomNode xmlThemes = docElem.firstChild();
	
	//if (xmlThemes.isNull() == false) {
	//QDomNode xmlTheme = xmlThemes.firstChild();
	QDomNode xmlTheme = docElem.firstChild();
	while (xmlTheme.isNull() == false) {
		ColorTheme theme;
		QDomElement elemName = xmlTheme.toElement();
		theme.name = elemName.attribute("name");
		QDomNode xmlStyle = xmlTheme.firstChild();			
		while (xmlStyle.isNull() == false) {
			TextStyle style;
			QDomElement elStyle = xmlStyle.toElement();
			QString styleName = elStyle.attribute("name");
			style.fontName = elStyle.attribute("fontName", "");
			style.fontSize = elStyle.attribute("fontSize", "-1").toInt();
			style.foreColor = QColor(0,0,0,100);
			style.backColor = QColor(0,0,0,100);
			QString str = "#" + elStyle.attribute("foreColor", "");
			if (str != "#") {
				style.foreColor = QColor(str);
			}
			str = "#" + elStyle.attribute("backColor", "");
			if (str != "#") {
				style.backColor = QColor(str);
			}
			/*if (c >= 0) {
				style.foreColor = c;
			} else {
				style.foreColor = QColor(0,0,0,100);
			}
			c = elStyle.attribute("backColor", "-1").toInt();
			if (c >= 0) {
				style.backColor = c;
			} else {
				style.backColor = QColor(0,0,0,100);
			}*/
			style.bold = (elStyle.attribute("bold", "0") == "1");
			style.underline = (elStyle.attribute("underline", "0") == "1");
			style.italic = (elStyle.attribute("italic", "0") == "1");				

			if (styleName != "") {
				theme.styles.insert(pair <QString, TextStyle> (styleName, style));
			}
			xmlStyle = xmlStyle.nextSibling();
		}
			
		colorThemes.insert (pair <QString, ColorTheme> (theme.name, theme));
		xmlTheme = xmlTheme.nextSibling();
	}		
	return 0;
}

//-----------------------------------------------------------------------------

QString Config::DecodeMacros(QString inputText, Project const * const project)
{
	QString outputText = inputText;

	QString boardName = "";
	QString buildCore = "";
	QString projectLibPaths = "";
	QString projectIncludePaths = "";
	map <QString, BoardDef>::const_iterator board = boards.end();
	map <QString, BuildDef>::const_iterator build = builds.end();

	if (project != NULL) {
		board = boards.find(project->boardName);
		build = builds.find(board->second.build_core);
		projectLibPaths = project->libPaths;
		projectIncludePaths = project->includePaths;
	}

	map <QString, QString> dictionary;

	if ( (board != boards.end() && build != builds.end() ) ) {


    QString corePath = arduinoInstall + "/hardware/arduino/cores/" + board->second.build_core;
    qDebug() << "corePath" << corePath;

		dictionary.insert (pair <QString, QString> ("$(ARDUINO_CORE)", corePath));

		QString variantPath = arduinoInstall + "/hardware/arduino/variants/" + board->second.build_variant;

		dictionary.insert (pair <QString, QString> ("$(ARDUINO_VARIANT)", variantPath));
	}

	dictionary.insert (pair <QString, QString> ("$(ARDUINO_LIBS)", arduinoInstall + "/libraries"));


  dictionary.insert (pair <QString, QString> ("$(LIBRARIES)", projectLibPaths
																															+ ";"
																															+ arduinoInstall
																															+ "/libraries;"
																															+ libPaths +
																															";" +
																															config.extraArduinoLibsSearchPaths));


	dictionary.insert (pair <QString, QString> ("$(INCLUDES)", includePaths + ";" + projectIncludePaths));
  

	map <QString, QString>::iterator dict;
	for (dict = dictionary.begin(); dict != dictionary.end(); dict++) {
		int pos = outputText.indexOf(dict->first);
		while (pos >= 0) {
			outputText.remove(pos, dict->first.length());
			outputText.insert(pos, dict->second);
			pos = outputText.indexOf(dict->first);
		}
	}
		

	return outputText;
}

//-----------------------------------------------------------------------------

QString Config::ReadXMLNode(QDomNode xml, QString attribute, QString defaultValue)
{
	QDomNode xml2 = xml.firstChildElement(attribute);
	if (xml2.isNull()) {
		return defaultValue;
	}
	
	QDomElement elemName = xml2.toElement();
	return elemName.text();	
}

//-----------------------------------------------------------------------------

bool Config::Save(void)
{
	QSettings settings(QSettings::IniFormat, QSettings::UserScope, 
		"MariaMole", "config");

	settings.beginGroup("main");
	settings.setValue("workspace", workspace);
	settings.setValue("useMenuButton", useMenuButton);
	settings.endGroup();
	
	settings.beginGroup("build");
	settings.setValue("includePaths", includePaths);
	settings.setValue("libPaths", libPaths);
	settings.setValue("libs", libs);
	//settings.setValue("arduinoCoreOpt", arduinoCoreOpt);
	settings.setValue("arduinoInstall", arduinoInstall);
	settings.setValue("uploadTimeout", uploadTimeout);	
	settings.setValue("avrPath", avrPath);	
	settings.endGroup();

	settings.beginGroup("arduino");
	settings.setValue("extraArduinoLibsSearchPaths", extraArduinoLibsSearchPaths);			
	settings.endGroup();

	settings.beginGroup("ui");
	settings.setValue("themeName", themeName);			
	settings.setValue("editorFontName", editorFontName);			
	settings.setValue("editorFontSize", editorFontSize);			
	settings.setValue("editorColorName", editorColorName);
	settings.endGroup();

	settings.beginGroup("compiler");
	settings.setValue("hideCompilerWarnings", hideCompilerWarnings);				 
	settings.endGroup();
	
	return true;
}

//-----------------------------------------------------------------------------

// decode a lib path in the format "$(XXXX)\path1\...\path_n" to  "full_path\path_n"
QString Config::DecodeLibraryPath(QString libPath)
{
	int pos = libPath.indexOf("$(");
	if (pos < 0) {
		return libPath;
	}

	QString macro = libPath;
	QString path1 = libPath;
	path1 = path1.remove(pos, libPath.length());
	macro = macro.remove(0, pos);
	pos = macro.indexOf(")");
	if (pos < 0) {
		return libPath;
	}
	QString path2 = macro;
	path2 = path2.remove(0, pos+1);
	macro = macro.remove(pos+1, macro.length());	

	macro = DecodeMacros(macro, NULL);
	QStringList list = macro.split(";");
	
	for (int i=0; i < list.count(); i++) {
		QString dirPath = list[i].trimmed();
		if (dirPath.length() < 2) {
			continue;
		}
		if ( (dirPath.at(dirPath.length()-1) == '/') || (dirPath.at(dirPath.length()-1) == '\\')) {
			dirPath = dirPath.left(dirPath.length()-1);
		}
		dirPath = path1 + dirPath + path2;
		if (QDir(dirPath).exists()) {
			return dirPath;
		}
	}

	return "";
}

//-----------------------------------------------------------------------------

QString Config::LocateFileUsingSearchPaths(QString filename, QString searchPaths, bool isDir)
{
    QStringList list = DecodeMacros(searchPaths, NULL).split(";");
	
	for (int i=0; i < list.count(); i++) {
		QString dirPath = list[i].trimmed();
		if (dirPath.length() < 2) {
			continue;
		}
		if ( (dirPath.at(dirPath.length()-1) == '/') || (dirPath.at(dirPath.length()-1) == '\\')) {
			dirPath = dirPath.left(dirPath.length()-1);
		}
		dirPath += "/" + filename;
		bool exists = false;
		if (isDir) {
			exists = QDir(dirPath).exists();
		} else {
			exists = QFileInfo(dirPath).exists();
		}
		
		if (exists) {
			return dirPath;
		}
	}

	return "";
}

//-----------------------------------------------------------------------------
