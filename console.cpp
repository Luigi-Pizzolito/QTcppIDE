#include "console.h"
#include <QtGui>

#define IDEVER "C++ IDE V1.0"

Console::Console(QWidget *parent) : QTextEdit(parent)
{
    setPlainText(IDEVER);
    setMaximumHeight(200);
    setReadOnly(true);


    redirect = new QDebugStream(std::cout, this);
}

//Console::~Console() {
//    delete redirect;
//}


void Console::clearLog() {
    clear();
    setPlainText(IDEVER);
}


void Console::keyPressEvent(QKeyEvent *ev)
{
    if (procRunning) {
        append("You Pressed Key " + ev->text());
    }

}

//void Console::keyReleaseEvent(QKeyEvent *ev)
//{
////    append("You Release Key " + ev->text());
//}


void Console::run() {
    procRunning = true;
    std::cout << "Running";
}
