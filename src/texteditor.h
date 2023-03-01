#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H

#include <Qsci/qsciapis.h>
#include <Qsci/qscilexercpp.h>
#include <Qsci/qsciscintilla.h>

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QLayout>
#include <QMessageBox>
#include <QObject>
#include <QPalette>
#include <QPlainTextEdit>
#include <QSizePolicy>
#include <QTabBar>
#include <QTabWidget>
#include <QToolBar>
#include <QWaitCondition>

// #include "gdbprogress.h"

class TextEditor : public QTabWidget {
  Q_OBJECT
 public:
  TextEditor(QWidget *parent = nullptr);
  // void init();
  void initsci();
  void newpage(QString name);
  void readfromfile(QString filename);
  void setcontent(QByteArray, int index = -1);
  void setselected(int line, int index = -1);
  bool changepage(QString name);
  void removeallbreakpoint();
  void addannotate(int line, QString);
  void addcurrentannotate(QString);
  void replacecurrentannotate(QString);
  QString getcurrentannotate();
  QString getfilename(int index);
  QByteArray GetContent(int index = -1);
  void SaveContent();

 private slots:
  void on_margin_clicked(int, int, Qt::KeyboardModifiers);
  void on_table_close(int index);
  void on_table_change(int index);
 public slots:
  void on_setpostion(QString name, int line, int index = -1);
  void on_sendcontent(QString, QString, int, int);
 signals:
  void listcodeforcurrentfile(QString, int, int);
  void addbreakpoint(QString, int);
  void removebreakpoint(QString, int);
  void sourcefilechange(QString);

 private:
  QPlainTextEdit *m_editor;
  bool m_iseditable;
  QWidget *m_mainwidget;
  QList<QsciScintilla *> m_scilist;
  QList<QString> m_filepathlist;

  // QsciScintilla *m_sci;

  int m_marknumber;
  QImage m_imagerihtarrow;
};

#endif  // TEXTEDITOR_H
