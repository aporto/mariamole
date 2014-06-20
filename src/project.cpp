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

QString Project::ReadXMLNode(QDomNode xml, QString attribute, QString defaultValue)
{
	QDomNode xml2 = xml.firstChildElement(attribute);
	if (xml2.isNull()) {
		return defaultValue;
	}
	
	QDomElement elemName = xml2.toElement();
	return elemName.text();	
}

//-----------------------------------------------------------------------------

bool Project::Load(QString workspace, QString filename)
{
	name = filename;

	QString filepath = QDir::cleanPath(workspace + QDir::separator() + filename + 
			QDir::separator() + filename + ".mmproj");
	QFile file(filepath);
	
	if (file.open(QIODevice::ReadOnly) == false) {
		return false;
	}

	QDomDocument doc("mydocument");
	if (doc.setContent(&file) == false) {
		file.close();
		return false;
	}

	file.close();
	QDomElement docElem = doc.documentElement();
	QDomNode xml = docElem;//.firstChild();
	bool ok = false;

	if (xml.isNull() == false) {
		QString nodeName = xml.nodeName();
		if (nodeName == "mm_project") {
			QDomElement elem = xml.toElement();
			current = (elem.attribute("current").toInt() != 0);
			serialPort = elem.attribute("serial_port");
			programmer = elem.attribute("programmer");
			boardName = elem.attribute("board");
			includePaths = elem.attribute("includePaths");
			libPaths = elem.attribute("libPaths");
			srcPaths = elem.attribute("srcPaths");
			libs = elem.attribute("libs");
			useCodeAutomation = elem.attribute("useCodeAutomation").toInt() == 1;
			linkPrintfVersion = elem.attribute("linkPrintfVersion").toInt(); 
			serialPortSpeed = elem.attribute("serialPortSpeed");	
			isLibrary = elem.attribute("isLibrary").toInt() == 1;

			QDomNode xmlFiles = xml.firstChild();
			while (xmlFiles.isNull() == false) {
				QString type = xmlFiles.nodeName();
				QDomNode xmlFile = xmlFiles.firstChild();				
				while (xmlFile.isNull() == false) {
					QDomElement fileElem = xmlFile.toElement();
					ProjectFile f;
					f.name = fileElem.attribute("name");				
					f.open = (fileElem.attribute("open") == "yes");				
					//f.open = (ReadXMLNode(fileElem, "OPEN", "") == "yes");
					if (type == "externals") {
						f.type = ptExternal;
					} else {
						f.type = ptSource;
					}
					files.push_back(f);
					xmlFile = xmlFile.nextSibling ();
				}
				xmlFiles = xmlFiles.nextSibling();
			}
		}
		ok = true;
	}

	return ok;
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

bool Project::Save(QString workspacePath)
{
	bool ok = false;

	QString path = workspacePath + "/" + name;
	QDir().mkdir(path + "/source");

	QString filepath = path + "/" + name + ".mmproj"; 
	QFile file(filepath);
	
	if (file.open(QIODevice::WriteOnly) == false) {
		return false;
	}

	QXmlStreamWriter* xmlWriter = new QXmlStreamWriter();
	xmlWriter->setDevice(&file);
	xmlWriter->setAutoFormatting(true);
	xmlWriter->writeStartDocument();

	xmlWriter->writeStartElement("mm_project");
	
	if (current) {
		xmlWriter->writeAttribute("current", "1");
	} else {
		xmlWriter->writeAttribute("current", "0");
	}

	xmlWriter->writeAttribute("serial_port", serialPort);
	xmlWriter->writeAttribute("programmer", programmer);
	xmlWriter->writeAttribute("board", boardName);
	xmlWriter->writeAttribute("includePaths", includePaths);
	xmlWriter->writeAttribute("libPaths", libPaths);
	xmlWriter->writeAttribute("srcPaths", srcPaths);
	xmlWriter->writeAttribute("libs", libs);
	if (useCodeAutomation) {
		xmlWriter->writeAttribute("useCodeAutomation", "1");
	} else {
		xmlWriter->writeAttribute("useCodeAutomation", "0");
	}
	xmlWriter->writeAttribute("linkPrintfVersion", QString::number((int)linkPrintfVersion));
	xmlWriter->writeAttribute("serialPortSpeed", serialPortSpeed);
	xmlWriter->writeAttribute("isLibrary", QString::number((int)isLibrary));

	xmlWriter->writeStartElement("files");
	for (int i=0; i < files.size(); i++) {
		if (files.at(i).type == ptSource) {
			xmlWriter->writeStartElement("file");
			xmlWriter->writeAttribute("name", files.at(i).name);
			if (files.at(i).open) {
				xmlWriter->writeAttribute("open", "yes");
			} else {
				xmlWriter->writeAttribute("open", "no");
			}
			xmlWriter->writeEndElement();
		}
	}
	xmlWriter->writeEndElement();

	xmlWriter->writeStartElement("externals");
	for (int i=0; i < files.size(); i++) {
		if (files.at(i).type == ptExternal) {
			xmlWriter->writeStartElement("external");
			xmlWriter->writeAttribute("name", files.at(i).name);
			if (files.at(i).open) {
				xmlWriter->writeAttribute("open", "yes");
			} else {
				xmlWriter->writeAttribute("open", "no");
			}			
			xmlWriter->writeEndElement();
		}
	}
	xmlWriter->writeEndElement();

	xmlWriter->writeEndElement(); // mm_project
	xmlWriter->writeEndDocument();
	delete xmlWriter;

	return true;		  
}

