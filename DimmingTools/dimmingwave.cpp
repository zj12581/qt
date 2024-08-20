#include "dimmingwave.h"
#include "vector"

DimmingWave::DimmingWave(uint16_t Light,uint32_t TimeRange)
{
    DimmingTime = TimeRange;
    DimmingLight = Light;
}
void DimmingWave::ResetMax(uint16_t Light,uint32_t TimeRange)
{
    DimmingTime = TimeRange;
    DimmingLight = Light;
}
void DimmingWave::CreatWave(QCustomPlot* customPlot,  QVector<double> Light, QVector<double> Time) const
{


    customPlot->graph(0)->setData(Light,Time);
    customPlot->xAxis->setRange(0,DimmingTime);
    customPlot->yAxis->setRange(0,DimmingLight);
    customPlot->xAxis2->setVisible(true);
    customPlot->xAxis2->setTickLabels(false);
    customPlot->yAxis2->setVisible(true);
    customPlot->yAxis2->setTickLabels(false);
    customPlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ScatterShape::ssPlus,5));
    customPlot->graph(0)->setLineStyle((QCPGraph::lsLine));
    customPlot->replot();
}

void DimmingWave::AddPointInVector(QCustomPlot* customPlot,double* data[],uint16_t datalen)
{
   std::vector<double> vectorx(data[0], data[0]+datalen);
   std::vector<double> vectory(data[1], data[1]+datalen);
   QVector<double> QVectorx = QVector<double>::fromStdVector(vectorx);
   QVector<double> QVectory = QVector<double>::fromStdVector(vectory);
   CreatWave(customPlot,QVectorx,QVectory);
}
void DimmingWave::CreatINI(char* path,double * Data[2],uint16_t PointNumber)
{

    char Ttile[100] ="DimmingPoint_NumBer";
    char PointTotalNumber[100]={0};
    sprintf(PointTotalNumber, "%d",PointNumber);
#ifdef UNICODE
     WritePrivateProfileStringA(Ttile,"number",PointTotalNumber,path);
#else
     WritePrivateProfileString(Ttile,"number",PointTotalNumber,path);
#endif
     char Point_NumBer[100];
     char Point_Light[100];
     char Point_Time[100];
    for(uint16_t i=0;i<PointNumber;i++)
    {
     memset(Point_NumBer,0,100);
     memset(Point_Light,0,100);
     memset(Point_Time,0,100);

     sprintf(Point_NumBer,"Point_%d",i+1);
     sprintf(Point_Time ,"%lld",(uint64_t)llround(Data[0][i]));
     sprintf(Point_Light,"%d",(uint32_t)llround(Data[1][i]));

#ifdef UNICODE
     WritePrivateProfileStringA(Point_NumBer,"Light",Point_Light,path);
     WritePrivateProfileStringA(Point_NumBer,"DimmingTime",Point_Time,path);
#else
     WritePrivateProfileString(Point_NumBer,"Light",Point_Light,path);
     WritePrivateProfileString(Point_NumBer,"DimmingTime",Point_Time,path);
#endif
    }
}
