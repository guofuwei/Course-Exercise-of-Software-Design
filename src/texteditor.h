﻿#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H

#include <QObject>
#include <QTabWidget>
#include <QPlainTextEdit>
#include <QPalette>
#include <QDebug>
#include <QTabBar>
#include <QSizePolicy>
#include <QLayout>
#include <QDir>
#include <QToolBar>
#include <QFile>
#include <QMessageBox>
#include <QFileInfo>
#include <QWaitCondition>

#include <Qsci/qsciscintilla.h>
#include <Qsci/qscilexercpp.h>
#include <Qsci/qsciapis.h>

//#include "gdbprogress.h"

class TextEditor :public QTabWidget
{
    Q_OBJECT
public:
    TextEditor(QWidget *parent=nullptr);
    void init();
    void initsci();
    void newpage(QString name);
    void readfromfile(QString filename);
    void setcontent(QByteArray,int index=-1);
    void setselected(int line,int index =-1);
    bool changepage(QString name);

private slots:
    void on_margin_clicked(int, int, Qt::KeyboardModifiers);
    void on_table_close(int index);
public slots:
    void on_setpostion(QString name,int line,int index=-1);
    void on_sendcontent(QString,QString,int,int);
signals:
    void listcodeforcurrentfile(QString,int,int);
private:
    QPlainTextEdit* m_editor;
    bool m_iseditable;
    QWidget *m_mainwidget;
    QList<QsciScintilla *> m_scilist;

    QsciScintilla *m_sci;

    int m_marknumber;
};

#endif // TEXTEDITOR_H
