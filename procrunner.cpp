#include "procrunner.h"

ProcRunner::ProcRunner(QWidget *parent, QTextEdit *text_edit) : parentw(parent),tedit(text_edit)
{
    proc = new QProcess(parentw);
}

void ProcRunner::run(QString program, QStringList arguments, QString cwd) {
    proc->setProcessChannelMode(QProcess::MergedChannels); // merge stdout and stderr
    proc->setWorkingDirectory(cwd);
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
    tedit->ensureCursorVisible();
}

void ProcRunner::procFinished(int exitCode, QProcess::ExitStatus exitStatus) {
    tedit->setTextColor(QColor(Qt::blue).lighter(160));
    QString str;
    if (exitStatus == QProcess::CrashExit) {
        str+="Process crashed with exit code ";
    } else {
        str+="Process finished with exit code ";
    }
    str+=QString::number(exitCode);
    tedit->append(str);
    tedit->ensureCursorVisible();
}


void ProcRunner::takeOutput() {
    if (proc->state() == QProcess::Running) {
        tedit->moveCursor(QTextCursor::End);
        tedit->setTextColor(Qt::white);
        tedit->insertPlainText(proc->readAllStandardOutput().toStdString().c_str());
        tedit->moveCursor(QTextCursor::End);
    }
}

void ProcRunner::takeInput(QString input) {
    if (proc->state() == QProcess::Running) {
        proc->write(input.toUtf8());

        tedit->moveCursor(QTextCursor::End);
        tedit->setTextColor(Qt::white);
        tedit->insertPlainText(input.toStdString().c_str());
        tedit->moveCursor(QTextCursor::End);
    }
}
