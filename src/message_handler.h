
#pragma once

//-----------------------------------------------------------------------------

#include <QString>
#include <QDir>

#include <vector>

#include "project.h"

//-----------------------------------------------------------------------------

using namespace std;

//-----------------------------------------------------------------------------

enum MessageHanndlerType {mtRegular, mtDebug, mtWarning, mtError};

//-----------------------------------------------------------------------------

class MessageHandler
{
public:
	MessageHandler(void);
	~MessageHandler(void);

	void Add(QString text, MessageHanndlerType type);
};

//-----------------------------------------------------------------------------

extern MessageHandler msg;

//-----------------------------------------------------------------------------

