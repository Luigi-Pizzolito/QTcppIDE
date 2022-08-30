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
    // load settings
    settings = new QSettings(COMPANY, APPNAME);

    // setup view layout
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
    // open last opened file
    QString lastfile = settings->value("LastOpenedFile").toString();
    if (!lastfile.isNull() && !lastfile.isEmpty())
           openFile(lastfile);
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
    fileList->createNewFileRequest();
}

void MainWindow::newFolder() {
    // prompt folder name
    QString dirp = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "/", QFileDialog::ShowDirsOnly);
    if (!dirp.isNull()) {
        // create new folder
        QDir dir;
        dir.mkpath(dirp);
        // switch to new folder
        fileList->dirP.setPath(dirp);
        // prompt create new main file
        fileList->setFileMain();
        fileList->createNewFileRequest();

    }
}

void MainWindow::openFile(const QString &path)
{
    // open folder
    fileList->openFolder(path);
    updateComms();

    // save to last opened file settings and update command generator
    if (!path.isNull()) {
        settings->setValue("LastOpenedFile", path);
    }
}

void MainWindow::deleteFile() {
    if (QMessageBox(QMessageBox::Information, "Delete "+QFileInfo(fileList->fileP).fileName(), "Are you sure you want to delete "+QFileInfo(fileList->fileP).fileName()+"?", QMessageBox::Yes|QMessageBox::No).exec() == QMessageBox::Yes) {
        fileList->dirP.remove(fileList->fileP);
        fileList->dirP.setPath(fileList->dirP.absolutePath());
        fileList->clear();
        if (fileList->dirP.isEmpty()) {
            // if there are n more files prompt to create
            editor->clear();
            fileList->setFileMain();
            fileList->createNewFileRequest();
        } else {
            // otherwise open first file
            QString ffile = fileList->dirP.absolutePath()+"/"+fileList->dirP.entryList().at(0);
            openFile(ffile);
        }
    }
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
        openFile(fileList->fileP);
}

void MainWindow::updateComms() {
    commG->update(&fileList->dirP);
}

void MainWindow::keyPressEvent(QKeyEvent *ev) {
    if (ev->key() == Qt::Key::Key_Escape) {
        // restore if documentation was showing during keypress
        showDocsRestore();
        editor->setFont(efont);
    }
}

//----- Setup menu functions
//todo: add keyboard shortcuts to all menus
void MainWindow::setupFileMenu()
{
    QMenu *fileMenu = new QMenu(tr("&File"), this);
    menuBar()->addMenu(fileMenu);

    fileMenu->addAction(tr("&New File"), QKeySequence::New,
                        this, &MainWindow::newFile);
    fileMenu->addAction(tr("&New Project"), QKeySequence(tr("Ctrl+Shift+n")),
                        this, &MainWindow::newFolder);
    fileMenu->addSeparator();
    fileMenu->addAction(tr("&Open"), QKeySequence::Open,
                        this, [this](){ openFile(); });
    fileMenu->addSeparator();
    //todo: implement save
    //todo: add aliases here for delete file
    fileMenu->addAction(tr("&Delete File"), QKeySequence(tr("Ctrl+Shift+w")), this, &MainWindow::deleteFile);
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
    efont.setFamily("Courier");
    efont.setFixedPitch(true);
    efont.setPointSize(10);
    // create editor
    editor = new CodeEditor;
    editor->setFont(efont);
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
