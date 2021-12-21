#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include"ll_1.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_resultBtn_clicked();

private:
    Ui::MainWindow *ui;
    LL_1* LL=nullptr;
};
#endif // MAINWINDOW_H
