#ifndef EDITORTAB_H
#define EDITORTAB_H

#include <QTabWidget>
#include <QFile>
#include <QMenu>
#include <QTextStream>

#include "editor.h"
#include "serialmonitor.h"
#include "mm_utils.h"
#include "config.h"

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
    bool allSaved(void);
	void closeAll(void);	
	void EnableAllSerialPorts(bool enable);
	void ConfigureAllTabs(void);
	void Search(QString text, bool caseSensitive, bool wholeWords);

public slots:
    void closeTab(int);
	void closeThis(void);
	void closeAllButThis(void);
	void onEditorTextChanged(void);
	void FormatCode(void);
	void ShowEditorMenu(const QPoint point);	

signals:
     void codeChanged(void);

protected:
	bool eventFilter(QObject *obj, QEvent *event);

private:
	MM::TabType tabType(int index);
	bool saveFile(int index);	
	QMenu * context;
	
	
};

#endif // EDITORTAB_H
