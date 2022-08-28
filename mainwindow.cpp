/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtWidgets>

#include "mainwindow.h"

#include "highlighter.h"
#include "qtexteditlineshighlighted.h"
#include "console.h"

#include <QWidget>
#include <QListWidget>
#include <QGridLayout>


//! [0]
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupFileMenu();
    setupEditor();
    setupConsole();


    //setup split view
    QWidget *widget = new QWidget(this);
    QGridLayout *layout = new QGridLayout(widget);

    setCentralWidget(widget);
    widget->setLayout(layout);

    layout->addWidget(editor, 0, 1, 1, 1);
    layout->setColumnStretch(1, 10);


    fileList = new FileManager(this, editor);
    //todo: add button for new file and delete file here
    layout->addWidget(fileList, 0, 0, 1, 1);
    layout->setColumnStretch(0, 2);

    layout->addWidget(console, 1, 0, 1, 2);
    layout->setRowStretch(0, 9);
    layout->setRowStretch(1, 3);

    setupHelpMenu();

    setWindowTitle(tr("C++ IDE"));
    //editor->setPlainText("Welcome to C++ IDE V1.0\nGo to File > Open and select a folder\ncontaining C/C++ files to get started.\n");
    showDocs();
}
//! [0]

void MainWindow::about()
{
    //todo: add about description here
    QString md = "";
    QFile file("../doc/about.md");
        if (file.open(QFile::ReadOnly | QFile::Text))
            md = file.readAll();
    //QMessageBox::about(this, tr("About C++ IDE"), md);
    QMessageBox aboutb;
    aboutb.setTextFormat(Qt::MarkdownText);
    aboutb.setText(md);
    aboutb.exec();
}

void MainWindow::newFile()
{
    editor->clear();
}

void MainWindow::openFile(const QString &path)
{
    fileList->openFolder(path);
}

//! [1]
void MainWindow::setupEditor()
{
    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(10);

    editor = new QTextEditLinesHighlighted;
    editor->setFont(font);

    highlighter = new Highlighter(editor->document());
}
//! [1]

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
    //todo: add help menu to temp load readme.md
    helpMenu->addAction(tr("&About"), this, &MainWindow::about);
    helpMenu->addAction(tr("About &Qt"), qApp, &QApplication::aboutQt);
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

void MainWindow::keyPressEvent(QKeyEvent *ev) {
    if (ev->key() == Qt::Key::Key_Escape) {
        // restore if documentation was showing during keypress
        showDocsRestore();
    }
}

void MainWindow::setupConsole() {
    console = new Console();
    QMenu *ConsoleMenu = new QMenu(tr("&Console"), this);
    menuBar()->addMenu(ConsoleMenu);
    ConsoleMenu->addAction(tr("Com&pile"), this, [this](){console->compile();});
    ConsoleMenu->addAction(tr("Ru&n"), this, [this](){console->run();});
    //todo: add wait for compile and then run
//    ConsoleMenu->addAction(tr("Compile and &Run"), this, [this](){console->compile();console->run();});
    ConsoleMenu->addSeparator();
    ConsoleMenu->addAction(tr("&Clear Console"), this, [this](){console->clearLog();});



    ConsoleMenu->addSeparator();
    configG = new ConfigGen(this);
    ConsoleMenu->addAction(tr("&Preferences"), this, [this](){configG->show();});
}
