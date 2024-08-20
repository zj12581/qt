
#ifndef _TEXT
#define _TEXT


#include<iostream>
using namespace std;

constexpr uint8_t TestNumber = 5;
constexpr uint16_t TestLeagth = 500;
struct ReadDID0x22information
{
    uint16_t DID;
    char Datadefault[TestLeagth*2];
    uint16_t DataSize;
    bool session01support;
};
struct WriteDID0x2Einformation
{
    uint8_t DataMin[TestLeagth];
    uint8_t DataMax[TestLeagth];
    char Datadefault[TestLeagth*2];
    uint16_t DataSize;
    uint16_t DID;
    bool NRC31;
    bool NRC33;
};
class TextGroup
{
public:
    TextGroup(string& TextFile);
    ~TextGroup();

    void CreatReadDIDTestGroup();
    void EndDIDTestGroup();
    void CreatWriteDIDTestGroup();
    void AddTestCaseInReadGroup(ReadDID0x22information* Readstruct);
    void AddTestCaseInWriteGroup(WriteDID0x2Einformation* WriteStruct, uint8_t(*testbuffer)[TestLeagth]);

private:
    /*enum { ReadSessioncase = 1,ReadTestCase,ReadNRC0x13TestCase };*/

    void ReadTestCaseStep(const char* DID, const char* casename)const;
    void ReadDataTestCaseStep(const char* DID, const char* casename,const char* data,const uint16_t Datasize)const;
    void WriteTestCaseStep(const char* DID, const char* casename,const char* TitleName, const char* data, uint16_t datasize) const;
    void CreatWriteTestCase( WriteDID0x2Einformation* WriteStruct, uint8_t(*testbuffer)[500]);
    void Uint8ToChar(uint8_t* intput, char* output, uint16_t size_t);
//private:
//

};
#endif
