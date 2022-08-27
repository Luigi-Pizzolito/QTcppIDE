#include "linenumberarea.h"
#include "qtexteditlineshighlighted.h"

LineNumberArea::LineNumberArea(QTextEdit *editor) : QWidget(editor) {
    codeEditor = editor;
}

QSize LineNumberArea::sizeHint() const {
    return QSize(((QTextEditLinesHighlighted *)codeEditor)->lineNumberAreaWidth(), 0);
}

void LineNumberArea::paintEvent(QPaintEvent *event) {
    ((QTextEditLinesHighlighted *)codeEditor)->lineNumberAreaPaintEvent(event);
}
