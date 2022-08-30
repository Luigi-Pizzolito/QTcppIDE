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

#include "mainwindow.h"
#include <QtWidgets>

#include "globaldefs.h"

#include "highlighter.h"
#include "codeeditor.h"
#include "console.h"

#include <QWidget>
#include <QListWidget>
#include <QGridLayout>

// #include <iostream>

//----- Main window startup
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    //setup view layout
    QWidget *widget = new QWidget(this);
    QGridLayout *layout = new QGridLayout(widget);

    setCentralWidget(widget);
    widget->setLayout(layout);

    // add editor
    setupEditor();
    layout->addWidget(editor, 0, 1, 1, 1);
    layout->setColumnStretch(1, 10);

    // add file manager
    setupFileMenu();
    fileList = new FileManager(this, editor);
    //todo: add button for new file and delete file here
    layout->addWidget(fileList, 0, 0, 1, 1);
    layout->setColumnStretch(0, 2);

    // add help
    setupHelpMenu();

    // add console
    setupConsole();
    layout->addWidget(console, 1, 0, 1, 2);
    layout->setRowStretch(0, 9);
    layout->setRowStretch(1, 3);

    // set window
    setWindowTitle(tr(APPHNAME));
    showDocs();
}

//----- Main window functions
void MainWindow::about()
{
    // show about message box with about markdown file.
    //todo: add about description here
    QString md = "";
    QFile file("../doc/about.md");
        if (file.open(QFile::ReadOnly | QFile::Text))
            md = file.readAll();
    QMessageBox aboutb;
    aboutb.setTextFormat(Qt::MarkdownText);
    aboutb.setText(md);
    aboutb.exec();
}

void MainWindow::newFile()
{
    //todo: make this actually create new file thru filemanager
    editor->clear();
}

void MainWindow::openFile(const QString &path)
{
    // open folder and update command generator
    fileList->openFolder(path);
    updateComms();
}

void MainWindow::showDocs() {
    if (!fileList->fileP.isNull()) // if a previous file was open, store it
        DocsPFile = fileList->fileP;

    // Open README Documentation
    QFile file("../README.md");
        if (file.open(QFile::ReadOnly | QFile::Text))
            editor->setMarkdown(file.readAll());
}

void MainWindow::showDocsRestore() {
    if (!DocsPFile.isNull()) //if there is a prior open document to restore
        fileList->openFolder(fileList->fileP);
}

void MainWindow::updateComms() {
    commG->update();
}

void MainWindow::keyPressEvent(QKeyEvent *ev) {
    if (ev->key() == Qt::Key::Key_Escape) {
        // restore if documentation was showing during keypress
        showDocsRestore();
    }
}

//----- Setup menu functions
//todo: add keyboard shortcuts to all menus
void MainWindow::setupFileMenu()
{
    QMenu *fileMenu = new QMenu(tr("&File"), this);
    menuBar()->addMenu(fileMenu);

    fileMenu->addAction(tr("&New"), QKeySequence::New,
                        this, &MainWindow::newFile);
    fileMenu->addSeparator();
    fileMenu->addAction(tr("&Open..."), QKeySequence::Open,
                        this, [this](){ openFile(); });
    fileMenu->addSeparator();
    //todo: implement save
    //todo: add aliases here for delete file
    fileMenu->addAction(tr("E&xit"), QKeySequence::Quit,
                        qApp, &QApplication::quit);
}

void MainWindow::setupHelpMenu()
{
    QMenu *helpMenu = new QMenu(tr("&Help"), this);
    menuBar()->addMenu(helpMenu);
    helpMenu->addAction(tr("&Documentation"), this, &MainWindow::showDocs);
    helpMenu->addSeparator();
    helpMenu->addAction(tr("&About"), this, &MainWindow::about);
    helpMenu->addAction(tr("About &Qt"), qApp, &QApplication::aboutQt);
}

//----- Setup functions
void MainWindow::setupEditor()
{
    // set font
    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(10);
    // create editor
    editor = new CodeEditor;
    editor->setFont(font);
    // set syntax highlighter
    highlighter = new Highlighter(editor->document());
}

void MainWindow::setupConsole() {
    console = new Console();

    QMenu *ConsoleMenu = new QMenu(tr("&Console"), this);
    menuBar()->addMenu(ConsoleMenu);
    ConsoleMenu->addAction(tr("Com&pile"), this, [this](){console->run(commG->compile(), fileList->dirP.absolutePath());});
    ConsoleMenu->addAction(tr("Ru&n"), this, [this](){console->run(commG->run(), fileList->dirP.absolutePath());});
    //todo: add wait for compile and then run
    //ConsoleMenu->addAction(tr("Compile and &Run"), this, [this](){console->compile();console->run();});
    ConsoleMenu->addSeparator();
    ConsoleMenu->addAction(tr("&Clear Console"), this, [this](){console->clearLog();});

    ConsoleMenu->addSeparator();
    configG = new ConfigGen(this);
    connect(configG, SIGNAL(finished(int)), this, SLOT(updateComms()));

    ConsoleMenu->addAction(tr("&Preferences"), this, [this](){configG->exec();});


    commG = new CommandGen(&fileList->dirP);
}