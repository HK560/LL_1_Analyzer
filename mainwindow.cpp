
#include "mainwindow.h"

#include "ui_mainwindow.h"
#pragma execution_character_set("utf-8")

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->ruleEdit->setText("E->TD\nD->+TD|%\nT->FU\nU->*FU|%\nF->(E)|i\n");
    ui->startSymbolEdit->setText("E");
    ui->strEdit->setText("i+i#");


}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::revStepInfo(QString info)
{
    qDebug()<<"rev";
    ui->logtextBrowser->append(info);
}

void MainWindow::on_resultBtn_clicked() {
    try {
        if(ui->startSymbolEdit->text().isEmpty()||
            ui->ruleEdit->toPlainText().isEmpty()||
            ui->strEdit->text().isEmpty())
            throw QString("不允许为空");
        QStringList ruleList = ui->ruleEdit->toPlainText().split('\n');
        QString str = ui->strEdit->text();
        QString startSymbol = ui->startSymbolEdit->text();
        LL = new LL_1(ruleList, startSymbol, str);
        connect(LL,SIGNAL(sendStepInfo(QString)),this,SLOT(revStepInfo(QString)));
        LL->createLLlist();
        LL->buildLLMaps();
        LL->analysis(ui->strEdit->text());
        delete LL;
    } catch (QString info) {
        QMessageBox::warning(NULL, "Warning", info);
    }
}

void MainWindow::on_clearBtn_clicked()
{
    ui->logtextBrowser->clear();
}

