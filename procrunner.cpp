/*
 * Process Runner Class
 * 
 * C++ IDE by Luigi Pizzolito
 *            Zhang Ruiqing
 *            Ruan Zihang
 *            Lin Zhenmin
 *            Yang Zhaoyi
 * For 1703-ECE Class by Nie Qing and Wu Hao
 * At Beijing Insitute of Techology
*/

#include "procrunner.h"

#include "globaldefs.h"

#ifdef _WIN32
    #include "Windows.h"
#endif

#include <iostream>

ProcRunner::ProcRunner(QWidget *parent, QTextEdit *text_edit) : parentw(parent),tedit(text_edit)
{
    // Create process runner
    proc = new QProcess(parentw);
}

void ProcRunner::run(QString program, QStringList arguments, QString cwd) {
    // set options and run process
    //todo: add error handling
    //todo: check errors from starting proccess (wrong bin path, etc)
    proc->setProcessChannelMode(QProcess::MergedChannels); // merge stdout and stderr
    proc->setWorkingDirectory(cwd);

#ifdef _WIN32
    proc->setCreateProcessArgumentsModifier([] (QProcess::CreateProcessArguments *args)
        {
            args->flags |= CREATE_NEW_CONSOLE;
            args->startupInfo->dwFlags &= ~STARTF_USESTDHANDLES;
        });
#endif

    proc->start(program, arguments);

    std::cout << program.toStdString() << " " << arguments.join(" ").toStdString() << "\n";

    proc->waitForStarted();
}

void ProcRunner::procStarted() {
    // started message to console
    tedit->setTextColor(CONSOLETCOLOR);
    QString strc = "打开进程 ";
    QString str = "Started process ";
    strc+=proc->program();
    str+=proc->program();
    strc+=" ";
    str+=" ";
    strc+=proc->arguments().join(" ");
    str+=proc->arguments().join(" ");
    strc+="\n";
    str+="\n";
    tedit->append(strc);
    tedit->ensureCursorVisible();
    tedit->append(str);
    tedit->ensureCursorVisible();
}

void ProcRunner::procFinished(int exitCode,QProcess::ExitStatus exitStatus) {
    // print process finished message to console
    tedit->setTextColor(CONSOLETCOLOR);
    QString str;
    QString strc;
    if (exitStatus == QProcess::CrashExit) {
        tedit->setTextColor(CONSOLEERRCOLOR);
        str+="Process crashed with exit code ";
        strc+="进程崩溃，退出码为";
    } else {
        str+="Process finished with exit code ";
        strc+="进程结束，退出码为";
    }
    str+=QString::number(exitCode);
    strc+=QString::number(exitCode);
    if (exitCode != 0) {
        tedit->setTextColor(CONSOLEERRCOLOR);
        str+="\n";
        str+=proc->errorString();
    }
    tedit->append(strc);
    tedit->ensureCursorVisible();
    tedit->append(str);
    tedit->ensureCursorVisible();
}


void ProcRunner::takeOutput() {
    // forward process output to console
    if (proc->state() == QProcess::Running) {
        tedit->moveCursor(QTextCursor::End);
        tedit->setTextColor(Qt::white);
        tedit->insertPlainText(proc->readAllStandardOutput().toStdString().c_str());
        tedit->moveCursor(QTextCursor::End);
    }
}

void ProcRunner::takeInput(QString input) {
    // forward console input to process
    if (proc->state() == QProcess::Running) {
        proc->write(input.toUtf8());

        tedit->moveCursor(QTextCursor::End);
        tedit->setTextColor(Qt::white);
        tedit->insertPlainText(input.toStdString().c_str());
        tedit->moveCursor(QTextCursor::End);
    }
}
