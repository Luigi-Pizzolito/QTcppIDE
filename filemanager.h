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

#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QSettings>
#include <QListWidget>
#include <QObject>
#include <QWidget>
#include <QTextEdit>
#include <QDir>

#include "console.h"
#include "statusbar.h"

#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QMap>

class FileManager : public QListWidget
{
    Q_OBJECT
public:
    explicit FileManager(QWidget *parent = 0, QTextEdit *editor=0, bool *showingDocs=0, Console *console=0);
    void passStatus(StatusBar* statusp);
    QDir dirP;
    QString fileP;
public slots:
    void openFolder(QString fileName);
    void openFile(QListWidgetItem* item);
    void createNewFileRequest();
    void createNewFiles();
    void setFileMain();
    void saveFile(bool prompt = false);
    void swapHC();
private:
    QWidget *parentw;
    QSettings *settings;
    QTextEdit *editor;
    bool *showingDocs;
    Console *console;
    StatusBar *status;
    void loadFile(QString fileName);
    // new file dialog
    void setupNewFileDialog();
    QDialog *newFileD;
    QFormLayout *flayout;
    QFont mfont;
    QLineEdit *fileN;
    QComboBox *fileT;
    QPushButton *okbtn;
    // file starter templates
    #define mainStarter "// %filen\n#include <iostream>\nusing namespace std;\n\nint main(int argc, char *argv[]) {\n    \n    return 0;\n}"
    #define cppStarter  "// %filen\n#include \"%filenl.h\"\n\n%file::%file() {\n    \n}\n\n%file::~%file() {\n    \n}"
    #define cStarter    "// %filen\n#include <stdio.h>\n\nint main(int argc, char *argv[]) {\n    \n    return 0;\n}"
    #define hStarter    "// %filen\n#pragma once\n\nclass %file {\n    public:\n        %file();\n        ~%file();\n    private:\n\n};"
    const QMap<QString,QString> FileTemplates {
            {"main", mainStarter},
            {"cpp", cppStarter},
            {"cxx", cppStarter},
            {"cc" , cppStarter},
            {"c"  , cStarter},
            {"h"  , hStarter},
            {"hpp", hStarter}
    };
};

#endif // FILEMANAGER_H
