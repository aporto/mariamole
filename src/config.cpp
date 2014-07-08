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
#ifdef Q_OS_WIN
	appPath = qApp->applicationDirPath(); // "C:/Users/aporto/Documents/GitHub/mariamole/build";
#endif

#ifdef Q_OS_LINUX
    appPath = QDir::homePath() + "/.mariamole";
#endif

#ifdef Q_OS_MAC
    appPath = QDir::homePath() + "/Library/mariamole";
#endif

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
    arduinoCoreOpt = settings.value("arduinoCoreOpt", "").toString();
	uploadTimeout = settings.value("uploadTimeout", 30).toInt();	
    settings.endGroup();
	
	settings.beginGroup("arduino");
	extraArduinoLibsSearchPaths = settings.value("extraArduinoLibsSearchPaths", "").toString();
	settings.endGroup();
	
	settings.beginGroup("ui");
    editorFontName  = settings.value("editorFontName", "Consolas").toString();
    editorFontSize  = settings.value("editorFontSize", "12").toInt();
    editorColorName = settings.value("editorColorName", "").toString();

	settings.endGroup();


	settings.beginGroup("compiler");
	hideCompilerWarnings = settings.value("hideCompilerWarnings", "0").toBool();
	settings.endGroup();
	
	int res = LoadHardwareDefinitions();
	if (res != 0) {
		return res;
	}

	return 0;
}

//-----------------------------------------------------------------------------

int Config::LoadHardwareDefinitions(void)
{
	QString filepath = QDir::cleanPath(appPath + QDir::separator() + "config" + 
                                            QDir::separator() + "hardware.xml");


#if defined(Q_OS_LINUX) || defined(Q_OS_MAC)
    if(!QFile::exists(filepath))
        filepath = "/etc/mariamole/config/hardware.xml";
#endif

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
		QString corePath = appPath + "/" + board->second.build_core; 
		corePath += "/" + board->second.build_core + "/cores"; 
		corePath += "/" + board->second.build_core;	
		dictionary.insert (pair <QString, QString> ("$(ARDUINO_CORE)", corePath));

		QString variantPath = appPath + "/" + board->second.build_core; 
		variantPath += "/" + board->second.build_core + "/variants"; 
		variantPath += "/" + board->second.build_variant;
		dictionary.insert (pair <QString, QString> ("$(ARDUINO_VARIANT)", variantPath));
	}

	//QString qApp->applicationDirPath() + ;
	dictionary.insert (pair <QString, QString> ("$(ARDUINO_LIBS)", appPath + "/arduino/arduino/libraries"));

	dictionary.insert (pair <QString, QString> ("$(LIBRARIES)", projectLibPaths + ";" + appPath + "/arduino/arduino/libraries;" + libPaths + ";" + config.extraArduinoLibsSearchPaths ));

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
		
	/*if ((board != boards.end() && build != builds.end()) {
		QString corePath = appPath + "/" + board->second.build_core; 
		corePath += "/" + board->second.build_core + "/cores"; 
		corePath += "/" + board->second.build_core;	
		int pos = outputText.indexOf("$(ARDUINO_CORE)");
		while (pos >= 0) {
			outputText.remove(pos, QString("$(ARDUINO_CORE)").size());
			outputText.insert(pos, corePath);
			pos = outputText.indexOf("$(ARDUINO_CORE)");
		}
	}

	QString variantPath = appPath + "/" + board->second.build_core; 
	variantPath += "/" + board->second.build_core + "/variants"; 
	variantPath += "/" + board->second.build_variant;
	pos = outputText.indexOf("$(ARDUINO_VARIANT)");
	while (pos >= 0) {
		outputText.remove(pos, QString("$(ARDUINO_VARIANT)").size());
		outputText.insert(pos, variantPath);
		pos = outputText.indexOf("$(ARDUINO_VARIANT)");
	}*/
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
    settings.setValue("arduinoCoreOpt", arduinoCoreOpt);
	settings.setValue("uploadTimeout", uploadTimeout);		
	settings.endGroup();

	settings.beginGroup("arduino");
	settings.setValue("extraArduinoLibsSearchPaths", extraArduinoLibsSearchPaths);			
	settings.endGroup();

	settings.beginGroup("ui");
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
