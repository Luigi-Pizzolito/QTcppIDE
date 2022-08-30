/*
 * Line Number Class
 * 
 * C++ IDE by Luigi Pizzolito
 *            Zhang Ruiqing
 *            Ruan Zihang
 *            Lin Zhenmin
 *            Yang Zhaoyi
 * For 1703-ECE Class by Nie Qing and Wu Hao
 * At Beijing Insitute of Techology
*/

#include "linenumberarea.h"
#include "codeeditor.h"

LineNumberArea::LineNumberArea(QTextEdit *editor) : QWidget(editor) {
    codeEditor = editor;
}

// forward/return from code editor functions

QSize LineNumberArea::sizeHint() const {
    return QSize(((CodeEditor *)codeEditor)->lineNumberAreaWidth(), 0);
}

void LineNumberArea::paintEvent(QPaintEvent *event) {
    ((CodeEditor *)codeEditor)->lineNumberAreaPaintEvent(event);
}
