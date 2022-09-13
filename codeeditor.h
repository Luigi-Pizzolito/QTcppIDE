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

#include "filemanager.h"

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
    // nested maps, to store the maps individually for each open file
    QMap<QString,QMap<int, int>> lineStarts; // store the y-coordinates where each line starts for breakpoint click event calculations
    QMap<QString,QMap<int, bool>> breakPoints; // store bools for each line to determine if there is a breakpoint
    void passFMP(FileManager *pfmp);

signals:


public slots:

    void resizeEvent(QResizeEvent *e);

    void generateBPs();

private slots:

    void updateLineNumberAreaWidth(int newBlockCount);
    void updateLineNumberArea(QRectF /*rect_f*/);
    void updateLineNumberArea(int /*slider_pos*/);
    void updateLineNumberArea();

    void highlightCurrentLine();

    void copyBPS();

private:

    QWidget *lineNumberArea;

    FileManager *fmp;

    QString bps;
    QString bps2;

    QDialog *BPd;
    QFormLayout *ly;
    QFont *mfont;

    QTextEdit *BPdisp;
    QTextEdit *BPdisp2;
    QLabel *BPl;
    QPushButton *BPok;

protected:
    void mouseMoveEvent(QMouseEvent *e);

};
#endif // CODEEDITOR_H
