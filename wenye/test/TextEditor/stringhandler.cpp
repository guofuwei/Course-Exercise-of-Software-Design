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
    return str.first(pos);
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
