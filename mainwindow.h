

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
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
public slots:
    void revStepInfo(QString info);
private slots:
    void on_resultBtn_clicked();

    void on_clearBtn_clicked();

private:
    Ui::MainWindow *ui;
    LL_1* LL=nullptr;
};
#endif // MAINWINDOW_H
