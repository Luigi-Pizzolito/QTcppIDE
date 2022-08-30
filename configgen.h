/*
 * Config Generator Class
 * 
 * C++ IDE by Luigi Pizzolito
 *            Zhang Ruiqing
 *            Ruan Zihang
 *            Lin Zhenmin
 *            Yang Zhaoyi
 * For 1703-ECE Class by Nie Qing and Wu Hao
 * At Beijing Insitute of Techology
*/

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

#include <QSettings>

class ConfigGen : public QDialog
{
    Q_OBJECT
public:
    explicit ConfigGen(QWidget *parent = 0);

private:
    //todo: createoutputdirs
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
    // Struct to store default configuration hardcoded
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
        {"Linux G++",     "g++ %in -I %inc -o %out -Wall", "*.cpp", "%root", "bin/%rootn", false, "xterm -e %out"},
        {"Linux GCC",     "gcc %in -I %inc -o %out -Wall", "*.c",   "%root", "bin/%rootn", false, "xterm -e %out"},
        {"Windows MinGW", "", "", "", "", false, ""}
    };

    // QSettings to store settings non-volatile
    QSettings *csettings;

private slots:
    // Internal GUI callbacks
    void loadPresetGUI(int i);
    void switchCustomPreset();
    void saveConfig(int Pi);
};

#endif // CONFIGGEN_H
