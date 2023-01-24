#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_progress=new GDbProgress();
    t=new TextEditor();
    m_laypout=new QGridLayout();
    m_statementlineedit=new QLineEdit();
    m_logdailog=new LogDialog();

    qDebug()<<m_progress->openMode();

    auto splitter=new QSplitter();
    splitter->addWidget(t);
    splitter->addWidget(m_logdailog);

    //ui->menubar->setStyleSheet("background-color:black;");


    //m_laypout->addWidget(ui->menubar);
    m_laypout->addWidget(m_statementlineedit);
    m_laypout->addWidget(splitter);


   QWidget *widget=new QWidget();
   widget->setLayout(m_laypout);
   this->setCentralWidget(widget);

   //this->t->setcontent(m_progress->run(QString("-mi")),-1);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionadd_triggered()
{
    auto filepath=QFileDialog::getOpenFileName();
    this->t->readfromfile(filepath);
}


void MainWindow::on_actionrunstatement_triggered()
{
    auto statement=this->m_statementlineedit->text();

    //this->t->setcontent(this->m_progress->run(statement),-1);
    QString  res=this->m_progress->run(statement);

    res=StringHandler::RemoveNumber(res);
    res=StringHandler::RemoveEndGdb(res);
    //StringHandler::GetLocalValue(res);

    this->m_logdailog->setlogcontent(res.toLocal8Bit());

    //StringHandler::RemoveEndGdb(m_logdailog->LogText());
}


void MainWindow::on_actionlist_triggered()
{
    t->setcontent(m_progress->listcode());

    auto str=m_progress->StartRun();
    //m_progress->run("n\nn\n");
    //str=m_progress->GetLocalInfo();
   // qDebug().noquote()<<str;

    this->m_logdailog->setlogcontent(str);

}


void MainWindow::on_actiontest_triggered()
{
   auto map=m_progress->GetLocalInfo();

}

