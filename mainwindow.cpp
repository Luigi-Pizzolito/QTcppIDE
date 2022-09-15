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

    // instantiate console
    console = new Console();

    // add file manager
    setupFileMenu();
    fileList = new FileManager(this, editor, &showingDocs, console);
    layout->addWidget(fileList, 0, 0, 1, 1);
    layout->setColumnStretch(0, 2);
    editor->passFMP(fileList);

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

    // add bottom status bar
    barStatus = new StatusBar(this, &fileList->dirP, &fileList->fileP, editor);
    setStatusBar(barStatus);
    fileList->passStatus(barStatus);
    // connect updates
    connect(editor, SIGNAL(textChanged()), barStatus, SLOT(update()));
    connect(editor, SIGNAL(cursorPositionChanged()), barStatus, SLOT(update()));
    connect(configG->okbtn, SIGNAL(clicked(bool)), barStatus, SLOT(update()));

    // set window
    setWindowTitle(tr(APPHNAME));
    // open last opened file
    QString lastfile = settings->value("LastOpenedFile").toString();
    if (!lastfile.isNull() && !lastfile.isEmpty() && QFileInfo(lastfile).exists()) {
        // if last opened file is saved, open it
        openFile(lastfile);
    } else {
        //otherwise show warning to open/create project first! AVOID mistake of null file
        console->setTextColor(QColor(Qt::green).darker(160));
        console->append("Please open a folder or create a new project to get started!");
        console->ensureCursorVisible();
    }
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
    fileList->saveFile(true); //prompt to save file before loosing work
    // prompt folder name
    QString dirp = QFileDialog::getExistingDirectory(this, tr("Open Directory 打开目录"), "/", QFileDialog::ShowDirsOnly);
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
    bool pfileexist = QFile(fileList->fileP).exists();
    // open folder
    bool wasShowingDocs = showingDocs;
    editor->setReadOnly(false);
    fileList->openFolder(path);
    barStatus->log("Opened file 打开文件: "+QFileInfo(path).fileName());
    // and update command generator
    updateComms();

    // save to last opened file settings and prompt to save previous file, if it was not deleted and if it is not readme
    if (!path.isNull() && pfileexist) {
        if (!wasShowingDocs) fileList->saveFile(true);
        settings->setValue("LastOpenedFile", path);
        editor->setReadOnly(false);
    }
}

void MainWindow::deleteFile() {
    if (QMessageBox(QMessageBox::Information, "Delete 删除 "+QFileInfo(fileList->fileP).fileName(), "Are you sure you want to delete "+QFileInfo(fileList->fileP).fileName()+"?", QMessageBox::Yes|QMessageBox::No).exec() == QMessageBox::Yes) {
        fileList->dirP.remove(fileList->fileP);
        QString deletedF = QFileInfo(fileList->fileP).fileName();
        fileList->dirP.setPath(fileList->dirP.absolutePath());
        fileList->clear();
        barStatus->log("Deleted file 删除文件 "+deletedF);
        if (fileList->dirP.isEmpty()) {
            // if there are n more files prompt to create
            editor->clear();
            fileList->setFileMain();
            fileList->createNewFileRequest();
        } else {
            // otherwise open first file
            QString ffile = fileList->dirP.absolutePath()+"/"+fileList->dirP.entryList().at(0);
            openFile(ffile);
            barStatus->log("Deleted file 删除文件 "+deletedF);
        }
    }
}

void MainWindow::showDocs() {
    if (!fileList->fileP.isNull()) { // if a previous file was open, save and then store it
        fileList->saveFile();
        DocsPFile = fileList->fileP;
    }

    // Open README Documentation
    QFile file(READMEDOCPATH);
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        editor->setMarkdown(file.readAll());
        showingDocs = true;
        editor->setReadOnly(true);
        // move cursor to line to, to avoid highlihgting title big font glitch/bug
        QTextCursor csr = editor->textCursor();
        csr.movePosition(QTextCursor::StartOfLine);
        csr.movePosition(QTextCursor::Start);
        csr.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, 1);
        editor->setTextCursor(csr);
    }
}

void MainWindow::showDocsRestore() {
    if (!DocsPFile.isNull()) { //if there is a prior open document to restore
        editor->setReadOnly(false);
        openFile(fileList->fileP);
        showingDocs = false;
    }
}

void MainWindow::updateComms() {
    commG->update(&fileList->dirP);
    console->clearLog();
}

void MainWindow::keyPressEvent(QKeyEvent *ev) {
    if (ev->key() == Qt::Key::Key_Escape && showingDocs) {
        // move cursor to line to, to avoid highlihgting title big font glitch/bug
        QTextCursor csr = editor->textCursor();
        csr.movePosition(QTextCursor::StartOfLine);
        csr.movePosition(QTextCursor::Start);
        csr.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, 1);
        editor->setTextCursor(csr);

        // restore if documentation was showing during keypress
        showDocsRestore();
        editor->setFont(efont);
    }
}

//----- Setup menu functions
void MainWindow::setupFileMenu()
{
    QMenu *fileMenu = new QMenu(tr("&File 文件"), this);
    menuBar()->addMenu(fileMenu);

    fileMenu->addAction(tr("&New File 新建"), QKeySequence::New,
                        this, &MainWindow::newFile);
    fileMenu->addAction(tr("&New Project 新项目"), QKeySequence(tr("Ctrl+Shift+n")),
                        this, &MainWindow::newFolder);
    fileMenu->addSeparator();
    fileMenu->addAction(tr("&Open 打开"), QKeySequence::Open,
                        this, [this](){ openFile(); });
    fileMenu->addSeparator();
    //todo: implement save
    fileMenu->addAction(tr("&Save 保存"), QKeySequence::Save, this, [=](){fileList->saveFile(false);});
    fileMenu->addSeparator();
    fileMenu->addAction(tr("&Delete File 删除建"), QKeySequence(tr("Ctrl+Shift+w")), this, &MainWindow::deleteFile);
    fileMenu->addAction(tr("E&xit 退出"), QKeySequence::Quit,
                        qApp, &QApplication::quit);
}

void MainWindow::setupHelpMenu()
{
    QMenu *helpMenu = new QMenu(tr("&Help 帮助"), this);
    menuBar()->addMenu(helpMenu);
    helpMenu->addAction(tr("&Documentation 文档"), QKeySequence(tr("Ctrl+h")), this, &MainWindow::showDocs);
    helpMenu->addSeparator();
    helpMenu->addAction(tr("&About 关于"), this, &MainWindow::about);
//    helpMenu->addAction(tr("About &Qt 关于Qt"), qApp, &QApplication::aboutQt);
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
    QMenu *ConsoleMenu = new QMenu(tr("&Run 运行"), this);
    menuBar()->addMenu(ConsoleMenu);
    ConsoleMenu->addAction(tr("Com&pile 编译"), QKeySequence(tr("Ctrl+b")), this, [this](){
        console->run(commG->compile(), fileList->dirP.absolutePath());
        barStatus->log("Compiling project... 编译中...");
    });
    ConsoleMenu->addAction(tr("Ru&n 运行"), QKeySequence(tr("Ctrl+e")), this, [this](){
        console->run(commG->run(), fileList->dirP.absolutePath());
        barStatus->log("Running project... 运行中...");
    });
    ConsoleMenu->addAction(tr("Compile and &Run 编译并运行"), QKeySequence(tr("Ctrl+r")), this, [this](){
        QStringList comms;
        comms << commG->compile();
        comms << commG->run();
        console->runBatch(comms, fileList->dirP.absolutePath());
        barStatus->log("Compiling and running project... 编译并运行中...");
    });
    ConsoleMenu->addAction(tr("Generate &Breakpoints 设置断点"), QKeySequence(tr("Ctrl+t")), this, [this](){
        barStatus->log("Generated breakpoints... 设置断点中...");
        editor->generateBPs();
    });
    ConsoleMenu->addAction(tr("Clea&n (Build and Breakpoints) 清除"), QKeySequence(tr("Ctrl+Alt+c")), this, [this]() {
        // remove bin/ folder
        QDir bin(fileList->dirP.absolutePath()+"/bin");
        if (bin.exists()) {
            bin.removeRecursively();
            barStatus->log("Cleaned project. 清除.");
        } else {
            barStatus->log("No build files to clean. 没有要清除的文件.");
        }
        // remove all breakpoints
        editor->breakPoints.insert(fileList->fileP, QMap<int,bool>{{}});
        editor->repaint();
    });
    ConsoleMenu->addSeparator();
    ConsoleMenu->addAction(tr("Stop/&Kill 结束"), QKeySequence(tr("Ctrl+Shift+c")), this, [this](){
        console->stop();
        barStatus->log("Killed running console processes. 结束控制台进程.");
    });
    ConsoleMenu->addAction(tr("C&lear Console 清除控制台"), QKeySequence(tr("Ctrl+l")), this, [this](){
        console->clearLog();
        barStatus->log("Cleared console log. 清除控制台日志.");
    });

    ConsoleMenu->addSeparator();
    configG = new ConfigGen(this);
    connect(configG, SIGNAL(finished(int)), this, SLOT(updateComms()));

    ConsoleMenu->addAction(tr("&Settings 设置"), QKeySequence(tr("Ctrl+,")), this, [this](){configG->exec();});


    commG = new CommandGen(&fileList->dirP);
}

void MainWindow::setupEditMenu() {
    // edit menu items
    QMenu *EditMenu = new QMenu(tr("&Edit 编辑"), this);
    menuBar()->addMenu(EditMenu);
    QAction *undo = new QAction(tr("&Undo 恢复"), this);
    undo->setShortcut(QKeySequence::Undo);
    EditMenu->addAction(undo);
    connect(undo,SIGNAL(triggered()),editor,SLOT(undo()));
    QAction *redo = new QAction(tr("&Redo 重做"), this);
    redo->setShortcut(QKeySequence::Redo);
    EditMenu->addAction(redo);
    connect(redo,SIGNAL(triggered()),editor,SLOT(redo()));
    EditMenu->addSeparator();
    QAction *cut = new QAction(tr("Cu&t 剪切"), this);
    cut->setShortcut(QKeySequence::Cut);
    EditMenu->addAction(cut);
    connect(cut,SIGNAL(triggered()),editor,SLOT(cut()));
    QAction *copy = new QAction(tr("&Copy 复制"), this);
    copy->setShortcut(QKeySequence::Copy);
    EditMenu->addAction(copy);
    connect(copy,SIGNAL(triggered()),editor,SLOT(copy()));
    QAction *paste = new QAction(tr("&Paste 粘贴"), this);
    paste->setShortcut(QKeySequence::Paste);
    EditMenu->addAction(paste);
    connect(paste,SIGNAL(triggered()),editor,SLOT(paste()));
    EditMenu->addSeparator();
    QAction *selectAll = new QAction(tr("Select &All 选择全部"), this);
    selectAll->setShortcut(QKeySequence::SelectAll);
    EditMenu->addAction(selectAll);
    connect(selectAll,SIGNAL(triggered()),editor,SLOT(selectAll()));
    EditMenu->addSeparator();
}

void MainWindow::setupViewMenu() {
    QMenu *ViewMenu = new QMenu(tr("&View 视图"), this);
    menuBar()->addMenu(ViewMenu);
    ViewMenu->addAction(tr("Zoom &In 放大"), QKeySequence::ZoomIn, this, [=](){
        editor->zoomIn();
    });
    ViewMenu->addAction(tr("Zoom &Out 缩小"), QKeySequence::ZoomOut, this, [=](){
        editor->zoomOut();
    });
    ViewMenu->addSeparator();
    ViewMenu->addAction(tr("&Goto Line 跳转至行"), QKeySequence(tr("Ctrl+g")), this, [=](){
        int line = QInputDialog::getInt(this, tr("Goto Line 转到线"), tr("Line Number, 线号:"), 1, 1);
        if (line > 0) {
            QTextCursor csr = editor->textCursor();
            csr.movePosition(QTextCursor::StartOfLine);
            csr.movePosition(QTextCursor::Start);
            csr.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, line-1);
            editor->setTextCursor(csr);
            barStatus->log("Moved cursor to line, 移动光标至行 "+QString::number(line));
        }
    });
}


//------ Search and Replace Menu

void MainWindow::setupSearchMenu(QTextEdit *search_object)//当前文档页的指针
{
    QMenu *SearchMenu = new QMenu(tr("&Search 搜索"), this);
    menuBar()->addMenu(SearchMenu);

    // search and replace menu items
    srMenu=new Search_Replace(search_object,this);
    SearchMenu->addAction(tr("&Search 搜索"), QKeySequence::Find, this, [=](){
        srMenu->setCurrentTab(1);
        srMenu->show();
        QTimer::singleShot(0, srMenu->ui->keywords, SLOT(setFocus()));
    });

    SearchMenu->addAction(tr("Search and &Replace 替换"), QKeySequence(tr("Ctrl+Shift+f")), this, [=](){
        srMenu->setCurrentTab(2);
        srMenu->show();
        QTimer::singleShot(0, srMenu->ui->keywords_2, SLOT(setFocus()));
    });

    SearchMenu->addSeparator();
    SearchMenu->addAction(tr("Switch Header/Source 切换头文件"), QKeySequence(tr("F4")), this, [=](){
        fileList->swapHC();
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
        undo->setText("&Undo 恢复");
        menu->addAction(undo);
        connect(undo,SIGNAL(triggered()),this,SLOT(CMundo()));

        QAction *redo = new QAction(menu);
        redo->setText("&Redo 重做");
        menu->addAction(redo);
        connect(redo,SIGNAL(triggered()),this,SLOT(CMredo()));

        QAction *cut = new QAction(menu);
        cut->setText("Cu&t 剪切");
        menu->addAction(cut);
        connect(cut,SIGNAL(triggered()),this,SLOT(CMcut()));

        QAction *copy = new QAction(menu);
        copy->setText("&Copy 复制");
        menu->addAction(copy);
        connect(copy,SIGNAL(triggered()),this,SLOT(CMcopy()));

        QAction *paste = new QAction(menu);
        paste->setText("&Paste 粘贴");
        menu->addAction(paste);
        connect(paste,SIGNAL(triggered()),this,SLOT(CMpaste()));

        QAction *selectAll = new QAction(menu);
        selectAll->setText("Select &All 选择全部");
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
