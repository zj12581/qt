#ifndef EXCELGET_H
#define EXCELGET_H
#include<qstring.h>

constexpr auto DIDMAXStrLen = 1000;
constexpr auto DIDMAXBufferLen = 500;
struct WriteDiDStruct
{
    uint8_t DataMin[DIDMAXBufferLen];
    uint8_t DataMax[DIDMAXBufferLen];

};
struct DIDList{
    uint16_t DID;
    char DIDDefaultData[DIDMAXStrLen];
    bool DIDWriteON;
    struct WriteDiDStruct* WriteONPtr;
    uint16_t DataSize;
    struct DIDList* right;
    bool DIDSessionSupport;
    bool DID2E_NRC31;
    bool DID2E_NRC33;
};

void* Pool_DIDList(uint32_t DIDMAXnumber);
void* WriteDataToPool(DIDList* ptr,const DIDList* Now);
void* GetDatatoPool(DIDList* Now,const DIDList* ptr);
void* DelPOOL(DIDList** Now);
#endif // EXCELGET_H
