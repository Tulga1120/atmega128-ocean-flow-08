#ifndef __MAIN_H
#define __MAIN_H

#include "config.h"

//-----------------------------------------------------
#define  SaveTimeInterval  60
#define  SaveByteNumDiff   80
#define  SaveByteNumDiff1  200
//-----------------------------------------------------
//用于调试
extern uint8 CS5463DebugEnable;
//-----------------------------------------------------
//辅助调试变量
SampleType SampleModeFlagAid = SampleStop;
SampleType SampleModeFlagOld = SampleStop;

extern struct IntegrationStructure IntegrationStruct;
uint8 IntegrationUPDF = FALSE;

extern float  TorqueGain;
extern uint16 TorqueOffset;
extern float  RotateGain;
extern float  RotateOffset;
extern float  Flow1Gain;
extern float  Flow1Offset;
extern float  Flow2Gain;
extern float  Flow2Offset;

extern float  RotateTime;
extern float  Flow1Time;
extern float  Flow2Time;
//-------------------------------
extern float  Flow1_Proportion;
extern float  Flow2_Proportion;
extern float  Rotate_Proportion;
extern float  Torque_Proportion;
extern float  Wheel_A;
extern float  Wheel_D;
extern float  Wheel_L;
extern uint16 Torque_Threshold;
//-------------------------------
extern float  VolGain_A;		
extern float  VolOffset_A;	
extern float  CurGain_A;		  
extern float  CurOffset_A;	
extern int32  PowerGain_A;	 
extern float  PowerOffset_A;	 
//-------------------------------
extern float  VolGain_B;		
extern float  VolOffset_B;	
extern float  CurGain_B;		  
extern float  CurOffset_B;	
extern int32  PowerGain_B;	 
extern float  PowerOffset_B;	
//-------------------------------
extern float  VolGain_C;		
extern float  VolOffset_C;	
extern float  CurGain_C;		  
extern float  CurOffset_C;	
extern int32  PowerGain_C;	 
extern float  PowerOffset_C;	
//-------------------------------
extern int16  TorqueCount;

extern float  RotateGain;
extern float  RotateOffset;
//-----------------------------------------------------
//-----------------------------------------------------
//本地变量
uint8  sti = 0;
uint16 EepromTimeCounter = 0x00;
//-----------------------------------------------------
extern uint16 BgLightDelayTime;
extern uint32 AddrCounterLastSaved;
extern SampleType SampleModeFlag;
extern bool   DataSaveEnableFlag;
extern uint16 T3OCRA_Count;
extern uint16 T3OCRB_Count;
extern uint8  SampleDenModeNum;
extern uint32 MemoryAddressCounter;
extern uint32 OldMemoryAddressCounter;
extern uint8  CurrentTimeBuf[4];
extern uint8  TimeBufForDen[3];
extern uint8  TimeBufForSpa[3];
struct SaveStructure SaveStruct;
struct SaveAidStructure SaveAidStruct;
//-----------------------------------------------------
//"uart.c"相关变量
extern uint8 RXStrBuffer[RXArrayLength];
extern uint8 rxi;
extern bool  UARTUPDF;
extern uint8 CmdStringArray[CmdStrRow][CmdStrCol];
extern uint8 CmdString[CmdStrLength];
//-----------------------------------------------------
//"lcd.c"相关变量
extern const uint8 AddrTable[4];
//-----------------------------------------------------
//"key.c"相关变量
extern uint8 KeyValue;
extern bool  HotKeyState;
//-----------------------------------------------------
//"PCF8563.c"相关变量
extern AlarmType TimeAlarm;
extern bool      TimeUPDF;
extern uint8     PCF8563OldSec;
extern uint16    BGLIGHTDELAYTIME;
//-----------------------------------------------------
//"menu.c"相关变量
extern bool MenuStateUPDF;
extern bool ScreenUPDF;
extern struct SCRTAB ScreenTAB; 
extern struct MenuStateStruct MenuState;
extern struct MenuStateStruct OldMenuState;
//-----------------------------------------------------
//被显示数据临时存放空间
uint8 PCArray[MAXDATAWIDTH];
//-----------------------------------------------------
//"CS5463.c"相关变量
extern bool   CS5463UPDF;
extern float  RMSCURREG[3];
extern float  RMSVOLREG[3];
extern float  ACTIVEPREG[3];
extern float  POWERFACTORREG[3];
extern uint8  CS5463_CSTMP;
extern uint32 STAREG[3];
//-----------------------------------------------------
//"measure.c"相关变量
extern bool   BatVolUPDF;
extern bool   MeasureUPDF;
extern bool   TorqueRotateSampleFlag;
extern uint16 ADCREG;
extern uint8  TorqueOvfCount;

extern uint32 RotateTimeCountStart0;
extern uint32 RotateTimeCountStartx;
extern uint32 RotateTimeCountEnd;
extern uint32 RotateTimeCountEndx;
extern uint8  RotateNumCount;
extern uint8  RotateNumCountx0;
extern uint8  RotateNumCountx1;

extern uint32 Flow1TimeCountStart0;
extern uint32 Flow1TimeCountStartx;
extern uint32 Flow1TimeCountEnd;
extern uint32 Flow1TimeCountEndx;
extern uint8  Flow1NumCount;
extern uint8  Flow1NumCountx0;
extern uint8  Flow1NumCountx1;

extern uint32 Flow2TimeCountStart0;
extern uint32 Flow2TimeCountStartx;
extern uint32 Flow2TimeCountEnd;
extern uint32 Flow2TimeCountEndx;
extern uint8  Flow2NumCount;
extern uint8  Flow2NumCountx0;
extern uint8  Flow2NumCountx1;

extern bool   FlowSampleFlag;
extern uint16 TorqueCountArray1[10];
extern uint16 TorqueCountArray2[10];
extern bool   TorqueArray_UseState;
extern bool   FirstIntoSpaTorRotSample;

void port_init(void);
void timer0_init(void);
void timer0_ovf_isr(void);
void timer1_init(void);
void timer1_ovf_isr(void);
void timer2_init(void);
void timer2_ovf_isr(void);
void timer3_init(void);
void timer3_compa_isr(void);
void timer3_compb_isr(void);
void int4_isr(void);
void int5_isr(void);
void adc_init(void);
void adc_isr(void);
void init_devices(void);

#endif
