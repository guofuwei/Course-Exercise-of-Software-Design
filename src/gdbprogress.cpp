#include "gdbprogress.h"

GDbProgress::GDbProgress()
{
  this->setProgram("gdb.exe");
  //this->setArguments(QStringList() << "-q" << ".\\t2.exe");
  this->start(QIODevice::ReadWrite);
  qDebug() << QDir::currentPath();

  //this->run("set height 0\n");
  //GetMainFileName();
}

QByteArray GDbProgress::readoutput()
{
  QByteArray bytes, output;
  QTime t;
  QWidget w;

  t.start();
  do {
    //qDebug()<<
      this->waitForReadyRead(200);
    bytes = this->readAll();
    output += bytes;
  } while (!bytes.isEmpty());

 // qDebug()<<"read takes:"<<t.elapsed()<<endl;
  return output;
}

QByteArray GDbProgress::run(QString statement)
{

  if (!statement.isEmpty() &&
      (statement.at(statement.length() - 1) != QChar('\n'))) {
    statement += "\n";
  }
  QString watch = this->readAll();
  if (this->state() == QProcess::Running) {
      QTime t;
      t.start();

     this->write(statement.toLatin1());

     auto str=this->readoutput();
     qDebug()<<statement<<":"<<t.elapsed()<<endl;
     return str;
  }
  return QByteArray();
}

QByteArray GDbProgress::listcode()
{
  auto t = this->readAll();
  if (this->state() == QProcess::Running) {
    QString res=this->run("l 1,1000000\n");
    //QString res = this->readoutput();
    res = StringHandler::RemoveNumber(res);
    res = StringHandler::RemoveEndGdb(res);
    return res.toLatin1();
  }
  return QByteArray();
}

QByteArray GDbProgress::StartRun()
{
  this->readAll();
  if (this->state() == QProcess::Running) {
    if (isrun == true) {
      return QString("the program is running").toLatin1();
    }
    this->write("tbreak main\n");
    this->waitForReadyRead();
    this->readAll();
    this->write("r\n");
    this->isrun = true;
    return this->readoutput();
  }
  return QByteArray();
}

void GDbProgress::ChangeProgram(QString FilePath)
{
    if(QFile(FilePath).exists()==false)
    {
        emit setlog("[gdb] changeprogram 文件不存在");
        return;
    }
    if(!m_filename.isEmpty())
      emit setlog(QString("[gdb] end ")+=m_filename);
    this->close();
    qDebug()<<this->state();
    m_filepath=FilePath;
    m_filepath=QFileInfo(FilePath).absolutePath();
    m_filename=QFileInfo(FilePath).fileName();
    this->setArguments(QStringList()<<FilePath);
    this->start();
    emit programload();
    isrun=false;
    this->run("set height 0\n");
}

bool GDbProgress::HaveSetProgram()
{
    return !(this->arguments()==QStringList());
}

QMap<QString, QPair<QString, QString>> GDbProgress::GetLocalInfo()
{
  QMap<QString, QPair<QString, QString>> res;
  QString statement;
  this->readAll();
  if (this->state() == QProcess::Running) {
    if (isrun == false) {
      return res;
    }
    auto output=this->run("info local\n");
    StringHandler::GetLocalValue(output, res);
    for (auto name : res.keys()) {
      statement += QString("whatis " + name + "\n").toLatin1();
    }
    QString str = this->run(statement);
    for (auto name : res.keys()) {
      auto t = str.mid(str.indexOf("=") + 1, str.indexOf("\n") - str.indexOf("="))
               .simplified();
      res[name].second = t;
      str = str.mid(str.indexOf("\n") + 1);
    }
    return res;
  }
  return res;
}

QList<QMap<QString, QString>> GDbProgress::GetBreakPointInfo()
{
  auto str = this->run("info break\n");
  return StringHandler::ToBreakPointInfo(str);
}

QList<QString> GDbProgress::GetLocalPos()
{
  QList<QString> res;
  QString str = this->run("where\n");
  res = StringHandler::FindLocalPos(str);
  return res;
}

QString GDbProgress::GetCurrentFileName()
{
  auto output = this->run("info source\n");
  return StringHandler::ToCurrentFileName(output);
}


QString GDbProgress::GetMainFileName()
{
  this->run("tbreak main\n");
  this->run("run\n");
   // qDebug()<<this->run("starti\n");
  m_filepath = GetCurrentFileName();
  m_filename = QFileInfo(m_filepath).fileName();
  auto watch=this->run("kill\n");
  
  return m_filename;
}
QString GDbProgress::FileName() { return m_filename; }

QString GDbProgress::FilePath()
{
    return m_filepath;
}
void GDbProgress::on_runprogram()
{
  QByteArray output;
  if (isrun == false) {
    output = this->run("r\n");
    this->waitForFinished(500);
    output+=this->readAll();
    isrun = true;
  } else {
    //output = this->run("c\n");
      emit setlog("[gdb] 程序已运行");
      return ;
  }
  auto str = QString(output);
  // qDebug().noquote()<<str;
  // if(str.indexOf())
  if (str.indexOf("exited normally") != -1) {
    emit setlog(QString("[gdb] ")+m_filepath+"程序已退出");
    isrun = false;
    emit setpostion("", 0, 0);
    emit update();
    return;
  }
  auto list = StringHandler::FindBreakPoint(str);
  if (list.isEmpty()) {
    return;
  }
  qDebug() << list;
  int line = list.at(4).toInt();
  emit setpostion(list.at(3), line, -1);
  emit update();
}

void GDbProgress::on_continueprogram()
{
    QByteArray output;
    if (isrun == true) {
      output = this->run("c\n");
     // isrun = true;
    } else {
        emit setlog("[gdb] 程序未运行");
        return ;
      //output = this->run("c\n");
    }
    auto str = QString(output);
    // qDebug().noquote()<<str;
    // if(str.indexOf())
    if (str.indexOf("exited normally") != -1) {
      emit setlog(QString("[gdb] ")+m_filepath+"程序已退出");
      isrun = false;
      emit setpostion("", 0, 0);
      emit update();
      return;
    }
    auto list = StringHandler::FindBreakPoint(str);
    if (list.isEmpty()) {
      return;
    }
    qDebug() << list;
    int line = list.at(4).toInt();
    emit setpostion(list.at(3), line, -1);
    emit update();

}
void GDbProgress::on_next()
{
  if (isrun == false) {
    // 提示
    qDebug() << "no run";
    emit setlog("[gdb]程序未运行");
    return;
  }
  auto str = this->run("n\n"); // 有待输出
  // 当输出存在std::string 存在闪回
  // https://stackoom.com/cn_en/question/k3qu
  if (str.indexOf("exited normally") != -1) {
    isrun = false;
    emit setlog(QString("[gdb] ")+m_filepath+"程序已退出");
    emit setpostion("", 0, 0);
    emit update();
    return;
  }
  if(0)
  {
      emit setlog(str);
  }
  auto list = this->GetLocalPos();
  if (list.isEmpty()) {
    return;
  }
  emit setpostion(list.at(0), list.at(1).toInt(), -1);
  emit update();
}
void GDbProgress::on_step()
{
  if (isrun == false) {
    // 提示
    emit setlog("[gdb] 程序未运行");
    qDebug() << "no run";
    return;
  }
  auto str = this->run("s\n");
  if (str.indexOf("exited normally") != -1) {
    emit setlog(QString("[gdb] ")+m_filepath+"程序已退出");
    isrun = false;
    emit setpostion("", 0, 0);
    emit update();
    return;
  }
  auto list = this->GetLocalPos();
  if (list.isEmpty()) {
    return;
  }
  emit setpostion(list.at(0), list.at(1).toInt(), -1);
  emit update();
}
void GDbProgress::on_finish()
{
  if (isrun == false) {
    // 提示
    emit setlog("[gdb] 程序未运行");
    qDebug() << "no run";
    return;
  }
  auto str = this->run("finish\n");
  if (str.indexOf("finish not meaningful in the outermost frame") != -1) {
    qDebug() << str;
    qDebug() << "no frame";
    emit setlog("[gdb] 已是最外层程序");
  }
  auto list = this->GetLocalPos();
  if (list.isEmpty()) {
    return;
  }
  emit setpostion(list.at(0), list.at(1).toInt(), -1);
  emit update();
}

void GDbProgress::on_addbreakpoint(QString filename, int line)
{
    if(!HaveSetProgram())
        return;
  QString statement("b ");
  statement += filename;
  statement += ":";
  statement += QString::number(line);
  statement += "\n";
  auto res = this->run(statement);
  qDebug() << res;
  emit updatebreakpoint();
}
void GDbProgress::on_removebreakpoint(QString filename, int line)
{
  QString statement("clear ");
  statement += filename;
  statement += ":";
  statement += QString::number(line);
  statement += "\n";
  auto res = this->run(statement);
  qDebug() << res;
  emit updatebreakpoint();
}
