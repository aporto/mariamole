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
	//setAutoCompletionThreshold(0);
	
    api = new QsciAPIs(lexer);
    api->prepare();

	LoadStyleSheet(this, "style_code_editor.css");
	
    setAutoCompletionThreshold(1);
    setAutoCompletionSource(QsciScintilla::AcsAll);

	connect(this, SIGNAL(cursorPositionChanged(int, int)), this, SLOT(onCursorPositionChanged(int, int)));

	setEditorStyle();

	lastModifiedTime = QDateTime::currentDateTime();
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
	QFont font ("Consolas", 14, QFont::Normal, false);
	QFontMetrics fontMetrics(font);

	QColor backColor = QColor(22, 30, 32);
	
	// margin
	setMarginLineNumbers(100, true);
	setMarginsFont(font);
	setMarginsBackgroundColor(backColor);//QColor(24,32,34));//QColor(20,28,30));
	setMarginsForegroundColor(QColor(42,50,52));
	setMarginWidth(0, fontMetrics.width("00000") + 6);	
	
	// General editor things
	setTabWidth(4);	
	ensureCursorVisible();
	setIndicatorForegroundColor (Qt::red, 1);	

	// compile error/warning mark
	markerDefine(QsciScintilla::RightArrow, 1); 
    setMarkerBackgroundColor(Qt::red, 1);	
	markerDefine(QsciScintilla::Circle, 0);
	setMarkerBackgroundColor(Qt::red, 0);
	setMarkerForegroundColor(Qt::white, 0);

	// caret
	setCaretLineVisible(true);
	setCaretWidth(2);
	setCaretForegroundColor(QColor(220, 200, 200));
	setCaretLineBackgroundColor(QColor(24, 32, 34));

	//SendScintilla (SC_TECHNOLOGY_DIRECTWRITE, 1);
	//SendScintilla (SCI_SETBUFFEREDDRAW, 1);
	//SendScintilla (SCI_SETTWOPHASEDRAW, 1);
	//SendScintilla (SCI_SETFONTQUALITY,  SC_EFF_QUALITY_NON_ANTIALIASED);//SC_EFF_QUALITY_DEFAULT);	

	// matched and unmatched braces coloring
	setBraceMatching(QsciScintilla::SloppyBraceMatch);
	setMatchedBraceBackgroundColor(backColor);
	setMatchedBraceForegroundColor(QColor(220, 220, 50));
	setUnmatchedBraceBackgroundColor(QColor(0,0,0));
	setUnmatchedBraceForegroundColor(QColor(255, 0, 0));

	// maim cpp styles
	setLexerStyle(-1, QColor (120, 120, 120),backColor);	
	//setLexerStyle(-1, Qt::red, Qt::blue);	
	//setLexerStyle(QsciLexerCPP::Default, Qt::blue, Qt::yellow);
	setLexerStyle(QsciLexerCPP::Default, QColor (0, 0, 120),backColor);
	//setLexerStyle(QsciLexerCPP::callSTYLE_CALLTIP, QColor (255,0,0), QColor(255,255,255));
	
	setLexerStyle(QsciLexerCPP::Comment, QColor (80, 80, 80),backColor);
	setLexerStyle(QsciLexerCPP::CommentDoc, QColor (80, 80, 80),backColor);		
	setLexerStyle(QsciLexerCPP::CommentLine, QColor (80, 80, 80),backColor);
	setLexerStyle(QsciLexerCPP::Number, QColor (0, 120, 0),backColor);
	setLexerStyle(QsciLexerCPP::Keyword, QColor (140, 100, 0),backColor);
	setLexerStyle(QsciLexerCPP::DoubleQuotedString, QColor (0, 120, 120),backColor);
	setLexerStyle(QsciLexerCPP::SingleQuotedString, QColor (0, 120, 120),backColor);
	setLexerStyle(QsciLexerCPP::PreProcessor, QColor (110, 80, 140),backColor);
	setLexerStyle(QsciLexerCPP::Operator, QColor (140, 140, 50),backColor);
	setLexerStyle(QsciLexerCPP::Identifier, QColor (140, 140, 140),backColor);
	setLexerStyle(QsciLexerCPP::UnclosedString, QColor (0, 255, 255),backColor);
	
	// secondary styles
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
	//setCaretLineBackgroundColor(QColor(16, 24, 26));
	markerDeleteAll();
}


void Editor::focusInEvent ( QFocusEvent * event )
{
	QsciScintilla::focusInEvent(event);

	// Check if file was modified
	QDateTime fileTime = QFileInfo(file).lastModified();
	if (lastModifiedTime < fileTime) {
		lastModifiedTime = QDateTime::currentDateTime();
		if (GetUserConfirmation("File was modified outsite editor. Do you want to reload it?\n" +file))  {
			QFile inFile(file);
			if (!inFile.open(QFile::ReadOnly)) {
				ErrorMessage(tr("Cannot read file %1:\n%2.").arg(file).arg(inFile.errorString()));			
				return;
			}
			QTextStream in(&inFile);
			QApplication::setOverrideCursor(Qt::WaitCursor);
			QString txt(in.readAll());
			setText(txt);				
			setModified(false);
			QApplication::restoreOverrideCursor();
		}
	}
}