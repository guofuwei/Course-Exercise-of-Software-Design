#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->splittermain->setSizes(QList<int>()<<this->width()*1/3<<this->width()*2/3);
    ui->splittermiddle->setSizes(QList<int>()<<this->height()*2/3<<this->height()*1/3);
}

MainWindow::~MainWindow()
{
    delete ui;
}

