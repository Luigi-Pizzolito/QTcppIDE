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

#include <iostream>

LineNumberArea::LineNumberArea(QTextEdit *editor) : QWidget(editor) {
    codeEditor = editor;
}

void LineNumberArea::passFMP(FileManager *pfmp) {
    fmp = pfmp;
};

// forward/return from code editor functions

QSize LineNumberArea::sizeHint() const {
    return QSize(((CodeEditor *)codeEditor)->lineNumberAreaWidth(), 0);
}

void LineNumberArea::paintEvent(QPaintEvent *event) {
    ((CodeEditor *)codeEditor)->lineNumberAreaPaintEvent(event);
}

// breakpoint handling when click event happens in line number area
void LineNumberArea::mousePressEvent(QMouseEvent *e) {
    // repaint editor to refresh line start indexes
    codeEditor->repaint();
    this->repaint();

    if (!*fmp->showingDocs) { //breakpoints disabled if showing docs
        // get current line based on line start indexes and click event coordinates
        QMap<int,int> lineS = ((CodeEditor *)codeEditor)->lineStarts.value(fmp->fileP);
        if (e->pos().x() <= 2*((CodeEditor *)codeEditor)->singleCharWidth()) { // if near the breakpoint area hitbox

            int line=0;
            // iterate over all y-triggers of line map in order, the first highest one when compared to click-y -1 is the line that was clicked
            for(auto l : lineS.toStdMap()) {
                if (e->pos().y() < l.second) {
                    line = l.first -1;
                    break;
                }
            }
            if (line==0) line=lineS.size(); // if no line y-triggers were bigger than click-y, we are on the last line of the file.

            // insert or remove breakpoint
            QMap<int, bool> bp = ((CodeEditor *)codeEditor)->breakPoints.value(fmp->fileP);
            if (!bp.contains(line)) {
                bp.insert(line, true);
            } else {
                bp.remove(line);
            }
            ((CodeEditor *)codeEditor)->breakPoints.insert(fmp->fileP, bp);
            // repaint editor to refresh breakpoints display
            codeEditor->repaint();
            this->repaint();

        }
    }
}
