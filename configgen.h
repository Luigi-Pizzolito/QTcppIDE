#ifndef CONFIGGEN_H
#define CONFIGGEN_H

#include <QDialog>
#include <QObject>
#include <QWidget>

#include <QFormLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>
#include <QLabel>

class ConfigGen : public QDialog
{
    Q_OBJECT
public:
    explicit ConfigGen(QWidget *parent = 0);
public slots:
    //todo: save, gencompile, genrun, gendebug
private:
    //todo: loadsave, createoutputdirs
    // GUI Elements
    QFormLayout *flayout;
    QComboBox *presetS;
    QFont mfont;
    QLineEdit *cargs;
    QLineEdit *cinf;
    QLineEdit *cicd;
    QLineEdit *coutf;
    QLabel *compilerabt;
    QCheckBox *extCon;
    QLineEdit *conCom;
    QPushButton *okbtn;
    // Struct to store default configuration hardcoded?
    struct ConfigSet {
        QString name;
        QString CompileArgs;
        QString InFiles;
        QString IncDirs;
        QString OutFile;
        bool    ExtCon;
        QString ConArgs;
    };
    #define numPresets 3
    ConfigSet ConfigDefaults[numPresets] = {
        {"Linux G++",     "g++ %in -I %inc -o %out -Wall", "*.cpp", "%root", "bin/%rootn", false, "xterm %out"},
        {"Linux GCC",     "gcc %in -I %inc -o %out -Wall", "*.c",   "%root", "bin/%rootn", false, "xterm %out"},
        {"Windows MinGW", "", "", "", "", false, ""}
    };
private slots:
    // Internal GUI callbacks
    void loadPresetGUI(int i);
    void switchCustomPreset();
    void saveConfig();
};

#endif // CONFIGGEN_H
