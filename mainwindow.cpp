#include "mainwindow.h"
#include "ui_mainwindow.h"
#pragma execution_character_set("utf-8")

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->ruleEdit->setText("E->TD\nD->+TD|%\nT->FU\nU->*FU|%\nF->(E)|i\n");
    ui->startSymbolEdit->setText("E");
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_resultBtn_clicked()
{
    QStringList ruleList=ui->ruleEdit->toPlainText().split('\n');
    QString str=ui->strEdit->text();
    QString startSymbol =ui->startSymbolEdit->text();
    LL=new LL_1(ruleList,startSymbol,str);
    LL->createLLlist();
    

}

