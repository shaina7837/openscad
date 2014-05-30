#include "legacyeditor.h"
#include "Preferences.h"

LegacyEditor::LegacyEditor(QWidget *parent) : QTextEdit(parent)
{
	setAcceptRichText(false);
	// This needed to avoid QTextEdit accepting filename drops as we want
	// to handle these ourselves in MainWindow
	setAcceptDrops(false);
	this->highlighter = new Highlighter(this->document());
}

void LegacyEditor::indentSelection()
{
	QTextCursor cursor = textCursor();
	int p1 = cursor.selectionStart();
	QString txt = cursor.selectedText();

	txt.replace(QString(QChar(8233)), QString(QChar(8233)) + QString("\t"));
	if (txt.endsWith(QString(QChar(8233)) + QString("\t")))
		txt.chop(1);
	txt = QString("\t") + txt;

	cursor.insertText(txt);
	int p2 = cursor.position();
	cursor.setPosition(p1, QTextCursor::MoveAnchor);
	cursor.setPosition(p2, QTextCursor::KeepAnchor);
	setTextCursor(cursor);
}

void LegacyEditor::unindentSelection()
{
	QTextCursor cursor = textCursor();
	int p1 = cursor.selectionStart();
	QString txt = cursor.selectedText();

	txt.replace(QString(QChar(8233)) + QString("\t"), QString(QChar(8233)));
	if (txt.startsWith(QString("\t")))
		txt.remove(0, 1);

	cursor.insertText(txt);
	int p2 = cursor.position();
	cursor.setPosition(p1, QTextCursor::MoveAnchor);
	cursor.setPosition(p2, QTextCursor::KeepAnchor);
	setTextCursor(cursor);
}

void LegacyEditor::commentSelection()
{
	QTextCursor cursor = textCursor();
	int p1 = cursor.selectionStart();
	QString txt = cursor.selectedText();

	txt.replace(QString(QChar(8233)), QString(QChar(8233)) + QString("//"));
	if (txt.endsWith(QString(QChar(8233)) + QString("//")))
		txt.chop(2);
	txt = QString("//") + txt;

	cursor.insertText(txt);
	int p2 = cursor.position();
	cursor.setPosition(p1, QTextCursor::MoveAnchor);
	cursor.setPosition(p2, QTextCursor::KeepAnchor);
	setTextCursor(cursor);
}

void LegacyEditor::uncommentSelection()
{
	QTextCursor cursor = textCursor();
	int p1 = cursor.selectionStart();
	QString txt = cursor.selectedText();

	txt.replace(QString(QChar(8233)) + QString("//"), QString(QChar(8233)));
	if (txt.startsWith(QString("//")))
		txt.remove(0, 2);

	cursor.insertText(txt);
	int p2 = cursor.position();
	cursor.setPosition(p1, QTextCursor::MoveAnchor);
	cursor.setPosition(p2, QTextCursor::KeepAnchor);
	setTextCursor(cursor);
}

void LegacyEditor::zoomIn()
{
	// See also QT's implementation in QLegacyEditor.cpp
	QSettings settings;
	QFont tmp_font = this->font() ;
	if ( font().pointSize() >= 1 )
		tmp_font.setPointSize( 1 + font().pointSize() );
	else
		tmp_font.setPointSize( 1 );
	settings.setValue("editor/fontsize", tmp_font.pointSize());
	this->setFont( tmp_font );
}

void LegacyEditor::zoomOut()
{
	QSettings settings;
	QFont tmp_font = this->font();
	if ( font().pointSize() >= 2 )
		tmp_font.setPointSize( -1 + font().pointSize() );
	else
		tmp_font.setPointSize( 1 );
	settings.setValue("editor/fontsize", tmp_font.pointSize());
	this->setFont( tmp_font );
}

void LegacyEditor::wheelEvent ( QWheelEvent * event )
{
	QSettings settings;
	bool wheelzoom_enabled = Preferences::inst()->getValue("editor/ctrlmousewheelzoom").toBool();
	if ((event->modifiers() == Qt::ControlModifier) && wheelzoom_enabled ) {
		if (event->delta() > 0 )
			zoomIn();
		else if (event->delta() < 0 )
			zoomOut();
	} else {
		QTextEdit::wheelEvent( event );
	}
}

void LegacyEditor::setPlainText(const QString &text)
{
	int y = verticalScrollBar()->sliderPosition();
	// Save current cursor position
	QTextCursor cursor = textCursor();
	int n = cursor.position();
	QTextEdit::setPlainText(text);
	// Restore cursor position
	if (n < text.length()) {
		cursor.setPosition(n);
		setTextCursor(cursor);
		verticalScrollBar()->setSliderPosition(y);
	}
}

void LegacyEditor::highlightError(int error_pos)
{
	highlighter->highlightError( error_pos );
        QTextCursor cursor = this->textCursor();
        cursor.setPosition( error_pos );
        this->setTextCursor( cursor );
}

void LegacyEditor::unhighlightLastError()
{
	highlighter->unhighlightLastError();
}

void LegacyEditor::setHighlightScheme(const QString &name)
{
	highlighter->assignFormatsToTokens( name );
	highlighter->rehighlight(); // slow on large files
}

QSize LegacyEditor::sizeHint() const
{
	if (initialSizeHint.width() <= 0) {
		return QTextEdit::sizeHint();
	} else {
		return initialSizeHint;
	}
}

void LegacyEditor::setInitialSizeHint(const QSize &size)
{
	initialSizeHint = size;
}

LegacyEditor::~LegacyEditor()
{
	delete highlighter;
}
