#ifndef EDITOR_H
#define EDITOR_H

#include <Qsci/qsciscintilla.h>
#include <Qsci/qscilexercpp.h>
#include <Qsci/qsciapis.h>


class Editor : public QsciScintilla
{

public:
    Editor(QWidget *parent = 0);
    Editor(QString &text);
    ~Editor();
    void setText(QString &text);

private:
    QsciAPIs *api;
    QsciLexerCPP *lexer;

};

#endif // EDITOR_H
