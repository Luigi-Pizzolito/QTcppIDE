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

#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QTextEdit>
#include <QWidget>

class CodeEditor : public QTextEdit
{
    Q_OBJECT

public:

    explicit CodeEditor(QWidget *parent = 0);

    int getFirstVisibleBlockId();
    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();

    // functions to draw breakpoints and maps to keep track of them
    int singleCharWidth();
    QMap<int, int> lineStarts;
    QMap<int, bool> breakPoints;

signals:


public slots:

    void resizeEvent(QResizeEvent *e);

private slots:

    void updateLineNumberAreaWidth(int newBlockCount);
    void updateLineNumberArea(QRectF /*rect_f*/);
    void updateLineNumberArea(int /*slider_pos*/);
    void updateLineNumberArea();

    void highlightCurrentLine();

private:

    QWidget *lineNumberArea;

};
#endif // CODEEDITOR_H
