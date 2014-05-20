#include "project.h"

//-----------------------------------------------------------------------------

Project::Project(void)
{
}

//-----------------------------------------------------------------------------

Project::~Project(void)
{
}

//-----------------------------------------------------------------------------

void Project::Clear(void)
{
	files.clear();}

//-----------------------------------------------------------------------------

bool Project::Load(QString workspace, QString filename)
{
	QString filepath = QDir::cleanPath(workspace + QDir::separator() + filename + 
			QDir::separator() + filename + ".mmproj");
	QFile file(filepath);
	
	if (file.open(QIODevice::ReadOnly) == false) {
		return false;
	}
	
	QXmlStreamReader xml;
	xml.setDevice(&file);

	if (xml.atEnd()) {
		return false;
	}

	QXmlStreamReader::TokenType token = xml.readNext();
	if(token == QXmlStreamReader::StartDocument) {
		token = xml.readNext();
	}
	
	if (xml.atEnd() || xml.hasError()) {
		return false;
	}

	QString rootName = xml.name().toString();
	if(rootName != "mm_project") {
		file.close();
		return false;
	}
	
	Clear();
	name = filename;

	QXmlStreamAttributes attributes = xml.attributes();
	if(attributes.hasAttribute("serial_port")) { serialPort = attributes.value("serial_port").toString(); }
	if(attributes.hasAttribute("programmer")) { programmer = attributes.value("programmer").toString(); }
	if(attributes.hasAttribute("board")) { boardName = attributes.value("board").toString(); }
	if(attributes.hasAttribute("includePaths")) { includePaths = attributes.value("includePaths").toString(); }
	if(attributes.hasAttribute("libPaths")) { libPaths = attributes.value("libPaths").toString(); }
	if(attributes.hasAttribute("srcPaths")) { srcPaths = attributes.value("srcPaths").toString(); }
	if(attributes.hasAttribute("libs")) { libs = attributes.value("libs").toString(); }
	if(attributes.hasAttribute("useCodeAutomation")) { useCodeAutomation = (attributes.value("useCodeAutomation").toInt() == 1); }
	if(attributes.hasAttribute("linkPrintfVersion")) { linkPrintfVersion = attributes.value("linkPrintfVersion").toInt(); }
	if(attributes.hasAttribute("serialPortSpeed")) { serialPortSpeed = attributes.value("serialPortSpeed").toString(); }	
		
	while (!xml.atEnd()) {
		token = xml.readNext();
		QString key = xml.name().toString();
		if  ( ( (key == "file") || (key == "external") ) || (key == "extra") ) {			
			attributes = xml.attributes();
			//token = xml.readNext();
			ProjectFile f;
			if(attributes.hasAttribute("name")) { 
				f.name = attributes.value("name").toString(); 
				if(attributes.hasAttribute("open")) { f.open = (attributes.value("open").toInt() == 1); }		
				if (key == "external") {
					f.type = ptExternal;				
				} else if (key == "extra") {
					f.type = ptExtra;				
				} else  {
					f.type = ptSource;
				}

				QFileInfo file(f.name);
				QString filename = file.fileName(); // Return only a file name
				if (filename != f.name) {
					f.name = filename;
					AddPath(srcPaths, file.absoluteFilePath());
				}
				files.push_back(f);
			}
		}
	}

	if (xml.hasError()) {
		Clear();
		file.close();
		return false;
	}
	
	file.close();
	return true;
}

//-----------------------------------------------------------------------------

void Project::AddPath(QString & pathList, QString newPath)
{
	QStringList paths = pathList.split(";");
	for (int i=0; i < paths.size(); i++) {
		if (paths.at(i).compare(newPath, Qt::CaseInsensitive) == 0) {
			return;
		}
	}
	pathList += ";" + newPath;
}

//----------------------------------------------------------------------------