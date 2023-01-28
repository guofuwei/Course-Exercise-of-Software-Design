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


//    auto i=exp.globalMatch(str);
//    while(i.hasNext())
//    {
//        auto match=i.next();
//        str.remove(match.captured(1));//注意只删除一次

//        if(match.captured(1).indexOf("12")!=-1)
//        {
//            qDebug()<<match.captured(1);
//            qDebug().noquote()<<str;
//        }

//    }


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

QList<QMap<QString, QString> > StringHandler::GetBreakPointInfo(QString  str)
{
    QList<QMap<QString, QString> > res;
    //QRegularExpression exp1("(\d)\s+([a-z]+)\s+([a-z]+)\s+([a-z]+)\s+([a-z\d]+)\s+([a-z\s().:\d]+)");
    QRegularExpression exp1("(\\d)\\s+([a-z]+)\\s+([a-z]+)\\s+([a-z]+)\\s+([a-z\\d]+)\\s+([a-z\\s().:\\d]+?)\n|"
                            "(\\d)\\s+([a-z]+)\\s+([a-z]+)\\s+([a-z]+)\\s+([a-z\\d]+)\\s+([a-z\\s().:\\d]+)$");

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
        qDebug().noquote()<<t.captured(0);
        qDebug().noquote()<<t.captured(2);
        qDebug().noquote()<<t.captured(3);

    }







    return res;

}
