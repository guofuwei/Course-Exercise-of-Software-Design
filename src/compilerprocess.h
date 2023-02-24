#ifndef COMPILERPROCESS_H
#define COMPILERPROCESS_H

#include <QObject>
#include <QProcess>
#include <QDebug>
#include <QFileInfo>
#include <QDir>

class CompilerProcess: public QProcess
{
    Q_OBJECT
public:
    CompilerProcess(QString FilePath);
    CompilerProcess();
    ~CompilerProcess();
    void compile();
    bool check();
private:
    QString m_filepath;
signals:
    void setlog(QString);
};

#endif // COMPILERPROCESS_H
