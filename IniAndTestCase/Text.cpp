#include<iostream>
using namespace std;
#include<fstream>
#include<time.h>
#include"text.h"
#include<stdio.h>
#include<string.h>
#define Generic_Negative_Response
using namespace std;
//ostringstream convert;
//constexpr uint8_t TestNumber = 7;
//constexpr uint16_t TestLeagth = 500;
#define uint8ToHexStr(dest, src, len)\
    for(int i = 0; i < len; i++ ){\
      sprintf(&dest[i * 2], "%02x", (unsigned char)src[i]);\
    }\
    dest[len * 2] = '\0';

char TestCaseName[100][100] = {

    "Test_0x22Service_01SupportSessionTest",
    "Test_0x22Service_01notSupportSessionTest",
    "Test_0x22Service_03SupportSessionTest",
    "Test_0x22ServiceDataTest",
    //"Test_0x22ServiceNRC_0x22Test",
    "Test_0x22ServiceNRC_0x13_Short_Test",
    "Test_0x22ServiceNRC_0x13_Long_Test",
    "Test_0x22ServiceNRC_0x31_Test",
    "Test_0x2EService_01SupportSessionTest",
    "Test_0x2EService_03SupportSessionTest",
    "Test_0x2EServiceWriteDataTest",
    "Test_0x2EServiceNRC_0x13_Short_Test",
    "Test_0x2EServiceNRC_0x13_Long_Test",
    "Test_0x2EServiceNRC_0x31_Test",
    "Test_0x2EServiceNRC_0x33Test"
};
enum {
    Test_0x22Service01SupportSessionTest,
    Test_0x22Service01notSupportSessionTest,
    Test_0x22Service03SupportSessionTest,
    Test_0x22ServiceDataTest,
    /*Test_0x22ServiceNRC_0x22Test, ,*/
    Test_0x22ServiceNRC_0x13_Short_Test ,
    Test_0x22ServiceNRC_0x13_Long_Test,
    Test_0x22ServiceNRC_0x31_Test,
    Test_0x2EService01SupportSessionTest,
    Test_0x2EService03SupportSessionTest,
    Test_0x2EServiceWriteDataTest,
    Test_0x2EServiceNRC_0x13_Short_Test,
    Test_0x2EServiceNRC_0x13_Long_Test,
    Test_0x2EServiceNRC_0x31_Test,
    Test_0x2EServiceNRC_0x33Test
};
ofstream  ofs;
/* 构造函数写入固定头*/
TextGroup::TextGroup(string &TextFile)
{
    ofs.open(TextFile, ios::out);//3.打开文件
    ofs << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>" << endl;
    ofs << "<testmodule title=\"Smartlight_UDS_Test\" version=\"\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns=\"http://www.vector-informatik.de/CANoe/TestModule/1.13\" xmlns:ai=\"http://www.vector-informatik.de/CANoe/TestModuleSchemaAppInfo/1.0\">" << endl;
    ofs << "<preparation>\n</preparation > " << endl;
    ofs.close();
    ofs.open(TextFile, ios::app);
}
/*析构关闭文件*/
TextGroup:: ~TextGroup()
{
    ofs << "<completion>\n</completion >\n</testmodule>" << endl;
    ofs.close();
}

void TextGroup::CreatReadDIDTestGroup()
{
    ofs<<"<testgroup title=\"UDS_Test 0x22 read DID test\">"<<endl;
}
void TextGroup::EndDIDTestGroup()
{
    ofs << "</testgroup>" << endl;
}
void TextGroup::CreatWriteDIDTestGroup()
{
    ofs << "<testgroup title=\"UDS_Test 0x2E write DID test\">" << endl;
}

void TextGroup::AddTestCaseInReadGroup(ReadDID0x22information* Readstruct)
{
    char DIDsting[5] = {0};
    //#pragma warning(disable : 4996)
    _ltoa(Readstruct->DID, DIDsting, 16);
    ofs << "<testgroup title=\"Read DID" << DIDsting <<"Test\" > "<< endl;
    if(Readstruct->session01support==1)
        ReadTestCaseStep(DIDsting, TestCaseName[Test_0x22Service01SupportSessionTest]);
    else
        ReadTestCaseStep(DIDsting, TestCaseName[Test_0x22Service01notSupportSessionTest]);
    ReadTestCaseStep(DIDsting, TestCaseName[Test_0x22Service03SupportSessionTest]);
    ReadDataTestCaseStep(DIDsting, TestCaseName[Test_0x22ServiceDataTest],Readstruct->Datadefault,Readstruct->DataSize);
#ifndef Generic_Negative_Response
    ReadTestCaseStep(DIDsting, TestCaseName[Test_0x22ServiceNRC_0x13_Short_Test]);
    ReadTestCaseStep(DIDsting, TestCaseName[Test_0x22ServiceNRC_0x13_Long_Test]);
    ReadTestCaseStep(DIDsting, TestCaseName[Test_0x22ServiceNRC_0x31_Test]);
#endif
     ofs << "</testgroup>" << endl;
}
void TextGroup::AddTestCaseInWriteGroup(WriteDID0x2Einformation* WriteStruct, uint8_t(*testbuffer)[TestLeagth])
{
    char DIDsting[5] = { 0 };
    char string[TestLeagth * 2] = { 0 };
    //char writetestname[100]={0};
    /*uint8_t testbuffer[TestNumber][TestLeagth] = { 0 };*/
    _ltoa(WriteStruct->DID, DIDsting, 16);
    ofs << "<testgroup title=\"Write DID" << DIDsting << "Test\" > " << endl;
#ifndef Generic_Negative_Response
    WriteTestCaseStep(DIDsting, TestCaseName[Test_0x2EService01SupportSessionTest],TestCaseName[Test_0x2EService01SupportSessionTest], WriteStruct->Datadefault, WriteStruct->DataSize);
#endif
    WriteTestCaseStep(DIDsting, TestCaseName[Test_0x2EService03SupportSessionTest],TestCaseName[Test_0x2EService03SupportSessionTest], WriteStruct->Datadefault, WriteStruct->DataSize);
    CreatWriteTestCase(WriteStruct, testbuffer);
    for (int i = 0; i < TestNumber; i++)
    {
        memset(string, 0, sizeof(string));
        Uint8ToChar(testbuffer[i], string, WriteStruct->DataSize);
        char writetestname[100] ={0};
        memcpy(writetestname,TestCaseName[Test_0x2EServiceWriteDataTest],100);
        sprintf(writetestname,"%s %d",TestCaseName[Test_0x2EServiceWriteDataTest],i);
        WriteTestCaseStep(DIDsting,TestCaseName[Test_0x2EServiceWriteDataTest], writetestname, string, WriteStruct->DataSize);
    }
    WriteTestCaseStep(DIDsting, TestCaseName[Test_0x2EServiceNRC_0x13_Long_Test],TestCaseName[Test_0x2EServiceNRC_0x13_Long_Test], WriteStruct->Datadefault, WriteStruct->DataSize);
    WriteTestCaseStep(DIDsting, TestCaseName[Test_0x2EServiceNRC_0x13_Short_Test],TestCaseName[Test_0x2EServiceNRC_0x13_Short_Test], WriteStruct->Datadefault, WriteStruct->DataSize);
    if(WriteStruct->NRC31==1){
         WriteTestCaseStep(DIDsting, TestCaseName[Test_0x2EServiceNRC_0x31_Test],TestCaseName[Test_0x2EServiceNRC_0x31_Test], WriteStruct->Datadefault, WriteStruct->DataSize);
    }
    if(WriteStruct->NRC33==1){
         WriteTestCaseStep(DIDsting, TestCaseName[Test_0x2EServiceNRC_0x33Test],TestCaseName[Test_0x2EServiceNRC_0x33Test], WriteStruct->Datadefault, WriteStruct->DataSize);
    }
    ofs << "</testgroup>" << endl;
}
void TextGroup::ReadTestCaseStep(const char* DID, const char* casename) const
{
    ofs << "<capltestcase name=\"" << casename << "\" title=\"SWTS-   "<<DID<<casename<<"\">" << endl;
    ofs << "<caplparam name=\"DID\" type=\"int\">0x" << DID << "</caplparam>" << endl;
    ofs << "<caplparam name=\"CaseName\" type=\"string\">SWTS- </caplparam>" << endl;
    ofs << "</capltestcase>" << endl;
}
void TextGroup::ReadDataTestCaseStep(const char* DID, const char* casename,const char* data,const uint16_t Datasize) const
{
    ofs << "<capltestcase name=\"" << casename << "\" title=\"SWTS-   "<<DID<<casename<<"\">" << endl;
    ofs << "<caplparam name=\"DID\" type=\"int\">0x" << DID << "</caplparam>" << endl;
    ofs << "<caplparam name=\"DefaultData\" type=\"string\">" << data << "</caplparam>" << endl;
    ofs << "<caplparam name=\"DataSize\" type=\"int\">" << Datasize << "</caplparam>" << endl;
    ofs << "<caplparam name=\"CaseName\" type=\"string\">SWTS- </caplparam>" << endl;
    ofs << "</capltestcase>" << endl;
}
void TextGroup::WriteTestCaseStep(const char* DID, const char* casename,const char* TitleName, const char* data, uint16_t datasize) const
{
    ofs << "<capltestcase name=\"" << casename << "\" title=\"SWTS-   "<<DID<<TitleName<<"\">" << endl;
    ofs << "<caplparam name=\"DID\" type=\"int\">0x" << DID << "</caplparam>" << endl;
    ofs << "<caplparam name=\"Data\" type=\"string\">" << data << "</caplparam>" << endl;
    ofs << "<caplparam name=\"size\" type=\"int\">" << datasize << "</caplparam>" << endl;
    ofs << "<caplparam name=\"CaseName\" type=\"string\">SWTS- </caplparam>" << endl;
    ofs << "</capltestcase>" << endl;
}
void TextGroup::CreatWriteTestCase(WriteDID0x2Einformation* WriteStruct, uint8_t(*testbuffer)[500])
{
    time_t unixtime = time(&unixtime);
    srand(unixtime);
    //memset(testbuffer, 0, TestNumber * TestLeagth);
    //memset(testcasebuffer, 0, TestLeagth);
    memcpy(testbuffer, WriteStruct->DataMin, WriteStruct->DataSize);
    memcpy(testbuffer + TestNumber - 1, WriteStruct->DataMax, WriteStruct->DataSize);
    for (int i = 0; i < WriteStruct->DataSize; i++)
    {
        if ((WriteStruct->DataSize + i) && (WriteStruct->DataMax + i))
        {
            testbuffer[1][i] = (*(WriteStruct->DataMin + i) + 1)>*(WriteStruct->DataMax + i) ?(*(WriteStruct->DataMin + i)):(*(WriteStruct->DataMin + i) + 1);
            testbuffer[2][i] = (uint8_t)((uint16_t)(WriteStruct->DataMin[i] + WriteStruct->DataMax[i]) / 2);
            //testbuffer[2][i] = rand() % ((*(WriteStruct->DataMax + i) - 2) - (*(WriteStruct->DataMin + i) + 2) + 1) + *(WriteStruct->DataMin + i);
            //testbuffer[3][i] = rand() % ((*(WriteStruct->DataMax + i) - 2) - (*(WriteStruct->DataMin + i) + 2) + 1) + *(WriteStruct->DataMin + i);
            //testbuffer[4][i] = rand() % ((*(WriteStruct->DataMax + i) - 2) - (*(WriteStruct->DataMin + i) + 2) + 1) + *(WriteStruct->DataMin + i);
            testbuffer[TestNumber - 2][i] = *(WriteStruct->DataMax + i) - 1 < *(WriteStruct->DataMin + i) ?(*(WriteStruct->DataMax + i)):(*(WriteStruct->DataMax + i) - 1);
        }
        else
            break;
    }

}
void TextGroup::Uint8ToChar(uint8_t* intput,char* output,uint16_t size_t)
{
    memset(output,0,size_t*2+1);
    char bytestr[3];
    for (int i = 0; i < size_t; i++)
    {
        memset(bytestr, 0, 3);
        sprintf(bytestr, "%.2x", intput[i]);
        strcat(output, bytestr);
    }
}
