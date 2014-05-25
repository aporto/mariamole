#include "editor.h"

Editor::Editor(QWidget *parent) :  QsciScintilla(parent)
{

    QsciLexerCPP *lexer = new QsciLexerCPP;
    this->setLexer(lexer);


    QsciAPIs *api = new QsciAPIs(lexer);

    api->prepare();

    this->setAutoCompletionThreshold(1);
    this->setAutoCompletionSource(QsciScintilla::AcsAll);



}


Editor::Editor(QString &text) : Editor()
{
    setText(text);
}


void Editor::setText(QString &text)
{
    QsciScintilla::setText(text);
}

