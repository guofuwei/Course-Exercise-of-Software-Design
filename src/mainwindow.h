#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSplitter>
#include <QTreeWidget>
#include <QTreeWidgetItem>

// #include "texteditor.h"//ui包含
#include "audiorecord.h"
#include "compilerprocess.h"
#include "gdbprogress.h"
#include "stringhandler.h"
// #include "stringhandler.h"
#include <QTreeWidgetItem>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
  void init();
  void DisableAll();
  void EnableAll();
  void CompileCode(QString filepath, QStringList extra = QStringList());
  void CompileCurrentPage();
  void BreakPointTreeWidgetUpdate();
  void LocalsTreeWidgetUpdate();
  void StackTreeWidgetUpdate();

 private slots:

  void on_actionRun_triggered();
  void on_actionNext_triggered();
  void on_actionStep_triggered();
  void on_actionFinish_triggered();
  void on_listfile(QString name, int line, int index);
  void on_tablechange(QString);
  void on_programload();
  void on_update();

  void on_actionOpen_Folder_triggered();

  void on_sourceTreeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);

  void on_actionContinue_triggered();

  void on_actioncompile_triggered();

  void on_pushButtonSavePic_clicked();

  void on_pushButtonClearAll_clicked();

  void on_pushButtonStartRecord_clicked();

  void on_pushButtonStopRecord_clicked();

  void on_variableDeleteToolButton_clicked();


  void on_pushButtonStartPlay_clicked();

  void on_pushButtonStopPlay_clicked();

 signals:

  void runprogram();
  void next();
  void step();
  void finish();
  void continueprogram();
  void setlog(QString);

 private:
  Ui::MainWindow *ui;
  GDbProgress *m_progress;
  QString m_sourceFilename;

  QString m_workdir;

  AudioRecord *m_audio_record;
};
#endif  // MAINWINDOW_H
