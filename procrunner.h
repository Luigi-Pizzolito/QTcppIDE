#ifndef PROCRUNNER_H
#define PROCRUNNER_H

#include <QWidget>
#include <QTextEdit>
#include <QProcess>

class ProcRunner
{
public:
    explicit ProcRunner(QWidget *parent = 0, QTextEdit *text_edit = 0);
    QProcess *proc;
public slots:
    void run(QString program, QStringList arguments, QString cwd);
    void takeInput(QString key);
    void takeOutput();
    void procStarted();
    void procFinished(int exitCode, QProcess::ExitStatus exitStatus);
private:
    QWidget *parentw;
    QTextEdit *tedit;
};


#endif // PROCRUNNER_H
