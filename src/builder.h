//-----------------------------------------------------------------------------
#ifndef MM_BUILDER__H
#define MM_BUILDER__H
//-----------------------------------------------------------------------------

#include <QObject>
#include <QWidget>
#include <QProgressDialog>

#include <QtSerialPort/QSerialPortInfo>
#include <QtSerialPort/QSerialPort>

#include "workspace.h"
#include "message_handler.h"
#include "launcher.h"
#include "buildwindow.h"

//-----------------------------------------------------------------------------

class Builder : public QWidget
{
    Q_OBJECT

public:
    Builder(QWidget *parent);
	~Builder(void);

    //Launcher * launcher;

	bool Clean(void);
	int Build(bool upload);	
	bool BurnBootLoader(void);
	int GetLastBuildStatus(void);			
	int GetBuildType(void);
	void ConfigureBootloaderBurner(QString programmerName, QString boardName, QString SerialPort);

private:
	bool running;
	Project * project;
	QString buildPath;
	int percentage;
	QString coreLib;
	int lastBuildStatus;
	QString blbProgrammerName, blbBoardName, blbSerialPort;
	BuildWindow * progress;

	bool Compile(int fileIndex);
	bool CompileFile(QString inputFile, bool testDate);//, bool silent);
	bool Link(void);
	void GetBinarySize(void);
	bool BuildCoreLib(void);
	bool Upload(void);
	QString GetLeonardoSerialPort(QString defaultPort);
	QString MangleFileName(QString inputFile);
	void ImportDeclarations(void);
	void SetPercentage(int value);
	bool GetCancel(void);
	int GetPercentage(void);	
};

//-----------------------------------------------------------------------------

//extern Builder builder;

//-----------------------------------------------------------------------------
#endif

