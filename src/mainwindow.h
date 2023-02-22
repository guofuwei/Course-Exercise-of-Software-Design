#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSplitter>
#include <QTreeWidget>

// #include "texteditor.h"
#include "gdbprogress.h"
#include "stringhandler.h"

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
  void BreakPointTreeWidgetUpdate();

private slots:
  void on_actionRun_triggered();
  void on_actionNext_triggered();
  void on_actionStep_triggered();
  void on_actionFinish_triggered();
  void on_update();

signals:
  void runprogram();
  void next();
  void step();
  void finish();

private:
  Ui::MainWindow *ui;
  GDbProgress *m_progress;
};
#endif // MAINWINDOW_H
