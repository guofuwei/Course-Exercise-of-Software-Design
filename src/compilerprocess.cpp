#include "compilerprocess.h"

CompilerProcess::CompilerProcess(QString FilePath) {
  // qDebug()<<FilePath;
  m_filepath = FilePath;
}

CompilerProcess::CompilerProcess() {}

CompilerProcess::~CompilerProcess() {}

void CompilerProcess::compile() {
  //    if(!this->check())
  //    {
  //        return;
  //    }
  auto fileinfo = QFileInfo(m_filepath);
  QString dst("./temp/");
  dst += fileinfo.completeBaseName();
  dst += ".exe";
  this->setArguments(QStringList() << m_filepath << "-g"
                                   << "-o" << dst);
  qDebug() << this->program();
  qDebug() << this->arguments();
  this->start();  //将MINGW bin加入系统路径
  // QString t=this->readAllStandardError();
  this->waitForFinished();
  QString information = this->readAllStandardError();
  if (information.isEmpty())
    emit setlog(QString("[compiler] ") + m_filepath + "编译成功");
  else
    emit setlog(QString("[compiler error] ") + information);
  this->close();
}

bool CompilerProcess::check() {
  auto FilePath = m_filepath;
  if (FilePath.isEmpty()) {
    emit setlog("[compiler error] 路径为空");
    return false;
  }
  auto file = QFile(FilePath);
  if (!file.exists()) {
    //文件不存在
    emit setlog("[compiler error] 文件不存在");
    return false;
  }
  auto fileinfo = QFileInfo(FilePath);
  // qDebug()<<fileinfo.suffix();
  if (fileinfo.suffix() == "c") {
    this->setProgram("gcc.exe");
  } else if (fileinfo.suffix() == "cpp") {
    this->setProgram("g++.exe");
  } else {
    //文件类型不对
    emit setlog("[compiler error]文件类型错误");
    return false;
  }

  this->setArguments(QStringList() << "-v");
  this->start();
  this->waitForFinished(100);
  QByteArray out = this->readAllStandardError();
  if (out.indexOf("gcc version") == -1) {
    //不存在软件
    emit setlog("[compiler error] 编译器不存在");
    return false;
  };
  //  qDebug() << this->state();
  this->close();
  //  qDebug() << this->state();
  QDir dir("./temp");
  if (!dir.exists()) {
    dir.mkpath(".");
  }

  return true;
}
