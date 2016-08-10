#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableWidget->setColumnWidth(0,200);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_checkBox_toggled(bool checked)
{
    if(checked)
    {
        ui->tableWidget->setEnabled(true);
    }
    else
    {
        ui->tableWidget->setEnabled(false);
    }
}
