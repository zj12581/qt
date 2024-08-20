#include <iostream>
#include"Transe.h"

//#define MaxLEDNumber 74
#   define Cx8C_LoopStart (140U)
#   define Cx90_LoopEnd (144U)
#   define Cx92_ExtroStart (146U)
#   define Cx96_ExtroEnd (150U)
 uint8_t LedNumber = 54;


static const uint16_t crc16_table[256] =
{
    0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241,
    0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440,
    0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40,
    0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841,
    0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40,
    0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41,
    0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641,
    0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040,
    0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240,
    0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441,
    0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41,
    0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840,
    0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41,
    0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40,
    0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640,
    0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041,
    0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240,
    0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441,
    0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41,
    0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840,
    0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41,
    0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40,
    0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640,
    0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041,
    0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241,
    0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440,
    0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40,
    0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841,
    0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40,
    0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41,
    0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641,
    0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040
};

static void crc16_calc(uint16_t* crc16, uint8_t uc)
{
    /*assure the input isn't NULL*/
    if (crc16 != NULL)
    {
        /*look up the CRC table*/
        (*crc16) = ((*crc16) >> 8U) ^ crc16_table[((*crc16) ^ (uint16_t)uc) & 0xffU];
    }
}
static void frameCRC16(uint16_t* formerCRC16, uint8_t* data, short count)
{
    short i = 0;

    /*assure the input isn't NULL*/
    if ((formerCRC16 != NULL) && (data != NULL))
    {
        /*big endian means calculation begins from the data at the higher address*/
        for (i = (count - 1); i >= 0; i--)
        {
            crc16_calc(formerCRC16, *(data + (uint8_t)i));
        }
    }
}
static void CheckSumCal(FrameStruct* Node, uint16_t* CRC)
{
    uint8_t NeedCRCData[4] = { 0 };
    if (Node)
    {
        switch (Node->FrameType)
        {
        case DataFrame:
            if (Node->DataFrameData)
            {
                for (uint8_t LedIndex = 0; LedIndex < LedNumber; LedIndex++)
                {
                    uint32_t TempData = ((Node->DataFrameData->LEDColorL[LedIndex] & 0xFFF) << 20) + ((Node->DataFrameData->LEDColorU[LedIndex] & 0x3FF) << 10) + ((Node->DataFrameData->LEDColorV[LedIndex]) & 0x3FF);
                    frameCRC16(CRC, (uint8_t*)&TempData, 4);
                }
            }
            break;
        case LoopStart:
#if 1
                NeedCRCData[0] = Node->LoopCnt;
                NeedCRCData[1] = Cx8C_LoopStart;
                frameCRC16(CRC, NeedCRCData, 2);
#else
                NeedCRCData[0] = Cx8C_LoopStart;
                frameCRC16(CRC, NeedCRCData, 1);
#endif
     
            break;
        case LoopEnd:
            NeedCRCData[0] = Cx90_LoopEnd;
            frameCRC16(CRC, NeedCRCData, 1);
            break;
        case EXTROStart:
            NeedCRCData[0] = Cx92_ExtroStart;
            frameCRC16(CRC, NeedCRCData, 1);
            break;
        case EXTROEnd:
            NeedCRCData[0] = Cx96_ExtroEnd;
            frameCRC16(CRC, NeedCRCData, 1);
            break;
        default:
            break;
        }

    }


}
uint16_t  CheckMovieCRC(FrameStruct* Head)
{
    FrameStruct* NowNode = NULL;
    uint16_t InitCrc = 0xFFFF;
    static uint16_t cnt;
    if (Head)
    {
        NowNode = Head->NextFrame;
    }
    while (NowNode)
    {
        CheckSumCal(NowNode, &InitCrc);   
        cnt++;
        NowNode = NowNode->NextFrame;
    }

    return InitCrc;
}
#include<string>
#include<fstream>

uint16_t ClientMovieParse(const char* path, FrameStruct* HEAD)
{
    std::ifstream ifs;
    ifs.open(path);
    std::string line;
    std::getline(ifs, line);
    uint16_t FrameNumber = 0;
    char CLEDNumber[100] = { 0 };
    if (HEAD)
    {
        memset(HEAD, 0, sizeof(FrameStruct));
    }
    else
    {
        return 0;
    }
    
    for (uint32_t i = 0; i < line.size(); i++)
    {
        if (line.data()[i] == ',')
        {
            memcpy(CLEDNumber, line.data(), i);
            LedNumber = atol(CLEDNumber);
            break;
        }
    }
    

    std::getline(ifs, line);
    std::getline(ifs, line);

   
    uint32_t LEDData[3 * MaxLEDNumber] = { 0 };
    FrameStruct* Node = HEAD;
    while (std::getline(ifs, line))
    {
        FrameNumber++;
        memset(LEDData, 0, sizeof(LEDData));
        uint32_t CommaCnt = 0;
        uint32_t LastCommaIndex = 0;
        uint16_t Dataindex = 0;
        char TempData[256] = { 0 };
        char* endptr;
        //uint8_t ControlFlag = 0;
        
        Node->NextFrame = (FrameStruct*)malloc(sizeof(FrameStruct));
        if(Node->NextFrame)
        {
            Node = Node->NextFrame;
            memset(Node, 0, sizeof(FrameStruct));
        }
        else
        {
            return 0;
        }
       
        

        for (uint32_t i = 0; i < line.size(); i++)
        {
            memset(TempData, 0, 256);
            if (line.data()[i] == ';' && CommaCnt == 2)
            {
                memcpy(TempData, line.data() + LastCommaIndex, i - LastCommaIndex);
                if (memcmp(TempData, "START", 6) == 0)
                {
                    Node->FrameType = EXTROStart;
                }
                else  if(memcmp(TempData, "END", 4) == 0)
                {
                    Node->FrameType = EXTROEnd;
                }
                else if (memcmp(TempData, "E", 2) == 0)
                {
                    Node->FrameType = LoopEnd;
                }
                else
                {
                    Node->FrameType = LoopStart;
                    Node->LoopCnt = atol(TempData);
                }
                break;
            }
            if (line.data()[i] == ',')
            {
                CommaCnt++;
               

                if (CommaCnt <= 2)
                {
                   
                    LastCommaIndex = i+1;
                    continue;
                }
                else
                {

                    Node->FrameType = DataFrame;
                    if (i - LastCommaIndex)
                    {
                        memcpy(TempData, line.data() + LastCommaIndex, i - LastCommaIndex);
                    }

                    LastCommaIndex = i+1;
                   
                    double Double_LEDData = strtod(TempData,&endptr);
                    if ((Dataindex + 1) % 3 == 0 )
                    {
                        LEDData[Dataindex] = Double_LEDData * 4000;
                    }
                    else
                    {
                        LEDData[Dataindex] = Double_LEDData * 1000;
                    }
                    Dataindex++;
                }
               
            }
        
        } 
        memcpy(TempData, line.data() + LastCommaIndex, line.size() - LastCommaIndex);
        double Double_LEDData = strtod(TempData, &endptr);
        if ((Dataindex + 1) % 3 == 0)
        {
            LEDData[Dataindex] = Double_LEDData * 4000;
        }
        else
        {
            LEDData[Dataindex] = Double_LEDData * 1000;
        }
        Dataindex++;

        Node->DataFrameData = (DataFrameStruct*)malloc(sizeof(DataFrameStruct));
        if (Node->DataFrameData)
        {
            memset(Node->DataFrameData, 0, sizeof(DataFrameStruct));
        }
        else
        {
            return 0;
        }
       
        
        uint16_t StartIndex=0,DataLen = 0;
        uint8_t StartFlag = 0,EndFlag = 0;
        for (uint16_t i = 0; i < LedNumber; i++)
        {
            Node->DataFrameData->LEDColorU[i] = LEDData[i * 3 + 0];
            Node->DataFrameData->LEDColorV[i] = LEDData[i * 3 + 1];
            Node->DataFrameData->LEDColorL[i] = LEDData[i * 3 + 2];
            if (StartFlag == 0 && (LEDData[i * 3 + 0] || LEDData[i * 3 + 1] || LEDData[i * 3 + 2]))
            {
                StartIndex = i;
                StartFlag = 1;
                DataLen = 1;
               
            }
            if (StartFlag&& EndFlag ==0)
            {
                for (uint16_t j = LedNumber - 1; j > i; j--)
                {
                    if ((LEDData[j * 3 + 0] || LEDData[j * 3 + 1] || LEDData[j * 3 + 2]))
                    {
                        DataLen = j - i + 1;
                        break;
                    }
                }
                EndFlag = 1;

            }
        }
        Node->DataFrameData->LEDStartIndex = StartIndex;
        Node->DataFrameData->LEDLen = DataLen;
        
    }
    ifs.close();
    return FrameNumber;
}
