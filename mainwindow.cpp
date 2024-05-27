#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "CustomGLWidget.h"
#include "CustomGLWgt_Rect.h"
#include "CustomGLWgt_colofulTri.h"

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







void MainWindow::on_action_triggered()
{

    switch (ui->stackedWidget->currentIndex()) {
    case 0:
        ui->GLWgt_Triangle->setWireframe(ui->action->isChecked());
        break;
    case 1:
        ui->GLWgt_Rectangle->setWireframe(ui->action->isChecked());
        break;
    case 2:
        ui->GLWgt_colorfulTriangle->setWireframe(ui->action->isChecked());
        break;
    default:
        break;
    }






}


void MainWindow::on_pushButton_3_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}


void MainWindow::on_pushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}


void MainWindow::on_pushButton_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

