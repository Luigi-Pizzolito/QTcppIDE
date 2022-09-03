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
    // call next proccess if doing batch jobs
    if (batchJob) {
        // run the next commands, if any and if previous was succesful
        if (batchOpts.first.size() > 0 && exitCode == 0 && exitStatus == QProcess::NormalExit) {
            runBatch(batchOpts.first, batchOpts.second);
        } else {
            // on fail or end of jobs, reset batch flag
            batchJob = false;
        }
    }
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

void Console::runBatch(QStringList comms, QString cwd) {
    // run command
    run(comms.front(), cwd);
    // store the rest of batch
    comms.pop_front();
    batchOpts = {comms, cwd};
    // callback is set above to call next command without ui blocking, set flag to do batch
    batchJob = true;
}

void Console::stop() {
    prunner->proc->kill();
}
