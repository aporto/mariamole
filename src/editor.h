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
    void setCodeText(QString &text);

private:

    QsciScintilla *codeEditor;

};

#endif // EDITOR_H
