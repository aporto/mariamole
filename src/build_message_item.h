#ifndef BUILD_MESSAGE_ITEM_H
#define BUILD_MESSAGE_ITEM_H

//-----------------------------------------------------------------------------

#include <QListWidget>
#include <QString>

#include "message_handler.h"

//-----------------------------------------------------------------------------

class BuildMessageItem : public QListWidgetItem {
public:  
	MMBuildMessage bm;     
};

#endif // MAINWINDOW_H
