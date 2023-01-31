#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{
    ui->splittermain->setSizes(QList<int>()<<this->width()*1/3<<this->width()*2/3);
    ui->splittermiddle->setSizes(QList<int>()<<this->height()*2/3<<this->height()*1/3);
    this->m_progress=new GDbProgress();


//测试
    ui->GuiTextEditor->newpage(m_progress->FileName());
    ui->GuiTextEditor->setcontent(m_progress->listcode());

    m_progress->run("b 11");
    m_progress->run("b add");
//测试


    connect(this,&MainWindow::runprogram,this->m_progress,&GDbProgress::on_runprogram);
    connect(this->m_progress,&GDbProgress::setpostion,this->ui->GuiTextEditor,&TextEditor::on_setpostion);
}


void MainWindow::on_actionRun_triggered()
{
    emit runprogram();
}

