#ifndef MINICONSOLE_H
#define MINICONSOLE_H

#include <QPlainTextEdit>
#include <QObject>
#include <QWidget>

class MiniConsole : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit MiniConsole(QWidget *parent = 0);
};

#endif // MINICONSOLE_H
