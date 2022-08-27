#ifndef QTEXTEDITLINESHIGHLIGHTED_H
#define QTEXTEDITLINESHIGHLIGHTED_H

#include <QTextEdit>
#include <QWidget>

class QTextEditLinesHighlighted : public QTextEdit
{
    Q_OBJECT

public:

    explicit QTextEditLinesHighlighted(QWidget *parent = 0);

    int getFirstVisibleBlockId();
    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();

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
#endif // QTEXTEDITLINESHIGHLIGHTED_H
