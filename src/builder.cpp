#include "builder.h"

Builder::Builder(QWidget *parent)
    : QWidget(parent)//,

{
	lastBuildStatus = 0;
	running = false;
	config.avrPath = config.arduinoInstall + "/hardware/tools/avr/bin";
}

//-----------------------------------------------------------------------------

Builder::~Builder(void)
{
}

//-----------------------------------------------------------------------------

bool Builder::Clean(void)
{
	msg.buildStage = 0;
	project = workspace.GetCurrentProject();
	if (project == NULL) {
		msg.Add("Could not find detect the current project", mtError);
		return false;
	}

	map <QString, BoardDef>::const_iterator board = config.boards.find(project->boardName);
	if (board == config.boards.end()) {
		msg.Add("Could not find board configuration for project: " + project->name, mtError);
		return false;
	}

	// Create the build directory
	buildPath = config.workspace + "/" + project->name + "/build"; 
	QDir dir = QDir(buildPath);
	bool ok = dir.removeRecursively();
	if (ok==false) {
		msg.Add("Error while cleaning project " + project->name + ".", mtError);
	}

	msg.ClearBuildInfo();
	lastBuildStatus = 0;
	return ok;
}

//-----------------------------------------------------------------------------

bool Builder::GetCancel(void)
{
	if (running) {
		return progress->wasCanceled();
	} else {
		return false;
	}
}

//-----------------------------------------------------------------------------

int Builder::GetLastBuildStatus(void)
{
	return lastBuildStatus;
}

//-----------------------------------------------------------------------------

int Builder::GetPercentage(void)
{
	return percentage;
    //return launcher->GetPercentage();;
}

//-----------------------------------------------------------------------------

void Builder::SetPercentage(int value)
{
	percentage = value;
	if (running) {
		progress->setValue(value);
	}
	qApp->processEvents();
	//QThread::sleep(1);
}

//-----------------------------------------------------------------------------
int Builder::Build(bool upload)
{	
	config.avrPath = config.arduinoInstall + "/hardware/tools/avr/bin";
	msg.ClearOutput();
	msg.ClearBuildMessages();

	progress = new BuildWindow((QWidget *)(this->parent())); //"Task in progress...", "Cancel", 0, 100, (QWidget *)this->parent());
	progress->setWindowModality(Qt::WindowModal);
	progress->SetPhase(BuildWindowTypes::compiling);
	SetPercentage(0);		
	progress->show();
	running = true;	

	project = workspace.GetCurrentProject();
	if (project == NULL) {
		return -1;
	}

	if (project->rebuild) {
		if (Clean()) {
			project->rebuild = false;
		}
	}

	map <QString, BoardDef>::const_iterator board = config.boards.find(project->boardName);
	if (board == config.boards.end()) {
		msg.Add("Could not find board configuration for project: " + project->name, mtError);
		return false;
	}

	// Create the build directory
	buildPath = config.workspace + "/" + project->name + "/build"; //QDir().tempPath() + "/mariamole/build/" + project->name;
	QDir().mkpath(buildPath);

	// Get core lib filename
	coreLib = buildPath + "/arduino_core_" + board->second.build_variant+".a";
	
	msg.ClearBuildInfo();

	msg.AddOutput("Detecting undeclared functions in main project file: " + project->name + ".cpp...", false);

	ImportDeclarations();
	bool ok = true;
	for (unsigned int i=0; i < project->files.size(); i++) {
		QString ext = QFileInfo(project->files.at(i).name).suffix().toUpper();
		if ((ext == "CPP") || (ext == "c")) {
			ok = ok & Compile(i);
			if (GetCancel()) {
				msg.Add("Build cancelled by the user!", mtRegular);
				ok = false;
				break;
			}
			SetPercentage(40 * i / project->files.size());			
			//SleepEx(1500, true);
		}
	}

	//QThread::sleep(5);

	if (GetCancel()) {
		msg.Add("Build cancelled by the user!", mtRegular);
		ok = false;
	}
	
	if (ok) {
		ok = Link();
		if (ok) {
			msg.Add("Project " + project->name + " successfully built!", mtSuccess);
			GetBinarySize();
		} else {
			msg.Add("Error linking project " + project->name, mtError);
		}
	} else {
		msg.Add("Error while building project " + project->name + ".", mtError);
	}

	if(ok && upload) {
		ok = Upload();
		if (ok) {
			msg.Add("Project " + project->name + " binaries successfully uploaded to board", mtSuccess);
		} else {
			if (project->programmer != "") {
				msg.Add("Error while uploading program to " + project->boardName 
					+ " using programmer " +  project->programmer + ". Please check the output window for details", mtError);
			} else {
				msg.Add("Error while uploading program to " + project->boardName 
					+ " via USB cable. Please check the output window for details", mtError);
			}
		}

		if (GetCancel()) {
			msg.Add("Build cancelled by the user!", mtRegular);
			ok = false;
		}
	}
	if (ok) {
		lastBuildStatus = 2;
	} else {
		lastBuildStatus = 1;
	}

	running = false;
	progress->hide();
	delete progress;
	return ok;
}

//-----------------------------------------------------------------------------

bool Builder::Upload(void)
{
	msg.buildStage = 3;
	
	map <QString, BoardDef>::const_iterator board = config.boards.find(project->boardName);
	map <QString, BuildDef>::const_iterator build = config.builds.find(board->second.build_core);
	map <QString, ProgrammerDef>::const_iterator programmer = config.programmers.find(project->programmer);

	if (build == config.builds.end()) {
		msg.Add("Error getting build configuration for board " + project->boardName, mtError);
		return false;
	}

	progress->SetPhase(BuildWindowTypes::uploading);
	SetPercentage(0);

	QString uploaderPath = config.avrPath + "/avrdude";
	
	msg.AddOutput("Uploading program to board " + project->boardName, false);	

	QString outputFile = buildPath + "/" + project->name + ".hex";
	QStringList arguments;
#if defined(Q_OS_WIN) || defined(Q_OS_MAC)
	QString confPath = config.arduinoInstall + "/hardware/tools/avr/etc/avrdude.conf";
#elif defined(Q_OS_LINUX)
	QString confPath = config.arduinoInstall + "/hardware/tools/avrdude.conf";
#endif
	arguments << "-C" << confPath;
	//arguments << "-q" << "-q"; // ultra quiet mode
	arguments << "-p" << board->second.build_mcu;
	
	QString speed = "";
	QString communication = "";
	if (project->programmer == "") {
		arguments << "-c" + board->second.upload_protocol;		
		communication = "serial";
		speed = board->second.upload_speed;;		
	} else {
		arguments << "-c" + programmer->second.protocol;
		communication = programmer->second.communication;
		speed = programmer->second.speed;				
	}

	if (speed == "") {
		speed = "19200";
	}

	if (communication == "serial") {
		if (board->second.name == "Arduino Leonardo") {
			QString leoPort = GetLeonardoSerialPort(project->serialPort);
            //arguments << "-P\\\\.\\" + leoPort;

#ifdef Q_OS_WIN
            arguments << "-P\\\\.\\" + leoPort;
#else
            arguments << "-P/dev/" + leoPort;
#endif
			msg.AddOutput("Leonardo board: Uploading to alternative serial port '" + leoPort + "' (Please check Leonardo docs if you have any questions about this)", false);			
		} else {

#if  defined(Q_OS_WIN)
            arguments << "-P\\\\.\\" + project->serialPort;
#endif

#if  defined(Q_OS_LINUX)
            arguments << "-P/dev/" + project->serialPort;
#endif

#if  defined(Q_OS_MAC)
            arguments << "-P/dev/tty." + project->serialPort;
#endif



		}
		if (speed != "") {
            arguments << "-b" + speed;
		}
	} else {
		arguments << "-P" + communication;
	}

	if (project->programmer == "") {
		arguments << "-D";
	} 

    arguments << "-q" << "-Uflash:w:" + outputFile + ":i";

    bool ok = launcher->RunCommand(uploaderPath, arguments, config.uploadTimeout, progress);
	
	return ok;
}

//-----------------------------------------------------------------------------

bool Builder::Compile(int fileIndex)
{
	msg.buildStage = 1;

	QString inputFile;	

	// Get the input file path
	inputFile = project->files.at(fileIndex).name;

	switch (project->files.at(fileIndex).type) {
		case ptSource: 
			inputFile = config.workspace +  "/" + //QDir::separator() + 
				project->name +  "/source/" + inputFile;
		break;

		case ptExternal: 
		case ptExtra:
            inputFile = config.LocateFileUsingSearchPaths(inputFile, "$(LIBRARIES)", false);
            qDebug() << "LibsPath" << inputFile;
		break; 
	}
    qDebug() << "inputFile: " << inputFile;
	return CompileFile(inputFile, true);//, false);
}

//-----------------------------------------------------------------------------

QString Builder::MangleFileName(QString inputFile)
{
	// Mangle it to avoid conflicting with other files with the same name from other directories
	// filename shall be a fullpath file name
	QString outputFile = QFileInfo(inputFile).fileName(); //project->files.at(fileIndex).name;
	QString folder = QFileInfo(inputFile).dir().path();
	folder = QFileInfo(folder).baseName();
	outputFile = buildPath + "/" + folder + "_" + outputFile + ".o";
	return outputFile;
}

//-----------------------------------------------------------------------------

bool Builder::CompileFile(QString inputFile, bool testDate) //, bool silent)
{
	// Define the output file.	
    msg.buildStage = 1;
	QString outputFile = MangleFileName(inputFile);
	
	// Check if the compiled object file is update. 
	// If yes, we won't compile it again to gain time
	/////testDate = false;
	if (testDate) {
		QDateTime inputTime = QFileInfo(inputFile).lastModified();
		QDateTime outputTime = QFileInfo(outputFile).lastModified();
		if (inputTime < outputTime) {
			msg.AddOutput("File is up-to-date: " + inputFile, false);
			return true;
		}
	}
	
	msg.AddOutput("Compiling file: " + inputFile, false);	

	// Get the compiler path	
	QString compilerPath = config.avrPath;

	if (QFileInfo(inputFile).suffix().toUpper() == "CPP") {
        compilerPath += "/avr-g++";
	} else {
        compilerPath += "/avr-gcc";
	}

	// Get the argument values from the project configuration
	map <QString, BoardDef>::const_iterator board = config.boards.find(project->boardName);
	if (board == config.boards.end()) {
		msg.Add("Could not find board configuration for project: " + project->name, mtError);
		return false;
	}
	
	// Create the list of argunts for the compiler

	QStringList arguments;
	arguments << "-c" << inputFile << "-o" << outputFile;
	arguments << "-g" << "-Os" << "-Wall" << "-fno-exceptions" << "-ffunction-sections";
	arguments << "-fdata-sections" << "-MMD" << "-DARDUINO=105";
	if (board->second.build_vid != "") {
		arguments << "-DUSB_VID=" + board->second.build_vid;
	}
	if (board->second.build_pid != "") {
		arguments << "-DUSB_PID=" + board->second.build_pid;
	}

	arguments << "-mmcu=" + board->second.build_mcu;;
	arguments << "-DF_CPU=" + board->second.build_f_cpu;

	// add all include paths from the project configurations
    QStringList projectIncludes = project->includePaths.split(";") + config.extraArduinoLibsSearchPaths.split(";");
	QStringList includes;
	includes << QFileInfo(inputFile).path();

	QString core = config.DecodeMacros("$(ARDUINO_CORE)", project);
	includes <<  core;
/*#if defined(Q_OS_WIN)
    includes <<  qApp->applicationDirPath() + "/arduino/arduino/cores/" + board->second.build_core;
#endif

#if defined(Q_OS_LINUX) || defined(Q_OS_MAC)
    qDebug() << "arduinoCoreOpt" << config.arduinoCoreOpt;
    includes << config.arduinoCoreOpt + "/arduino/cores/" + board->second.build_core;
#endif
*/
	QString variant = config.DecodeMacros("$(ARDUINO_VARIANT)", project);
	includes << variant;

	for (int l=0; l < projectIncludes.count(); l++) {
		projectIncludes[l] = projectIncludes[l].trimmed();
		if (projectIncludes[l].length() < 2) {
			continue;
        }
		if (projectIncludes[l].indexOf("/") > 0) {
			QString path = config.DecodeLibraryPath(projectIncludes[l]).trimmed();
			if (path.length() > 1) {
				includes.append(path);
			}
		} else {
			QString includePaths = config.DecodeMacros(projectIncludes[l], project).trimmed();
			QStringList tempInc = includePaths.split(";");
			for (int i=0; i < tempInc.size(); i++) {
				tempInc[i] = tempInc[i].trimmed();
				if (tempInc[i].length() > 1) {
					includes.append(tempInc[i]);
				}
			}
		}
	}

	for (int i=0; i < includes.size(); i++) {
		arguments << "-I"  << includes[i];
	}

    bool ok = launcher->RunCommand(compilerPath, arguments);

    qDebug() << "args: " << arguments;
   /* bool l = (compilerPath == "avr-g++");
    QStringList ls;
    ls << "-V";
    ok = launcher->RunCommand("avr-g++", arguments);
*/
    return ok;
}

//-----------------------------------------------------------------------------

void Builder::GetBinarySize(void)
{
	msg.buildStage = 4;
	// Define the output file 
	QString binFile = buildPath + "/" + project->name + ".elf";
	
	// TODO: Check if the binary file is update. 
	// If yes, we won't compile it again to gain time

	// Get the compiler path	
	QString linkerPath = config.avrPath + "/avr-size";

	// get linker arguments
	map <QString, BoardDef>::const_iterator board = config.boards.find(project->boardName);
	QStringList arguments;
	arguments << "-C" << "--mcu=" + board->second.build_mcu;
	arguments << binFile;

    launcher->RunCommand(linkerPath, arguments);
}

//-----------------------------------------------------------------------------

bool Builder::Link(void)
{

	// Define the output file 
	QString binFile = buildPath + "/" + project->name + ".elf";
	
	// TODO: Check if the binary file is update. 
	// If yes, we won't compile it again to gain time

	if (BuildCoreLib() == false) {
		msg.Add("Error building core lib for project " 
			+ project->name + ", targeting board " 
			+ project->boardName, mtError);
		return false;
	}
	
        msg.buildStage = 2;

	msg.AddOutput("Linking project:" + project->name, false);	

	// Get the compiler path	
	QString linkerPath = config.avrPath + "/avr-gcc";

	// get linker arguments
	map <QString, BoardDef>::const_iterator board = config.boards.find(project->boardName);
	QStringList arguments;
	arguments << "-Os" << "-Wl,--gc-section";	
	arguments << "-mmcu=" + board->second.build_mcu;
	arguments << "-o" << binFile;
	for (unsigned int i=0; i < project->files.size(); i++) {
		QString ext = QFileInfo(project->files.at(i).name).suffix().toUpper();
		if ( (ext == "CPP") || (ext == "C") ) {
			QString objFile = MangleFileName(workspace.GetFullFilePath(project->name, project->files.at(i).name));
			arguments << objFile;		

			}
	}

	arguments << coreLib;		
	arguments << "-lm";	

        bool ok = launcher->RunCommand(linkerPath, arguments);

	SetPercentage(90);

	// Finally, convert the elf file into hex format
	if (ok) {
		msg.AddOutput("Converting ELF to HEX...");
		linkerPath = config.avrPath + "/avr-objcopy";
		arguments.clear();
		arguments << "-O" << "ihex" << "-R" << ".eeprom";
		arguments << binFile;
		arguments << buildPath + "/" + project->name + ".hex";
		
        ok = launcher->RunCommand(linkerPath, arguments);
	} else {
		msg.Add("Error linking the project " + project->name, mtError);
	}

	SetPercentage(100);
	return ok;
}

//-----------------------------------------------------------------------------

bool Builder::BuildCoreLib(void)
{
	if (QFileInfo(coreLib).exists()) {
		return true;
	}

	map <QString, BoardDef>::const_iterator board = config.boards.find(project->boardName);
	map <QString, BuildDef>::const_iterator bld = config.builds.find(board->second.build_core);

	if (bld == config.builds.end()) {
		msg.Add("Error getting build configuration for board " + project->boardName, mtError);
		return false;
	}

	//buildType = 1;
	progress->SetPhase(BuildWindowTypes::linkingCore);
	int oldPercent = GetPercentage();
	SetPercentage(0);

	QString linkerPath = config.avrPath + "/avr-ar";
	
	msg.AddOutput("Linking core lib for board :" + project->boardName, false);	
	
	QStringList coreFiles = bld->second.coreLibs.split(";");
    qDebug() << "Core Libs:" << bld->second.coreLibs;
	bool ok = true;
	
	for (int i=0; i < coreFiles.size(); i++) {
		coreFiles[i] = coreFiles[i].trimmed();
		if (coreFiles[i] == "") {
			continue;
		}
		QString inputFile = config.DecodeMacros(coreFiles.at(i), project);	

		if (GetCancel()) {
			msg.Add("Build cancelled by the user!", mtRegular);
			ok = false;
			break;
		}
		ok = ok && CompileFile (inputFile, false);//, true);
		if (ok) {
			QString outputFile = MangleFileName(inputFile);
			QStringList arguments;
			arguments << "rcs" << coreLib;
			arguments << outputFile;
            msg.buildStage = 2;
            ok = launcher->RunCommand(linkerPath, arguments);
		}
		SetPercentage(100 * i / coreFiles.size());
	}

	SetPercentage(oldPercent);
	progress->SetPhase(BuildWindowTypes::compiling);
	//buildType = 0;
	return ok;
}

//-----------------------------------------------------------------------------

QString Builder::GetLeonardoSerialPort(QString defaultPort)
{
	QStringList before, after;
	QSerialPortInfo serialList;
	QSerialPort port;
	
	for (int i=0; i < serialList.availablePorts().count();i++) {
		before.append(serialList.availablePorts().at(i).portName());
	}
	
#ifdef Q_OS_WIN
	bool open = PrepareSerialPort(project->serialPort, project->serialPortSpeed);
#else
	port.setPortName(project->serialPort);
	port.setFlowControl(QSerialPort::NoFlowControl);
	port.setBaudRate(QSerialPort::Baud1200);
	port.setParity(QSerialPort::NoParity);
	port.setStopBits(QSerialPort::OneStop);
	port.setDataBits(QSerialPort::Data8);
	bool open = port.open (QIODevice::ReadWrite);
#endif 
	if (open) {	
		char buffer[20] = "Hello!\r\n";
		port.write ((const char *)buffer);
		QThread::msleep(100);
		port.close();

		int counter = 30;
		after.clear();
		while (counter > 0) {
			QSerialPortInfo newSerialList;
			for (int i=0; i < newSerialList.availablePorts().count();i++) {
				after.append(newSerialList.availablePorts().at(i).portName());
			}

			for (int i=0; i < after.count(); i++) {
				if (before.indexOf(after.at(i)) < 0) {
					return after.at(i);				
				}
			}
			QThread::msleep(10);
			counter--;
		}
	} else {
		msg.Add("Invalid serial port for project " + project->name + ": '" + project->serialPort + "'", mtError);
	}
	
	msg.Add("Could not detect Leonardo serial port for programming: " + project->name, mtWarning);
	return project->serialPort;
}

//-----------------------------------------------------------------------------

void Builder::ImportDeclarations(void)
{
	QString inFileName = config.workspace + "/" + project->name + "/source/" + project->name + ".cpp"; //main.cpp

	if (QFileInfo(inFileName).exists() == false) {
		return;
	}

	QFile inFile(inFileName);
	inFile.open(QFile::ReadOnly | QFile::Text);
	QTextStream in(&inFile);
	QString code = in.readAll();

	int p1 = code.indexOf("/*");
	while (p1 >= 0) {
		QString left = code.left(p1);
		code = code.right(code.length() - p1);
		int p2 = code.indexOf("*/");
		if (p2 >=0) {			
			QString right = code.right(code.length() - p2 - 2);
			code = left + right;
		}
		p1 = code.indexOf("/*");
	}

	QStringList lines = code.split("\n");
	QStringList list;

	int i=1;
	while (i < lines.count()) {
		if (lines[i].trimmed().indexOf("{") == 0) {
			lines[i-1] += " " + lines[i];
			lines.erase(lines.begin()+i);
		} else {
			i++;
		}
	}

	for (int i=0; i < lines.count(); i++) {
		QRegExp rx("(\\ |\\,|\\(|\\)|\\.|\\:|\\t)"); //RegEx for ' ' or ',' or '.' or ':' or '\t'
		QStringList words = lines[i].trimmed().split(rx);
		int w = 0;
		while (w < words.count()) {
			words[w] = words[w].trimmed().toUpper();
			if (words[w] == "") {
				words.erase(words.begin() + w);
			} else {
				w++;
			}
		}

		if (words.count() < 4) { // at least "TYPE", "FUNCTION_NAME", "(", ")"
			continue;
		}

		bool ignore = false;
		ignore = ignore || (words.indexOf("IF") >= 0);
		ignore = ignore || (words.indexOf("ELSE") >= 0);
		ignore = ignore || (words.indexOf("WHILE") >= 0);
		ignore = ignore || (words.indexOf("DO") >= 0);
		ignore = ignore || (words.indexOf("SWITCH") >= 0);
		ignore = ignore || (lines[i].indexOf(";") >= 0);
		ignore = ignore || (lines[i].indexOf(".") >= 0);
		ignore = ignore || (lines[i].indexOf("->") >= 0);
		ignore = ignore || (lines[i].indexOf("=") >= 0);
		ignore = ignore || (lines[i].indexOf("==") >= 0);
		ignore = ignore || (lines[i].indexOf("\"") >= 0); // any argument with " may be considered a call, and not a definition
		ignore = ignore || (lines[i].indexOf("'") >= 0); // idem

		// check if any argument is a number. If yes, then it shall be considered a function call, and not a definiton
		for (int j=0; j < words.count(); j++) {
			double num = words.at(j).toDouble();
			if (QString::number(num) == words.at(j)) {
				ignore = true;
				break;
			}
		}

		// if the first word is not an recognized data type, ignore
		bool validDataType = false;
		validDataType = validDataType || (words.at(0) == "VOID");
		validDataType = validDataType || (words.at(0) == "INT");
		validDataType = validDataType || (words.at(0) == "CHAR");
		validDataType = validDataType || (words.at(0) == "SHORT");
		validDataType = validDataType || (words.at(0) == "UNSIGNED");
		validDataType = validDataType || (words.at(0) == "SIGNED");
		validDataType = validDataType || (words.at(0) == "LONG");
		validDataType = validDataType || (words.at(0) == "FLOAT");
		validDataType = validDataType || (words.at(0) == "DOUBLE");
		validDataType = validDataType || (words.at(0) == "BYTE");
		validDataType = validDataType || (words.at(0) == "INT16");
		validDataType = validDataType || (words.at(0) == "INT32");
		validDataType = validDataType || (words.at(0) == "INT64");
		validDataType = validDataType || (words.at(0) == "BOOL");
		ignore = ignore || (validDataType == false);
		
		if (lines[i].indexOf("//") >= 0) {
			lines[i] = lines[i].left(lines[i].indexOf("//"));
		}

		if (ignore) {
			continue;
		}

		int p1 = lines[i].indexOf("(");
		int p2 = lines[i].indexOf(")");
		if ((p1 > 0) && (p2 > p1)) {
			QString def = lines[i].trimmed();
			int p = def.lastIndexOf(")");
			if (p >= 0) {
				def = def.left(p+1) + ";";
				list.append(def);
			}
		}
	}

	QString outFileName = config.workspace + "/" + project->name + "/source/mariamole_auto_generated.h";

	if (QFileInfo(outFileName).exists() == false) {
		return;
	}

	QFile inFile2(outFileName);
	inFile2.open(QFile::ReadOnly | QFile::Text);
	QTextStream in2(&inFile2);
	code = in2.readAll();

	QStringList header = code.split("\n");
	//QStringList alreadyAdded;
	int index = -1;// = header.count() - 2;
	bool foundBegin = false;
	int k=0;
	while (k < header.count()) {
		QString line = header[k].trimmed();
		if (line == "/*--MARIMOLE-DEF_END--*/") {
			foundBegin = false;
		}
		if (foundBegin) {
			header.erase(header.begin() + k);
		} else {
			++k;
		}
		if (line == "/*--MARIMOLE-DEF_BEGIN--*/") {
			foundBegin = true;
			index = k;
		}
	}
	
	if (index < 0) {
		return;
	}
		
	for (int i=0; i < list.count(); i++) {
		header.insert(index, list[i]);
	}

	QFile outFile(outFileName);
	outFile.open(QFile::WriteOnly);
	QTextStream out(&outFile);
	out << header.join("\n");
}

//-----------------------------------------------------------------------------

bool Builder::BurnBootLoader(void)
{
	map <QString, BoardDef>::const_iterator board = config.boards.find(blbBoardName);

	if (board == config.boards.end()) {
		msg.Add("Error getting board configuration for: " + blbBoardName, mtError);
		return false;
	}

	map <QString, BuildDef>::const_iterator build = config.builds.find(board->second.build_core);
	map <QString, ProgrammerDef>::const_iterator programmer = config.programmers.find(blbProgrammerName);

	if (build == config.builds.end()) {
		msg.Add("Error getting build configuration for board " + blbBoardName, mtError);
		return false;
	}

	if (programmer == config.programmers.end()) {
		msg.Add("Error getting programmer configuration for board " + blbBoardName, mtError);
		return false;
	}

	progress->SetPhase(BuildWindowTypes::bootloader);
	SetPercentage(0);

	QString uploaderPath = config.avrPath + "/avrdude";
	
	msg.AddOutput("Burning bootloader to board " + blbBoardName, false);	

	QStringList arguments;

	//TODO: insert the bootloader burner protocol code here

    bool ok = launcher->RunCommand(uploaderPath, arguments, config.uploadTimeout);

	return ok;
}

//-----------------------------------------------------------------------------

void Builder::ConfigureBootloaderBurner(QString programmerName, QString boardName, QString SerialPort)
{
	blbProgrammerName = programmerName;
	blbBoardName = boardName;
	blbSerialPort = SerialPort;
}

//-----------------------------------------------------------------------------



