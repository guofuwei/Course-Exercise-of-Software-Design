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

    QByteArray RunNext();
    QByteArray RunStep();

    QMap<QString,QPair<QString,QString>> GetLocalInfo();
    QList<QMap<QString, QString> > GetBreakPointInfo();


    QList<QString>  GetLocalPos();
    QString GetCurrentFileName();
    QString GetMainFileName();
    QString FileName();
public slots:
    void on_runprogram();
    void on_next();
    void on_step();
    void on_listcodeforcurrentfile(QString,int,int);


private:
    bool isrun=0;
    QString m_filename;
signals:
    void setcontent(QString,QString,int,int);
    void setpostion(QString,int,int);
};

#endif // GDBPROGRESS_H
