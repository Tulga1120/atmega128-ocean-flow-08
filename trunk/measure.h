#ifndef __MEASURE_H
#define __MEASURE_H

#define	 FLOW_PORT			            PORTF
#define	 FLOW_NUM1		                BIT_NUM3
#define	 FLOW_NUM2		                BIT_NUM2
#define	 FLOW_NUM3		                BIT_NUM1
#define	 BOTHNOT			            0x00
#define	 THEFLOW1			            0x01
#define	 THEFLOW2			            0x02

#define  DenSaveStructLen               18
#define  SpaSaveStructLen               32
#define  SaveStructDiffLen              20

//#define  Flow1Gain				    0.964
//Flow1Gain 小于 1.0时相当于提高流速
//#define  Flow2Gain				    0.960
//Flow2Gain 小于 1.0时相当于提高流速
//#define  RotateGain				    0.991
//RotateGain 小于 1.0时相当于提高转速
//#define  TorqueGain		            1
//TorqueGain 小于 1.0时相当于降低扭矩
#define  TorqueCountGainAll		        TorqueGain*20
//#define  BatGain				        1.9928
#define  BatGain				        2.4

/*#define  Flow_Proportion	            0.2
#define  Flow1_Proportion	            Flow_Proportion
#define  Flow2_Proportion	            Flow_Proportion
#define  Rotate_Proportion	            1.0
#define  Torque_Proportion	            1.0
#define  Wheel_A 			            1.0
#define  Wheel_D 			            1.5
#define  Wheel_L 			            1.3
#define  Torque_Threshold               5000*/

#define	 SetFlow_Enable(FLOW_NUM)	    SET_REGBIT(FLOW_PORT,(FLOW_NUM))
#define	 SetFlow_Disable(FLOW_NUM)	    CLR_REGBIT(FLOW_PORT,(FLOW_NUM))
#define	 SelectFreqChannel(x)		    PORTD = (PORTD & 0xCF) | ((x)<<4)
#define	 ReadFreqChannel()			    ((PIND & 0x30)>>4)
#define	 AVGTORCNT()				    ((TorqueArray_UseState)?average(TorqueCountArray1,10):average(TorqueCountArray2,10))
#define	 SUMTORCNT()				    ((TorqueArray_UseState)?Sum(TorqueCountArray1,10):Sum(TorqueCountArray2,10))
//-----------------------------------------------------
#define  FLOWFLAG					    0x01
#define  ROTATEFLAG					    0x02
#define  POWERFLAG					    0x04

#define  MAX_ADDRESS				    0x3FFFFF
//-----------------------------------------------------
//宏定义
#define  StopTimer1()				    TCCR1B = 0x00
#define  StartTimer1()				    TCCR1B = 0x07
#define  StopTimer2()				    TCCR2 = 0x00
#define  StartTimer2()				    TCCR2 = 0x07
#define  StopTimer3()				    TCCR3B = 0x00
#define  StartTimer3()				    TCCR3B = 0x03
#define  StopTimer123()				    StopTimer1();StopTimer2();StopTimer3()
#define  StartTimer123()			    StartTimer1();StartTimer2();StartTimer3()
//-----------------------------------------------------
//函数宏定义
#define Start_DenTorRotSample()			Start_TorRotSample(0x01)
#define Start_SpaTorRotSample()			Start_TorRotSample(0x01)
#define Save_DenTorqueRotateCount()		Save_TorqueRotateCount(30,4)
#define Save_SpaTorqueRotateCount()		Save_TorqueRotateCount(4,4)
#define Save_DenFlowCountForShow()		SaveFlowCountForShow(4,4)
#define Save_SpaFlowCountForShow()		SaveFlowCountForShow(4,4)
//-----------------------------------------------------
//函数声明
uint16 average(uint16 data[], uint8 num);
uint16 Sum(uint16 data[], uint8 num);
void  InitMeasurePara_FromEeprom(void);
//
void  Start_TorRotSample(uint8 TimeInterval);//启动转矩转速采样
void  Save_TorqueRotateCount(uint8 QuitTimeInterval, uint8 ShowTimeInterval);//保存转矩转速,T3比较中断B中调用
void  Save_RotateTimeCount(void);//T1溢出中断中调用,
void  SaveFlowCountForShow(uint8 FlowTimeInterval,	 uint8 ShowInterval);
void  Save_Flow1TimeCount(void);
void  Save_Flow2TimeCount(void);
void  PrepareForMeasureFlow(void);
void  SaveTorqueCountArrayToFlash(void);
void  SaveDataArrayToFlash(uint8 pt[],uint8 num);
void  MoveSpaDataToSaveStruct(void);
void  MoveDenDataToSaveStruct(uint8 flag);
void  AddPowerParaToSaveAidStruct(void);
void  SaveDenPackageHeadToFlash(void);//

void  SaveSpaPackageToFlash(void);
void  SaveDenPackageToFlash(void);

void  ModifyTorqueCountArray(void);
void  ModifyTorqueCountArray(void);
float emendation(float BaseValue, float MaxVal,	 float Diff, float CurValue);
//-----------------------------------------------------
#endif