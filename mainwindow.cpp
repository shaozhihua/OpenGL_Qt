#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "CustomGLWidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    resize(1920,1080);

}

MainWindow::~MainWindow()
{
    delete ui;
}




void MainWindow::on_btn_wireFrame_clicked()
{
    ui->openGLWidget->setWireframe(!(ui->btn_wireFrame->isChecked()));
}


void MainWindow::on_pushButton_clicked()
{
    ui->openGLWidget->drawShape(CustomGLWidget::Triangle);
}


void MainWindow::on_pushButton_2_clicked()
{
    ui->openGLWidget->drawShape(CustomGLWidget::None);
}

