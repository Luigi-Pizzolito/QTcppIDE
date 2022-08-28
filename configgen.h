#ifndef CONFIGGEN_H
#define CONFIGGEN_H

#include <QDialog>
#include <QObject>
#include <QWidget>

class ConfigGen : public QDialog
{
    Q_OBJECT
public:
    explicit ConfigGen(QWidget *parent = 0);
public slots:
    void show();
    //todo: save, gencompile, genrun, gendebug
private:
    //todo: loadsave, createoutputdirs
};

#endif // CONFIGGEN_H
