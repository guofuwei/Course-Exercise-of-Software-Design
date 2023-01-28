#include "logdialog.h"

LogDialog::LogDialog()
{
    m_logtextedit=new QTextEdit();
    this->addTab(m_logtextedit,"log");
}

void LogDialog::setlogcontent(QByteArray content)
{
    this->m_logtextedit->append(content);
    if(!content.isEmpty())
      this->m_logtextedit->append("\n");
    this->m_logtextedit->moveCursor(QTextCursor::End);
    this->setCurrentIndex(0);

}

QString LogDialog::LogText()
{
   return this->m_logtextedit->toPlainText();
}
