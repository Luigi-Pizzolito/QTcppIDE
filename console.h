#ifndef CONSOLE_H
#define CONSOLE_H

#include <QObject>
#include <QWidget>

#include "procrunner.h"


class Console : public QTextEdit
{
    Q_OBJECT
public:
    explicit Console(QWidget *parent = 0);
public slots:
    void clearLog();
    void run();
private:
    QDebugStream *redirect;
    ProcRunner *prunner;
protected:
    void keyPressEvent(QKeyEvent *event);
//    void keyReleaseEvent(QKeyEvent *event);
};

#endif // CONSOLE_H
