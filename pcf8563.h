#ifndef __PCF8563_H
#define __PCF8563_H

/**********************************************************
 *                     TWI define                         *
 **********************************************************/
#define TWI_START                               0x08
#define TWI_RESTART                             0x10
#define MT_SLA_ACK   	                        0x18
#define MT_SLA_NACK  	                        0x20
#define MT_DATA_ACK  	                        0x28
#define MT_DATA_NACK 	                        0x30
#define MR_SLA_ACK   	                        0x40
#define MR_SLA_NACK   	                        0x48
#define MR_DATA_ACK  	                        0x50
#define MR_DATA_NACK  	                        0x58

//PCF8563¼Ä´æÆ÷µØÖ·
#define PCF8563Addr		                        0xA3
#define CtrlStatus_Reg1                         0x00
#define CtrlStatus_Reg2                         0x01
#define SecRegAddr		                        0x02
#define MinRegAddr		                        0x03
#define HourRegAddr		                        0x04
#define DayRegAddr		                        0x05
#define WeekdayRegAddr	                        0x06
#define MonthRegAddr	                        0x07
#define YearRegAddr	 	                        0x08
#define MinAlarmAddr	                        0x09
#define HourAlarmAddr	                        0x0A
#define DayAlarmAddr	                        0x0B
#define WeekdayAlarmAddr                        0x0C
#define CLKOut_Control	                        0x0D
#define Timer_Control	                        0x0E
#define TimerReg		                        0x0F

#define PCF_AIE			                        1

//#define BGLIGHTDELAYTIME                      600
//-------------------------------------------------------------------
#define PCF8563AlarmEn() 	 				    PCF8563_WriteByte(CtrlStatus_Reg2,BIT(PCF_AIE));\
											    EIMSK |= BIT(INT5)
#define PCF8563AlarmDisEn()  				    PCF8563_WriteByte(CtrlStatus_Reg2,0x00);\
											    EIMSK &= ~BIT(INT5)
#define PCF8563_ReadByte(RegAddr)				TWI_ReadByte(PCF8563Addr,RegAddr)
#define PCF8563_ReadArray(StartReg,data,num)	TWI_ReadArray(PCF8563Addr,StartReg,data,num)
#define PCF8563_WriteByte(RegAddr,data)			TWI_WriteByte(PCF8563Addr,RegAddr,data)
#define PCF8563_WriteArray(RegAddr,ch,num) 		TWI_WriteArray(PCF8563Addr,RegAddr,ch,num)
//-------------------------------------------------------------------

void  TWI_Init(void);
void  TWI_Start(void);
void  TWI_SendSlaveAddr_W(uint8 SlaveAddr);
void  TWI_SendSlaveAddr_R(uint8 SlaveAddr);
void  TWI_SendData_ACK(uint8 data);
void  TWI_SendData_NACK(uint8 data);
void  TWI_Stop(void);
void  TWI_Restart(void);
void  TWI_WriteByte(uint8 SlaveAddr,uint8 reg,uint8 data);
void  TWI_WriteArray(uint8 SlaveAddr, uint8 RegAddr, uint8 data[], uint8 num);
uint8 TWI_ReceiveData_ACK(void);
uint8 TWI_ReceiveData_NACK(void);
uint8 TWI_ReadByte(uint8 SlaveAddr,uint8 reg);
void  ERROR(void);
uint8 BCD_DEC(uint8 BCDCode);
uint8 DEC_BCD(uint8 DECCode);
void  TWI_ReadArray(uint8 SlaveAddr, uint8 reg, uint8 data[], uint8 num);
void  BCD_Separate(uint8 BCDCode,uint8 ch[]);
void  ReadCurTimeToBuffer(uint8 TimeBuf[],uint8 num);
void  ReadCurDateToBuffer(uint8 DateBuf[],uint8 num);
void  SetTime(uint8 Hour,uint8 Min,uint8 Sec);
void  SetDate(uint8 Year,uint8 Month,uint8 Day);
void  SetAlarm(uint8 Hour,uint8 Min);
void  TimeAddOneMin(uint8 Timech[]);
void  TimeAddMins(uint8 Timech[],uint8 mins);
void  TimeAddSecs(uint8 Timech[],uint8 secs);
bool  CompareTime(uint8 TimeBuf1[], uint8 TimeBuf2[], uint8 num);
#endif