#ifndef LOGDIALOG_H
#define LOGDIALOG_H

#include <QObject>
#include <QTextEdit>
#include <QTabWidget>

class LogDialog:public QTabWidget
{
public:
    LogDialog();
    void setlogcontent(QByteArray content);
    QString  LogText();

private:
    QTextEdit* m_logtextedit;
};

#endif // LOGDIALOG_H
