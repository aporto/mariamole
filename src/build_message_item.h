#ifndef BUILD_MESSAGE_ITEM_H
#define BUILD_MESSAGE_ITEM_H

//-----------------------------------------------------------------------------

#include <QListWidget>
#include <QString>

#include "message_handler.h"

//-----------------------------------------------------------------------------

/*struct BuildMessage {
	QString text;
	QString file;
	int line, column;		
};*/

class BuildMessageItem : public QListWidgetItem {
public:  
	MMBuildMessage bm;     
};

#endif // MAINWINDOW_H
