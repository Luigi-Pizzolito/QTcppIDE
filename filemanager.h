/*
 * File Manager Class
 * 
 * C++ IDE by Luigi Pizzolito
 *            Zhang Ruiqing
 *            Ruan Zihang
 *            Lin Zhenmin
 *            Yang Zhaoyi
 * For 1703-ECE Class by Nie Qing and Wu Hao
 * At Beijing Insitute of Techology
*/

#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QSettings>
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
    QSettings *settings;
    QTextEdit *editor;
    void loadFile(QString fileName);
};

#endif // FILEMANAGER_H
