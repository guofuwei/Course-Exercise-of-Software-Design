﻿#ifndef GDBPROGRESS_H
#define GDBPROGRESS_H
#include <QProcess>
#include <QDebug>
#include <QPair>
#include <QDir>
#include <QDateTime>
#include <QTime>
#include <QWidget>
#include "stringhandler.h"

class GDbProgress : public QProcess
{
  Q_OBJECT
public:
  GDbProgress();
  QByteArray readoutput();
  QByteArray run(QString);
  QByteArray listcode();
  QByteArray StartRun();
  void ChangeProgram(QString FilePath);
  bool HaveSetProgram();

  QMap<QString, QPair<QString, QString>> GetLocalInfo();
  QList<QMap<QString, QString>> GetBreakPointInfo();
  QList<QMap<QString, QString>> GetStackInformation();
  QString GetExpression(QString statement);

  QList<QString> GetLocalPos();
  QString GetCurrentFileName();
  QString GetMainFileName();
  QString FileName();
  QString FilePath();
public slots:
  void on_runprogram();
  void on_continueprogram();
  void on_next();
  void on_step();
  void on_finish();
  void on_addbreakpoint(QString, int);
  void on_removebreakpoint(QString, int);

private:
  bool isrun = 0;
  QString m_filename;
  QString m_filepath;
signals:
  void setcontent(QString, QString, int, int);
  void setpostion(QString, int, int);
  void update();
  void updatebreakpoint();
  void setlog(QString);
  void programload();

};

#endif // GDBPROGRESS_H
