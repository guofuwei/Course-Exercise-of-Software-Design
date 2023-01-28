#include "gdbprogress.h"

GDbProgress::GDbProgress()
{
    this->setProgram(".\\bin\\gdb.exe");
    //this->setArguments(QStringList()<<"-q"<<".\\test.exe");//-q删除帮助信息
    this->setArguments(QStringList()<<"-q"<<".\\t.exe");
    this->start(QIODevice::ReadWrite);
}

QByteArray GDbProgress::readoutput()
{
     //qDebug()<<this->state();
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
    this->readAll();
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
        //qDebug()<<res;

        for(auto name:res.keys())
        {
            this->write(QString("whatis "+name+"\n").toLatin1());
            auto str=this->readoutput();
            str=str.mid(str.indexOf("=")+1,str.indexOf("\n")-str.indexOf("=")).simplified();

            res[name].second=str;
        }
        //qDebug().noquote()<<

        return res;
    }
    return res;
}


