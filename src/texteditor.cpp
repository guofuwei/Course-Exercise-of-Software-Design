#include "texteditor.h"

// http://t.csdn.cn/CHxMr  viewpor
TextEditor::TextEditor(QWidget *parent) : QTabWidget(parent)
{
  m_iseditable = 1;
  m_marknumber = 0;
  this->m_imagerihtarrow =
      QImage("://resources/icons-icons/arrow-right.png").scaled(QSize(16, 16));
  this->setTabsClosable(1);
  connect(this, SIGNAL(tabCloseRequested(int)), this,
          SLOT(on_table_close(int)));
  connect(this, SIGNAL(currentChanged(int)), this, SLOT(on_table_change(int)));
}

void TextEditor::initsci()
{
  auto sciScintilla = new QsciScintilla(this);
  QsciLexerCPP *textLexer = new QsciLexerCPP; // 创建一个词法分析器
  QsciAPIs *apis = new QsciAPIs(textLexer);
  apis->load("apis.txt");
  apis->prepare();
  //    textLexer->setPaper(QColor(255,255,255));//文本区域背景色
  //    textLexer->setColor(QColor(0,170,0),QsciLexerCPP::Comment);
  //    textLexer->setColor(QColor(0,170,0),QsciLexerCPP::CommentLine);//设置自带的注释行为灰色
  //    textLexer->setColor(QColor(170,0,0),QsciLexerCPP::PreProcessor);//标签
  //    textLexer->setColor(QColor(Qt::blue),QsciLexerCPP::Keyword);//id选择器
  //    m_sci->setLexer(textLexer);//给QsciScintilla设置词法分析器
  //    m_sci->setFolding(QsciScintilla::BoxedTreeFoldStyle);//折叠样式
  //    m_sci->setFoldMarginColors(Qt::gray,Qt::lightGray);//折叠栏颜色
  //    m_sci->setWrapIndentMode(QsciScintilla::WrapIndentSame);
  //    m_sci->setTabIndents(true);//True如果行前空格数少于tabWidth，补齐空格数,False如果在文字前tab同true，如果在行首tab，则直接增加tabwidth个空格
  //    m_sci->setAutoCompletionSource(QsciScintilla::AcsAll);
  //    //设置源，自动补全所有地方出现的
  //    m_sci->setAutoCompletionCaseSensitivity(true); //设置自动补全大小写敏感
  //    m_sci->setAutoCompletionThreshold(1);
  //    m_sci->setCaretLineVisible(true);//选中高亮
  //    m_sci->setBraceMatching(QsciScintilla::SloppyBraceMatch);//括号
  //    m_sci->setMarginType(0,QsciScintilla::NumberMargin);//设置编号为0的页边显示行号。
  //    m_sci->setMarginLineNumbers(0,true);//对该页边启用行号
  //    m_sci->setMarginWidth(0,15);//设置页边宽度
  //    //m_sci->setmargin
  //    m_sci->setMarginType(1,QsciScintilla::SymbolMargin);
  //    m_sci->setWrapMode(QsciScintilla::WrapWord);
  //    m_sci->setMarginSensitivity(1, true);
  //    m_sci->setMarginsBackgroundColor(QColor("#bbfaae"));
  //    m_sci->setMarginMarkerMask(1, 0x02);
  //    connect(m_sci, SIGNAL(marginClicked(int, int,
  //    Qt::KeyboardModifiers)),this,
  //            SLOT(on_margin_clicked(int, int, Qt::KeyboardModifiers)));
  //    //m_sci->setMarkerForegroundColor(QColor(Qt::red));
  sciScintilla->setLexer(textLexer); // 给QsciScintilla设置词法分析器
  sciScintilla->setReadOnly(0);
  sciScintilla->markerDefine(m_imagerihtarrow, 2);
  sciScintilla->markerDefine(QsciScintilla::Circle, 1);
  sciScintilla->setWrapIndentMode(QsciScintilla::WrapIndentSame);
  sciScintilla->setTabIndents(
      true); // True如果行前空格数少于tabWidth，补齐空格数,False如果在文字前tab同true，如果在行首tab，则直接增加tabwidth个空格
  sciScintilla->setAutoIndent(true);
  sciScintilla->setIndentationGuides(true);
  sciScintilla->setAutoCompletionSource(
      QsciScintilla::AcsAll); // 设置源，自动补全所有地方出现的
  sciScintilla->setAutoCompletionCaseSensitivity(
      true); // 设置自动补全大小写敏感
  sciScintilla->setAutoCompletionThreshold(1);
  sciScintilla->setCaretLineVisible(true);                         // 选中高亮
  sciScintilla->setBraceMatching(QsciScintilla::SloppyBraceMatch); // 括号
  // sciScintilla->setWhitespaceVisibility(QsciScintilla::WsVisible);//此时空格为点，\t为箭头
  // sciScintilla->setWhitespaceSize(2);//空格点大小
  sciScintilla->setFolding(QsciScintilla::BoxedTreeFoldStyle); // 折叠样式
  sciScintilla->setFoldMarginColors(Qt::gray, Qt::lightGray);  // 折叠栏颜色
  sciScintilla->setMarginType(
      0, QsciScintilla::NumberMargin);         // 设置编号为0的页边显示行号。
  sciScintilla->setMarginLineNumbers(0, true); // 对该页边启用行号
  sciScintilla->setMarginWidth(0, 50);         // 设置页边宽度
  // m_sci->setmargin
  sciScintilla->setMarginType(1, QsciScintilla::SymbolMargin);
  sciScintilla->setWrapMode(QsciScintilla::WrapWord);
  sciScintilla->setMarginSensitivity(1, true);
  sciScintilla->setMarginsBackgroundColor(QColor("#bbfaae"));
  sciScintilla->setMarginMarkerMask(1, 0b00110);
  sciScintilla->markerDefine(QsciScintilla::Circle, 1);
  connect(sciScintilla, SIGNAL(marginClicked(int, int, Qt::KeyboardModifiers)),
          this, SLOT(on_margin_clicked(int, int, Qt::KeyboardModifiers)));
  m_scilist.append(sciScintilla);
}

void TextEditor::newpage(QString path)
{
  auto name = QFileInfo(path).fileName();
  for (int i = 0; i < this->count(); i++)
  {
    if (this->tabText(i) == name)
    {
      this->setCurrentIndex(i);
      return;
    }
  }
  initsci();
  this->m_filepathlist.append(path);
  this->addTab(m_scilist.last(), name);
}

void TextEditor::readfromfile(QString filepath)
{
  QFile file(filepath);
  auto filename = QFileInfo(file).fileName();
  // qDebug() << filename;
  if (!file.open(QIODevice::ReadOnly))
  {
    QMessageBox::warning(0, "warning", "can not open");
    return;
  }
  this->newpage(filepath);
  this->m_scilist.last()->setText(file.readAll());
  file.close();
}

void TextEditor::setcontent(QByteArray content, int index)
{
  if (index == -1)
  {
    index = currentIndex();
  }
  m_scilist.at(index)->setText(content);
}

void TextEditor::setselected(int line, int index)
{
  if (index == -1)
  {
    index = currentIndex();
  }
  m_scilist.at(index)->setCursorPosition(line, index);
}

bool TextEditor::changepage(QString name)
{
  for (int i = 0; i < this->count(); i++)
  {
    if (this->tabText(i) == name)
    {
      this->setCurrentIndex(i);
      return true;
    }
  }
  return false;
}

void TextEditor::removeallbreakpoint()
{
  auto i = currentIndex();
  if (i == -1)
    return;
  this->m_scilist.at(i)->markerDeleteAll(1);
  this->m_scilist.at(i)->markerDeleteAll(2);
}

void TextEditor::addannotate(int line, QString str)
{
  if (this->count() <= 0)
  {
    return;
  }
  auto index = this->currentIndex();
  this->m_scilist.at(index)->annotate(line, str, 0);
}

void TextEditor::addcurrentannotate(QString str)
{
  if (this->count() <= 0)
  {
    return;
  }
  auto index = this->currentIndex();
  int line, pos;
  this->m_scilist.at(index)->getCursorPosition(&line, &pos);
  auto text = this->m_scilist.at(index)->text(line);
  if (text.endsWith("\r\n"))
    this->m_scilist.at(index)->insertAt(
        str, line, this->m_scilist.at(index)->lineLength(line) - 2);
  else if (text.endsWith("\n"))
  {
    this->m_scilist.at(index)->insertAt(
        str, line, this->m_scilist.at(index)->lineLength(line) - 1);
  }
}

void TextEditor::replacecurrentannotate(QString str)
{
  if (this->count() <= 0)
  {
    return;
  }
  auto index = this->currentIndex();
  int line, pos;
  this->m_scilist.at(index)->getCursorPosition(&line, &pos);
  auto text = this->m_scilist.at(index)->text(line);
  if (text.isEmpty() || text == "\r" || text == "\n" || text == "\r\n")
  {
    this->m_scilist.at(index)->insertAt(str, line, 0);
  }
  else
  {
    if (text.endsWith("\r\n"))
      this->m_scilist.at(index)->setSelection(line, 0, line, text.length() - 2);
    else if (text.endsWith("\n") | text.endsWith("\r"))
    {
      this->m_scilist.at(index)->setSelection(line, 0, line, text.length() - 1);
    }
    else
    {
      this->m_scilist.at(index)->setSelection(line, 0, line, text.length());
    }
    this->m_scilist.at(index)->replaceSelectedText(str);
  }
}

QString TextEditor::getcurrentannotate()
{
  if (this->count() <= 0)
  {
    return QString();
  }
  auto index = this->currentIndex();
  int line, pos;
  this->m_scilist.at(index)->getCursorPosition(&line, &pos);
  return this->m_scilist.at(index)->text(line);
}

QString TextEditor::getfilename(int index)
{
  if (this->count() > 0)
    return this->m_filepathlist.at(index);
  return QString();
}

QByteArray TextEditor::GetContent(int index)
{
  if (index == -1)
  {
    index = currentIndex();
  }
  if (index == -1)
    return QByteArray();
  return m_scilist.at(index)->text().toLatin1();
}

void TextEditor::on_margin_clicked(int m, int n, Qt::KeyboardModifiers)
{
  auto index = this->currentIndex();
  auto m_sci = m_scilist.at(index);
  if ((m_sci->markersAtLine(n) & 0x02) == 2)
  {
    // 获取对应行的mask
    m_sci->markerDelete(n, 1);
    emit removebreakpoint(this->tabText(index), n + 1);
  }
  else
  {
    // m_sci->markerDefine(QsciScintilla::Circle,1);
    m_sci->markerAdd(n, 1);
    emit addbreakpoint(this->tabText(index), n + 1);
    // m_sci->markerAdd(n,2);
  }
  // qDebug()<<n<<"\t"<<m;
  m_sci->autoCompleteFromDocument();
  // qDebug()<<m_sci->lines();//行数
  //
  // m_sci->setCaretLineBackgroundColor(QColor(Qt::darkYellow));
}

void TextEditor::on_table_close(int index)
{
  this->m_scilist.at(index)->close();
  this->m_scilist.removeAt(index);
  this->m_filepathlist.removeAt(index);
  this->removeTab(index);
}

void TextEditor::on_table_change(int index)
{
  if (this->count() == 0)
    return;
  auto i = this->currentIndex();
  auto source_filename = this->tabText(i);
  auto source_filepath = this->m_filepathlist.at(i);
  emit sourcefilechange(source_filepath);
}

void TextEditor::on_setpostion(QString name, int line, int index)
{
  if (this->count() == 0)
    return;
  auto i = this->currentIndex();
  this->m_scilist.at(i)->markerDeleteAll(2);
  if (name.isEmpty())
  {
    return;
  }
  auto filename = QFileInfo(name).fileName();
  if (!this->changepage(filename))
  {
    // 发出错误信号
    for (auto m_sci : m_scilist)
    {
      // m_sci->clearAnnotations();
      m_sci->markerDeleteAll(2);
    }
    newpage(filename);
    this->changepage(filename);
    i = this->currentIndex();
    emit listcodeforcurrentfile(name, line, index);
    return;
  }
  i = this->currentIndex();
  // this->m_scilist.at(i)->setSelection(line-1,0,line-1,this->m_scilist.at(i)->lineLength(line));
  // this->m_scilist.at(i)->indent(line);
  // this->m_scilist.at(i)->clearAnnotations();
  // this->m_scilist.at(i)->annotate(line-1,"here",1);
  this->m_scilist.at(i)->markerDeleteAll(2);
  this->m_scilist.at(i)->markerAdd(line - 1, 2);
  this->m_scilist.at(i)->setCursorPosition(line - 1, 0);
}

void TextEditor::on_sendcontent(QString str, QString name, int line,
                                int index)
{
  auto i = this->currentIndex();
  this->m_scilist.at(i)->setText(str);
  on_setpostion(name, line, index);
}
