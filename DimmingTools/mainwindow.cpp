#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dimmingwave.h"
#include"fstream"
#include"direct.h"
#include <Fit.h>
const uint8_t MAXPoint = 64;
static double LEDLight[MAXPoint];
static double LEDDimming[MAXPoint];
static uint16_t NowIndex;
uint16_t MaxLight = 0;
uint16_t LastMaxLight = 0;
uint32_t MaxTimer= 0;
uint16_t LastMaxTimer = 0;
DimmingWave Coordinates(MaxTimer,MaxLight);
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->Waveform -> addGraph();
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::PutWave(double *buffer[2],uint16_t Index)
{
    Coordinates.ResetMax(MaxLight+500,MaxTimer+1000);
    Coordinates.AddPointInVector(ui->Waveform,buffer,Index);
    char  Number[100]={0};
    itoa(Index,Number,10);
    ui->PointNumber->setText(Number);
}
void MainWindow::on_WritePoint_clicked()
{
    double* XYbuffer[2] = {LEDDimming,LEDLight};

    if(NowIndex>=MAXPoint)
    {
       // QMessageBox::StandardButton defaultBtn = QMessageBox::NoButton; // 缺省按钮
        QString dlgTitle="消息框";
        QString strInfo="已达到最大点数，请保存结果（The maximum number of points has been reached, please save the result）";

        // 弹窗分类 Question information warning critical
        QMessageBox::question(this, dlgTitle, strInfo,QMessageBox::Yes);
    }
    else
    {

        LEDLight[NowIndex]  = ui->LightData->text().toDouble();
        if(MaxLight<LEDLight[NowIndex])
        {
            LastMaxLight = MaxLight;
            MaxLight = LEDLight[NowIndex];

        }
        LEDDimming[NowIndex]  = ui->TimeData->text().toDouble();
        if(MaxTimer<LEDDimming[NowIndex])
        {
            LastMaxTimer= MaxTimer;
            MaxTimer = LEDDimming[NowIndex];

        }
        NowIndex++;
        PutWave(XYbuffer,NowIndex);
    }

}







void MainWindow::on_Revocation_clicked()
{
    if(NowIndex)
    {
        NowIndex--;
        double* XYbuffer[2] = {LEDDimming,LEDLight};
        PutWave(XYbuffer,NowIndex);
    }
}

void MainWindow::on_Save_clicked()
{
    std::ofstream ofs;
    char SavePath[1024] = {0};
    double* XYbuffer[2] = {LEDDimming,LEDLight};
    uint16_t Number = 0;
    uint16_t PathIndex[512] = {0};
    char path[512] = {0};
    char LastPath[512] = {0};
    getcwd(path,512);
    for(uint16_t i = 0;i<512;i++)
    {
        if(path[i]=='\\')
          {
            PathIndex[Number] = i;
            Number++;
        }
    }
    if(Number>=2)
    {
        memcpy(LastPath,path,PathIndex[Number-2]);
    }
    else
    {
        memcpy(LastPath,path,sizeof(path));
    }
    sprintf(SavePath,"%s\\Dimming\\PointSave.txt",LastPath);
    Coordinates.CreatINI(SavePath,XYbuffer,NowIndex);
    NowIndex = 0;
    MaxTimer = 0;
    MaxLight = 0;
    memset(LEDLight,0,sizeof (LEDLight));
    memset(LEDDimming,0,sizeof (LEDDimming));
    PutWave(XYbuffer,NowIndex);
}


//void MainWindow::on_Fit_clicked()
//{

//}
