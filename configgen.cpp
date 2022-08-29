#include "configgen.h"

#include <iostream>

ConfigGen::ConfigGen(QWidget *parent):QDialog(parent)
{
    // Setup config screen GUI Elements
    setWindowTitle("Preferences");
    flayout = new QFormLayout();
    setLayout(flayout);

    // Preset Selector
    presetS = new QComboBox();
    for (auto &preset : ConfigDefaults) {
        presetS->addItem(preset.name);
    }
    presetS->addItem("Custom");
    flayout->addRow(tr("Compiler Settings Preset"), presetS);
    connect(presetS, SIGNAL(currentIndexChanged(int)), this, SLOT(loadPresetGUI(int)));

    // Monospace Font for LineEdits
    mfont.setFamily("Courier");
    mfont.setFixedPitch(true);
    mfont.setPointSize(10);

    // Compiler Settings
    cargs = new QLineEdit();
    cargs->setMinimumWidth(350);
    cargs->setFont(mfont);
    flayout->addRow(tr("Compiler Arguments"), cargs);
    connect(cargs, SIGNAL(textEdited(QString)), this, SLOT(switchCustomPreset()));

    cinf = new QLineEdit();
    cinf->setMinimumWidth(350);
    cinf->setFont(mfont);
    flayout->addRow(tr("Input Files"), cinf);
    connect(cinf, SIGNAL(textEdited(QString)), this, SLOT(switchCustomPreset()));

    cicd = new QLineEdit();
    cicd->setMinimumWidth(350);
    cicd->setFont(mfont);
    flayout->addRow(tr("Include Directories"), cicd);
    connect(cicd, SIGNAL(textEdited(QString)), this, SLOT(switchCustomPreset()));

    coutf = new QLineEdit();
    coutf->setMinimumWidth(350);
    coutf->setFont(mfont);
    flayout->addRow(tr("Output File"), coutf);
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
On Windows systems, it is recommended to install MinGW to *C:\\MinGW*.\n\n\
    ");
    flayout->addRow(tr("Help"), compilerabt);

    // Settings for launching in external console.
    extCon = new QCheckBox();
    flayout->addRow(tr("Launch in External Console"), extCon);
    connect(extCon, SIGNAL(stateChanged(int)), this, SLOT(switchCustomPreset()));

    conCom = new QLineEdit();
    conCom->setMinimumWidth(350);
    conCom->setFont(mfont);
    flayout->addRow(tr("Ext. Console Arguments"), conCom);
    connect(conCom, SIGNAL(textEdited(QString)), this, SLOT(switchCustomPreset()));

    // Close button
    okbtn = new QPushButton("OK", this);
    flayout->addWidget(okbtn);
    connect(okbtn, SIGNAL(clicked(bool)), this, SLOT(close()));



    //default load if not set, add here
    loadPresetGUI(0);

}

void ConfigGen::loadPresetGUI(int i) {
    if (i<numPresets) {
        // load selected default preset
        cargs->setText(ConfigDefaults[i].CompileArgs);
        cinf->setText(ConfigDefaults[i].InFiles);
        cicd->setText(ConfigDefaults[i].IncDirs);
        coutf->setText(ConfigDefaults[i].OutFile);
        extCon->setChecked(ConfigDefaults[i].ExtCon);
        conCom->setText(ConfigDefaults[i].ConArgs);
        saveConfig();
    } // else custom selected
}

void ConfigGen::switchCustomPreset() {
    presetS->setCurrentIndex(numPresets); //Switch to custom preset when any field is changed.
    saveConfig();
}

void ConfigGen::saveConfig() {
    std::cout << "saved config: "<<cargs->text().toStdString()<<"\n";
}
