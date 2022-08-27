#ifndef CONSOLE_H
#define CONSOLE_H

#include <QPlainTextEdit>
#include <QObject>
#include <QWidget>

class Console : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit Console(QWidget *parent = 0);
};

#endif // CONSOLE_H
