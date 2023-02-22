#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "texteditor.h"
#include <QFileDialog>
#include <QTreeWidgetItem>
#include <utils.h>

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent), ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  this->init();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::init()
{
  ui->splittermain->setSizes(QList<int>()
                             << this->width() * 1 / 3 << this->width() * 2 / 3);
  ui->splittermiddle->setSizes(QList<int>() << this->height() * 2 / 3
                               << this->height() * 1 / 3);
  this->m_progress = new GDbProgress();
  // 测试
  ui->GuiTextEditor->newpage(m_progress->FileName());
  ui->GuiTextEditor->setcontent(m_progress->listcode());
  ui->sourceTreeWidget->clear();
  // m_progress->run("b main");
  //  m_progress->run("b add");
  //  测试
  connect(this, &MainWindow::runprogram, this->m_progress,
          &GDbProgress::on_runprogram);
  connect(this, &MainWindow::next, this->m_progress, &GDbProgress::on_next);
  connect(this, &MainWindow::step, this->m_progress, &GDbProgress::on_step);
  connect(this, &MainWindow::finish, this->m_progress, &GDbProgress::on_finish);
  connect(this->m_progress, &GDbProgress::setpostion, this->ui->GuiTextEditor,
          &TextEditor::on_setpostion);
  connect(this->m_progress, &GDbProgress::update, this, &MainWindow::on_update);
  connect(this->m_progress, &GDbProgress::setcontent, this->ui->GuiTextEditor,
          &TextEditor::on_sendcontent);
  connect(this->ui->GuiTextEditor, &TextEditor::listcodeforcurrentfile,
          this->m_progress, &GDbProgress::on_listcodeforcurrentfile);
  connect(this->ui->GuiTextEditor, &TextEditor::addbreakpoint, this->m_progress, &GDbProgress::on_addbreakpoint);
  connect(this->ui->GuiTextEditor, &TextEditor::removebreakpoint, this->m_progress, &GDbProgress::on_removebreakpoint);
  //    connect(this,&MainWindow::runprogram,this->m_progress,&GDbProgress::on_runprogram);
  //    connect(this->m_progress,&GDbProgress::setpostion,this->ui->GuiTextEditor,&TextEditor::on_setpostion);
}

void MainWindow::BreakPointTreeWidgetUpdate()
{
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

void MainWindow::LocalsTreeWidgetUpdate()
{
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

void MainWindow::on_actionRun_triggered() { emit runprogram(); }

void MainWindow::on_actionNext_triggered() { emit next(); }

void MainWindow::on_actionStep_triggered() { emit step(); }

void MainWindow::on_actionOpen_Folder_triggered()
{
  QStringList sourceFilePatterns = QStringList({"*.cpp", "*.c", "*.cc"});
  QStringList headerFilePatterns = QStringList({"*.hpp", "*.h"});
  QString filename = QFileDialog::getExistingDirectory();
  //  qDebug() << filename << endl;
  m_sourceFilename = filename;
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
    if (fileInfo->at(i).fileName() == "." || fileInfo->at(i).fileName() == "..") {
      continue;
    }
    QTreeWidgetItem *item = new QTreeWidgetItem();
    item->setText(1, fileInfo->at(i).fileName());
    if (Cesd::matches(sourceFilePatterns, fileInfo->at(i).fileName(), QRegExp::Wildcard)) {
      topSourceItem->addChild(item);
    } else if (Cesd::matches(headerFilePatterns, fileInfo->at(i).fileName(), QRegExp::Wildcard)) {
      topHeaderItem->addChild(item);
    }
  }
}

void MainWindow::on_sourceTreeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
  QString fullFilename = m_sourceFilename + "/" + item->text(column);
  qDebug() << fullFilename << endl;
  ui->GuiTextEditor->readfromfile(fullFilename);
}
void MainWindow::on_actionFinish_triggered()
{
  emit finish();
}

void MainWindow::on_update()
{
  BreakPointTreeWidgetUpdate();
  LocalsTreeWidgetUpdate();
}

