#ifndef EDITOR_H
#define EDITOR_H


#include <qwidget.h>
#include <QFileInfo>
#include <QLabel>
#include <QTextStream>
#include <QApplication>
#include <QColor>
#include <QKeyEvent>
#include <QMenu>
#include <QDateTime>
#include <QDesktopServices>
#include <QSpacerItem>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStyle>
//QScintilla
#include <Qsci/qsciscintilla.h>
#include <Qsci/qscilexercpp.h>
#include <Qsci/qsciapis.h>


#include "mm_utils.h"
#include "config.h"

class Editor : public QsciScintilla
{
	Q_OBJECT

public:
	Editor(QWidget *parent);
    ~Editor();
	QString GetFileName(void);
	void setEditorStyle (void);
	void CodeBeautifier(void);
	bool Open(QString filename);
	bool Save(void);
	void Configure(void);

public slots:
    void ShowEditorMenu(const QPoint point);
	void HelpWithThis(void);
	void MenuUndo(void);
	void MenuRedo(void);
	void MenuCut(void);
	void MenuCopy(void);
	void MenuPaste(void);
	//void MenuDelete(void);
	void MenuSelectAll(void);
	void onCursorPositionChanged (int line, int index); 

signals:
	void ctrlUPressed(void);
	void searchPressed(void);

protected: 
	void focusInEvent ( QFocusEvent * event );
	void mousePressEvent ( QMouseEvent * event );
	void keyPressEvent(QKeyEvent * event);
	
private:
	QsciAPIs *api;
    QsciLexerCPP *lexer;
	QString file;
    QDateTime lastModifiedTime;
	QMenu * context;
	QAction * actionHelpWithThis;
	QLabel * lblCursorPosition;
	QColor caretLineBackColor;
	void SetFileName(QString filename);
	void setLexerStyle(int styleId, QString styleName);
	//void setLexerStyle (int style, QColor foreground, QColor background, bool bold = false, bool italic = false, bool underline = false);
	QString GetRefenceForWordUnderCursor(void);
};

#endif // EDITOR_H
