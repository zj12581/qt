#ifndef DIMMINGWAVE_H
#define DIMMINGWAVE_H
#include <qcustomplot.h>
#include "iostream"
class DimmingWave
{
public:
    DimmingWave(uint16_t Light,uint32_t TimeRange);
    void ResetMax(uint16_t Light,uint32_t TimeRange);
    void AddPointInVector(QCustomPlot* customPlot,double* data[],uint16_t datalen);
    void CreatINI(char* path,double * Data[2],uint16_t PointNumber);
private:
   void CreatWave(QCustomPlot* customPlot,  QVector<double> Light, QVector<double> Time) const;
public:

private:
   uint32_t DimmingTime = 0;
   uint16_t DimmingLight = 0;
};

#endif // DIMMINGWAVE_H
