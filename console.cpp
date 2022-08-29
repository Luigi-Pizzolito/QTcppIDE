#include "console.h"
#include <QtGui>

#include "globaldefs.h"

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
    append(APPHNAME " " IDEVER);

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
    append(APPHNAME " " IDEVER);
    ensureCursorVisible();
}


void Console::keyPressEvent(QKeyEvent *ev)
{
        prunner->takeInput(ev->text());
}

//todo: get these from configgen
//todo: check errors from starting proccess (wrong bin path, etc)
//todo: open IDE setup if not yet setup
void Console::run(QString comm, QString cwd) {
//    QStringList args;
//    args << ".." << "test";
//    prunner->run("/home/luigipizzolito/Desktop/stdecho/stdecho", args);
//    append(comm);

    QStringList args = comm.split(" ");
    QString bin = args[0];
    args.removeFirst();
    prunner->run(bin, args, cwd);
}
