#include "message_handler.h"

//-----------------------------------------------------------------------------

MessageHandler msg;

//-----------------------------------------------------------------------------

MessageHandler::MessageHandler(void)
{
	semaphore = new QSemaphore(1);
	lastBuildProgramSize = 0;
	lastBuildDataSize = 0;
	lastBuildEEPROMSize = 0;
}

//-----------------------------------------------------------------------------

MessageHandler::~MessageHandler(void)
{
	delete semaphore;
}

//-----------------------------------------------------------------------------

void MessageHandler::Add(QString text, MessageHanndlerType type)
{
	semaphore->acquire();
	MMBuildMessage bm;
	bm.line = -1;
	bm.column = -1;
	bm.type = type;
	bm.text = text;
	bm.file = "";
	mmbuildMessages.push(bm);
	semaphore->release();
}

//-----------------------------------------------------------------------------

void MessageHandler::AddOutput(QString text, bool parseText)
{
	semaphore->acquire();
	
	output += text + "\n";		

	if (parseText) {
		bool parseThis = false;
		int pos = 0;
		if (buildStage == 2) { // link
			parseThis = true;
			QString subText = text.right(text.length() - 3);
			pos = subText.indexOf(":") + 3;
		} else if (buildStage == 4) { // get binary size
			parseThis = (text.indexOf(" bytes (") > 0) && (text.indexOf(" Full)") > 0);
		} else {
			pos = text.indexOf(": error: ");
			if (pos < 0 ) {
				pos = text.indexOf(": warning: ");
			}
			parseThis = pos > 0;
		}
		if (parseThis) {
			MMBuildMessage bm;
			bm.type = mtUnknown;
			bm.file = "";
			bm.line = -1;
			bm.column = -1;
			if (buildStage == 1) {
				ParseCompilerMessage(text, bm);
			} else if (buildStage == 2) { 
				ParseLinkerMessage(text, bm);
			} else if (buildStage == 3) { 
				ParseUploaderMessage(text, bm);
			} else if (buildStage == 4) {
				ParseStatisticMessage(text, bm);			
			}
			
			if (bm.type != mtUnknown) {
				mmbuildMessages.push(bm);
			}
		}
	}		
	
	semaphore->release();
}

//-----------------------------------------------------------------------------

void MessageHandler::ParseCompilerMessage(QString text, MMBuildMessage &bm)
{
	QString subText = text.right(text.length() - 3);
	int pos = subText.indexOf(":") + 3;
	QString part1 = text.left(pos);
	QString part2 = text.right(text.length() - part1.length() - 1);
	QStringList list = part1.split(":");
			
	// in windows, paths have a ":" for separating the drive letter. 
	// So, we need to check if the first ":" is the driver separator OR the
	// compiler result separator
	if (list[0].size() == 1) {
		list[0] += ":" + list[1];
		list.erase(list.begin() + 1);
	}
			
	bm.file = list[0];	

	list = part2.split(":");

	if (list.count() < 2) {
		return;
	}

	QString num1 = QString::number(list[0].toInt());
	QString num2 = QString::number(list[1].toInt());
	if (num1 == list[0]) {
		bm.line = num1.toInt();
		if (num2 == list[1]) {
			bm.column = num2.toInt();
			list.removeAt(1);
		} else {
			bm.column = -1;
			
		}
		list.removeAt(0);
	} else {
		bm.line = -1;
	}
	
	/*if (list.size() > 1) {
		bm.line = list[1].toInt();
		if (list.count() > 2) {
			bm.column = list[2].toInt();
		} else {
			bm.column = -1;
		}
	} else {
		bm.line = -1;
		bm.column = -1;
	}*/

	/*pos = part2.indexOf(":");
	if (pos < 0) {
		return;
	}*/

	if (list.count() < 2) {
		return;
	}

	QString type = list[0]; //part2.left(pos);
	if (type == " error") {
		bm.type = mtError;
	} else if (type == " warning") {
		bm.type = mtWarning;
	}

	list.removeAt(0);
	bm.text = list[0];
	while (list.count() > 1) {
		list.removeAt(0);
		bm.text = bm.text + ":" + list[0];
	}

	//bm.text = part2.right ( part2.length() - type.length() - 2);
}


//-----------------------------------------------------------------------------

void MessageHandler::ParseLinkerMessage(QString text, MMBuildMessage &bm)
{
	QString subText = text.right(text.length() - 3);
	int pos = subText.indexOf(":") + 3;
	QString part1 = text.left(pos);
	QString part2 = text.right(text.length() - part1.length() - 1);
	QStringList list = part1.split(":");
			
	// in windows, paths have a ":" for separating the drive letter. 
	// So, we need to check if the first ":" is the driver separator OR the
	// compiler result separator
	if (list[0].size() == 1) {
		list[0] += ":" + list[1];
		list.erase(list.begin() + 1);
	}
			
	bm.file = list[0];			
	if (list.size() > 1) {
		bm.line = list[1].toInt();
		if (list.count() > 2) {
			bm.column = list[2].toInt();
		} else {
			bm.column = -1;
		}
	} else {
		bm.line = -1;
		bm.column = -1;
	}

	list = part2.split(":");
	int i=0;
	while (i < list.count()) {
		if (list[i].trimmed() == "") {
			list.removeAt(i);
		} else {
			i++;
		}
	}
	if (list.count() > 1) {
		bm.type = mtError;
		bm.line = list[0].toInt();
		bm.column = -1;
		bm.text = list[1];
		for (int k=2; k < list.count(); k++) {
			bm.text += ":" + list[k];
		}
						
	}
}

//-----------------------------------------------------------------------------

void MessageHandler::ParseUploaderMessage(QString text, MMBuildMessage &bm)
{
	int pos = text.indexOf("avrdude: ");						
	if (pos == 0) {
		MMBuildMessage bm;
		bm.type = mtError;
		bm.text = text.right (text.length() - 9);
		bm.column = -1;
		bm.line = -1;
		bm.file = "";
	}
}

//-----------------------------------------------------------------------------

void MessageHandler::ParseStatisticMessage(QString text, MMBuildMessage &bm)
{
	int pos = text.indexOf(": ");
	if (pos > 0) {
		QString sizeType = text.left(pos);
		QString bytes = text.right(text.length() - sizeType.length() - 1);
		bytes = bytes.trimmed();
		pos = bytes.indexOf(" (");
		float fpercent = 0.0;
		if (pos > 0) {
			QString percent = bytes.right(bytes.length() - pos - 2);
			pos = percent.indexOf("% ");
			if (pos > 0) {
				// convert string to float, forcing "." as the decimal point (USA style)
				percent = percent.left(pos);
				QLocale loc(QLocale::English, QLocale::UnitedStates);
				fpercent = loc.toFloat(percent);								
			}
		}

		pos = bytes.indexOf(" bytes");
		if(pos > 0) {
			bytes = bytes.left(pos).trimmed();
			if (sizeType == "Program") {
				lastBuildProgramSize = bytes.toInt();
				lastBuildProgramPercent = fpercent;
			} else if (sizeType == "Data") {
				lastBuildDataSize = bytes.toInt();
				lastBuildDataPercent = fpercent;
			} else if (sizeType == "EEPROM") {
				lastBuildEEPROMSize = bytes.toInt();
				lastBuildEEPROMPercent = fpercent;
			}
							
		}
	}
}

//-----------------------------------------------------------------------------

void MessageHandler::ClearBuildInfo(void)
{
	lastBuildProgramSize = -1;
	lastBuildDataSize = -1;
	lastBuildEEPROMSize = -1;
}

//-----------------------------------------------------------------------------

void MessageHandler::GetLastSucessfullBuildInfo(int &programSize, float &ppercent, 
	int &dataSize, float &dpercent, int &EEPROMSize, float &epercent)
{
	programSize = lastBuildProgramSize;
	dataSize = lastBuildDataSize;
	EEPROMSize = lastBuildEEPROMSize;
	ppercent = lastBuildProgramPercent;
	dpercent = lastBuildDataPercent;
	epercent = lastBuildEEPROMPercent;
}

//-----------------------------------------------------------------------------

void MessageHandler::ClearOutput(void)
{
	//return;
	semaphore->acquire();
	output = "<<<CLEAR>>>\n";
	semaphore->release();
}

//-----------------------------------------------------------------------------

QString MessageHandler::GetOutput(void)
{
	//return "";
	semaphore->acquire();
	QString temp = output;
	output = "";

	semaphore->release();
	return temp;	
}

//-----------------------------------------------------------------------------

bool MessageHandler::HasOutput(void)
{
	//return false;

	return output != "";
}

//-----------------------------------------------------------------------------

void MessageHandler::ClearBuildMessages(void)
{
	//return;
	semaphore->acquire();
	queue<MMBuildMessage> empty;
	swap(mmbuildMessages, empty);
	//mmbuildMessages.clear();
	semaphore->release();
}

//-----------------------------------------------------------------------------

int MessageHandler::BuildMessagesCount(void)
{
	//return 0;
	semaphore->acquire();	
	int res = mmbuildMessages.size();
	semaphore->release();
	return res;
}

//-----------------------------------------------------------------------------

MMBuildMessage MessageHandler::GetBuildMessage(void)
//bool MessageHandler::GetBuildMessage(unsigned int index, MMBuildMessage * const message)
{
	//return false;
	MMBuildMessage bm;
	bm.type = mtUnknown;	

	//bool res = false;
	semaphore->acquire();
	if (mmbuildMessages.size() > 0) {
		bm = mmbuildMessages.front();
		mmbuildMessages.pop();
	//if (index < mmbuildMessages.size()) {
		//memcpy(message, &(mmbuildMessages.at(index)), sizeof(MMBuildMessage));
		//res = true;
	}
	semaphore->release();

	return bm;
}

//-----------------------------------------------------------------------------