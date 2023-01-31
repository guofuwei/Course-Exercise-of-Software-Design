#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSplitter>

#include <QTextEdit>
#include <QFileDialog>
#include <QLineEdit>
#include <QLayout>
#include <QSplitter>
#include "texteditor.h"
#include "gdbprogress.h"
#include "logdialog.h"
#include "stringhandler.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent=nullptr);
    ~MainWindow();

private slots:
    void on_actionadd_triggered();

    void on_actionrunstatement_triggered();

    void on_actionlist_triggered();

    void on_actiontest_triggered();

private:
    Ui::MainWindow *ui;

    QLineEdit *m_statementlineedit;
    QGridLayout *m_laypout;

    TextEditor * t;
    GDbProgress *m_progress;
    LogDialog *m_logdailog;

signals:
    void runprogram();

};
#endif // MAINWINDOW_H
