#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::init() {
  ui->splittermain->setSizes(QList<int>()
                             << this->width() * 1 / 3 << this->width() * 2 / 3);
  ui->splittermiddle->setSizes(QList<int>() << this->height() * 2 / 3
                                            << this->height() * 1 / 3);
  this->m_progress = new GDbProgress();

  // 测试
  ui->GuiTextEditor->newpage(m_progress->FileName());
  ui->GuiTextEditor->setcontent(m_progress->listcode());

  m_progress->run("b main");
  // m_progress->run("b add");
  // 测试

  connect(this, &MainWindow::runprogram, this->m_progress,
          &GDbProgress::on_runprogram);
  connect(this, &MainWindow::next, this->m_progress, &GDbProgress::on_next);
  connect(this, &MainWindow::step, this->m_progress, &GDbProgress::on_step);
  connect(this->m_progress, &GDbProgress::setpostion, this->ui->GuiTextEditor,
          &TextEditor::on_setpostion);
  connect(this->m_progress, &GDbProgress::setcontent, this->ui->GuiTextEditor,
          &TextEditor::on_sendcontent);
  connect(this->ui->GuiTextEditor, &TextEditor::listcodeforcurrentfile,
          this->m_progress, &GDbProgress::on_listcodeforcurrentfile);

  //    connect(this,&MainWindow::runprogram,this->m_progress,&GDbProgress::on_runprogram);
  //    connect(this->m_progress,&GDbProgress::setpostion,this->ui->GuiTextEditor,&TextEditor::on_setpostion);
}

void MainWindow::on_actionRun_triggered() { emit runprogram(); }

void MainWindow::on_actionNext_triggered() { emit next(); }

void MainWindow::on_actionStep_triggered() { emit step(); }
