#include "builder.h"
//-----------------------------------------------------------------------------

Builder builder;

//-----------------------------------------------------------------------------

Builder::Builder(void)
{
	lastBuildStatus = 0;
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
	if (ok) {
		msg.Add("All " + project->name + " build files were successfully erased.", mtSuccess);
	} else {
		msg.Add("Error while cleaning project " + project->name + ".", mtError);
	}

	msg.ClearBuildInfo();
	lastBuildStatus = 0;
	return ok;
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
}

//-----------------------------------------------------------------------------

void Builder::Cancel(void)
{
	cancel = true;
}

//-----------------------------------------------------------------------------

int Builder::GetBuildType(void)
{
	return buildType;
}

//-----------------------------------------------------------------------------

int Builder::Build(bool upload)
{	
	percentage = 0;
	cancel = false;
	buildType = 0;

	msg.ClearOutput();
	msg.ClearBuildMessages();

	project = workspace.GetCurrentProject();
	if (project == NULL) {
		return -1;
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
	//config.coreLibsPath = buildPath;
	coreLib = buildPath + "/arduino_core_" + board->second.build_variant+".a";
	
	msg.ClearBuildInfo();
	bool ok = true;
	for (int i=0; i < project->files.size(); i++) {
		QString ext = QFileInfo(project->files.at(i).name).suffix().toUpper();
		if ((ext == "CPP") || (ext == "c")) {
			ok = ok & Compile(i);
			if (cancel) {
				msg.Add("Build cancelled by the user!", mtRegular);
				ok = false;
				break;
			}
			percentage = 40 * i / project->files.size();
			//SleepEx(1500, true);
		}
	}

	if (cancel) {
		msg.Add("Build cancelled by the user!", mtRegular);
		ok = false;
	}
	
	if (ok) {
		ok = Link();
		if (ok) {
			msg.Add("Project " + project->name + " successfully built!", mtSuccess);
			GetBinarySize();
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
	}
	if (ok) {
		lastBuildStatus = 2;
	} else {
		lastBuildStatus = 1;
	}
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

	buildType = 2;
	percentage = 0;

	QString uploaderPath = qApp->applicationDirPath() + 
		"/arduino/avr/bin/avrdude";
	
	msg.AddOutput("Uploading program to board " + project->boardName, false);	

	QString outputFile = buildPath + "/" + project->name + ".hex";
	QString arguments = "-C\"" + qApp->applicationDirPath() 
		+ "/arduino/avr/etc/avrdude.conf\"";
	//arguments += " -q -q"; // ultra quiet mode
	arguments += " -p " + board->second.build_mcu;
	
	QString speed = "";
	QString communication = "";
	if (project->programmer == "") {
		arguments += " -c" + board->second.upload_protocol;		
		communication = "serial";
		speed = board->second.upload_speed;;		
	} else {
		arguments += " -c" + programmer->second.protocol;
		communication = programmer->second.communication;
		speed = programmer->second.speed;				
	}

	if (speed == "") {
		speed = "19200";
	}

	if (communication == "serial") {
		arguments += " -P\\\\.\\";
		if (board->second.name == "Arduino Leonardo") {
			QString leoPort = GetLeonardoSerialPort(project->serialPort);
			arguments += leoPort;
			msg.AddOutput("Leonardo board: Uploading to alternative serial port '" + leoPort + "' (Please check Leonardo docs if you have any questions about this)", false);			
		} else {
			arguments += project->serialPort;
		}
		if (speed != "") {
			arguments += " -b" + speed;
		}
	} else {
		arguments += " -P" + communication;
	}

	if (project->programmer == "") {
		arguments += " -D";
	} 

	QString tempOutput = buildPath + "programmer_output.log";

	arguments += " -q -q -Uflash:w:\"" + outputFile + "\":i";
	arguments += " >> \"" + tempOutput + "\"";
	
	msg.AddOutput(">> " + uploaderPath + " " + arguments, false);

	QProcess uploaderProc;

#ifdef Q_OS_WIN
	uploaderProc.setNativeArguments(arguments);
#else
	QStringList arglist; arglist << arguments; uploaderProc.setArguments(arglist);
#endif
	uploaderProc.setProcessChannelMode(QProcess::MergedChannels);
	uploaderProc.start(uploaderPath);//QIODevice::ReadWrite);
	
	//uploaderProc.waitForFinished();
	int timeOut = 0;
	bool running = true;
	while (running && (timeOut < config.uploadTimeout)) {
		running = !(uploaderProc.waitForFinished(500));
		percentage = 50 * timeOut / config.uploadTimeout;
		timeOut++;
	}

	if (running) {
		uploaderProc.kill();
	}

	/*int timeOut = -1;
	while (uploaderProc.state() != QProcess::NotRunning) {
		timeOut++;
		if (timeOut == config.uploadTimeout) {
			uploaderProc.terminate();
			uploaderProc.kill();
			break;
		}
		QThread::sleep(1);
		percentage = 100 * timeOut / config.uploadTimeout;
	}

	*/

	bool ok = uploaderProc.exitStatus() == QProcess::NormalExit;
	if (ok) {
		ok = uploaderProc.exitCode() == 0;
	}

	QByteArray byteArray = uploaderProc.readAllStandardOutput();
	QStringList strLines = QString(byteArray).split("\n");
	for (int i=0; i < strLines.count(); i++) {
		QString txt = strLines.at(i).trimmed();
		if (txt != "") {
			msg.AddOutput(strLines.at(i));
		}
		//qDebug(strLines.at(i).toLocal8Bit());
	}
//QString line= p.readAllStandardOutput();

// Wait for it to start
//]]if(!process.waitForStarted())
//]]return 0;

//// Continue reading the data until EOF reached
//QByteArray data;
/*while ( process.state() != QProcess::NotRunning) {
char * buffer[1024];
memset(buffer, 0, sizeof(buffer));
int k = process.read(1024);
QString line = process.readLine();
qDebug(line);
}
*/
/*QString str = process.readLine();

process.waitForReadyRead();
data.append(process.readAll());

// Output the data
qDebug(data.data());
qDebug("Done!");*/ 







	/*QProcess uploaderProc;
	uploaderProc.setArguments(arguments);
	uploaderProc.setProcessChannelMode(QProcess::MergedChannels);
	
	uploaderProc.setStandardErrorFile(tempOutput);
	uploaderProc.start(uploaderPath);

	int timeOut = -1;
	while (uploaderProc.state() != QProcess::NotRunning) {
		timeOut++;
		if (timeOut == config.uploadTimeout) {
			uploaderProc.terminate();
			uploaderProc.kill();
			break;
		}
		QThread::sleep(1);
		percentage = 100 * timeOut / config.uploadTimeout;
	}
	
	//uploaderProc.waitForFinished();
	
	bool ok = uploaderProc.exitStatus() == QProcess::NormalExit;
	if (ok) {
		ok = uploaderProc.exitCode() == 0;
	}	
	/*QTextStream computerOutput(uploaderProc.readAll());
	while (computerOutput.atEnd() == false) {
		msg.AddOutput(computerOutput.readLine());
	}*/
	/*QFile file(tempOutput);
	QTextStream in(&file);	
	while(!in.atEnd()) {
		QString line = in.readLine();
		msg.AddOutput(line);
	}*/
	return ok;
}

//-----------------------------------------------------------------------------

bool Builder::Compile(int fileIndex)
{
	msg.buildStage = 1;
	//msg.ClearOutput();

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
			inputFile = project->name;
		break; 
	}

	return CompileFile(inputFile, true, false);
}

//-----------------------------------------------------------------------------

bool Builder::CompileFile(QString inputFile, bool testDate, bool silent)
{
	// Define the output file.
	// Mangle it to avoid conflicting with other files with the same name from other directories
	QString outputFile = QFileInfo(inputFile).fileName(); //project->files.at(fileIndex).name;
	QString folder = QFileInfo(inputFile).dir().path();
	folder = QFileInfo(folder).baseName();
	outputFile = buildPath + "/" + folder + "_" + outputFile + ".o";

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
	QString compilerPath = qApp->applicationDirPath() + 
		"/arduino/avr/bin/";

	if (QFileInfo(inputFile).suffix().toUpper() == "CPP") {
		compilerPath += "avr-g++";
	} else {
		compilerPath += "avr-gcc";
	}
	
	
	// Get the argument values from the project configuration	
	map <QString, BoardDef>::const_iterator board = config.boards.find(project->boardName);
	if (board == config.boards.end()) {
		msg.Add("Could not find board configuration for project: " + project->name, mtError);
		return false;
	}
	
	// Create the list of argunts for the compiler

    QString arguments;
    arguments = " -c \"" + inputFile + "\" -o \"" + outputFile + "\"";
	arguments += " -g -Os -Wall -fno-exceptions -ffunction-sections -fdata-sections -MMD -DARDUINO=105";
	if (board->second.build_vid != "") {
		arguments += " -DUSB_VID=" + board->second.build_vid;
	}
	if (board->second.build_pid != "") {
		arguments += " -DUSB_PID=" + board->second.build_pid;
	}
	
	arguments += " -mmcu=" + board->second.build_mcu;;
	arguments += " -DF_CPU=" + board->second.build_f_cpu;
	//arguments << " -l \"" + coreLib + "\"";

	// Add the include paths
	arguments += " -I\"" + QFileInfo(inputFile).path() + "\"";

	QStringList includes = project->includePaths.split(";");		
	for (int i=0; i < includes.size(); i++) {
		includes[i] = includes.at(i).trimmed();
		if (includes[i] != "") {
			arguments += " -I\"" + includes.at(i) + "\"";
		}
	}
	includes = config.includePaths.split(";");		
	for (int i=0; i < includes.size(); i++) {
		includes[i] = includes.at(i).trimmed();
		if (includes[i] != "") {
			arguments += " -I\"" + includes.at(i) + "\"";
		}
	}

	arguments += " -I\"" + config.DecodeMacros("$(ARDUINO_CORE)", project) + "\"";
	arguments += " -I\"" + config.DecodeMacros("$(ARDUINO_VARIANT)", project) + "\"";

	msg.AddOutput(">> " + compilerPath + " " + arguments, false);

	QProcess compilerProc;
#ifdef Q_OS_WIN
	compilerProc.setNativeArguments(arguments);
#else
	QStringList arglist; arglist << arguments; compilerProc.setArguments(arglist);
#endif
	compilerProc.setProcessChannelMode(QProcess::MergedChannels);
    compilerProc.start(compilerPath);
	compilerProc.waitForFinished();	

	bool ok = compilerProc.exitStatus() == QProcess::NormalExit;
	if (ok) {
		ok = compilerProc.exitCode() == 0;
	}
	
	QTextStream computerOutput(compilerProc.readAll());
	while (computerOutput.atEnd() == false) {
		msg.AddOutput(computerOutput.readLine());
	}

	return ok;
}

//-----------------------------------------------------------------------------

void Builder::GetBinarySize(void)
{
	msg.buildStage = 4;
	// Define the output file 
	QString binFile = buildPath + "/" + project->name + ".elf";
	
	// to do: Check if the binary file is update. 
	// If yes, we won't compile it again to gain time

	
	// Get the compiler path	
	QString linkerPath = qApp->applicationDirPath() + 
		"/arduino/avr/bin/avr-size";

	// get linker arguments
	map <QString, BoardDef>::const_iterator board = config.boards.find(project->boardName);
    QString arguments;
	arguments = " -C --mcu=" + board->second.build_mcu;
	arguments += " \"" + binFile + "\"";
	
	QProcess linkerProc;

#ifdef Q_OS_WIN
	linkerProc.setNativeArguments(arguments);
#else
	QStringList arglist; arglist << arguments; linkerProc.setArguments(arglist);
#endif

	linkerProc.setProcessChannelMode(QProcess::MergedChannels);
    linkerProc.start(linkerPath);
	linkerProc.waitForFinished();

	bool ok = linkerProc.exitStatus() == QProcess::NormalExit;
	if (ok) {
		ok = linkerProc.exitCode() == 0;
	}	
	QTextStream computerOutput(linkerProc.readAll());
	while (computerOutput.atEnd() == false) {
		msg.AddOutput(computerOutput.readLine());
	}	
}

//-----------------------------------------------------------------------------

bool Builder::Link(void)
{
	msg.buildStage = 2;
	msg.ClearOutput();	

	// Define the output file 
	QString binFile = buildPath + "/" + project->name + ".elf";
	
	// to do: Check if the binary file is update. 
	// If yes, we won't compile it again to gain time

	if (BuildCoreLib() == false) {
		msg.Add("Error building core lib for project " 
			+ project->name + ", targeting board " 
			+ project->boardName, mtError);
		return false;
	}
	
	msg.AddOutput("Linking project:" + project->name, false);	

	// Get the compiler path	
	QString linkerPath = qApp->applicationDirPath() + 
		"/arduino/avr/bin/avr-gcc";

	// get linker arguments
	map <QString, BoardDef>::const_iterator board = config.boards.find(project->boardName);
    QString arguments;
	arguments += "-Os -Wl,--gc-section";	
	arguments += " -mmcu=" + board->second.build_mcu;
	arguments += " -o \"" + binFile + "\"";
	for (int i=0; i < project->files.size(); i++) {
		QString ext = QFileInfo(project->files.at(i).name).suffix().toUpper();
		if ( (ext == "CPP") || (ext == "C") ) {
			QString objFile = project->files.at(i).name + ".o";//QFileInfo(inputFile).fileName(); //project->files.at(fileIndex).name;
			//QString folder = QFileInfo(project->files.at(i).name).dir().path();
			//folder = QFileInfo(folder).baseName();
			objFile = buildPath + "/" +  "source_" + objFile;			
			arguments += " \"" + objFile + "\"";		
		}
	}

	arguments += " \"" + coreLib + "\"";		
	arguments += " -lm";	

	msg.AddOutput(">> " + linkerPath + " " + arguments, false);
	QProcess linkerProc;

#ifdef Q_OS_WIN
	linkerProc.setNativeArguments(arguments);
#else
	QStringList arglist; arglist << arguments; linkerProc.setArguments(arglist);
#endif

	linkerProc.setProcessChannelMode(QProcess::MergedChannels);
	linkerProc.start(linkerPath);
	linkerProc.waitForFinished();

	bool ok = linkerProc.exitStatus() == QProcess::NormalExit;
	if (ok) {
		ok = linkerProc.exitCode() == 0;
	}	
	QTextStream computerOutput(linkerProc.readAll());
	while (computerOutput.atEnd() == false) {
		msg.AddOutput(computerOutput.readLine());
	}

	percentage = 90;

	// Finally, convert the elf file into hex format
	if (ok) {
		msg.AddOutput("Converting ELF to HEX...");
		linkerPath = qApp->applicationDirPath() + 
			"/arduino/avr/bin/avr-objcopy";
		arguments = " -O ihex -R .eeprom";
		arguments += " \"" + binFile +"\"";
		arguments += " \"" + buildPath + project->name + ".hex\"";
		
		msg.AddOutput(">> " + linkerPath + " " + arguments, false);
		QProcess hexProc;
    
#ifdef Q_OS_WIN
		hexProc.setNativeArguments(arguments);
#else
		QStringList arglist; arglist << arguments; hexProc.setArguments(arglist);
#endif
		hexProc.setProcessChannelMode(QProcess::MergedChannels);
		hexProc.start(linkerPath);
		hexProc.waitForFinished();
		ok = hexProc.exitStatus() == QProcess::NormalExit;
		if (ok) {
			ok = hexProc.exitCode() == 0;
		}	
		QTextStream computerOutput(hexProc.readAll());
		while (computerOutput.atEnd() == false) {
			msg.AddOutput(computerOutput.readLine());
		}
	} else {
		msg.Add("Error linking the project " + project->name, mtError);
	}

	percentage = 100;
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

	buildType = 1;
	int oldPercent = percentage;
	percentage = 0;

	QString linkerPath = qApp->applicationDirPath() + 
		"/arduino/avr/bin/avr-ar";
	
	msg.AddOutput("Linking core lib for board :" + project->boardName, false);	
	
	QStringList coreFiles = bld->second.coreLibs.split(";");
	bool ok = true;
	
	for (int i=0; i < coreFiles.size(); i++) {
		coreFiles[i] = coreFiles[i].trimmed();
		if (coreFiles[i] == "") {
			continue;
		}
		QString inputFile = config.DecodeMacros(coreFiles.at(i), project);	
		ok = ok && CompileFile (inputFile, false, true);
		if (ok) {
			QString outputFile = buildPath + "/" + QFileInfo(inputFile).fileName() + ".o";
			QString arguments = "rcs \"" + coreLib + "\"";
			arguments += " \"" + outputFile + "\"";


			QStringList arglist;
			arglist << arguments;

			msg.AddOutput(">> " + linkerPath + " " + arguments, false);
			QProcess linkerProc;
#ifdef Q_OS_WIN
			linkerProc.setNativeArguments(arguments);
#else
			QStringList arglist; arglist << arguments; linkerProc.setArguments(arglist);
#endif
			linkerProc.setProcessChannelMode(QProcess::MergedChannels);
			linkerProc.start(linkerPath);
			linkerProc.waitForFinished();
			ok = linkerProc.exitStatus() == QProcess::NormalExit;
			if (ok) {
				ok = linkerProc.exitCode() == 0;
			}	
			QTextStream computerOutput(linkerProc.readAll());
			while (computerOutput.atEnd() == false) {
				msg.AddOutput(computerOutput.readLine());
			}
		}
		percentage = 100 * i / coreFiles.size();
	}

	percentage = oldPercent;

	buildType = 0;
	if (ok == false) {
		return false;
	}

	return true;
}

//-----------------------------------------------------------------------------

QString Builder::GetLeonardoSerialPort(QString defaultPort)
{
	//to do!!!
	// According to Arduino official doc.,
	// before uploading to Leonardo we need to reset it first
	// Then it will be assigned a different serial port, and we need
	// to find it, and then use it to upload the new software
	return defaultPort;
}

