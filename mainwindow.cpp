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
#include "ui_search_replace.h"

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

    // add search menu
    setupSearchMenu(editor);

    // add console
    setupConsole();
    layout->addWidget(console, 1, 0, 1, 2);
    layout->setRowStretch(0, 9);
    layout->setRowStretch(1, 3);

    // add help
    setupHelpMenu();

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
    console->clearLog();
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
    helpMenu->addAction(tr("&Documentation"), QKeySequence(tr("Ctrl+h")), this, &MainWindow::showDocs);
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
    efont.setPointSize(9);
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
    ConsoleMenu->addAction(tr("Com&pile"), QKeySequence(tr("Ctrl+b")), this, [this](){console->run(commG->compile(), fileList->dirP.absolutePath());});
    ConsoleMenu->addAction(tr("Ru&n"), QKeySequence(tr("Ctrl+e")), this, [this](){console->run(commG->run(), fileList->dirP.absolutePath());});
    ConsoleMenu->addAction(tr("Compile and &Run"), QKeySequence(tr("Ctrl+r")), this, [this](){
        QStringList comms;
        comms << commG->compile();
        comms << commG->run();
        console->runBatch(comms, fileList->dirP.absolutePath());
    });
    ConsoleMenu->addSeparator();
    ConsoleMenu->addAction(tr("Stop Pro&cess"), QKeySequence(tr("Ctrl+Shift+c")), this, [this](){console->stop();});
    ConsoleMenu->addAction(tr("C&lear Console"), QKeySequence(tr("Ctrl+l")), this, [this](){console->clearLog();});

    ConsoleMenu->addSeparator();
    configG = new ConfigGen(this);
    connect(configG, SIGNAL(finished(int)), this, SLOT(updateComms()));

    ConsoleMenu->addAction(tr("&Preferences"), QKeySequence(tr("Ctrl+,")), this, [this](){configG->exec();});


    commG = new CommandGen(&fileList->dirP);
}



//------ Search and Replace Menu

void MainWindow::setupSearchMenu(QTextEdit *search_object)//当前文档页的指针
{
    QMenu *SearchMenu = new QMenu(tr("&Edit"), this);
    menuBar()->addMenu(SearchMenu);

    // edit menu items
    QAction *undo = new QAction(tr("&Undo"), SearchMenu);
    undo->setShortcut(QKeySequence::Undo);
    SearchMenu->addAction(undo);
    connect(undo,SIGNAL(triggered()),search_object,SLOT(undo()));
    QAction *redo = new QAction(tr("&Redo"), SearchMenu);
    redo->setShortcut(QKeySequence::Redo);
    SearchMenu->addAction(redo);
    connect(redo,SIGNAL(triggered()),search_object,SLOT(redo()));
    SearchMenu->addSeparator();
    QAction *cut = new QAction(tr("Cu&t"), SearchMenu);
    cut->setShortcut(QKeySequence::Cut);
    SearchMenu->addAction(cut);
    connect(cut,SIGNAL(triggered()),search_object,SLOT(cut()));
    QAction *copy = new QAction(tr("&Copy"), SearchMenu);
    copy->setShortcut(QKeySequence::Copy);
    SearchMenu->addAction(copy);
    connect(copy,SIGNAL(triggered()),search_object,SLOT(copy()));
    QAction *paste = new QAction(tr("&Paste"), SearchMenu);
    paste->setShortcut(QKeySequence::Paste);
    SearchMenu->addAction(paste);
    connect(paste,SIGNAL(triggered()),search_object,SLOT(paste()));
    SearchMenu->addSeparator();
    QAction *selectAll = new QAction(tr("Select &All"), SearchMenu);
    selectAll->setShortcut(QKeySequence::SelectAll);
    SearchMenu->addAction(selectAll);
    connect(selectAll,SIGNAL(triggered()),search_object,SLOT(selectAll()));
    SearchMenu->addSeparator();

    // search and replace menu items
    srMenu=new Search_Replace(search_object,this);
    SearchMenu->addAction(tr("&Search"), QKeySequence::Find, this, [=](){
        srMenu->setCurrentTab(1);
        srMenu->show();
        QTimer::singleShot(0, srMenu->ui->keywords, SLOT(setFocus()));
    });

    SearchMenu->addAction(tr("Search and &Replace"), QKeySequence(tr("Ctrl+Shift+f")), this, [=](){
        srMenu->setCurrentTab(2);
        srMenu->show();
        QTimer::singleShot(0, srMenu->ui->keywords_2, SLOT(setFocus()));
    });
}


//------ Right click editor menu for windows only
#ifdef _WIN32
void QWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::RightButton)
    {
        QPointF pos = event->globalPosition();

        QMenu *menu=new QMenu(this);

        QAction *undo = new QAction(menu);
        undo->setText("&Undo");
        menu->addAction(undo);
        connect(undo,SIGNAL(triggered()),this,SLOT(CMundo()));

        QAction *redo = new QAction(menu);
        redo->setText("&Redo");
        menu->addAction(redo);
        connect(redo,SIGNAL(triggered()),this,SLOT(CMredo()));

        QAction *cut = new QAction(menu);
        cut->setText("Cu&t");
        menu->addAction(cut);
        connect(cut,SIGNAL(triggered()),this,SLOT(CMcut()));

        QAction *copy = new QAction(menu);
        copy->setText("&Copy");
        menu->addAction(copy);
        connect(copy,SIGNAL(triggered()),this,SLOT(CMcopy()));

        QAction *paste = new QAction(menu);
        paste->setText("&Paste");
        menu->addAction(paste);
        connect(paste,SIGNAL(triggered()),this,SLOT(CMpaste()));

        QAction *selectAll = new QAction(menu);
        selectAll->setText("Select &All");
        menu->addAction(selectAll);
        connect(selectAll,SIGNAL(triggered()),this,SLOT(CMselectAll()));

        menu->move(pos.rx(),pos.ry());
        menu->show();

   }
}

void MainWindow::CMundo(){
    editor->undo();
}

void MainWindow::CMredo(){
    editor->redo();
}

void MainWindow::CMcut(){
    editor->cut();
}

void MainWindow::CMcopy(){
    editor->copy();
}

void MainWindow::CMpaste(){
    editor->paste();
}

void MainWindow::CMselectAll(){
    editor->selectAll();
}
#endif
