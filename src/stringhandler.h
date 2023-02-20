#ifndef STRINGHANDLER_H
#define STRINGHANDLER_H

#include <QObject>
#include <QString>
#include <QRegularExpression>
#include <QMap>
#include <QPair>
#include <QDebug>

class StringHandler
{
public:
    StringHandler();
    static QString  RemoveNumber(QString);//除去list时的行号
    static QString  RemoveEndGdb(QString);
    static QMap<QString,QPair<QString,QString>> GetLocalValue(QString,QMap<QString, QPair<QString, QString> >&res);
                                                             // =QMap<QString, QPair<QString, QString>>());//获得本地变量的值
    static QMap<QString,QPair<QString,QString>> GetLocalType(QString,QMap<QString, QPair<QString, QString> >&res);
                                                              //=QMap<QString, QPair<QString, QString>>());//获得本地变量的类型
    static QList<QMap<QString,QString>> ToBreakPointInfo(QString);
    static QString ToCurrentFileName(QString);
    static QList<QString> FindBreakPoint(QString);
    static QList<QString> FindLocalPos(QString);

};

#endif // STRINGHANDLER_H
