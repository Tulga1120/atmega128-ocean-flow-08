#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <iom128v.h>
#include <macros.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <eeprom.h>

typedef unsigned char       bool;
typedef unsigned char 	    uint8;
typedef char 			    int8;
typedef unsigned int	    uint16;
typedef int				    int16;
typedef unsigned short      uint16s;                    /* Unsigned 16 bit quantity                           */
typedef signed   short      int16s;                     /* Signed   16 bit quantity                           */
typedef unsigned long       uint32;                     /* Unsigned 32 bit quantity                           */
typedef signed   long       int32;                      /* Signed   32 bit quantity                           */
typedef float               fp32;                       /* Single precision floating point                    */
typedef double              fp64;                       /* Double precision floating point                    */
//----------------------------------------------------------------------
#ifndef NULL
#define NULL                0x00
#endif
//----------------------------------------------------------------------
#define  FALSE  		    ((bool)0x00)
#define  TRUE     		    ((bool)0x01)
#define  SHOWTRUE   	    ((bool)0x02)
#define  SAVETRUE   	    ((bool)0x04)
#define  QUITTRUE   	    ((bool)0x08)
#define  UPDATETRUE   	    ((bool)0x10)
//----------------------------------------------------------------------
typedef  enum
{
    CHANGED,
    CHANGING,
    NOTCHANGE
}ChangeType;
//----------------------------------------------------------------------
typedef uint8               AlarmType;
#define  NOTALARM  	  	 	0x00
#define  ALARMED_10MINS  	0x01
#define  ALARMED_20SECS     0x02
//----------------------------------------------------------------------
typedef  enum
{   
    DenTorRotSampleStart,
    DenTorRotSampling,
	DenFlowSampleStart,
	DenFlowSampling,
	SpaTorRotSampleStart,
	SpaTorRotSampling,
	SpaFlowSampleStart,
	SpaFlowSampling,
	SampleStop
}SampleType;
//----------------------------------------------------------------------
typedef  enum
{
    MenuG1=1,
    MenuG2=2,
    MenuG3=3,
    MenuG4=4
}MenuGrade;
//----------------------------------------------------------------------
struct _VolCurStructure
{
    uint16 Vol[3];//
    uint16 Cur[3];//带"//"的为稀疏型模式下要存的数据
};
#define  VolCurStructLen  12        //未使用

struct SaveStructure
{
    uint16 head;///带"///"的为稀疏型模式下要存的数据
	uint8  TimeBuffer[2];
	struct _VolCurStructure VolCurStruct;///
	uint16 TorqueCountAVG;///
	uint16 CP;///
	//------------------------------------------------
	//在这中间的为密集型模式下要存的数据
    uint16 Flow1TimeCountAVG;///
    uint16 Flow2TimeCountAVG;///
	uint16 RotateTimeCountAVG;///
	int16  ACTIVEPREGSUM;///
	int16  POWERFACTORREGAVG;///
	uint16 RotateNumCount;///在稀疏型模式作为尾来存储
    uint16 RMSVOLREGAVG;
    uint16 RMSCURREGAVG;
	uint16 tail;//在密集型模式作为尾来存储
	//------------------------------------------------
};


struct SaveAidStructure
{
    float  Power;
	float  PowerFactor;
	float  Vol[3];
    float  Cur[3];
	uint8  PowerNum;
	uint32 Flow1TimeCountSUM;
	uint16 Flow1Num;
	uint32 Flow2TimeCountSUM;
	uint16 Flow2Num;
	uint32 RotateTimeCountSUM;
	uint8  RotateNum;
	uint32 TorqueCountSUM;
	uint8  TorqueNum;
};
//----------------------------------------------------------------------
struct IntegrationStructure
{
    float OverallPower;
    float TorquePower;
    float AveragePowerFactor;
    float PowerGetEfficiency;
    float AverageFlowSpeed;
};
//----------------------------------------------------------------------
//----------------------------------------------------------------------
#define  BIT_NUM23     	  0x800000
#define  BIT_NUM22     	  0x400000
#define  BIT_NUM21     	  0x200000
#define  BIT_NUM20    	  0x100000
#define  BIT_NUM19     	  0x080000
#define  BIT_NUM18     	  0x040000
#define  BIT_NUM17     	  0x020000
#define  BIT_NUM16     	  0x010000
#define  BIT_NUM15     	  0x008000
#define  BIT_NUM14     	  0x004000
#define  BIT_NUM13     	  0x002000
#define  BIT_NUM12    	  0x001000
#define  BIT_NUM11     	  0x000800
#define  BIT_NUM10     	  0x000400
#define  BIT_NUM9     	  0x000200
#define  BIT_NUM8     	  0x000100
#define  BIT_NUM7     	  0x000080
#define  BIT_NUM6     	  0x000040
#define  BIT_NUM5     	  0x000020
#define  BIT_NUM4    	  0x000010
#define  BIT_NUM3     	  0x000008
#define  BIT_NUM2     	  0x000004
#define  BIT_NUM1     	  0x000002
#define  BIT_NUM0     	  0x000001

#include "uart.h"
#include "common.h"
#include "memory.h"
#include "menu.h"
#include "key.h"
#include "cs5463.h"
#include "lcd.h"
#include "pcf8563.h"
#include "uart.h"
#include "memory.h"
#include "measure.h"

#endif
