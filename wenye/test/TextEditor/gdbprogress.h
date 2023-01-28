#ifndef GDBPROGRESS_H
#define GDBPROGRESS_H
#include <QProcess>
#include <QDebug>
#include <QPair>
#include "stringhandler.h"


class GDbProgress:public QProcess
{
    Q_OBJECT
public:
    GDbProgress();
    QByteArray readoutput();
    QByteArray run(QString);
    QByteArray listcode();
    QByteArray StartRun();
    //QByteArray GetLocalInfo();
    //QByteArray AddBreakPoint(int);
    QMap<QString,QPair<QString,QString>> GetLocalInfo();

private:
    bool isrun=0;
};

#endif // GDBPROGRESS_H
