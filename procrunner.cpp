#include "procrunner.h"

ProcRunner::ProcRunner(QWidget *parent)
{
    parentw = parent;
}

void ProcRunner::run(QString program, QStringList arguments) {
    proc = new QProcess(parentw);
    proc->start(program, arguments);
    procRunning = true;
}
