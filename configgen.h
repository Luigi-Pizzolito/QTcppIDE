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
    QPushButton *okbtn;

private:
    // GUI Elements
    QFormLayout *flayout;
    QComboBox *presetS;
    QFont mfont;
    QLineEdit *cargs;
    QLineEdit *cinf;
    QLineEdit *cicd;
    QLineEdit *coutf;
    QLabel *compilerabt;
    #ifndef _WIN32
    QCheckBox *extCon;
    #endif
    QLineEdit *conCom;

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
#ifdef _WIN32
    #define numPresets 4
    ConfigSet ConfigDefaults[numPresets] = {
        {"Windows GCC with w64devkit",                                "gcc %in -I %inc -o %out -static -Wall",    "*.c",   "%root", "bin/%rootn", true,   "cmd /k cd bin & %rootn & pause & exit"},
        {"Windows G++ with w64devkit",                                "g++ %in -I %inc -o %out -static -Wall",    "*.cpp", "%root", "bin/%rootn", true,   "cmd /k cd bin & %rootn & pause & exit"},
        {"Windows GCC with w64devkit and GDB Debugger (调试器)",      "gcc -g %in -I %inc -o %out -static -Wall",    "*.c", "%root", "bin/%rootn", true,   "cmd /k cd bin & gdb %rootn & pause & exit"},
        {"Windows G++ with w64devkit and GDB Debugger (调试器)",      "g++ -g %in -I %inc -o %out -static -Wall",    "*.cpp", "%root", "bin/%rootn", true,   "cmd /k cd bin & gdb %rootn & pause & exit"}
    };
#else
    #define numPresets 4
    ConfigSet ConfigDefaults[numPresets] = {
        {"Linux G++",                       "g++ %in -I %inc -o %out -Wall",            "*.cpp", "%root", "bin/%rootn", false,  "alacritty -e %out"},
        {"Linux GCC",                       "gcc %in -I %inc -o %out -Wall",            "*.c",   "%root", "bin/%rootn", false,  "alacritty -e %out"},
        {"Linux G++ with LLDB",             "g++ -g %in -I %inc -o %out -Wall",         "*.cpp", "%root", "bin/%rootn", true,   "alacritty -e lldb %out"},
        {"Linux GCC with LLDB",             "gcc -g %in -I %inc -o %out -Wall",         "*.c",   "%root", "bin/%rootn", true,   "alacritty -e lldb %out"}
    };
#endif
    // QSettings to store settings non-volatile
    QSettings *csettings;

private slots:
    // Internal GUI callbacks
    void loadPresetGUI(int i);
    void switchCustomPreset();
    void saveConfig(int Pi);
};

#endif // CONFIGGEN_H
