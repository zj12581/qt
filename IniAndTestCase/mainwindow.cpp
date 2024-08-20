#include "mainwindow.h"
#include "ui_TestCase.h"
#include"QFileDialog"
#include"Function.h"

static FUNC ChooseFileptr=ChooseFile;
static FUNCvoid Creat[]={
    CreatINI,
    CreatTestCase
};
enum
{
   CreatINIFunc,
   CreatTestCaseFunc
};

ExcelTools::ExcelTools(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ExcelTools)
{
    ui->setupUi(this);
}

ExcelTools::~ExcelTools()
{
    delete ui;
}


void ExcelTools::on_ChoonseFile_clicked()
{
    QString filePath= QFileDialog::getOpenFileName(this,tr("File"),"../","xlsx files(*.xlsx);;csv files(*.csv);;all files(*.*)");
    ui->OpenFile->setText(filePath);
    if(filePath.size())
    ChooseFileptr(filePath);
}


void ExcelTools::on_CreatIni_clicked()
{
    Creat[CreatINIFunc]();
}



void ExcelTools::on_CreatXmlTest_clicked()
{
    Creat[CreatTestCaseFunc]();
}

