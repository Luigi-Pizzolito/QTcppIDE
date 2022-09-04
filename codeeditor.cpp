/*
 * Code Editor Class
 * 
 * C++ IDE by Luigi Pizzolito
 *            Zhang Ruiqing
 *            Ruan Zihang
 *            Lin Zhenmin
 *            Yang Zhaoyi
 * For 1703-ECE Class by Nie Qing and Wu Hao
 * At Beijing Insitute of Techology
 *
 * This file includes code that is part of the examples of the Qt Toolkit.
*/

#include "globaldefs.h"

#include "codeeditor.h"
#include "linenumberarea.h"

CodeEditor::CodeEditor(QWidget *parent) :
    QTextEdit(parent)
{
    // Line numbers
    lineNumberArea = new LineNumberArea(this);
    // Signals to update line number area
    connect(this->document(), SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(updateLineNumberArea/*_2*/(int)));
    connect(this, SIGNAL(textChanged()), this, SLOT(updateLineNumberArea()));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(updateLineNumberArea()));
    // Highlight current line
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));
    // Initial update
    updateLineNumberAreaWidth(0);
    setMouseTracking(true);
}

void CodeEditor::passFMP(FileManager *pfmp) {
    fmp = pfmp;
    ((LineNumberArea*)lineNumberArea)->passFMP(fmp);
};

int CodeEditor::lineNumberAreaWidth()
{
    // calculate max line number area width given the amount of digits
    int digits = 1;
    int max = qMax(1, this->document()->blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    int space = 13 +  fontMetrics().horizontalAdvance(QLatin1Char('9')) * ((digits)  +1/*for BP width, add one char width*/);

    return space;
}

int CodeEditor::singleCharWidth() {
    // needed to calculate where to draw breakpoints
    return 13+fontMetrics().horizontalAdvance(QLatin1Char('9'));
}

void CodeEditor::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}


void CodeEditor::updateLineNumberArea(QRectF /*rect_f*/)
{
    CodeEditor::updateLineNumberArea();
}
void CodeEditor::updateLineNumberArea(int /*slider_pos*/)
{
    CodeEditor::updateLineNumberArea();
}
void CodeEditor::updateLineNumberArea()
{
    /*
     * When the signal is emitted, the sliderPosition has been adjusted according to the action,
     * but the value has not yet been propagated (meaning the valueChanged() signal was not yet emitted),
     * and the visual display has not been updated. In slots connected to this signal you can thus safely
     * adjust any action by calling setSliderPosition() yourself, based on both the action and the
     * slider's value.
     */
    // Make sure the sliderPosition triggers one last time the valueChanged() signal with the actual value!
    this->verticalScrollBar()->setSliderPosition(this->verticalScrollBar()->sliderPosition());

    // Since "QTextEdit" does not have an "updateRequest(...)" signal, we chose
    // to grab the imformations from "sliderPosition()" and "contentsRect()".
    // See the necessary connections used (Class constructor implementation part).

    QRect rect =  this->contentsRect();
    lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());
    updateLineNumberAreaWidth(0);
    //----------
    int dy = this->verticalScrollBar()->sliderPosition();
    if (dy > -1) {
        lineNumberArea->scroll(0, dy);
    }

    // Adjust slider to always see the number of the currently being edited line...
    int first_block_id = getFirstVisibleBlockId();
    if (first_block_id == 0 || this->textCursor().block().blockNumber() == first_block_id-1)
        this->verticalScrollBar()->setSliderPosition(dy-this->document()->documentMargin());

}


void CodeEditor::resizeEvent(QResizeEvent *e)
{
    // Update QTextEdit on resize
    QTextEdit::resizeEvent(e);
    // Update line number area rectangle on resize
    QRect cr = this->contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}


int CodeEditor::getFirstVisibleBlockId()
{
    // Detect the first block for which bounding rect - once translated
    // in absolute coordinated - is contained by the editor's text area

    // Costly way of doing but since "blockBoundingGeometry(...)" doesn't
    // exists for "QTextEdit"...

    QTextCursor curs = QTextCursor(this->document());
    curs.movePosition(QTextCursor::Start);
    for(int i=0; i < this->document()->blockCount(); ++i)
    {
        QTextBlock block = curs.block();

        QRect r1 = this->viewport()->geometry();
        QRect r2 = this->document()->documentLayout()->blockBoundingRect(block).translated(
                    this->viewport()->geometry().x(), this->viewport()->geometry().y() - (
                        this->verticalScrollBar()->sliderPosition()
                        ) ).toRect();

        if (r1.contains(r2, true)) { return i; }

        curs.movePosition(QTextCursor::NextBlock);
    }

    return 0;
}

void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    // Draw line number area

    this->verticalScrollBar()->setSliderPosition(this->verticalScrollBar()->sliderPosition());

    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), Qt::lightGray);
    int blockNumber = this->getFirstVisibleBlockId();

    QTextBlock block = this->document()->findBlockByNumber(blockNumber);
    QTextBlock prev_block = (blockNumber > 0) ? this->document()->findBlockByNumber(blockNumber-1) : block;
    int translate_y = (blockNumber > 0) ? -this->verticalScrollBar()->sliderPosition() : 0;

    int top = this->viewport()->geometry().top();

    // Adjust text position according to the previous "non entirely visible" block
    // if applicable. Also takes in consideration the document's margin offset.
    int additional_margin;
    if (blockNumber == 0)
        // Simply adjust to document's margin
        additional_margin = (int) this->document()->documentMargin() -1 - this->verticalScrollBar()->sliderPosition();
    else
        // Getting the height of the visible part of the previous "non entirely visible" block
        additional_margin = (int) this->document()->documentLayout()->blockBoundingRect(prev_block)
                .translated(0, translate_y).intersected(this->viewport()->geometry()).height();

    // Shift the starting point
    top += additional_margin;

    int bottom = top + (int) this->document()->documentLayout()->blockBoundingRect(block).height();

    QColor col_1 = QColor(Qt::blue).lighter(100);      // Current line (custom green)   col_1(90, 255, 30)
    QColor col_0(120, 120, 120);    // Other lines  (custom darkgrey)

    // Draw the numbers (displaying the current line number in green)
    while (block.isValid() && top <= event->rect().bottom()) {
        QString number = QString::number(blockNumber + 1);
        if (block.isVisible() && bottom >= event->rect().top()) {

            painter.setPen(QColor(120, 120, 120));
            painter.setPen((this->textCursor().blockNumber() == blockNumber) ? col_1 : col_0);
            painter.drawText(-5, top,
                             lineNumberArea->width(), fontMetrics().height(),
                             Qt::AlignRight, number);
            //BP
            if (!*fmp->showingDocs) { //breakpoints disabled if showing docs
                // draw hover point
                if (((LineNumberArea*)lineNumberArea)->hoverBP == number.toInt()) {
                    painter.setPen(QColor(255,0,0).darker(180));
                    painter.drawText(5, top, lineNumberArea->width(),
                                     fontMetrics().height(), Qt::AlignVCenter | Qt::TextDontClip,
                                     "●");
                }

                // if there is a breakpoint for this line on map, draw a red circle
                QMap<int, bool> fileBreakPoints = breakPoints.value(fmp->fileP);
                if (fileBreakPoints.value(number.toInt(),false)) {
                    painter.setPen(Qt::red);
                    painter.drawText(5, top, lineNumberArea->width(),
                                     fontMetrics().height(), Qt::AlignVCenter | Qt::TextDontClip,
                                     "●");
                }

            }
            //~BP
        }

        //BP
        // insert the start of line y-coordinate into the line start index for breakpoint calculations
        if (!*fmp->showingDocs) { // breakpoints disabled if showing docs
            if (!lineStarts.contains(fmp->fileP)) {
                lineStarts.insert(fmp->fileP, QMap<int,int> {{number.toInt(), top}});
            } else {
                QMap<int,int> bkpBPs = lineStarts.value(fmp->fileP);
                bkpBPs.insert(number.toInt(), top);
                lineStarts.insert(fmp->fileP, bkpBPs);
            }
        }
        //~BP

        block = block.next();
        top = bottom;
        bottom = top + (int) this->document()->documentLayout()->blockBoundingRect(block).height();
        ++blockNumber;
    }

}

void CodeEditor::highlightCurrentLine()
{
    // Draw highlight on current line

    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;

        QColor lineColor = CONSOLETCOLOR;

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}

void CodeEditor::mouseMoveEvent(QMouseEvent *e){
    ((LineNumberArea*)lineNumberArea)->hoverBP=0;
}
