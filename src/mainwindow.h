#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSplitter>

//#include "texteditor.h"
#include "gdbprogress.h"
#include "stringhandler.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void init();

private slots:
    void on_actionRun_triggered();
signals:
    void runprogram();
private:
    Ui::MainWindow *ui;
    GDbProgress *m_progress;
};
#endif // MAINWINDOW_H
