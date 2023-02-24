#include "stringhandler.h"

StringHandler::StringHandler()
{

}

QString StringHandler::RemoveNumber(QString str)
{
    if(str.isEmpty())
      return QString("");
    QString res;
    //QRegularExpression exp("((?<=\n)\\d+\\s+|^\\d+\\s+)");
    QRegularExpression exp("((?<=\n)\\d+\\t|^\\d+\\s+)");

    str.remove(exp);
    //qDebug().noquote()<<str;

    //qDebug().noquote()<<str;
    return str;
}

QString StringHandler::RemoveEndGdb(QString str)
{
    if(str.isEmpty())
      return QString("");
    QString res;

    auto pos=str.lastIndexOf("(gdb)");
    if(pos==-1)
       return str;
    //qDebug().noquote()<<str.first(pos);
    //str.mid(0,pos);
    return str.mid(0,pos);
}



QMap<QString, QPair<QString, QString> > StringHandler::GetLocalValue(QString str,QMap<QString, QPair<QString, QString>> &res)
{
    QPair<QString, QString> pair;
    //QRegularExpression exp1("(?<=\n).*(?==)|^.*(?==)");
    QRegularExpression exp1("(?<=\n)[^ ].*?(?==)|^.*(?==)");
    //QRegularExpression exp2("(?<==).*(?=\r|\n)|(?<==).*$");
    QRegularExpression exp2("(?<==).*[^}\\s](?=\r|\n)|(?<==).*$|{.*\n.*}|{.*}");



    auto matchname=exp1.globalMatch(str);
    auto matchvalue=exp2.globalMatch(str);

    while(matchname.hasNext())
    {
        auto t1=matchname.next();
        auto t2=matchvalue.next();
        auto name=t1.captured(0).simplified();
        auto value=t2.captured(0).simplified();
        res[name].first=value;
    }
    //qDebug().noquote()<<res;
    return res;
}

QMap<QString, QPair<QString, QString> > StringHandler::GetLocalType(QString str, QMap<QString, QPair<QString, QString> > &res)
{


    return res;

}

QList<QMap<QString, QString> > StringHandler::ToBreakPointInfo(QString  str)
{
    QList<QMap<QString, QString> > res;
    //QRegularExpression exp1("(\d)\s+([a-z]+)\s+([a-z]+)\s+([a-z]+)\s+([a-z\d]+)\s+([a-z\s().:\d]+)");
//    QRegularExpression exp1("(\\d)\\s+([a-z]+)\\s+([a-z]+)\\s+([a-z]+)\\s+([a-z\\d]+)\\s+([a-z\\s().:\\d]+?)\n|"
//                            "(\\d)\\s+([a-z]+)\\s+([a-z]+)\\s+([a-z]+)\\s+([a-z\\d]+)\\s+([a-z\\s().:\\d]+)$");
    QRegularExpression exp1("(\\d)\\s+([a-z]+)\\s+([a-z]+)\\s+([a-z]+)\\s+(0x[a-z\\d]+)\\s+([a-z\\s/()A-Z:]+?..*\\d)");

    auto i=exp1.globalMatch(str);
    while(i.hasNext())
    {
        QMap<QString, QString> map;
        auto t=i.next();
        map["number"]=t.captured(1);
        map["type"]=t.captured(2);
        map["keep"]=t.captured(3);
        map["enable"]=t.captured(4);
        map["address"]=t.captured(5);
        map["file"]=t.captured(6).remove("\r\n").simplified();
        res.append(map);
    }
    //qDebug()<<res;
    return res;

}

QString StringHandler::ToCurrentFileName(QString str)
{
    auto begin=str.indexOf("Located in")+10;
    auto end=str.indexOf("Contains");
    //qDebug()<<str.mid(begin,end-begin).simplified();
    return str.mid(begin,end-begin).simplified();
}

QList<QString> StringHandler::FindBreakPoint(QString str)
{
    QList<QString> res;
    QRegularExpression exp("(Breakpoint|Temporary breakpoint)\\s*(\\d+),\\s*(.*)at(.*):(\\d+)");
    auto i=exp.globalMatch(str);
    while(i.hasNext())
    {
        auto t=i.next();
        res<<t.captured(1).simplified();
        res<<t.captured(2).simplified();
        res<<t.captured(3).simplified();
        res<<t.captured(4).simplified();
        res<<t.captured(5).simplified();
        return res;

    }
    return res;
}

QList<QString> StringHandler::FindLocalPos(QString str)
{
    QList<QString> res;
    QRegularExpression exp("#\\d+\\s*[\\d\\D]*?at\\s+(.*):(\\d+)");
    //
    //#\\d+\\s*.*at\\s+(.*):(\\d+)
    auto i=exp.globalMatch(str);
    while(i.hasNext())
    {
        auto t=i.next();
        res<<t.captured(1);
        res<<t.captured(2);
        return res;
    }
    return res;
}
