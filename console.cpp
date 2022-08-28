#include "console.h"
#include <QtGui>

#define IDEVER "C++ IDE V1.0"

Console::Console(QWidget *parent) : QTextEdit(parent)
{
//    setMaximumHeight(200);
    setMinimumHeight(200);
    setReadOnly(true);
    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(10);
    setFont(font);

    setTextColor(QColor(Qt::blue).lighter(160));
    append(IDEVER);

    prunner = new ProcRunner(this, this);
    connect(prunner->proc, SIGNAL(readyReadStandardOutput()), this, SLOT(processProcOutput()));  // connect process signals with your code
    connect(prunner->proc, SIGNAL(readyReadStandardError()), this, SLOT(processProcOutput()));  // same here
    connect(prunner->proc, SIGNAL(started()), this, SLOT(processProcStarted()));
    connect(prunner->proc, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(processProcFinished(int,QProcess::ExitStatus)));

}

void Console::processProcOutput() {
    prunner->takeOutput();
}

void Console::processProcStarted() {
    prunner->procStarted();
}

void Console::processProcFinished(int exitCode, QProcess::ExitStatus exitStatus) {
    prunner->procFinished(exitCode, exitStatus);
}

//Console::~Console() {
//    delete procrunner;
//    delete redirect;
//}


void Console::clearLog() {
    clear();
    setTextColor(QColor(Qt::blue).lighter(160));
    append(IDEVER);
    ensureCursorVisible();
}


void Console::keyPressEvent(QKeyEvent *ev)
{
        prunner->takeInput(ev->text());
}

//todo: get these from configgen
//todo: check errors from starting proccess (wrong bin path, etc)
void Console::run() {
    QStringList args;
    args << ".." << "test";
    prunner->run("/home/luigipizzolito/Desktop/stdecho/stdecho", args);
}

void Console::compile() {
    QStringList args;
    args << "stdecho.cpp" << "-Wall" << "--verbose" << "-o" << "stdecho";
    prunner->proc->setWorkingDirectory("/home/luigipizzolito/Desktop/stdecho");
    prunner->run("g++", args);
}
