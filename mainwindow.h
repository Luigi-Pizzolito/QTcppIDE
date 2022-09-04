/*
 * Main Window Class
 * 
 * C++ IDE by Luigi Pizzolito
 *            Zhang Ruiqing
 *            Ruan Zihang
 *            Lin Zhenmin
 *            Yang Zhaoyi
 * For 1703-ECE Class by Nie Qing and Wu Hao
 * At Beijing Insitute of Techology
 *
 * This file includes code that is part of the examples of the Qt Toolkit.
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "highlighter.h"

#include "console.h"
#include "filemanager.h"
#include "configgen.h"
#include "commandgen.h"
#include "search_replace.h"
#include "statusbar.h"

#include <QMainWindow>
#include <QSettings>
#include <QListWidgetItem>

QT_BEGIN_NAMESPACE
class QTextEdit;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

public slots:
    // main window functions
    void about();
    void showDocs();
    void showDocsRestore();
    void newFile();
    void newFolder();
    void openFile(const QString &path = QString());
    void deleteFile();

    // for right click menu on windows only
    #ifdef _WIN32
    void CMundo();
    void CMredo();
    void CMcut();
    void CMcopy();
    void CMpaste();
    void CMselectAll();
    #endif


private:
    // settings
    QSettings *settings;
    // setup functions
    void setupEditor();
    void setupFileMenu();
    void setupHelpMenu();
    void setupConsole();
    void setupSearchMenu(QTextEdit *search_object);
    void setupEditMenu();
    void setupViewMenu();
    // code editor
    QFont efont;
    QTextEdit *editor;
    Highlighter *highlighter;
    // other GUI elements
    StatusBar *barStatus;
    Console *console;
    ConfigGen *configG;
    CommandGen *commG;
    FileManager *fileList;
    Search_Replace *srMenu;
    // last opened file flag
    QString DocsPFile;
    // showing documentation flag (to implement readonly docs)
    bool showingDocs = true;
private slots:
    void updateComms();

protected:
    // global keypress handling
    void keyPressEvent(QKeyEvent *event);
};

#endif // MAINWINDOW_H
