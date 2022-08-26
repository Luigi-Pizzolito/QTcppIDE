#include "miniconsole.h"

MiniConsole::MiniConsole(QWidget *parent) : QPlainTextEdit(parent)
{
    setPlainText("console goes here...");
    setMaximumHeight(200);
    setReadOnly(true);
}
