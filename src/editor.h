#ifndef EDITOR_H
#define EDITOR_H

#include <qwidget.h>
//#include <Qsci/qsciscintilla.h>
//#include <Qsci/qscilexercpp.h>
//#include <Qsci/qsciapis.h>


class Editor : public QWidget //QsciScintilla
{
    Q_OBJECT


public:
	// Alex: comentei aqui
	/* Editor(QWidget *parent = 0);
    Editor(QString &text);
    ~Editor();
	*/
    void setText(QString &text);

private:
 //   QsciAPIs *api;
   // QsciLexerCPP *lexer;

};

#endif // EDITOR_H
