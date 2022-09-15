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

#include "configgen.h"

#include "globaldefs.h"

ConfigGen::ConfigGen(QWidget *parent):QDialog(parent)
{
    // Setup config screen GUI Elements
    setWindowTitle("Preferences 偏好");
    flayout = new QFormLayout();
    setLayout(flayout);

    // Preset Selector
    presetS = new QComboBox();
    for (auto &preset : ConfigDefaults) {
        presetS->addItem(preset.name);
    }
    presetS->addItem("Custom 习惯");
    flayout->addRow(tr("Compiler Settings Preset 编译器设置预设"), presetS);
    connect(presetS, SIGNAL(currentIndexChanged(int)), this, SLOT(loadPresetGUI(int)));

    // Monospace Font for LineEdits
    mfont.setFamily("Courier");
    mfont.setFixedPitch(true);
    mfont.setPointSize(10);

    // Compiler Settings
    cargs = new QLineEdit();
    cargs->setMinimumWidth(350);
    cargs->setFont(mfont);
    flayout->addRow(tr("Compiler Arguments 编译器参数"), cargs);
    connect(cargs, SIGNAL(textEdited(QString)), this, SLOT(switchCustomPreset()));

    cinf = new QLineEdit();
    cinf->setMinimumWidth(350);
    cinf->setFont(mfont);
    flayout->addRow(tr("Input Files 输入文件"), cinf);
    connect(cinf, SIGNAL(textEdited(QString)), this, SLOT(switchCustomPreset()));

    cicd = new QLineEdit();
    cicd->setMinimumWidth(350);
    cicd->setFont(mfont);
    flayout->addRow(tr("Include Directories 包括目录"), cicd);
    connect(cicd, SIGNAL(textEdited(QString)), this, SLOT(switchCustomPreset()));

    coutf = new QLineEdit();
    coutf->setMinimumWidth(350);
    coutf->setFont(mfont);
    flayout->addRow(tr("Output File 输出文件"), coutf);
    connect(coutf, SIGNAL(textEdited(QString)), this, SLOT(switchCustomPreset()));

    // Help label explaining fields system.
    compilerabt = new QLabel();
    compilerabt->setTextFormat(Qt::MarkdownText);
    compilerabt->setText(
"Here you can set which compiler to use and the command line flags that get passed to the compiler.\n\n\
There are also default presets to pick from that should work on systems with those respective compilers installed.\n\n\
The following fields are available:\n\n\
**%root**: Absolute path to the project root directory.\n\n\
**%rootn**: Name of the project root directory.\n\n\
**%in**: Parsed list of input files relative paths, may only be used in compiler arguments section.\n\n\
**%inc**: Parsed include directories absolute path expression, may only be used in compiler arguments section.\n\n\
**%out**: Parsed output absolute path expression, may only be used in compiler arguments section.\n\n\
On Linux systems, you probably already have gcc/g++ installed. Otherwise you may install via your package manager.\n\n\
On Windows systems, it is recommended to install w64devkit to *C:\\w64devkit*, and add it to your PATH.\n\n\
您可以在此处设置要使用的编译器以及传递给编译器的命令行标志。\n\n\
还有一些默认预设可供选择，它们应该适用于安装了相应编译器的系统。\n\n\
以下字段可用：\n\n\
**%root**：项目根目录的绝对路径。\n\n\
**%rootn**：项目根目录的名称。\n\n\
**%in**: 输入文件相对路径的解析列表，只能用于编译器参数部分。\n\n\
**%inc**: 解析的包含目录绝对路径表达式，只能在编译器参数部分使用。\n\n\
**%out**: 解析后的输出绝对路径表达式，只能在编译器参数部分使用。\n\n\
在 Linux 系统上，您可能已经安装了 gcc/g++。 否则你可以通过你的软件包管理器安装。\n\n\
在 Windows 系统上，建议将 w64devkit 安装到 *C:\\w64devkit*，并将其添加到您的 PATH。\n\n\
    ");
    flayout->addRow(tr("Help 帮助"), compilerabt);

    // Settings for launching in external console.
#ifndef _WIN32
    extCon = new QCheckBox();
    flayout->addRow(tr("Launch in External Console 外部控制台中启动"), extCon);
    connect(extCon, SIGNAL(stateChanged(int)), this, SLOT(switchCustomPreset()));
#endif
    conCom = new QLineEdit();
    conCom->setMinimumWidth(350);
    conCom->setFont(mfont);
    flayout->addRow(tr("Ext. Console Arguments 外部控制台参数"), conCom);
    connect(conCom, SIGNAL(textEdited(QString)), this, SLOT(switchCustomPreset()));

    // Close button
    okbtn = new QPushButton("OK", this);
    flayout->addWidget(okbtn);
    connect(okbtn, SIGNAL(clicked(bool)), this, SLOT(close()));


    // Load Settings
    csettings = new QSettings(COMPANY, APPNAME);
    int cspreset = csettings->value("Pindex", 0).toInt();    //load preset index, default to 0 (first preset)
    presetS->setCurrentIndex(cspreset);
    loadPresetGUI(cspreset); //load to GUI and save


}

void ConfigGen::loadPresetGUI(int i) {
    if (i>numPresets) i=0; // fix in case invalid preset was saved from previous version of the program.
    if (i<numPresets) {
        // load selected default preset
        cargs->setText(ConfigDefaults[i].CompileArgs);
        cinf->setText(ConfigDefaults[i].InFiles);
        cicd->setText(ConfigDefaults[i].IncDirs);
        coutf->setText(ConfigDefaults[i].OutFile);
        #ifndef _WIN32
        extCon->setChecked(ConfigDefaults[i].ExtCon);
        #endif
        conCom->setText(ConfigDefaults[i].ConArgs);
        // save to non-volatile
        saveConfig(i);

        presetS->setCurrentIndex(i);
    } else {
        // load custom preset from settings
        cargs->setText(csettings->value("CompileArgs").toString());
        cinf->setText(csettings->value("InFiles").toString());
        cicd->setText(csettings->value("IncDirs").toString());
        coutf->setText(csettings->value("OutFile").toString());
        conCom->setText(csettings->value("ConArgs").toString());
        #ifndef _WIN32
        extCon->setChecked(csettings->value("ExtCon").toBool());
        #endif
    }
}

void ConfigGen::switchCustomPreset() {
    //Switch to custom preset when any field is changed.
    saveConfig(numPresets);
    csettings->setValue("Pindex", numPresets);
    presetS->setCurrentIndex(numPresets);
}

void ConfigGen::saveConfig(int Pi) {
    // save to settings (non-volatile)
    csettings->setValue("Pindex",       Pi);
    csettings->setValue("CompileArgs",  cargs->text());
    csettings->setValue("InFiles",      cinf->text());
    csettings->setValue("IncDirs",      cicd->text());
    csettings->setValue("OutFile",      coutf->text());
    csettings->setValue("ConArgs",      conCom->text());
    #ifndef _WIN32
    csettings->setValue("ExtCon",       extCon->isChecked());
    #endif
    csettings->sync();
}
