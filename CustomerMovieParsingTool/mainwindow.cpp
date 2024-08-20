#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include"QFileDialog"
#include"ClientMovieconversiontool.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_ChoonseFile_clicked()
{
    QString FilePath = QFileDialog::getExistingDirectory(this);
    FilePath = FilePath+"\\";
    ui->OpenFile->setText(FilePath);
    std::string Path  =FilePath .toStdString();
    GettxtNumber(Path);
    SelectFolderTranse(Path,ui->progressBar);

}

