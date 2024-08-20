// ClientMovieconversiontool.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <filesystem>
#include <fstream>
#include<Windows.h>
#include <string>
#include"Transe.h"
#include"ClientMovieconversiontool.h"

typedef struct MovieStruct
{
    uint32_t FrameNumber;
    uint16_t SlotID;
    uint16_t MovieID;
    uint16_t CRC;
}SMovieStruct;

typedef struct
{
    uint32_t V : 10;
    uint32_t U : 10;
    uint32_t L : 12;

}SLedCofig;

static uint16_t   NowMovieIndex;;
SMovieStruct NowMovieInfo = { 0 };
#define StartSlot (40u);
static FrameStruct HEAD = { 0 };
void DeleteNode(FrameStruct* Node)
{
    
    while (Node)
    {
        
        FrameStruct* DeleteNode = Node;
        Node = Node->NextFrame;
        free(DeleteNode);
    }
  
}

char Name[][100] = { ",U",",V",",L" };

void WriteMovieCSV(const char* CSVpath, FrameStruct* Head)
{
    std::ofstream output_file(CSVpath);

    FrameStruct* Node = Head->NextFrame;
    output_file << "FrameType";
    output_file << ",StartIndex,LEDLen";
    for (uint16_t index = 0; index < 74; index++)
    {
        output_file << ",LED_" << index + 1 << Name[0] << Name[1] << Name[2];
    }
    output_file << ",End" << std::endl;
    output_file << "MovieID" << "," << NowMovieInfo.MovieID << "," << "SlotID" << "," << NowMovieInfo.SlotID << "," << "CRC" << "," << NowMovieInfo.CRC << ", " << "TotalFrame" << "," << NowMovieInfo.FrameNumber << ", , , , , , ," << std::endl;
    for (uint32_t i = 0; i < NowMovieInfo.FrameNumber; i++)
    {
        //UDataUnion TempLEDData[74] = { 0 };
        char FrameType[256] = { 0 };
      
        if (Node->FrameType == DataFrame)
        {
            uint32_t StartIndex = Node->DataFrameData->LEDStartIndex;
            uint32_t FrameLen = Node->DataFrameData->LEDLen;
            output_file << "," << StartIndex << "," << FrameLen;

            for (uint8_t j = 0; j < LedNumber; j++)
            {

                output_file << ",," << Node->DataFrameData->LEDColorU[j] << "," << Node->DataFrameData->LEDColorV[j] << "," << Node->DataFrameData->LEDColorL[j];
            }
            output_file <<",";
            output_file << std::endl;
        }
        else if (Node->FrameType == LoopStart)
        {
            output_file << "LoopStart," << Node->LoopCnt << ",,,," << std::endl;
           
        }
        else if (Node->FrameType == LoopEnd)
        {
            output_file << "LoopEnd" << ",,,," << std::endl;
        }
        else if (Node->FrameType == EXTROStart)
        {
            output_file << "EXTROStart" << ",,,," << std::endl;

        }
        else if (Node->FrameType == EXTROEnd)
        {
            output_file << "EXTROEnd" << ",,,," << std::endl;

        }
        else
        {
            break;
        }
        Node = Node->NextFrame;
    }
    output_file.close();
}
static void parseonemovie(const char* path1, const char* path2)
{

    NowMovieInfo.FrameNumber = ClientMovieParse(path1, &HEAD);
    NowMovieInfo.SlotID = NowMovieIndex + StartSlot;
    NowMovieInfo.MovieID = NowMovieIndex + 10*StartSlot;
    NowMovieInfo .CRC = CheckMovieCRC(&HEAD);


    if(NowMovieInfo.FrameNumber)
    {
        WriteMovieCSV(path2, &HEAD);
    }
    DeleteNode(HEAD.NextFrame);
    HEAD.NextFrame = NULL;
}
static uint16_t TotalCnt = 0;
void GettxtNumber(std::string& FolderPath)
{

    for (const auto& entry : std::filesystem::directory_iterator(FolderPath)) {
        if (entry.is_regular_file() && entry.path().extension() == ".txt") {
            TotalCnt++;
        }
    }
}
void SelectFolderTranse(std::string& FolderPath,QProgressBar* Progressbar )
{

    std::ofstream CsvListNameFile(FolderPath + "MovieList.csv");
    Progressbar->setMaximum(TotalCnt);
    Progressbar->setMinimum(0);
    for (const auto& entry : std::filesystem::directory_iterator(FolderPath)) {
        if (entry.is_regular_file() && entry.path().extension() == ".txt") {
            std::string txt_filename = entry.path().filename().string();
            std::string csv_filename = txt_filename.substr(0, txt_filename.find_last_of(".")) + ".csv"; // 用于创建 CSV 文件名
            char TextPath[256] = { 0 };
            char CSVPath[256] = { 0 };
            sprintf(CSVPath, "%s%s", FolderPath.data(), csv_filename.data());
            sprintf(TextPath, "%s%s", FolderPath.data(), txt_filename.data());
            parseonemovie(TextPath, CSVPath);
            NowMovieIndex++;
            Progressbar->setValue(NowMovieIndex);
            CsvListNameFile << csv_filename << std::endl;
        }
    }
    TotalCnt = 0;
    NowMovieIndex = 0;
    CsvListNameFile.close();
}

