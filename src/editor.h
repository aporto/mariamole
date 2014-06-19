#ifndef EDITOR_H
#define EDITOR_H

//#include <QsciScintilla>
#include <qwidget.h>
#include <Qsci/qsciscintilla.h>
#include <Qsci/qscilexercpp.h>
#include <Qsci/qsciapis.h>
#include <QFileInfo>
#include <QTextStream>
#include <QApplication>

class Editor : public QsciScintilla
{
	Q_OBJECT

public:
	Editor(QWidget *parent);
	~Editor();
	void SetFileName(QString filename);
	QString GetFileName(void);
	

private:
	QsciAPIs *api;
    QsciLexerCPP *lexer;
	QString file;
};

#endif // EDITOR_H
