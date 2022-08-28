#include "console.h"
#include <QtGui>

#define IDEVER "C++ IDE V1.0"

Console::Console(QWidget *parent) : QTextEdit(parent)
{
    setTextColor(QColor(Qt::blue).lighter(160));
    append(IDEVER);

    setMaximumHeight(500);
    setReadOnly(true);
//    setCenterOnScroll(true);

    prunner = new ProcRunner(this, this);
    connect(prunner->proc, SIGNAL(readyReadStandardOutput()), this, SLOT(processProcOutput()));  // connect process signals with your code
    connect(prunner->proc, SIGNAL(readyReadStandardError()), this, SLOT(processProcOutput()));  // same here
    connect(prunner->proc, SIGNAL(started()), this, SLOT(processProcStarted()));
    connect(prunner->proc, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(processProcFinished(int,QProcess::ExitStatus)));

//    redirect = new QDebugStream(std::cout, this);
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
//        append("You Pressed Key " + ev->text());
        prunner->takeInput(ev->text());

}

//void Console::keyReleaseEvent(QKeyEvent *ev)
//{
////    append("You Release Key " + ev->text());
//}


void Console::run() {
//    prunner->procRunning = true;
    QStringList args;
//    args << "$(</dev/stdin)";
    args << ".." << "test";
    prunner->run("/home/luigipizzolito/Desktop/stdecho/stdecho", args);

//    std::cout << "Running";
}

void Console::compile() {
    QStringList args;
    args << "stdecho.cpp" << "-Wall" << "--verbose" << "-o" << "stdecho";
    prunner->proc->setWorkingDirectory("/home/luigipizzolito/Desktop/stdecho");
    prunner->run("g++", args);
}
