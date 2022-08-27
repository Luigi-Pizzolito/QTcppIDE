#include "console.h"
#include <QtGui>

Console::Console(QWidget *parent) : QPlainTextEdit(parent)
{
    setPlainText("console goes here...");
    setMaximumHeight(200);
    setReadOnly(true);

}
