#include "config.h"

//-----------------------------------------------------
//用于调试
bool  CS5463DebugEnable = TRUE;
//-----------------------------------------------------
float ACTIVEPREG[3];
float POWERFACTORREG[3];
float RMSVOLREG[3];
float RMSCURREG[3];
//-----------------------------------------------------
float VolGain_A;		
float VolOffset_A;	
float CurGain_A;		  
float CurOffset_A;	
int32 PowerGain_A;	 
float PowerOffset_A;	 
//-------------------------------
float VolGain_B;		
float VolOffset_B;	
float CurGain_B;		  
float CurOffset_B;	
int32 PowerGain_B;	 
float PowerOffset_B;	
//-------------------------------
float VolGain_C;		
float VolOffset_C;	
float CurGain_C;		  
float CurOffset_C;	
int32 PowerGain_C;	 
float PowerOffset_C;	
//-----------------------------------------------------
bool  CS5463UPDF = FALSE;
uint8 CS5463_CSTMP = 0x00;
uint32 STAREG[3] = {0x00,0x00,0x00};
//-----------------------------------------------------

void cs5463_PortInit(void)
{ 
	 CS5463_DDR   = CS5463_SCLK  | CS5463_SDI | CS5463_RESET | CS5463_CSABC;
	 CS5463_PORT  = CS5463_RESET | CS5463_SDO | CS5463_CSABC;

	 CS5463_DDR1  &= ~CS5463_INT;
	 CS5463_PORT1 |= CS5463_INT;	//输入上拉
}

void cs5463_RegInit(void)
{
	CS5463_WriteReg(Page_Reg,Page0,CS5463_CSABC);	//向Page Register写入Page0
	CS5463_WriteReg(Config_Reg,/*CS_IMODE |*/ CS_K0,CS5463_CSABC);
	CS5463_WriteReg(Status_Reg,0xFFFFFF,CS5463_CSABC);
	CS5463_WriteReg(Mask_Reg,CS_DRDY,CS5463_CSABC);//打开 Data Ready 中断
	CS5463_WriteReg(Ctrl_Reg,CS_INTOD,CS5463_CSABC);//CS5463中断输出引脚设为上拉输出
    CS5463_WriteReg(CycleCount_Reg,4000,CS5463_CSABC); 
}

void cs5463_write(uint32 Cmd, uint32 data, uint8 CS5463_CS)
{
     uint8 i;

	 data |= Cmd<<24;
	 CLR_REGBIT(CS5463_PORT,CS5463_CS);

	 for(i=0;i<32;i++)
	 {
	  	if(data & 0x80000000)
		{
		    SET_REGBIT(CS5463_PORT,CS5463_SDI);
		}
		else
		{
		    CLR_REGBIT(CS5463_PORT,CS5463_SDI);
		}
		SET_REGBIT(CS5463_PORT,CS5463_SCLK);
		data<<=1;
		CLR_REGBIT(CS5463_PORT,CS5463_SCLK);
	 }
	 
	 SET_REGBIT(CS5463_PORT,CS5463_CS);
}

uint32 cs5463_read(uint8 Cmd, uint8 CS5463_CS)
{
 	 uint32 data=0x00000000;
	 uint8 i;

	 CLR_REGBIT(CS5463_PORT,CS5463_CS);
	 
	 for(i=0;i<8;i++)
	 {
	  	if(Cmd & 0x80)
		{
		    SET_REGBIT(CS5463_PORT,CS5463_SDI);
		}
		else
		{
		    CLR_REGBIT(CS5463_PORT,CS5463_SDI);
		}
		SET_REGBIT(CS5463_PORT,CS5463_SCLK);
		Cmd <<= 1;
		CLR_REGBIT(CS5463_PORT,CS5463_SCLK);
	 }
	 
	 SET_REGBIT(CS5463_PORT,CS5463_SDI);
	 data = 0x00;
	 for(i=0;i<24;i++)
	 {
		SET_REGBIT(CS5463_PORT,CS5463_SCLK);
		data <<= 1;
		data |= READ_REGBIT(CS5463_PIN,CS5463_SDOBIT);
		CLR_REGBIT(CS5463_PORT,CS5463_SCLK);
	 }
	 
	 SET_REGBIT(CS5463_PORT,CS5463_CS);
	 CLR_REGBIT(CS5463_PORT,CS5463_SDI);
	 
 	 return data;
}

float RegValue2Float(int32 data,bool tflag)
{
    if(tflag)
	{
        if( data&BIT_NUM23 ){data |= 0xFF000000;}
		return ((float)data)/0x007FFFFF;
	}
	else
	{
        return ((float)data)/0x00FFFFFF;
	}
}

void InitPowerPara_FromEeprom(void)
{
	EEPROM_READ(CurGain_A_Addr,CurGain_A);
	EEPROM_READ(VolGain_A_Addr,VolGain_A);
	EEPROM_READ(PowerGain_A_Addr,PowerGain_A);

	EEPROM_READ(CurGain_B_Addr,CurGain_B);
	EEPROM_READ(VolGain_B_Addr,VolGain_B);
	EEPROM_READ(PowerGain_B_Addr,PowerGain_B);

	EEPROM_READ(CurGain_C_Addr,CurGain_C);
	EEPROM_READ(VolGain_C_Addr,VolGain_C);
	EEPROM_READ(PowerGain_C_Addr,PowerGain_C);
	
	EEPROM_READ(CurOffset_A_Addr,CurOffset_A);
	EEPROM_READ(VolOffset_A_Addr,VolOffset_A);
	EEPROM_READ(PowerOffset_A_Addr,PowerOffset_A);

	EEPROM_READ(CurOffset_B_Addr,CurOffset_B);
	EEPROM_READ(VolOffset_B_Addr,VolOffset_B);
	EEPROM_READ(PowerOffset_B_Addr,PowerOffset_B);

	EEPROM_READ(CurOffset_C_Addr,CurOffset_C);
	EEPROM_READ(VolOffset_C_Addr,VolOffset_C);
	EEPROM_READ(PowerOffset_C_Addr,PowerOffset_C);
}

void CS5463_HarwareReset(void)
{
    CS5463_DDR |= CS5463_RESET;
    CS5463_PORT &= ~CS5463_RESET;
    delay_1us();
    CS5463_PORT |= CS5463_RESET;
}