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

#include "globaldefs.h"

#include "filemanager.h"
#include <QDir>
#include <QFileInfo>
#include <QFileDialog>

FileManager::FileManager(QWidget *parent, QTextEdit *editor) : QListWidget(parent),parentw(parent),editor(editor)
{
    // load settings
    settings = new QSettings(COMPANY, APPNAME);
    // open file when list item is clicked
    connect(this, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(openFile(QListWidgetItem*)));
}

void FileManager::openFile(QListWidgetItem* item) {
    // get file path and load
    QString fpath = dirP.absolutePath() + "/" + item->text();
    loadFile(fpath);
    // save last opened file to settings
    settings->setValue("LastOpenedFile", fpath);
}

void FileManager::openFolder(QString fileName) {
    //file opening part

    if (fileName.isNull()) {
        // Display dialog if no file is pased
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
        // open file
        loadFile(fileName);
    }



}

void FileManager::loadFile(QString fileName) {
    // read file to text editor
    fileP = fileName;
    QFile file(fileName);
    if (file.open(QFile::ReadOnly | QFile::Text))
        editor->setPlainText(file.readAll());
}
