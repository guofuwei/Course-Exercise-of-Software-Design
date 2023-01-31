#include "gdbprogress.h"

GDbProgress::GDbProgress()
{
    this->setProgram(".\\bin\\gdb.exe");
    //this->setArguments(QStringList()<<"-q"<<".\\test.exe");//-q删除帮助信息
    this->setArguments(QStringList()<<"-q"<<".\\t.exe");
    this->start(QIODevice::ReadWrite);
    this->run("set height 0\n");
}

QByteArray GDbProgress::readoutput()
{
     QByteArray bytes,output;
     do
     {
    this->waitForReadyRead(1000);
     bytes=this->readAll();
     output+=bytes;
    }while(!bytes.isEmpty());

    return output;
}

QByteArray GDbProgress::run(QString statement)
{
    if(!statement.isEmpty()&&(statement.at(statement.length()-1)!=QChar('\n')))
      statement+="\n";
    QString watch=this->readAll();
    if(this->state()==QProcess::Running)
    {
        this->write(statement.toLatin1());
        return this->readoutput();
    }
    return QByteArray();
}

QByteArray GDbProgress::listcode()
{
    auto t=this->readAll();
    if(this->state()==QProcess::Running)
    {
        this->write("l 1,1000000\n");
        QString res=this->readoutput();
        res=StringHandler::RemoveNumber(res);
        res=StringHandler::RemoveEndGdb(res);
        return res.toLatin1();
    }
    return QByteArray();

}

QByteArray GDbProgress::StartRun()
{
    this->readAll();

    if(this->state()==QProcess::Running)
    {
        if(isrun==true)
            return QString("the program is running").toLatin1();
        this->write("tbreak main\n");
        this->waitForReadyRead();
        this->readAll();

        this->write("r\n");
        this->isrun=true;
        return this->readoutput();
    }

    return QByteArray();
}

QMap<QString,QPair<QString,QString>> GDbProgress::GetLocalInfo()
{
    QMap<QString,QPair<QString,QString>> res;
    this->readAll();
    if(this->state()==QProcess::Running)
    {
        if(isrun==false)
            return res;
        this->write("info local\n");
        StringHandler::GetLocalValue(this->readoutput(),res);

        for(auto name:res.keys())
        {
            this->write(QString("whatis "+name+"\n").toLatin1());
            auto str=this->readoutput();
            str=str.mid(str.indexOf("=")+1,str.indexOf("\n")-str.indexOf("=")).simplified();

            res[name].second=str;
        }

        return res;
    }
    return res;
}

QList<QMap<QString, QString> > GDbProgress::GetBreakPointInfo()
{
    auto str=this->run("info break\n");
    return StringHandler::ToBreakPointInfo(str);
}

QString GDbProgress::GetCurrentFileName()
{
    auto  output=this->run("info source\n");
    return StringHandler::ToCurrentFileName(output);

}

void GDbProgress::on_runprogram()
{
    QByteArray output;
    if(isrun==false)
    {
      output=this->run("r\n");
      isrun=true;
    }
    else
    {
       output=this->run("c\n");
    }
    auto str=QString(output);
    //qDebug().noquote()<<str;
    //if(str.indexOf())

    if(str.indexOf("exited normally")!=-1)
    {
        isrun=false;
        return;
    }

    auto list=StringHandler::FindBreakPoint(str);
    if(list.isEmpty())
        return;
    qDebug()<<list;
    int line=list.at(4).toInt();
    emit setpostion(list.at(3),line,-1);
}


