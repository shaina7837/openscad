#include <QObject>
#include <QString>
#include <QWidget>
#include <QWheelEvent>
#include <QScrollBar>
#include <QTextEdit>
#include "highlighter.h"

class LegacyEditor : public QTextEdit
{
	Q_OBJECT
public:
	LegacyEditor(QWidget *parent);
	~LegacyEditor();
        QSize sizeHint() const;
        void setInitialSizeHint(const QSize &size);
	void wheelEvent ( QWheelEvent * event );

public slots:
	 void zoomIn();
        void zoomOut();
        void setLineWrapping(bool on) { if(on) setWordWrapMode(QTextOption::WrapAnywhere); }
        void setContentModified(bool y) { document()->setModified(y); }
        bool isContentModified() { return document()->isModified(); }
        void indentSelection();
        void unindentSelection();
        void commentSelection();
        void uncommentSelection();
        void setPlainText(const QString &text);
        void highlightError(int error_pos);
        void unhighlightLastError();
        void setHighlightScheme(const QString &name);
private:
                Highlighter *highlighter;
	QSize initialSizeHint;

};
