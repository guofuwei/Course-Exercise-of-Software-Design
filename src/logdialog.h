#ifndef LOGDIALOG_H
#define LOGDIALOG_H

#include <QObject>
#include <QTextEdit>
#include <QTabWidget>

class LogDialog:public QTabWidget
{
public:
    LogDialog(QWidget *parent=nullptr);
    void setlogcontent(QByteArray content);
    QString  LogText();

private:
    QTextEdit* m_logtextedit;
public slots:
    void on_setcontent(QString);

};

#endif // LOGDIALOG_H
