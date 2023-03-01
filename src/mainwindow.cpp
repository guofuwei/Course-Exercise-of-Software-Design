#include "mainwindow.h"

#include <utils.h>

#include <QFileDialog>
#include <QTreeWidgetItem>

#include "QtGui/private/qzipreader_p.h"
#include "QtGui/private/qzipwriter_p.h"
#include "texteditor.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  this->init();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::init() {
  // 调整ui样式
  ui->splittermain->setSizes(QList<int>()
                             << this->width() * 1 / 4 << this->width() * 1 / 2
                             << this->width() * 1 / 4);
  ui->splittermiddle->setSizes(QList<int>() << this->height() * 2 / 3
                                            << this->height() * 1 / 3);
  this->DisableAll();
  this->m_progress = new GDbProgress();
  connect(this, &MainWindow::setlog, this->ui->LogWidget,
          &LogDialog::on_setcontent);

  // this->CompileCurrentPage();

  //  this->CompileCode("./test.cpp");
  //  this->m_progress->ChangeProgram("./te.exe");

  // 测试
  //  ui->GuiTextEditor->newpage(m_progress->FilePath());
  //  ui->GuiTextEditor->setcontent(m_progress->listcode());
  ui->sourceTreeWidget->clear();
  ui->RecordTimerWidget->setStyleSheet(QString("QLabel{font-size:48px;}"));
  ui->RecordTimerWidget->reset();
  ui->RecordTimerWidget->showTime();
  m_audio_record =
      new AudioRecord(ui->plainTextEditAudioLog, ui->RecordTimerWidget);
  // 为了保存图片和音频的设置
  m_audio_record->SetTextEditor(ui->GuiTextEditor);
  ui->PaintWidget->SetTextEditor(ui->GuiTextEditor);
  ui->PaintWidget->SetPlainTextLog(ui->plainTextEditPaintLog);
  ui->plainTextEditAudioLog->setReadOnly(true);
  ui->plainTextEditPaintLog->setReadOnly(true);
  //  连接信号函数
  connect(this, &MainWindow::runprogram, this->m_progress,
          &GDbProgress::on_runprogram);
  connect(this, &MainWindow::continueprogram, this->m_progress,
          &GDbProgress::on_continueprogram);
  connect(this, &MainWindow::next, this->m_progress, &GDbProgress::on_next);
  connect(this, &MainWindow::step, this->m_progress, &GDbProgress::on_step);
  connect(this, &MainWindow::finish, this->m_progress, &GDbProgress::on_finish);
  connect(this->m_progress, &GDbProgress::setpostion, this->ui->GuiTextEditor,
          &TextEditor::on_setpostion);
  connect(this->m_progress, &GDbProgress::update, this, &MainWindow::on_update);
  connect(this->m_progress, &GDbProgress::setcontent, this->ui->GuiTextEditor,
          &TextEditor::on_sendcontent);
  //  connect(this->ui->GuiTextEditor, &TextEditor::listcodeforcurrentfile,
  //          this->m_progress, &GDbProgress::on_listcodeforcurrentfile);
  connect(this->ui->GuiTextEditor, &TextEditor::listcodeforcurrentfile, this,
          &MainWindow::on_listfile);
  connect(this->ui->GuiTextEditor, &TextEditor::addbreakpoint, this->m_progress,
          &GDbProgress::on_addbreakpoint);
  connect(this->ui->GuiTextEditor, &TextEditor::removebreakpoint,
          this->m_progress, &GDbProgress::on_removebreakpoint);
  connect(this->m_progress, &GDbProgress::setlog, this->ui->LogWidget,
          &LogDialog::on_setcontent);
  connect(this->m_progress, &GDbProgress::programload, this,
          &MainWindow::on_programload);
  connect(this->ui->GuiTextEditor, &TextEditor::sourcefilechange, this,
          &MainWindow::on_tablechange);
  connect(this->m_progress, &GDbProgress::updatebreakpoint, this,
          &MainWindow::BreakPointTreeWidgetUpdate);

  // connect(this->ui->GuiTextEditor, &TextEditor::currentChanged,
  // this,&MainWindow::on_table_change);
}

void MainWindow::DisableAll() {
  // this->ui->toolBar->hide();
  this->ui->actionRun->setEnabled(0);
  this->ui->actionContinue->setEnabled(0);
  this->ui->actionNext->setEnabled(0);
  this->ui->actionStep->setEnabled(0);
  this->ui->actionFinish->setEnabled(0);
}

void MainWindow::EnableAll() {
  this->ui->actionRun->setEnabled(1);
  this->ui->actionContinue->setEnabled(1);
  this->ui->actionNext->setEnabled(1);
  this->ui->actionStep->setEnabled(1);
  this->ui->actionFinish->setEnabled(1);
}

void MainWindow::CompileCode(QString filepath, QStringList extra) {
  CompilerProcess *t = new CompilerProcess(filepath);
  connect(t, &CompilerProcess::setlog, this->ui->LogWidget,
          &LogDialog::on_setcontent);
  if (!t->check()) {
    return;
  }
  auto index = this->ui->GuiTextEditor->currentIndex();
  auto data = this->ui->GuiTextEditor->GetContent(index);
  auto sourcefile = QFile(m_sourceFilename);
  if (!sourcefile.open(QIODevice::ReadWrite | QIODevice::Truncate)) {
    emit setlog(QString("[main] 无法打开") + m_sourceFilename);
    return;
  }
  qDebug() << sourcefile.write(data);
  sourcefile.close();
  t->compile();
  // https://www.coder.work/article/6491894
  // disconnect(t,&CompilerProcess::setlog,this->ui->LogWidget,&LogDialog::on_setcontent);
  auto filename = QFileInfo(m_sourceFilename).baseName();
  this->m_progress->ChangeProgram(QString("./temp/") + filename + ".exe");
  t->deleteLater();
}

void MainWindow::CompileCurrentPage() {
  auto index = this->ui->GuiTextEditor->currentIndex();
  if (index == -1) {
    emit setlog("[main] CompileCurrentPage 无打开页面");
    return;
  }
  auto filename = this->ui->GuiTextEditor->tabText(index);
  if (QFileInfo(filename).suffix() != "cpp" |
      QFileInfo(filename).suffix() != "c") {
    emit setlog("[main] CompileCurrentPage 文件不为cpp或c");
    return;
  }

  // this->ui->GuiTextEditor->m_scilist.at()
}

void MainWindow::BreakPointTreeWidgetUpdate() {
  auto list = this->m_progress->GetBreakPointInfo();
  ui->breakpointsTreeWidget->clear();
  for (auto breakpoint : list) {
    QString str;
    QTreeWidgetItem *t = new QTreeWidgetItem();
    t->setText(0, breakpoint["number"]);
    t->setText(1, breakpoint["type"]);
    t->setText(2, breakpoint["keep"]);
    t->setText(3, breakpoint["enable"]);
    t->setText(4, breakpoint["address"]);
    auto begin = breakpoint["file"].indexOf("in ") + 3;
    auto end = breakpoint["file"].indexOf("at");
    str = breakpoint["file"].mid(begin, end - begin);
    t->setText(5, str);
    str = breakpoint["file"].mid(end + 2);
    t->setText(6, str);
    ui->breakpointsTreeWidget->addTopLevelItem(t);
  }
}

void MainWindow::LocalsTreeWidgetUpdate() {
  auto map = this->m_progress->GetLocalInfo();
  ui->localsTreeWidget->clear();
  for (auto iter = map.begin(); iter != map.end(); iter++) {
    QTreeWidgetItem *t = new QTreeWidgetItem();
    t->setText(0, iter.key());
    t->setText(1, iter.value().second);
    t->setText(2, iter.value().first);
    ui->localsTreeWidget->addTopLevelItem(t);
  }
}

void MainWindow::StackTreeWidgetUpdate() {
  auto map = this->m_progress->GetStackInformation();
  ui->StackTreeWidget->clear();
  for (auto stackinfo : map) {
    QTreeWidgetItem *t = new QTreeWidgetItem();
    t->setText(0, stackinfo["level"]);
    t->setText(1, stackinfo["func"]);
    t->setText(2, stackinfo["line"]);
    t->setText(3, stackinfo["arg"]);
    t->setText(4, stackinfo["file"]);

    ui->StackTreeWidget->addTopLevelItem(t);
  }
}

void MainWindow::on_actionRun_triggered() { emit runprogram(); }

void MainWindow::on_actionNext_triggered() { emit next(); }

void MainWindow::on_actionStep_triggered() { emit step(); }

void MainWindow::on_actionOpen_Folder_triggered() {
  QStringList sourceFilePatterns = QStringList({"*.cpp", "*.c", "*.cc"});
  QStringList headerFilePatterns = QStringList({"*.hpp", "*.h"});
  QString filename =
      QFileDialog::getExistingDirectory(this, "open ", "F:\\tool\\minGw\\code");
  //  qDebug() << filename << endl;
  m_workdir = filename;
  QDir *dir = new QDir(filename);
  //  qDebug() << Cesd::matches(sourceFilePatterns, "aaa.cpp");
  QStringList filter;
  QList<QFileInfo> *fileInfo = new QList<QFileInfo>(dir->entryInfoList(filter));
  // 下面进行ui的编写
  ui->sourceTreeWidget->clear();
  QTreeWidgetItem *topSourceItem = new QTreeWidgetItem();
  topSourceItem->setText(0, "SourceFiles");
  ui->sourceTreeWidget->addTopLevelItem(topSourceItem);
  QTreeWidgetItem *topHeaderItem = new QTreeWidgetItem();
  topHeaderItem->setText(0, "HeaderFiles");
  ui->sourceTreeWidget->addTopLevelItem(topHeaderItem);
  for (int i = 0; i < fileInfo->count(); i++) {
    if (fileInfo->at(i).fileName() == "." ||
        fileInfo->at(i).fileName() == "..") {
      continue;
    }
    QTreeWidgetItem *item = new QTreeWidgetItem();
    item->setText(1, fileInfo->at(i).fileName());
    if (Cesd::matches(sourceFilePatterns, fileInfo->at(i).fileName(),
                      QRegExp::Wildcard)) {
      topSourceItem->addChild(item);
    } else if (Cesd::matches(headerFilePatterns, fileInfo->at(i).fileName(),
                             QRegExp::Wildcard)) {
      topHeaderItem->addChild(item);
    }
  }
}

void MainWindow::on_sourceTreeWidget_itemDoubleClicked(QTreeWidgetItem *item,
                                                       int column) {
  QString fullFilename = m_workdir + "/" + item->text(1);
  // QString fullFilename = m_workdir + "/" + item->text(column);
  // qDebug() << fullFilename << endl;
  ui->GuiTextEditor->readfromfile(fullFilename);
}
void MainWindow::on_actionFinish_triggered() { emit finish(); }

void MainWindow::on_listfile(QString name, int line, int index) {
  auto content = this->m_progress->listcode();
  auto i = this->ui->GuiTextEditor->currentIndex();
  this->ui->GuiTextEditor->setcontent(content, i);
  this->ui->GuiTextEditor->on_setpostion(name, line, index);
}

void MainWindow::on_tablechange(QString filepath) {
  m_sourceFilename = filepath;
  QFileInfo fileInfo = QFileInfo(filepath);
  ui->PaintWidget->SetFilename(fileInfo.fileName());
  m_audio_record->SetFilename(fileInfo.fileName());
  //  qDebug() << m_sourceFilename;
}

void MainWindow::on_programload() {
  this->EnableAll();
  this->ui->breakpointsTreeWidget->clear();
  this->ui->localsTreeWidget->clear();
  this->ui->statusbar->showMessage(m_sourceFilename);
  this->ui->GuiTextEditor->removeallbreakpoint();
}
void MainWindow::on_update() {
  BreakPointTreeWidgetUpdate();
  LocalsTreeWidgetUpdate();
  StackTreeWidgetUpdate();
  // this->ui->GuiTextEditor->addannotate(2,"hello");
  // m_progress->GetStackInformation();
  qDebug() << m_progress->state();
}
void MainWindow::on_actionContinue_triggered() { emit continueprogram(); }

void MainWindow::on_actioncompile_triggered() {
  this->CompileCode(m_sourceFilename);
}

void MainWindow::on_pushButtonSavePic_clicked() { ui->PaintWidget->SavePic(); }

void MainWindow::on_pushButtonClearAll_clicked() {
  ui->PaintWidget->ClearAll();
}
void MainWindow::on_pushButtonStartRecord_clicked() {
  if (ui->RecordTimerWidget->IsRun()) {
    m_audio_record->AudioLog("已开始录音，请勿重复点击\n");
    return;
  }
  if (!m_audio_record->StartRecording()) {
    m_audio_record->AudioLog("No Open File\n");
    return;
  }
  ui->RecordTimerWidget->reset();
  ui->RecordTimerWidget->start();
}
void MainWindow::on_pushButtonStopRecord_clicked() {
  if (!ui->RecordTimerWidget->IsRun()) {
    m_audio_record->AudioLog("当前状态未录音\n");
    return;
  }
  if (!m_audio_record->StopRecording()) {
    return;
  }
  ui->RecordTimerWidget->reset();
}

void MainWindow::on_pushButtonStartPlay_clicked() {
  if (ui->RecordTimerWidget->IsRun()) {
    m_audio_record->AudioLog("已开始播放，请勿重复点击\n");
    return;
  }
  if (!m_audio_record->StartPlaying()) {
    m_audio_record->AudioLog("暂时没有音频文件\n");
    return;
  }
  ui->RecordTimerWidget->reset();
  ui->RecordTimerWidget->start();
}

void MainWindow::on_pushButtonStopPlay_clicked() {
  if (!ui->RecordTimerWidget->IsRun()) {
    m_audio_record->AudioLog("当前状态未播放\n");
    return;
  }
  if (!m_audio_record->StopPlaying()) {
    return;
  }
  ui->RecordTimerWidget->reset();
}

void MainWindow::on_variableDeleteToolButton_clicked() {
  // this->ui->GuiTextEditor->replacecurrentannotate("hhh");
  // this->ui->GuiTextEditor->addcurrentannotate("hi");
  auto text = ui->variableAddLineEdit->text();
  if (text.isEmpty()) return;
  auto Value = this->m_progress->GetExpression(text);
  if (Value.isEmpty()) {
    return;
  }
  QTreeWidgetItem *t = new QTreeWidgetItem();
  t->setText(0, text);
  t->setText(2, Value);
  ui->localsTreeWidget->addTopLevelItem(t);
}

void MainWindow::on_actionsave_triggered() {
  if (this->ui->GuiTextEditor->count() <= 0) {
    QMessageBox::critical(this, "", "未打开文件");
    return;
  }
  auto index = this->ui->GuiTextEditor->currentIndex();
  auto filename = this->ui->GuiTextEditor->getfilename(index);
  if (QMessageBox::question(this, "", QString("是否保存文件 ") + filename) ==
      QMessageBox::Yes) {
    auto file = QFile(filename);
    if (!file.open(QIODevice::ReadWrite | QIODevice::Truncate))
      QMessageBox::critical(this, "", "文件打开失败");
    file.write(this->ui->GuiTextEditor->GetContent(index));
    file.close();
  };
}

void MainWindow::on_actionsavesomewhere_triggered() {
  if (this->ui->GuiTextEditor->count() <= 0) {
    QMessageBox::critical(this, "", "未打开文件");
    return;
  }
  auto index = this->ui->GuiTextEditor->currentIndex();
  auto filepath = QFileDialog::getSaveFileName();
  auto file = QFile(filepath);
  if (filepath.isEmpty()) return;
  if (!file.open(QIODevice::ReadWrite))
    QMessageBox::critical(this, "", "文件打开失败");
  file.write(this->ui->GuiTextEditor->GetContent(index));
  file.close();
}

void MainWindow::on_pushButtonLoadPic_clicked() { ui->PaintWidget->LoadPic(); }

void MainWindow::on_actionAbout_Qt_triggered() {
  QMessageBox::aboutQt(this, "About Qt");
}

void MainWindow::on_actionExport_File_triggered() {
  if (m_sourceFilename == "") {
    QMessageBox::warning(this, "", "未打开文件");
    return;
  }
  QList<QString> text_lines_code = ui->GuiTextEditor->getlines();
  QString kPicRoot = "./comment/pic/";
  QString kAudioRoot = "./comment/audio/";
  int start_pos, stop_pos;
  QList<QString> parse_result;
  QString filename;
  QFileInfoList fileList;
  for (const auto &code : text_lines_code) {
    //    qDebug() << "code:" << code;
    if (code == "") continue;
    start_pos = Cesd::GetCommentStartPos(code, 1);
    if (start_pos != -1) {
      stop_pos = Cesd::GetCommentStopPos(code, start_pos);
      QString pic_comment = code.mid(start_pos, stop_pos - start_pos + 1);
      //  qDebug() << "Pic Comment:" << pic_comment;
      parse_result = Cesd::ParseComment(pic_comment, 1);
      filename = parse_result[0] + "-" + parse_result[1] + ".jpg";
      fileList.push_back(QFileInfo(kPicRoot + filename));
      //      qDebug() << kPicRoot + filename;
    }
    // get audio files
    start_pos = Cesd::GetCommentStartPos(code, 2);
    if (start_pos == -1) continue;
    stop_pos = Cesd::GetCommentStopPos(code, start_pos);
    QString audio_comment = code.mid(start_pos, stop_pos - start_pos + 1);
    //  qDebug() << "Pic Comment:" << pic_comment;
    parse_result = Cesd::ParseComment(audio_comment, 2);
    filename = parse_result[0] + "-" + parse_result[1] + ".pcm";
    fileList.push_back(QFileInfo(kAudioRoot + filename));
    //    qDebug() << kAudioRoot + filename;
  }
  fileList.push_back(QFileInfo(m_sourceFilename));
  QString zip_filename =
      QFileDialog::getSaveFileName(this, tr("保存"), "new.zip", "Zip (*.zip)");
  if (zip_filename.length() > 0) {
    QZipWriter *writer = new QZipWriter(zip_filename);
    // 遍历文件列表
    foreach (QFileInfo fileInfo, fileList) {
      // 添加需要压缩的文件
      QFile file(fileInfo.filePath());
      if (!file.exists()) continue;
      file.open(QIODevice::ReadOnly);
      writer->addFile(fileInfo.fileName(), file.readAll());
      file.close();
    }
    writer->close();
    if (writer) {
      delete writer;
      writer = nullptr;
    }
    QMessageBox::information(this, "", "导出成功");
  }
}

void MainWindow::on_actionImport_File_triggered() {
  QString zip_filename =
      QFileDialog::getOpenFileName(this, tr("打开"), "", "Zip (*.zip)");
  if (zip_filename.length() <= 0) {
    QMessageBox::warning(this, "", "未选择文件");
    return;
  }
  QString kPicPath = "./comment/pic/";
  QString kAudioPath = "./comment/audio/";
  QDir temp_dir("./unzip_temp");
  if (!temp_dir.exists()) {
    temp_dir.mkpath(".");
  }
  QZipReader reader(zip_filename);
  reader.extractAll("./unzip_temp");
  // pic
  QStringList picNameFilters;
  picNameFilters << "*.jpg";
  QStringList picFiles = temp_dir.entryList(
      picNameFilters, QDir::Files | QDir::Readable, QDir::Name);
  foreach (QString filename, picFiles) {
    qDebug() << "filename:" << filename;
    int ret = QFile::copy("./unzip_temp/" + filename, kPicPath + filename);
    qDebug() << "pic copy ret:" << ret;
  }
  // audio
  QStringList audioNameFilters;
  audioNameFilters << "*.pcm";
  QStringList audioFiles = temp_dir.entryList(
      audioNameFilters, QDir::Files | QDir::Readable, QDir::Name);
  foreach (QString filename, audioFiles) {
    qDebug() << "filename:" << filename;
    int ret = QFile::copy("./unzip_temp/" + filename, kAudioPath + filename);
    qDebug() << "audio copy ret:" << ret;
  }
  temp_dir.removeRecursively();
  QMessageBox::information(this, "", "导入成功");
}
