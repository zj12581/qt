#include"stdint.h"
#include"ExcelGet.h"
#include "ExcelEngine.h"
#include "ExcelGet.h"
#include <direct.h>
#include"QtDebug"
#include <sstream>
#include <stdlib.h>
#include <windows.h>
#include <map>
#include"string.h"
#include"text.h"

static DIDList* Findptr=nullptr;
DIDList  Excellist;
constexpr auto DIDDataStart = 3;
static uint32_t DID_MAX_Number_u32=0;
using namespace std;
enum
{
  EXCEL_DID=1,
  EXCEL_DID_DataSize,
  EXCEL_DID_DefaultData,
  EXCEL_DID_WriteON,
  EXCEL_DID_MIN,
  EXCEL_DID_MAX,
  EXCEL_DID_Session,
  EXCEL_DID_WriteNRC31,
  EXCEL_DID_WriteNRC33
};



void ChooseFile(QString filePath)
{
    QString DID_MAX_Number_Str=nullptr;
    ExcelEngine Exceloperations(filePath);
    if(Exceloperations.open()){
    DID_MAX_Number_Str=Exceloperations.getCellString("DIDList",1,2);
    Exceloperations.QstringtoU16ptr(DID_MAX_Number_Str,&DID_MAX_Number_u32,10);
    Findptr= (DIDList*)DelPOOL(&Findptr);
    Findptr =(DIDList*)Pool_DIDList(DID_MAX_Number_u32);
    if(Findptr)
    {
  // 按行数读取 写入结构体  从结构体拷贝进池子
    for(uint32_t i=0;i<DID_MAX_Number_u32;i++)
    {
       memset(&Excellist,0,sizeof(DIDList));
       QString DID_str=Exceloperations.getCellString("DIDList",i+DIDDataStart,EXCEL_DID);
       Exceloperations.QstringtoU16ptr(DID_str,&Excellist.DID,16);
       DID_str=Exceloperations.getCellString("DIDList",i+DIDDataStart,EXCEL_DID_DataSize);
       Exceloperations.QstringtoU16ptr(DID_str,&Excellist.DataSize,10);
       DID_str=Exceloperations.getCellString("DIDList",i+DIDDataStart,EXCEL_DID_DefaultData);
       QByteArray ARY = DID_str.toLatin1(); // must
       memcpy(Excellist.DIDDefaultData,ARY.data(),ARY.size());//Exceloperations.longstrtol(ARY.data(),DIDlist.DID_DaTa[i],DIDlist.DID_DataSize[i]);
       DID_str=Exceloperations.getCellString("DIDList",i+DIDDataStart,EXCEL_DID_WriteON);
       ARY = DID_str.toLatin1();
       Excellist.DIDWriteON=atoi(ARY.data());
      if(Excellist.DIDWriteON==1)
      {
         Excellist.WriteONPtr=(WriteDiDStruct*)malloc(sizeof(WriteDiDStruct));
         DID_str=Exceloperations.getCellString("DIDList",i+DIDDataStart,EXCEL_DID_MIN);
         ARY = DID_str.toLatin1();
         Exceloperations.longstrtol(ARY.data(), Excellist.WriteONPtr->DataMin,Excellist.DataSize);
         // Excellist.WriteONPtr->DataMax
         DID_str=Exceloperations.getCellString("DIDList",i+DIDDataStart,EXCEL_DID_MAX);
         ARY = DID_str.toLatin1();
         Exceloperations.longstrtol(ARY.data(), Excellist.WriteONPtr->DataMax,Excellist.DataSize);
      }
      DID_str=Exceloperations.getCellString("DIDList",i+DIDDataStart,EXCEL_DID_Session);
      ARY = DID_str.toLatin1();
      Excellist.DIDSessionSupport=atoi(ARY.data());

      DID_str=Exceloperations.getCellString("DIDList",i+DIDDataStart,EXCEL_DID_WriteNRC31);
      ARY = DID_str.toLatin1();
      Excellist.DID2E_NRC31=atoi(ARY.data());

      DID_str=Exceloperations.getCellString("DIDList",i+DIDDataStart,EXCEL_DID_WriteNRC33);
      ARY = DID_str.toLatin1();
      Excellist.DID2E_NRC33=atoi(ARY.data());

      Findptr=(DIDList*)WriteDataToPool(Findptr,&Excellist);
       if(!Findptr)
           break;
      }
     }
    Exceloperations.close();
    }
  }

void CreatINI()
{
    char* app = (char*)malloc(256);
    app = _getcwd(app, 128*sizeof(app));
    char filepath[1024] = { 0 };
    sprintf(filepath, "%s\\DIDdata.ini", app);
    char Ttile[100] ="DID_NumBer";
    //GetPrivateProfileIntA(Ttile, "DeviceIndex", 0, filepath);
    char DID_TotalNumber[100]={0};
    sprintf(DID_TotalNumber, "%d",DID_MAX_Number_u32);
#ifdef UNICODE
     WritePrivateProfileStringA(Ttile,"number",DID_TotalNumber,filepath);
#else
    WritePrivateProfileString(Ttile,"number",DID_TotalNumber,filepath);
#endif
     char DID_NumBer[100];
     char DID_Size[100];
     char DID[100];
    for(uint32_t i=0;i<DID_MAX_Number_u32;i++)
    {
     DIDList now;
     memset(DID_NumBer,0,100);
     memset(DID_Size,0,100);
     memset(DID,0,100);
     Findptr=(DIDList*)GetDatatoPool(&now,Findptr);
     sprintf(DID,"0x%x",now.DID);
     sprintf(DID_NumBer,"DID_%d",i+1);
     sprintf(DID_Size,"%d",now.DataSize);
#ifdef UNICODE
     WritePrivateProfileStringA(DID_NumBer,"DID",DID,filepath);
     WritePrivateProfileStringA(DID_NumBer,"DIDdatasize",DID_Size,filepath);
     WritePrivateProfileStringA(DID_NumBer,"DIDdata",now.DIDDefaultData,filepath);
#else
     WritePrivateProfileString(DID_NumBer,"DID",DID,filepath);
     WritePrivateProfileString(DID_NumBer,"DIDdatasize",DID_Size,filepath);
     WritePrivateProfileString(DID_NumBer,"DIDdata",DIDlist.DID_DaTa[i],filepath);
#endif
     if(!Findptr)
       break;
}
}
void CreatTestCase()
{
    uint8_t  MAXPATH = 100;
    uint8_t testbuffer[TestNumber][TestLeagth];
    uint8_t(*ptr)[TestLeagth] = testbuffer;
    char* local=(char*) malloc(sizeof(char)*MAXPATH);
    local= getcwd(local, MAXPATH);
    strcat(local,"\\DIDTest.vxt");
    string File=local;
    TextGroup Creat(File);
    ReadDID0x22information testReadstruct;
    WriteDID0x2Einformation testWritestruct;
    DIDList GettoPoll;
    Creat.CreatReadDIDTestGroup();
    for(uint32_t i=0;i<DID_MAX_Number_u32;i++)
    {
        memset(&testReadstruct,0,sizeof(testReadstruct));
        memset(&GettoPoll,0,sizeof(GettoPoll));
        Findptr=(DIDList*)GetDatatoPool(&GettoPoll,Findptr);
        testReadstruct.DID=GettoPoll.DID;
        testReadstruct.DataSize=GettoPoll.DataSize;
        testReadstruct.session01support=GettoPoll.DIDSessionSupport;
        memcpy(testReadstruct.Datadefault,GettoPoll.DIDDefaultData,testReadstruct.DataSize*2+1);
        Creat.AddTestCaseInReadGroup(&testReadstruct);
    }
    Creat.EndDIDTestGroup();
    Creat.CreatWriteDIDTestGroup();
    for(uint32_t i=0;i<DID_MAX_Number_u32;i++)
    {
        memset(&GettoPoll,0,sizeof(GettoPoll));
        Findptr=(DIDList*)GetDatatoPool(&GettoPoll,Findptr);
        if(GettoPoll.DIDWriteON==1)
         {

            memset(testbuffer,0,TestLeagth*TestNumber);
            memset(&testWritestruct,0,sizeof(testWritestruct));
            testWritestruct.DID=GettoPoll.DID;
            testWritestruct.DataSize=GettoPoll.DataSize;
            memcpy(testWritestruct.DataMax,GettoPoll.WriteONPtr->DataMax,GettoPoll.DataSize*2+1);
            memcpy(testWritestruct.DataMin,GettoPoll.WriteONPtr->DataMin,GettoPoll.DataSize*2+1);
            memcpy(testWritestruct.Datadefault,GettoPoll.DIDDefaultData,GettoPoll.DataSize*2+1);
            testWritestruct.NRC31=GettoPoll.DID2E_NRC31;
            testWritestruct.NRC33=GettoPoll.DID2E_NRC33;
            Creat.AddTestCaseInWriteGroup(&testWritestruct,ptr);

         }
    }
   Creat.EndDIDTestGroup();
}
