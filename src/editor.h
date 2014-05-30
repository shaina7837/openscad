#include <QObject>
#include <QString>
#include <QWidget>
#include <QWheelEvent>
#include <QScrollBar>
#include <QTextEdit>
#include <QVBoxLayout>
#include "highlighter.h"
#include "legacyeditor.h"

class Editor : public QWidget
{
	Q_OBJECT
public:
	Editor(QWidget *parent);
	~Editor();
	LegacyEditor *legacy;
        QSize sizeHint() const;
        void setInitialSizeHint(const QSize &size);
	void setTabStopWidth(int width);
	QString	toPlainText();
	QTextCursor textCursor() const;
	void setTextCursor (const QTextCursor &cursor);
	QTextDocument *document() { legacy->document(); }
	bool find(const QString & exp, QTextDocument::FindFlags options = 0);
public slots:
	void zoomIn();
	void zoomOut();
	void setLineWrapping(bool on) { legacy->setLineWrapping(on); }
	void setContentModified(bool y) { legacy->setContentModified(y); }
	bool isContentModified() { legacy->isContentModified(); }
	void indentSelection();
	void unindentSelection();
	void commentSelection();
	void uncommentSelection();
	void setPlainText(const QString &text);
	void highlightError(int error_pos);
	void unhighlightLastError();
	void setHighlightScheme(const QString &name);
	void insertPlainText(const QString &text);
	void wheelEvent (QWheelEvent * event);
	void undo();
	void redo();
	void cut();
	void copy();
	void paste();
private:
	Highlighter *highlighter;
	//LegacyEditor *legacy;
        QSize initialSizeHint;
	QVBoxLayout *legacyeditorLayout;
};
