/*
 * Console Class
 * 
 * C++ IDE by Luigi Pizzolito
 *            Zhang Ruiqing
 *            Ruan Zihang
 *            Lin Zhenmin
 *            Yang Zhaoyi
 * For 1703-ECE Class by Nie Qing and Wu Hao
 * At Beijing Insitute of Techology
*/

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
    void run(QString comm, QString cwd);
    void processProcOutput();
    void processProcStarted();
    void processProcFinished(int exitCode, QProcess::ExitStatus exitStatus);
private:
    ProcRunner *prunner;
protected:
    void keyPressEvent(QKeyEvent *event);
};

#endif // CONSOLE_H
