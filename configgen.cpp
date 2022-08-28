#include "configgen.h"

#include <QFormLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>


ConfigGen::ConfigGen(QWidget *parent):QDialog(parent)
{
    //todo: add defaults loader based on events of preset selection

    setWindowTitle("Preferences");
    QFormLayout *flayout = new QFormLayout();
    setLayout(flayout);

    QComboBox *presetS = new QComboBox();
    presetS->addItem("Linux G++");
    presetS->addItem("Linux GCC");
    presetS->addItem("Windows MinGW");
    presetS->addItem("Custom");
    flayout->addRow(tr("Compiler Settings Preset"), presetS);
    //todo: add hooks for onchanged and set this value on chage callback

    QFont mfont;
    mfont.setFamily("Courier");
    mfont.setFixedPitch(true);
    mfont.setPointSize(10);

    QLineEdit *cargs = new QLineEdit();
    cargs->setMinimumWidth(350);
    cargs->setFont(mfont);
    cargs->setText("g++ %in -I %inc -o %out --verbose -Wall");
    flayout->addRow(tr("Compiler Arguments"), cargs);
    //todo: add hooks for done editing / onchanged

    QLineEdit *cinf = new QLineEdit();
    cinf->setMinimumWidth(350);
    cinf->setFont(mfont);
    cinf->setText("*.cpp");
    flayout->addRow(tr("Input Files"), cinf);
    //todo: add hooks for done editing / onchanged

    QLineEdit *cicd = new QLineEdit();
    cicd->setMinimumWidth(350);
    cicd->setFont(mfont);
    cicd->setText("%root");
    flayout->addRow(tr("Include Directories"), cicd);
    //todo: add hooks for done editing / onchanged

    QLineEdit *coutf = new QLineEdit();
    coutf->setMinimumWidth(350);
    coutf->setFont(mfont);
    coutf->setText("bin/%rootn");
    flayout->addRow(tr("Output File"), coutf);

    QLabel *compilerabt = new QLabel();
    compilerabt->setTextFormat(Qt::MarkdownText);
    compilerabt->setText(
"Here you can set which compiler to use and the command line flags that get passed to the compiler.\n\n\
There are also default presets to pick from that should work on systems with those respective compilers installed.\n\n\
The following fields are available:\n\n\
**%in**: Parsed list of input files relative paths, determined by input file expression.\n\n\
**%root**: Absolute path to the project root directory.\n\n\
**%rootn**: Name of the project root directory.\n\n\
**%inc**: Parsed include directories absolute path expression, may only be used in compiler arguments section.\n\n\
**%out**: Parsed output absolute path expression, may only be used in compiler arguments section.\n\n\
On Linux systems, you probably already have gcc/g++ installed. Otherwise you may install via your package manager.\n\n\
On Windows systems, it is recommended to install MinGW to *C:\\MinGW*.\n\n\
    ");
    flayout->addRow(tr("Help"), compilerabt);

    QPushButton *okbtn = new QPushButton("OK", this);
    okbtn->setDefault(true);
    okbtn->setAutoDefault(true);
    flayout->addWidget(okbtn);
    //todo: figure out how to make on enter default to save and close dialog

}

void ConfigGen::show() {
    exec();
}
