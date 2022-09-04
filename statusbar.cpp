#include "statusbar.h"
#include "globaldefs.h"

StatusBar::StatusBar(QWidget *parent, QDir *folder, QString *file, QTextEdit *editor) : parent(parent),folder(folder),file(file),editor(editor)
{
    settings = new QSettings(COMPANY, APPNAME);
    // construct
    folderE = new QLabel(this);
    folderE->setStyleSheet("padding: 0 5px;");
    addPermanentWidget(folderE);

    fileE = new QLabel(this);
    fileE->setStyleSheet("padding: 0 5px;");
    addPermanentWidget(fileE);

    cursorE = new QLabel(this);
    cursorE->setStyleSheet("padding: 0 5px;");
    addPermanentWidget(cursorE);

    compilerE = new QLabel(this);
    compilerE->setStyleSheet("padding: 0 5px;");
    addPermanentWidget(compilerE);

    setSizeGripEnabled(false);
    // update
    update();
}

void StatusBar::update() {
    // refresh label texts
    folderE->setText(folder->absolutePath().split("/").last());
    fileE->setText(QFileInfo(*file).fileName());
    cursorE->setText(QString::number( \
                                     1 + editor->textCursor().blockNumber()) \
                     +":"+ \
                     QString::number( \
                                     1 + editor->textCursor().positionInBlock()) \
                     );
    compilerE->setText(settings->value("CompileArgs").toString().split(" ").first());
}

void StatusBar::log(QString msg) {
    // show message for 5s
    showMessage(msg, 5*1000);
}
