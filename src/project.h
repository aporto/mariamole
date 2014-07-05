#pragma once

//-----------------------------------------------------------------------------

#include <QString>
#include <QtXml/QtXml>
#include <QtXml/QDomNode>
#include <QDir>
#include <vector>

//-----------------------------------------------------------------------------

enum ProjectFileType {ptSource, ptExternal, ptExtra};

struct ProjectFile {
	QString name;
	bool open;
	ProjectFileType type;
};

//-----------------------------------------------------------------------------

//using namespace std;

//-----------------------------------------------------------------------------

class Project
{
public:
	Project(void);
	~Project(void);

	void Clear(void);
	bool Load(QString workspace, QString filename);	
	bool Save(QString workspacePath);
	void AddPath(QString & pathList, QString newPath);

	bool current;
	bool rebuild;

	QString name;
	QString programmer;
	QString boardName;
    std::vector <ProjectFile> files;
	QString serialPort;
	QString includePaths;
	QString libPaths;
	QString srcPaths;
	QString libs;
	bool isLibrary;

	bool useCodeAutomation;
	int linkPrintfVersion;
	QString serialPortSpeed;	
private:
	QString ReadXMLNode(QDomNode xml, QString attribute, QString defaultValue);
};

//-----------------------------------------------------------------------------

