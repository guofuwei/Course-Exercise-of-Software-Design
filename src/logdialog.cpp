#include "logdialog.h"

LogDialog::LogDialog(QWidget *parent):QTabWidget(parent)
{
    m_logtextedit=new QTextEdit();
    this->setTabPosition(QTabWidget::West);
    this->setTabShape(QTabWidget::Triangular);
    this->addTab(m_logtextedit,"log1");
    m_logtextedit->setReadOnly(1);

}

void LogDialog::setlogcontent(QByteArray content)
{
    this->m_logtextedit->append(content);
//    if(!content.isEmpty())
//      this->m_logtextedit->append("\n");
    this->m_logtextedit->moveCursor(QTextCursor::End);
    this->setCurrentIndex(0);
}

QString LogDialog::LogText()
{
    return this->m_logtextedit->toPlainText();
}

void LogDialog::on_setcontent(QString str)
{
    this->setlogcontent(str.toUtf8());
}
