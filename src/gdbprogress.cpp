#include "gdbprogress.h"

GDbProgress::GDbProgress()
{
  this->setProgram("gdb.exe");
  this->setArguments(QStringList() << "-q"
                     << ".\\test.exe");
  this->start(QIODevice::ReadWrite);
  qDebug() << this->state();
  qDebug() << QDir::currentPath();
  this->run("set height 0\n");
  GetMainFileName();
}

QByteArray GDbProgress::readoutput()
{
  QByteArray bytes, output;
  do {
    this->waitForReadyRead(400);
    bytes = this->readAll();
    output += bytes;
  } while (!bytes.isEmpty());
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
    this->write(statement.toLatin1());
    return this->readoutput();
  }
  return QByteArray();
}

QByteArray GDbProgress::listcode()
{
  auto t = this->readAll();
  if (this->state() == QProcess::Running) {
    this->write("l 1,1000000\n");
    QString res = this->readoutput();
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

QMap<QString, QPair<QString, QString>> GDbProgress::GetLocalInfo()
{
  QMap<QString, QPair<QString, QString>> res;
  this->readAll();
  if (this->state() == QProcess::Running) {
    if (isrun == false) {
      return res;
    }
    this->write("info local\n");
    StringHandler::GetLocalValue(this->readoutput(), res);
    for (auto name : res.keys()) {
      this->write(QString("whatis " + name + "\n").toLatin1());
      auto str = this->readoutput();
      str = str.mid(str.indexOf("=") + 1, str.indexOf("\n") - str.indexOf("="))
            .simplified();
      res[name].second = str;
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
  //qDebug() << this->run("break 6\n");
   this->run("tbreak 2\n");
  // qDebug() << this->run("y\n");
  this->run("run\n");
  m_filename = GetCurrentFileName();
  this->run("c\n");
  return m_filename;
}

QString GDbProgress::FileName() { return m_filename; }

void GDbProgress::on_runprogram()
{
  QByteArray output;
  if (isrun == false) {
    output = this->run("r\n");
    isrun = true;
  } else {
    output = this->run("c\n");
  }
  auto str = QString(output);
  // qDebug().noquote()<<str;
  // if(str.indexOf())
  if (str.indexOf("exited normally") != -1) {
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
    return;
  }
  auto str = this->run("n\n"); // 有待输出
  // 当输出存在std::string 存在闪回
  // https://stackoom.com/cn_en/question/k3qu
  if (str.indexOf("exited normally") != -1) {
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

void GDbProgress::on_step()
{
  if (isrun == false) {
    // 提示
    qDebug() << "no run";
    return;
  }
  auto str = this->run("s\n");
  if (str.indexOf("exited normally") != -1) {
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
    qDebug() << "no run";
    return;
  }
  auto str = this->run("finish\n");
  if (str.indexOf("finish not meaningful in the outermost frame") != -1) {
    qDebug() << str;
    qDebug() << "no frame";
  }
  auto list = this->GetLocalPos();
  if (list.isEmpty()) {
    return;
  }
  emit setpostion(list.at(0), list.at(1).toInt(), -1);
  emit update();
}

void GDbProgress::on_listcodeforcurrentfile(QString name, int line, int index)
{
  // emit setpostion(name,line,index);
  emit setcontent(this->listcode(), name, line, index);
}

void GDbProgress::on_addbreakpoint(QString filename, int line)
{
  QString statement("b ");
  statement += filename;
  statement += ":";
  statement += QString::number(line);
  statement += "\n";
  auto res = this->run(statement);
  qDebug() << res;
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
}
