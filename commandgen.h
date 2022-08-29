#ifndef COMMANDGEN_H
#define COMMANDGEN_H

#include <QSettings>
#include <QDir>

class CommandGen
{
public:
    CommandGen(QDir *dir);

    void update();

    QString compile();
    QString run();
    QString debug();

private:
    QSettings *csettings;
    class ReplaceEngine {
        public:
            ReplaceEngine(QDir *dir, QSettings *cfg);
            void update();
            QString Gin;
            QString Ginc;
            QString Gout;
            QString Gext;
            QDir *dirP;
        private:

            QSettings *Cfg;
            QString Groot;
            QString Grootn;
            QString rootRplr(QString str);
    };
    ReplaceEngine *rplr;
};

#endif // COMMANDGEN_H
