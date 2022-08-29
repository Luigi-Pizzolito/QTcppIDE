#include "commandgen.h"

#include "globaldefs.h"

CommandGen::CommandGen(QDir *dir)
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
    return comm;
}

QString CommandGen::run() {
    QString comm;
    if (csettings->value("ExtCon").toBool()) {
        comm = rplr->Gext;
    } else {
        comm = rplr->Gout;
    }
    return comm;
}

QString CommandGen::debug() {
    QString comm;
    return comm;
}

CommandGen::ReplaceEngine::ReplaceEngine(QDir *dir, QSettings *cfg) : dirP(dir),Cfg(cfg) {
    update();
}

void CommandGen::update() {
    rplr->update();
}

void CommandGen::ReplaceEngine::update() {
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
    str.replace("%rootn", Grootn);
    str.replace("%root", Groot);
    return str;
}
