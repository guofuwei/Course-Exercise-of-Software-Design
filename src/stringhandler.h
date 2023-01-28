#ifndef STRINGHANDLER_H
#define STRINGHANDLER_H

#include <QObject>
#include <QString>
#include <QRegularExpression>
#include <QMap>
#include <QPair>

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

};

#endif // STRINGHANDLER_H
