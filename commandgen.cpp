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

#include "commandgen.h"

#include "globaldefs.h"

CommandGen::CommandGen(QDir *dir) : dir(dir)
{
    // Load Settings
    csettings = new QSettings(COMPANY, APPNAME);
    // Start ReplaceEngine
    rplr = new ReplaceEngine(dir, csettings);
}

QString CommandGen::compile() {
    // Create output path
    QString outdir = rplr->Gout;
    QStringList outp = outdir.split("/");
    outp.removeLast();
    outdir =  outp.join("/");
    rplr->dirP->mkpath(outdir);

    // Generate command
    QString comm;
    comm = csettings->value("CompileArgs").toString();
    comm.replace("%out", rplr->Gout);
    comm.replace("%inc", rplr->Ginc);
    comm.replace("%in", rplr->Gin);
#ifdef _WIN32
    comm = "cmd /k echo Compiling... & "+comm;
    comm+=" & pause & exit";
#endif
    return comm;
}

QString CommandGen::run() {
    QString comm;
    // return run command, depending if external console is selected
    if (csettings->value("ExtCon").toBool()) {
        comm = rplr->Gext;
    } else {
        // for UNIX systems, just run the output file
        comm = rplr->Gout;
        // for WINDOWS we must launch it inside cmd, cd to the directory first and pause after.
#ifdef _WIN32
        comm = "cmd /k ";
        if (rplr->Gout.contains("/")) {
            QStringList subp = rplr->Gout.split("/");
            QString exe = subp.back();
            subp.pop_back();
            comm += "cd " + subp.join("/") + " & ";
            comm += exe;
        } else {
            comm += rplr->Gout;

        }
        comm += " & pause & exit";
#endif
    }
    return comm;
}

QString CommandGen::debug() {
    QString comm;
    //todo: return debug command
    return comm;
}

CommandGen::ReplaceEngine::ReplaceEngine(QDir *dir, QSettings *cfg) : dirP(dir),Cfg(cfg) {
    update(dir); // initial update
}

void CommandGen::update(QDir *dir) {
    rplr->update(dir); // forward update to replace engine
}

void CommandGen::ReplaceEngine::update(QDir *dir) {
    // update internal generated strings
    dirP = dir;
    // generate %root and %rootn
    Groot = dirP->absolutePath();
    Grootn = dirP->dirName();
    // generate %in
    QStringList filters = Cfg->value("InFiles").toString().split(" ");
    dirP->setNameFilters(filters);
    Gin = dirP->entryList().join(" ");
    // generate %inc
    Ginc = rootRplr(Cfg->value("IncDirs").toString());
    // generate %out
    Gout = rootRplr(Cfg->value("OutFile").toString());
    // generate ext
    Gext = rootRplr(Cfg->value("ConArgs").toString()).replace("%out", "./"+Gout);
}

QString CommandGen::ReplaceEngine::rootRplr(QString str) {
    // utility for replacing root dir labels
    str.replace("%rootn", Grootn);
    str.replace("%root", Groot);
    return str;
}
