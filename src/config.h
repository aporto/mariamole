#pragma once

//-----------------------------------------------------------------------------

#include <qstring.h>
#include <qsettings.h>
#include <QDir>
#include <QtXml/QtXml>

#include <map>

#include "project.h"

//-----------------------------------------------------------------------------

using namespace std;

//-----------------------------------------------------------------------------

struct BoardDef {
	QString name;
	QString type;
	QString upload_protocol;
	QString upload_maxsize;
	QString upload_speed;
	QString fuses_low, fuses_high, fuses_extended;
	QString bootloader_path, bootloader_file;
	QString bootloader_unlockbits, bootloader_lock_bits;
	QString build_mcu, build_f_cpu, build_core, build_variant;
	QString build_vid, build_pid;
	bool disable_flushing;
};

struct ProgrammerDef {
	QString name;
	QString communication;
	QString protocol;
	QString force;
	QString delay;
	QString speed;
};

struct BuildDef {
	QString name;
	QString coreLibs;	
};


//-----------------------------------------------------------------------------

class Config
{
public:
	Config(void);
	~Config(void);

	int Load(void);
	bool Save(void);
	QString DecodeMacros(QString inputText, Project const * const project);

	QString workspace;
	QString includePaths;
	QString libPaths;
	QString libs;
	QString extraArduinoLibsSearchPaths;
	//QString coreLibsPath;
	unsigned int uploadTimeout;
	map <QString, BoardDef> boards;
	map <QString, ProgrammerDef> programmers;
	map <QString, BuildDef> builds;
	
private:	
	QString appPath;
	vector <QString> macros;

	int LoadHardwareDefinitions(void);
	QString ReadXMLNode(QDomNode xml, QString attribute, QString defaultValue);
};

//-----------------------------------------------------------------------------

extern Config config;

//-----------------------------------------------------------------------------
