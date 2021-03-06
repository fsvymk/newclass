#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "parser.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::pe(QString str){
    ui->plainTextEdit_2->appendPlainText(str);
}

void MainWindow::on_pushButton_clicked()
{
    Parser P;
    P.script = ui->plainTextEdit->toPlainText();
    int i = P.compile();

    QString output = P.hwSequenceCompiledHex + "\n" + P.modulesHex;
    ui->plainTextEdit_2->appendPlainText(output);
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->plainTextEdit_2->clear();
}
