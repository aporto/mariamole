#ifndef MESSAGE_HANDLER__H__
#define MESSAGE_HANDLER__H__

//-----------------------------------------------------------------------------

#include <QString>
#include <QDir>
#include <QLocale>
#include <queue>

#include "project.h"

//-----------------------------------------------------------------------------

using namespace std;

//-----------------------------------------------------------------------------

enum MessageHanndlerType {mtUnknown, mtRegular, mtDebug, mtWarning, mtError, mtSuccess};

//-----------------------------------------------------------------------------

struct MMBuildMessage {
	QString file;
	QString text;
	int line;
	int column;
	MessageHanndlerType type;
};

//-----------------------------------------------------------------------------

class MessageHandler
{
public:
	MessageHandler(void);
	~MessageHandler(void);

	void Add(QString text, MessageHanndlerType type);
	void AddOutput(QString text, bool parseText = true);
	void ClearOutput(void);
	QString GetOutput(void);
	bool HasOutput(void);
	void ClearBuildInfo(void);
	void GetLastSucessfullBuildInfo(int &programSize, float &ppercent, 
				int &dataSize, float &dpercent, int &EEPROMSize, float &epercent);
	void ClearBuildMessages(void);
	int BuildMessagesCount(void);
	//bool GetBuildMessage(unsigned int index, MMBuildMessage * const message);
	MMBuildMessage GetBuildMessage(void);
	int buildStage;
private:
	queue <MMBuildMessage> mmbuildMessages;
	QString output;
	QSemaphore * semaphore;
	int lastBuildProgramSize;
	int lastBuildDataSize;
	int lastBuildEEPROMSize;	
	float lastBuildProgramPercent;
	float lastBuildDataPercent;
	float lastBuildEEPROMPercent;
};

//-----------------------------------------------------------------------------

extern MessageHandler msg;

//-----------------------------------------------------------------------------

#endif