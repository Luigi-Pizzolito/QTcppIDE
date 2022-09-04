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

#ifndef LINENUMBERAREA_H
#define LINENUMBERAREA_H

#include <QWidget>
#include <QtWidgets>

#include "filemanager.h"

class LineNumberArea : public QWidget
{
    Q_OBJECT
public:
    LineNumberArea(QTextEdit *editor);
    QSize sizeHint() const;

    void passFMP(FileManager *pfmp);

    int hoverBP=0;

protected:
    void paintEvent(QPaintEvent *event);

    void mousePressEvent(QMouseEvent *e);

    void mouseMoveEvent(QMouseEvent *e);

private:
    QTextEdit *codeEditor;

    FileManager *fmp;

signals:

};

#endif // LINENUMBERAREA_H
