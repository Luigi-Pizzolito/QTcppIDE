#ifndef LINENUMBERAREA_H
#define LINENUMBERAREA_H

#include <QWidget>
#include <QtWidgets>

class LineNumberArea : public QWidget
{
    Q_OBJECT
public:
    LineNumberArea(QTextEdit *editor);
    QSize sizeHint() const;

protected:
    void paintEvent(QPaintEvent *event);

private:
    QTextEdit *codeEditor;

signals:

};

#endif // LINENUMBERAREA_H
