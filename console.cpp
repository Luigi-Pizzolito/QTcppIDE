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

#include "console.h"
#include <QtGui>

#include "globaldefs.h"

Console::Console(QWidget *parent) : QTextEdit(parent)
{
    // Setup console widget
    setMinimumHeight(200);
    setReadOnly(true);
    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(10);
    setFont(font);
    // Print starter line
    setTextColor(QColor(Qt::blue).lighter(160));
    append(APPHNAME " " IDEVER);
    // Setup process runner
    prunner = new ProcRunner(this, this);
    connect(prunner->proc, SIGNAL(readyReadStandardOutput()), this, SLOT(processProcOutput()));  // connect process signals with your code
    connect(prunner->proc, SIGNAL(readyReadStandardError()), this, SLOT(processProcOutput()));  // same here
    connect(prunner->proc, SIGNAL(started()), this, SLOT(processProcStarted()));                // connect process start/finish
    connect(prunner->proc, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(processProcFinished(int,QProcess::ExitStatus)));

}

// Forwards events to process runner

void Console::processProcOutput() {
    prunner->takeOutput();
}

void Console::processProcStarted() {
    prunner->procStarted();
}

void Console::processProcFinished(int exitCode, QProcess::ExitStatus exitStatus) {
    prunner->procFinished(exitCode, exitStatus);
}

void Console::clearLog() {
    // clear console
    clear();
    setTextColor(QColor(Qt::blue).lighter(160));
    append(APPHNAME " " IDEVER);
    ensureCursorVisible();
}

// Forward keypresses to process
void Console::keyPressEvent(QKeyEvent *ev)
{
    prunner->takeInput(ev->text());
}

// start process
void Console::run(QString comm, QString cwd) {
    QStringList args = comm.split(" ");
    QString bin = args[0];
    args.removeFirst();
    prunner->run(bin, args, cwd);
}
