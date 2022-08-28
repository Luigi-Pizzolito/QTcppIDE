#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QListWidget>
#include <QObject>
#include <QWidget>
#include <QTextEdit>
#include <QDir>

class FileManager : public QListWidget
{
    Q_OBJECT
public:
    explicit FileManager(QWidget *parent = 0, QTextEdit *editor=0);
    QDir dirP;
    QString fileP;
public slots:
    void openFolder(QString fileName);
    void openFile(QListWidgetItem* item);
private:
    QWidget *parentw;
    QTextEdit *editor;
    void loadFile(QString fileName);
};

#endif // FILEMANAGER_H
