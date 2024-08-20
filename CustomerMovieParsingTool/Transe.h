#pragma once
#include<stdint.h>
#define CRT_SECURE_NO_WARNINGS
//#define MCU1LEDNUMBER 62
//#define MCU2LEDNUMBER 62
//#define MCU3LEDNUMBER 62
//#define MCU4LEDNUMBER 62
//#define MCU5LEDNUMBER 68
//#define MCU6LEDNUMBER 63
//#define MCU7LEDNUMBER 63
//#define MCU8LEDNUMBER 68
#define MaxLEDNumber 74//(MCU1LEDNUMBER+MCU2LEDNUMBER+MCU3LEDNUMBER+MCU4LEDNUMBER+MCU5LEDNUMBER+MCU6LEDNUMBER+MCU7LEDNUMBER+MCU8LEDNUMBER)
#define RTE_OK 1
#define RTE_NOT_OK 0
struct FrameStruct;

struct DataFrameStruct
{
	uint16_t LEDColorU[MaxLEDNumber];
	uint16_t LEDColorV[MaxLEDNumber];
	uint32_t LEDColorL[MaxLEDNumber];
	uint8_t  LEDStartIndex;
	uint8_t  LEDLen;
};
//struct EXTROStartStruct
//{
//	uint16
//};
struct FrameStruct
{

	uint8_t FrameType;
	uint16_t LoopCnt;
	DataFrameStruct* DataFrameData;
	FrameStruct* NextFrame;


};


typedef struct
{
	uint16_t TotalFrame;
	uint16_t MovieID;
	uint16_t SlotID;
	uint16_t CRC;
}SMovieInfo;
enum LEDInformation
{
	LedIndex,
	Data_U,
	Data_V,
	Data_L
};
enum FrameType
{
	DataFrame,
	LoopStart,
	LoopEnd,
	EXTROStart,
	EXTROEnd

};
extern  uint8_t LedNumber;
uint16_t  CheckMovieCRC(FrameStruct* Head);
uint16_t ClientMovieParse(const char* path, FrameStruct* HEAD);
