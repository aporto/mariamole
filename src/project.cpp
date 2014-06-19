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
				


/*
				if (xmlFiles.nodeName() == "files") {
					QDomNode xmlFiles = xml.firstChild();
					if (xmlFiles.nodeName() == "files") {
						
					
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
	if(attributes.hasAttribute("isLibrary")) { isLibrary = (attributes.value("isLibrary").toInt() == 1); }	
		
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
	*/
	//file.close();
/*	return true;
}
*/
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
	//xmlWriter->writeAttribute(i.key(), i.value());

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
	xmlWriter->writeAttribute("linkPrintfVersion", QString::number(linkPrintfVersion));
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

QString Project::FindExternalFile(QString filepath)
{
	return "";
}