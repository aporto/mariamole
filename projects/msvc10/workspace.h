#pragma once

//-----------------------------------------------------------------------------

#include <QString>
#include <QDir>

#include <vector>

#include "project.h"
#include "message_handler.h"

//-----------------------------------------------------------------------------

using namespace std;

//-----------------------------------------------------------------------------

class Workspace
{
public:
	Workspace(void);
	~Workspace(void);

	bool Open(QString workPath);

	vector <Project> projects;

private:
	QString path;	
};

//-----------------------------------------------------------------------------

extern Workspace workspace;

//-----------------------------------------------------------------------------

