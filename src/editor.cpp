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
	//QFont font("Inconsolata", 14, QFont::Normal);
	QFont font ("Consolas", 14, QFont::Normal, false);
	//font.setWeight(72);
	QFontMetrics fontMetrics(font);

	QColor backColor = QColor(22, 30, 32);
	
	// margin
	setMarginLineNumbers(100, true);
	setMarginsFont(font);
	setMarginsBackgroundColor(QColor(24,32,34));//QColor(20,28,30));
	setMarginsForegroundColor(QColor(42,50,52));
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

	

	//lexer->setFont(font, -1);
	//lexer->setPaper(QColor(backColor), -1);
	setCaretForegroundColor(QColor(220, 200, 200));
	setCaretLineBackgroundColor(QColor(36, 42, 44));

	SendScintilla (SCI_SETBUFFEREDDRAW, 0);
	SendScintilla (SCI_SETTWOPHASEDRAW, 0);
	SendScintilla (SCI_SETFONTQUALITY, SC_EFF_QUALITY_DEFAULT);	

	setMatchedBraceBackgroundColor(backColor);
	setMatchedBraceForegroundColor(QColor(255, 0, 0));

	setLexerStyle(-1, QColor (120, 120, 120),backColor);
	setLexerStyle(QsciLexerCPP::Default, QColor (0, 0, 120),backColor);
	setLexerStyle(QsciLexerCPP::Comment, QColor (80, 80, 80),backColor);
	setLexerStyle(QsciLexerCPP::CommentDoc, QColor (80, 80, 80),backColor);		
	setLexerStyle(QsciLexerCPP::CommentLine, QColor (80, 80, 80),backColor);
	setLexerStyle(QsciLexerCPP::Number, QColor (0, 120, 0),backColor);
	setLexerStyle(QsciLexerCPP::Keyword, QColor (120, 120, 0),backColor);
	setLexerStyle(QsciLexerCPP::DoubleQuotedString, QColor (0, 120, 120),backColor);
	setLexerStyle(QsciLexerCPP::SingleQuotedString, QColor (0, 120, 120),backColor);
	setLexerStyle(QsciLexerCPP::PreProcessor, QColor (110, 80, 140),backColor);
	setLexerStyle(QsciLexerCPP::Operator, QColor (140, 140, 140),backColor);
	setLexerStyle(QsciLexerCPP::Identifier, QColor (140, 140, 140),backColor);
	setLexerStyle(QsciLexerCPP::UnclosedString, QColor (0, 255, 255),backColor);
	
	
	setLexerStyle(QsciLexerCPP::UUID, QColor (150, 150, 150),backColor);	
	setLexerStyle(QsciLexerCPP::VerbatimString, QColor (150, 150, 150),backColor);
	setLexerStyle(QsciLexerCPP::Regex, QColor (150, 150, 150),backColor);
	setLexerStyle(QsciLexerCPP::CommentLineDoc, QColor (150, 150, 150),backColor);
	setLexerStyle(QsciLexerCPP::KeywordSet2, QColor (150, 150, 150),backColor);
	setLexerStyle(QsciLexerCPP::CommentDocKeyword, QColor (150, 150, 150),backColor);
	setLexerStyle(QsciLexerCPP::CommentDocKeywordError, QColor (150, 150, 150),backColor);
	setLexerStyle(QsciLexerCPP::GlobalClass, QColor (150, 150, 150),backColor);
	setLexerStyle(QsciLexerCPP::RawString, QColor (150, 150, 150),backColor);
	setLexerStyle(QsciLexerCPP::TripleQuotedVerbatimString, QColor (150, 150, 150),backColor);
	setLexerStyle(QsciLexerCPP::HashQuotedString, QColor (150, 150, 150),backColor);
	setLexerStyle(QsciLexerCPP::PreProcessorComment, QColor (150, 150, 150),backColor);
	setLexerStyle(QsciLexerCPP::PreProcessorCommentLineDoc, QColor (150, 150, 150),backColor); 	
	
}

void Editor::setLexerStyle(int style, QColor foreground, QColor background, bool bold, bool italic, bool underline)
{
	QFont font("Consolas", -1, QFont::Normal, false);
	lexer->setFont(font, style);
	lexer->setColor(foreground, style);
	lexer->setPaper(background, style);		
}


void Editor::onCursorPositionChanged(int line, int index)
{
	setCaretLineBackgroundColor(QColor(16, 24, 26));
	markerDeleteAll();
}