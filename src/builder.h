//-----------------------------------------------------------------------------
#ifndef MM_BUILDER__H
#define MM_BUILDER__H
//-----------------------------------------------------------------------------

#include <QProcess>
#include "workspace.h" 
#include "message_handler.h"

#include <Windows.h>

//-----------------------------------------------------------------------------

class Builder
{
public:
	Builder(void);
	~Builder(void);

	void Cancel(void);
	bool Clean(void);
	int Build(bool upload);	
	int GetLastBuildStatus(void);		
	int GetPercentage(void);
	int GetBuildType(void);
private:
	Project * project;
	QString buildPath;
	int percentage;
	bool cancel;
	QString coreLib;
	int buildType;
	int lastBuildStatus;

	bool Compile(int fileIndex);
	bool CompileFile(QString inputFile, bool testDate, bool silent);
	bool Link(void);
	void GetBinarySize(void);
	bool BuildCoreLib(void);
	bool Upload(void);
	QString GetLeonardoSerialPort(QString defaultPort);
};

//-----------------------------------------------------------------------------

extern Builder builder;

//-----------------------------------------------------------------------------
#endif