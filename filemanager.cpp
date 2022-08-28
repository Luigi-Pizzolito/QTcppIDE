#include "filemanager.h"
#include <QDir>
#include <QFileInfo>
#include <QFileDialog>

//#include <iostream>

FileManager::FileManager(QWidget *parent, QTextEdit *editor) : QListWidget(parent),parentw(parent),editor(editor)
{
    connect(this, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(openFile(QListWidgetItem*)));
}

void FileManager::openFile(QListWidgetItem* item) {
    QString fpath = dirP.absolutePath() + "/" + item->text();
    loadFile(fpath);
}

void FileManager::openFolder(QString fileName) {
    //file opening part

    if (fileName.isNull()) {
        fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", "C++ Files (*.cpp *.hpp *.h *.cxx *.cc);;C Files (*.c *.h);;Any files (*)");
    }

    if (!fileName.isEmpty()) {

        //todo: save current file if there is contetn before opening, add flag above in isNull to detect opening over previous

        // file manager part

        dirP = QFileInfo(fileName).absoluteDir();
        QStringList filters;
        filters << "*.cpp" << "*.hpp" << "*.h" << "*.cxx" << "*.cc";
        dirP.setNameFilters(filters);
        QStringList dirI = dirP.entryList();
        clear();
        for ( const auto& i : dirI ) {
            addItem(i);
        }
        // Select currently open file's list item
        setCurrentItem(findItems(QFileInfo(fileName).fileName(), Qt::MatchExactly)[0]);
        //end

        loadFile(fileName);
    }



}

void FileManager::loadFile(QString fileName) {
    fileP = fileName;
    QFile file(fileName);
    if (file.open(QFile::ReadOnly | QFile::Text))
        editor->setPlainText(file.readAll());
}
