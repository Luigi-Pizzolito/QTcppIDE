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

 #include <iostream>

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
    fileList = new FileManager(this, editor, &showingDocs);
    layout->addWidget(fileList, 0, 0, 1, 1);
    layout->setColumnStretch(0, 2);

    // add edit menu
    setupEditMenu();

    // add search menu
    setupSearchMenu(editor);

    // add console
    setupConsole();
    layout->addWidget(console, 1, 0, 1, 2);
    layout->setRowStretch(0, 9);
    layout->setRowStretch(1, 3);

    // add view menu
    setupViewMenu();

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

void MainWindow::saveFile() {
    // check if (read-only) documentation is open
    if (showingDocs) {
        console->log("Please select and open a file before saving.");
    } else {
        //todo: save file here
        QFile file(fileList->fileP);
        if(file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream fstream(&file);
            fstream << editor->toPlainText();
            file.close();
            console->log("Saved file "+fileList->fileP);
        } else {
            console->err("Error saving file "+fileList->fileP);
        }
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
    QFile file(READMEDOCPATH);
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        editor->setMarkdown(file.readAll());
        showingDocs = true;
    }
}

void MainWindow::showDocsRestore() {
    if (!DocsPFile.isNull()) { //if there is a prior open document to restore
        openFile(fileList->fileP);
        showingDocs = false;
    }
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
    fileMenu->addAction(tr("&Save"), QKeySequence::Save, this, &MainWindow::saveFile);
    fileMenu->addSeparator();
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
    efont.setPointSize(10);
    // create editor
    editor = new CodeEditor;
    editor->setFont(efont);
    // set syntax highlighter
    highlighter = new Highlighter(editor->document());
}

void MainWindow::setupConsole() {
    console = new Console();

    QMenu *ConsoleMenu = new QMenu(tr("&Run"), this);
    menuBar()->addMenu(ConsoleMenu);
    ConsoleMenu->addAction(tr("Com&pile"), QKeySequence(tr("Ctrl+b")), this, [this](){console->run(commG->compile(), fileList->dirP.absolutePath());});
    ConsoleMenu->addAction(tr("Ru&n"), QKeySequence(tr("Ctrl+e")), this, [this](){console->run(commG->run(), fileList->dirP.absolutePath());});
    ConsoleMenu->addAction(tr("Compile and &Run"), QKeySequence(tr("Ctrl+r")), this, [this](){
        QStringList comms;
        comms << commG->compile();
        comms << commG->run();
        console->runBatch(comms, fileList->dirP.absolutePath());
    });
    ConsoleMenu->addAction(tr("Clea&n"), QKeySequence(tr("Ctrl+Alt+c")), this, [this]() {
        QDir bin(fileList->dirP.absolutePath()+"/bin");
        if (bin.exists()) {
            bin.removeRecursively();
            console->log("Cleaned project.");
        } else {
            console->log("No build files to clean.");
        }
    });
    ConsoleMenu->addSeparator();
    ConsoleMenu->addAction(tr("Stop/&Kill"), QKeySequence(tr("Ctrl+Shift+c")), this, [this](){console->stop();});
    ConsoleMenu->addAction(tr("C&lear Console"), QKeySequence(tr("Ctrl+l")), this, [this](){console->clearLog();});

    ConsoleMenu->addSeparator();
    configG = new ConfigGen(this);
    connect(configG, SIGNAL(finished(int)), this, SLOT(updateComms()));

    ConsoleMenu->addAction(tr("&Settings"), QKeySequence(tr("Ctrl+,")), this, [this](){configG->exec();});


    commG = new CommandGen(&fileList->dirP);
}

void MainWindow::setupEditMenu() {
    // edit menu items
    QMenu *EditMenu = new QMenu(tr("&Edit"), this);
    menuBar()->addMenu(EditMenu);
    QAction *undo = new QAction(tr("&Undo"), this);
    undo->setShortcut(QKeySequence::Undo);
    EditMenu->addAction(undo);
    connect(undo,SIGNAL(triggered()),editor,SLOT(undo()));
    QAction *redo = new QAction(tr("&Redo"), this);
    redo->setShortcut(QKeySequence::Redo);
    EditMenu->addAction(redo);
    connect(redo,SIGNAL(triggered()),editor,SLOT(redo()));
    EditMenu->addSeparator();
    QAction *cut = new QAction(tr("Cu&t"), this);
    cut->setShortcut(QKeySequence::Cut);
    EditMenu->addAction(cut);
    connect(cut,SIGNAL(triggered()),editor,SLOT(cut()));
    QAction *copy = new QAction(tr("&Copy"), this);
    copy->setShortcut(QKeySequence::Copy);
    EditMenu->addAction(copy);
    connect(copy,SIGNAL(triggered()),editor,SLOT(copy()));
    QAction *paste = new QAction(tr("&Paste"), this);
    paste->setShortcut(QKeySequence::Paste);
    EditMenu->addAction(paste);
    connect(paste,SIGNAL(triggered()),editor,SLOT(paste()));
    EditMenu->addSeparator();
    QAction *selectAll = new QAction(tr("Select &All"), this);
    selectAll->setShortcut(QKeySequence::SelectAll);
    EditMenu->addAction(selectAll);
    connect(selectAll,SIGNAL(triggered()),editor,SLOT(selectAll()));
    EditMenu->addSeparator();
}

void MainWindow::setupViewMenu() {
    QMenu *ViewMenu = new QMenu(tr("&View"), this);
    menuBar()->addMenu(ViewMenu);
    ViewMenu->addAction(tr("Zoom &In"), QKeySequence::ZoomIn, this, [=](){
        editor->zoomIn();
    });
    ViewMenu->addAction(tr("Zoom &Out"), QKeySequence::ZoomOut, this, [=](){
        editor->zoomOut();
    });
    ViewMenu->addSeparator();
    ViewMenu->addAction(tr("&Goto Line"), QKeySequence(tr("Ctrl+g")), this, [=](){
        int line = QInputDialog::getInt(this, tr("Goto Line"), tr("Line Number:"), 1, 1);
        if (line > 0) {
            QTextCursor csr = editor->textCursor();
            csr.movePosition(QTextCursor::StartOfLine);
            csr.movePosition(QTextCursor::Start);
            csr.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, line-1);
            editor->setTextCursor(csr);
        }
    });
}


//------ Search and Replace Menu

void MainWindow::setupSearchMenu(QTextEdit *search_object)//当前文档页的指针
{
    QMenu *SearchMenu = new QMenu(tr("&Search"), this);
    menuBar()->addMenu(SearchMenu);

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
