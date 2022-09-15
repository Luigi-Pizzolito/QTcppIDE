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
#include <QMessageBox>

#include <QTime>
#include <QCoreApplication>
#include <QEventLoop>
void delay( int millisecondsToWait )
{
    QTime dieTime = QTime::currentTime().addMSecs( millisecondsToWait );
    while( QTime::currentTime() < dieTime )
    {
        QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
    }
}

FileManager::FileManager(QWidget *parent, QTextEdit *editor, bool *showingDocs, Console *console) : QListWidget(parent),showingDocs(showingDocs),parentw(parent),editor(editor),console(console)
{
    // load settings
    settings = new QSettings(COMPANY, APPNAME);
    // open file when list item is clicked
    connect(this, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(openFile(QListWidgetItem*)));
    // setup new file dialog
    setupNewFileDialog();
}

void FileManager::setupNewFileDialog() {
    // create new file dialog
    newFileD = new QDialog();
    flayout = new QFormLayout();
    newFileD->setLayout(flayout);

    fileN = new QLineEdit();
    mfont.setFamily("Courier");
    mfont.setFixedPitch(true);
    mfont.setPointSize(10);
    fileN->setFont(mfont);
    fileN->setMinimumWidth(200);
    flayout->addRow(tr("Filename 文件名"), fileN);

    fileT = new QComboBox();
    //fileT->setEditable(true);
    fileT->setFont(mfont);
    fileT->addItem("cpp/h");
    fileT->insertSeparator(1);
    fileT->addItem("cpp");
    fileT->addItem("c");
    fileT->addItem("h");
    fileT->insertSeparator(5);
    fileT->addItem("cxx");
    fileT->addItem("cc");
    fileT->addItem("hpp");
    fileT->insertSeparator(9);
    fileT->addItem("cpp/hpp");
    fileT->addItem("cxx/h");
    fileT->addItem("cxx/hpp");
    fileT->setCurrentIndex(0);
    flayout->addRow(tr("Filetype 文件类型"), fileT);

    okbtn = new QPushButton("OK", this);
    connect(okbtn, SIGNAL(clicked(bool)), this, SLOT(createNewFiles())); //change to other func in this
    flayout->addWidget(okbtn);
}

void FileManager::openFile(QListWidgetItem* item) {
    QString fpath = dirP.absolutePath() + "/" + item->text();
    if (dirP.exists(fpath)) {
        // save current file savefirst
        saveFile();
        // get file path and load
        loadFile(fpath);
        // save last opened file to settings
        settings->setValue("LastOpenedFile", fpath);
        *showingDocs = false;
    }
}

void FileManager::swapHC() {
    // check if matching h/cpp file exists
    // get all files with the same basename
    QStringList files = dirP.entryList(QStringList()<<QFileInfo(fileP).baseName()+".*",QDir::Files);
    QString currSufx = QFileInfo(fileP).suffix();
    foreach (QString file, files) {
        file = dirP.absolutePath()+"/"+file;
        if (QFileInfo(file).suffix() != currSufx) {
            // found another file with different suffix, open and break
            saveFile();
            openFolder(file);
            status->log("Found, 找到 "+QFileInfo(file).fileName());
            break;
        }
    }
}

void FileManager::passStatus(StatusBar* statusp) {
    status = statusp;
}

void FileManager::saveFile(bool prompt) {
    // check if (read-only) documentation is open
    if (*showingDocs) {
//        status->log("Please select and open a file before saving.");
    } else {
        // prompt to ask to confirm?
        int promptRes;
        if (prompt)
            promptRes = QMessageBox(QMessageBox::Information, "Save, 保存 "+QFileInfo(fileP).fileName(), "Save "+QFileInfo(fileP).fileName()+" first? Or discard changes?\n先保存"+QFileInfo(fileP).fileName()+"？还是丢弃修改？", QMessageBox::Yes|QMessageBox::No).exec();
        if (prompt==false || promptRes==QMessageBox::Yes) {
            //save file here
            QFile file(fileP);
            if(file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                QTextStream fstream(&file);
                fstream << editor->toPlainText();
                file.close();
                status->log("Saved file, 保存文件 "+QFileInfo(fileP).fileName()); //todo: changethis to barStatus->log
            } else {
                status->log("Error saving file, 保存文件错误 "+QFileInfo(fileP).fileName());
                console->err("Error saving file, 保存文件错误 "+fileP);
            }
        }
    }
}

void FileManager::openFolder(QString fileName) {
    //file opening part

    if (fileName.isNull()) {
        // Display dialog if no file is pased
        fileName = QFileDialog::getOpenFileName(this, tr("Open File 打开文件"), "", "C++ Files (*.cpp *.hpp *.h *.cxx *.cc);;C Files (*.c *.h);;Any files (*)");
    }

    if (!fileName.isEmpty() && dirP.exists(fileName)) {

        //todo: save current file if there is contetn before opening, add flag above in isNull to detect opening over previous
        // file manager part
        dirP = QFileInfo(fileName).absoluteDir();
        QStringList filters;
        filters << "*.cpp" << "*.hpp" << "*.h" << "*.c" << "*.cxx" << "*.cc";
        dirP.setNameFilters(filters);
        QStringList dirI = dirP.entryList();
        clear();
        for ( const auto& i : dirI ) {
            addItem(i);
            item(count()-1)->setFont(mfont);
        }
        // Select currently open file's list item
        setCurrentItem(findItems(QFileInfo(fileName).fileName(), Qt::MatchExactly).at(0));
        // open file
        loadFile(fileName);
        settings->setValue("LastOpenedFile", fileName);
        *showingDocs = false;
    }

}

void FileManager::loadFile(QString fileName) {
    // read file to text editor
    fileP = fileName;
    QFile file(fileName);
    if (file.open(QFile::ReadOnly | QFile::Text))
        editor->setPlainText(file.readAll());
}


void FileManager::createNewFileRequest() { //change to bool, rename to request
    // save previous file
    saveFile(true);
    // open new file dialog
    newFileD->exec();
}

void FileManager::setFileMain() {
    fileN->setText("main");
    fileT->setCurrentIndex(1);
    fileT->setCurrentText("cpp");
}

void FileManager::createNewFiles() {
    // parse requested files to create
    QStringList files;
    if (fileT->currentText().contains("/")) {
        // create multiple files
        for (const auto &ext : fileT->currentText().split("/")) {
            files << fileN->text() +"."+ ext;
        }
    } else {
        //create just one file
        files << fileN->text() +"."+ fileT->currentText();
    }
    // create files
    for (const auto &file : files) {
        QString fpath = dirP.absolutePath()+"/"+file.toLower();
        QFile qfile(fpath);
        qfile.open(QIODevice::WriteOnly);
        // generate and write file templates
        QString templatef = FileTemplates[QFileInfo(fpath).suffix()];
        if (file.toLower()=="main.cpp") {
            templatef = FileTemplates["main"];
        } else if (!fileT->currentText().contains("/") && !file.endsWith(".c")) {
            templatef = "// %filen\n\n";
        }
        templatef.replace("%filenl",fileN->text().toLower());
        templatef.replace("%filen",file.toLower());
        templatef.replace("%file",fileN->text());
        qfile.write(templatef.toUtf8());
        qfile.close();
    }
    status->log("Created files "+fileN->text()+" of type "+fileT->currentText());
    status->log("创建 "+fileT->currentText()+" 类型的文件 "+fileN->text());
    // close dialog
    newFileD->close();
    // reset create file dialog
    fileN->clear();
    //fileT->clearEditText();
    fileT->setCurrentIndex(0);
    // open file
    delay(100); //delay to maake sure files were created
    openFolder(dirP.absolutePath()+"/"+files[0].toLower());
}
