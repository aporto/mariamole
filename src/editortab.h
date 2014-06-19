#ifndef EDITORTAB_H
#define EDITORTAB_H

#include <QTabWidget>
#include <QFile>
#include <QTextStream>

#include "editor.h"
#include "mm_utils.h"

namespace MM {
	enum TabType {undefinedTab, codeTab, serialTab};
}

class EditorTab : public QTabWidget
{
	Q_OBJECT

public:
	EditorTab(QWidget *parent);
	~EditorTab();
	int fileIndex(QString filename);
	bool openFile(QString filename, int highlightLine=-1);	
	bool saveAllFiles(void);	
	void closeAll(void);	

public slots:
    void closeTab(int);

private:
	MM::TabType tabType(int index);
	bool saveFile(int index);	
	
};

#endif // EDITORTAB_H
