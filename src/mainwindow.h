#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSplitter>
#include <QTreeWidget>

// #include "texteditor.h"
#include "gdbprogress.h"
#include "stringhandler.h"
#include <QTreeWidgetItem>

QT_BEGIN_NAMESPACE
namespace Ui {
  class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
  void init();
  void BreakPointTreeWidgetUpdate();
  void LocalsTreeWidgetUpdate();

private slots:
  void on_actionRun_triggered();
  void on_actionNext_triggered();
  void on_actionStep_triggered();
  void on_actionFinish_triggered();
  void on_update();

  void on_actionOpen_Folder_triggered();

  void on_sourceTreeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);

  void on_actionContinue_triggered();

signals:
  void runprogram();
  void next();
  void step();
  void finish();
  void continueprogram();

private:
  Ui::MainWindow *ui;
  GDbProgress *m_progress;
  QString m_sourceFilename;

};
#endif // MAINWINDOW_H
