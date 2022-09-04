#ifndef STATUSBAR_H
#define STATUSBAR_H

#include <QStatusBar>
#include <QObject>
#include <QWidget>

#include <QDir>
#include <QTextEdit>
#include <QSettings>

#include <QLabel>

class StatusBar : public QStatusBar
{
    Q_OBJECT
public:
    explicit StatusBar(QWidget *parent=0, QDir *folder=0, QString *file=0, QTextEdit *editor=0);
public slots:
    void update();
private:
    // initialisers
    QWidget *parent;
    QDir *folder;
    QString *file;
    QTextEdit *editor;
    QSettings *settings;
    // elements
    QLabel *folderE;
    QLabel *fileE;
    QLabel *cursorE;
    QLabel *compilerE;
};

#endif // STATUSBAR_H
