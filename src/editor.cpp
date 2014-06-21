#include "editor.h"

Editor::Editor(QWidget *parent)
	: QsciScintilla(parent)
{
	// identify this widget as a code editor
	// WindoweIconText is used only for windows widgets. 
	// So, it's free for us to use it here
	setWindowIconText("editor");
	//setEolMode(QsciScintilla::EolUnix);

	lexer = new QsciLexerCPP;
    this->setLexer(lexer);

    api = new QsciAPIs(lexer);

    api->prepare();

    this->setAutoCompletionThreshold(1);
    this->setAutoCompletionSource(QsciScintilla::AcsAll);

	connect(this, SIGNAL(cursorPositionChanged(int, int)), this, SLOT(onCursorPositionChanged(int, int)));

	setEditorStyle();
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

void Editor::setEditorStyle (void)
{
	QFont font("Inconsolata", 14, QFont::Normal);
	//font.setWeight(72);
	QFontMetrics fontMetrics(font);
	
	// margin
	setMarginLineNumbers(100, true);
	setMarginsFont(font);
	setMarginsBackgroundColor(QColor(20,28,30));
	setMarginsForegroundColor(QColor(62,70,72));
	setMarginWidth(0, fontMetrics.width("00000") + 6);
	setIndicatorForegroundColor (Qt::red, 1);	
	setTabWidth(4);
	
	// brace matching
	setBraceMatching(QsciScintilla::SloppyBraceMatch);

	// error line marker
	markerDefine(QsciScintilla::RightArrow, 1); 
    setMarkerBackgroundColor(Qt::red, 1);
	ensureCursorVisible();

	//lexer->setColor(Qt::yellow, -1);
	
	//setCaretLineVisible(true);
	setCaretWidth(2);
	markerDefine(QsciScintilla::Circle, 0);
	setMarkerBackgroundColor(Qt::red, 0);
	setMarkerForegroundColor(Qt::white, 0);

	lexer->setFont(font, -1);
	lexer->setPaper(QColor(14, 22, 24), -1);
	setCaretForegroundColor(QColor(220, 200, 200));
	setCaretLineBackgroundColor(QColor(16, 24, 26));

	setLexerStyle(QsciLexerCPP::Default, QColor (190, 190, 190), QColor(14, 22, 24));
	setLexerStyle(QsciLexerCPP::Comment, QColor (80, 80, 80), QColor(14, 22, 24));
	setLexerStyle(QsciLexerCPP::Number, QColor (0, 150, 0), QColor(14, 22, 24));
	setLexerStyle(QsciLexerCPP::Keyword, QColor (210, 210, 0), QColor(14, 22, 24));
	setLexerStyle(QsciLexerCPP::DoubleQuotedString, QColor (0, 220, 220), QColor(14, 22, 24));
	setLexerStyle(QsciLexerCPP::SingleQuotedString, QColor (0, 220, 220), QColor(14, 22, 24));
	setLexerStyle(QsciLexerCPP::PreProcessor, QColor (230, 0, 230), QColor(14, 22, 24));
	setLexerStyle(QsciLexerCPP::Operator, QColor (230, 230, 230), QColor(14, 22, 24));
	setLexerStyle(QsciLexerCPP::Identifier, QColor (220, 220, 220), QColor(14, 22, 24));
	setLexerStyle(QsciLexerCPP::UnclosedString, QColor (0, 255, 255), QColor(14, 22, 24));

	setLexerStyle(QsciLexerCPP::CommentLine, QColor (80, 80, 80), QColor(14, 22, 24));
	setLexerStyle(QsciLexerCPP::CommentDoc, QColor (50, 50, 50), QColor(14, 22, 24));		
	setLexerStyle(QsciLexerCPP::UUID, QColor (150, 150, 150), QColor(14, 22, 24));	
	setLexerStyle(QsciLexerCPP::VerbatimString, QColor (150, 150, 150), QColor(14, 22, 24));
	setLexerStyle(QsciLexerCPP::Regex, QColor (150, 150, 150), QColor(14, 22, 24));
	setLexerStyle(QsciLexerCPP::CommentLineDoc, QColor (150, 150, 150), QColor(14, 22, 24));
	setLexerStyle(QsciLexerCPP::KeywordSet2, QColor (150, 150, 150), QColor(14, 22, 24));
	setLexerStyle(QsciLexerCPP::CommentDocKeyword, QColor (150, 150, 150), QColor(14, 22, 24));
	setLexerStyle(QsciLexerCPP::CommentDocKeywordError, QColor (150, 150, 150), QColor(14, 22, 24));
	setLexerStyle(QsciLexerCPP::GlobalClass, QColor (150, 150, 150), QColor(14, 22, 24));
	setLexerStyle(QsciLexerCPP::RawString, QColor (150, 150, 150), QColor(14, 22, 24));
	setLexerStyle(QsciLexerCPP::TripleQuotedVerbatimString, QColor (150, 150, 150), QColor(14, 22, 24));
	setLexerStyle(QsciLexerCPP::HashQuotedString, QColor (150, 150, 150), QColor(14, 22, 24));
	setLexerStyle(QsciLexerCPP::PreProcessorComment, QColor (150, 150, 150), QColor(14, 22, 24));
	setLexerStyle(QsciLexerCPP::PreProcessorCommentLineDoc, QColor (150, 150, 150), QColor(14, 22, 24)); 	
	
}

void Editor::setLexerStyle(int style, QColor foreground, QColor background, bool bold, bool italic, bool underline)
{
	QFont font("Inconsolata", -1, QFont::Normal);
	lexer->setFont(font, style);
	lexer->setColor(foreground, style);
	lexer->setPaper(background, style);		
}


void Editor::onCursorPositionChanged(int line, int index)
{
	setCaretLineBackgroundColor(QColor(16, 24, 26));
	markerDeleteAll();
}