#include "console.h"
#include <QtGui>

#define IDEVER "C++ IDE V1.0"

Console::Console(QWidget *parent) : QTextEdit(parent)
{
    setPlainText(IDEVER);
    setMaximumHeight(200);
    setReadOnly(true);

    prunner = new ProcRunner();
    redirect = new QDebugStream(std::cout, this);
}

//Console::~Console() {
//    delete procrunner;
//    delete redirect;
//}


void Console::clearLog() {
    clear();
    setPlainText(IDEVER);
}


void Console::keyPressEvent(QKeyEvent *ev)
{
    if (prunner->procRunning) {
        append("You Pressed Key " + ev->text());
    }

}

//void Console::keyReleaseEvent(QKeyEvent *ev)
//{
////    append("You Release Key " + ev->text());
//}


void Console::run() {
//    prunner->procRunning = true;
    QStringList args;
    args << "hi";
    prunner->run("notify-send", args);
    std::cout << "Running";
}
