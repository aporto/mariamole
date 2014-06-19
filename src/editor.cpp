#include "editor.h"

Editor::Editor(QWidget *parent)
	: QsciScintilla(parent)
{
	// identify this widget as a code editor
	// WindoweIconText is used only for windows widgets. 
	// So, it's free for us to use it here
	setWindowIconText("editor");

	lexer = new QsciLexerCPP;
    this->setLexer(lexer);
	

    api = new QsciAPIs(lexer);

    api->prepare();

    this->setAutoCompletionThreshold(1);
    this->setAutoCompletionSource(QsciScintilla::AcsAll);

	setIndicatorForegroundColor (Qt::red, 1);	
}


Editor::~Editor()
{
	delete lexer;
//    delete api; api parent's (lexer) will delete it
}

void Editor::SetFileName(QString filename)
{
	file = filename;
}

QString Editor::GetFileName(void)
{
	return file;
}