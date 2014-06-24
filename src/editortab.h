#ifndef EDITORTAB_H
#define EDITORTAB_H

#include <QTabWidget>
#include <QFile>
#include <QTextStream>

#include "editor.h"
#include "serialmonitor.h"
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
	int portIndex(QString port);
	bool openFile(QString filename, int highlightLine=-1);	
	bool openSerialPort(QString port, QString speed);	
	bool saveAllFiles(void);	
	void closeAll(void);	
	void EnableAllSerialPorts(bool enable);

public slots:
    void closeTab(int);
	void onEditorTextChanged(void);
	void FormatCode(void);

signals:
     void codeChanged(void);

private:
	MM::TabType tabType(int index);
	bool saveFile(int index);	
	
};

#endif // EDITORTAB_H
