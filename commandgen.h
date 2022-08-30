/*
 * Command Generator Class
 * 
 * C++ IDE by Luigi Pizzolito
 *            Zhang Ruiqing
 *            Ruan Zihang
 *            Lin Zhenmin
 *            Yang Zhaoyi
 * For 1703-ECE Class by Nie Qing and Wu Hao
 * At Beijing Insitute of Techology
*/

#ifndef COMMANDGEN_H
#define COMMANDGEN_H

#include <QSettings>
#include <QDir>

class CommandGen
{
public:
    CommandGen(QDir *dir);
    void update(QDir *dir);
    // commands to generate
    QString compile();
    QString run();
    QString debug();

private:
    QSettings *csettings;
    class ReplaceEngine {
        public:
            ReplaceEngine(QDir *dir, QSettings *cfg);
            void update(QDir *dir);
            // Strings to generate
            QString Gin;
            QString Ginc;
            QString Gout;
            QString Gext;
            QDir *dirP;
        private:
            // Internal strings and utilities
            QSettings *Cfg;
            QString Groot;
            QString Grootn;
            QString rootRplr(QString str);
    };
    ReplaceEngine *rplr;
};

#endif // COMMANDGEN_H
