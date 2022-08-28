#include "procrunner.h"

ProcRunner::ProcRunner(QWidget *parent, QTextEdit *text_edit)
{
    parentw = parent;
    tedit = text_edit;
    proc = new QProcess(parentw);
}

void ProcRunner::run(QString program, QStringList arguments) {
    proc->setProcessChannelMode(QProcess::MergedChannels); // merge stdout and stderr
//    proc->setProcessChannelMode(QProcess::ForwardedChannels);

    proc->start(program, arguments);
    proc->waitForStarted();
}

void ProcRunner::procStarted() {
    tedit->setTextColor(QColor(Qt::blue).lighter(160));
    QString str = "Started process ";
    str+=proc->program();
    str+=" ";
    str+=proc->arguments().join(" ");
    str+="\n";
    tedit->append(str);
}

void ProcRunner::procFinished(int exitCode, QProcess::ExitStatus exitStatus) {
    tedit->setTextColor(QColor(Qt::blue).lighter(160));
    QString str = "Process finished with exit code ";
    str+= QString::number(exitCode);
    tedit->append(str);
}


void ProcRunner::takeOutput() {
//    std::cout << proc->readAllStandardOutput();
//    printf("%s", proc->readAllStandardOutput().toStdString().c_str());
//    tedit->append(proc->readAllStandardOutput().toStdString().c_str());
    if (proc->state() == QProcess::Running) {
        tedit->moveCursor(QTextCursor::End);
        tedit->setTextColor(Qt::white);
        tedit->insertPlainText(proc->readAllStandardOutput().toStdString().c_str());
        tedit->moveCursor(QTextCursor::End);
    }
}

void ProcRunner::takeInput(QString input) {
//    proc->write(input.toStdString().c_str());
    if (proc->state() == QProcess::Running) {
        proc->write(input.toUtf8());

        tedit->moveCursor(QTextCursor::End);
        tedit->setTextColor(Qt::white);
        tedit->insertPlainText(input.toStdString().c_str());
        tedit->moveCursor(QTextCursor::End);
    }
//    std::cout << input.toStdString();
}
