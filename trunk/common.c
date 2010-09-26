
#include "config.h"
//------------------------------------------------------------------
extern uint8 PCArray[MAXDATAWIDTH];
int16 status;
extern SampleType SampleModeFlag;
extern uint16 TorqueCountArray1[10];
extern uint16 TorqueCountArray2[10];
//------------------------------------------------------------------

void delay_1us(void)
{
    asm("nop");
}

void delay_us(uint16 n)
{
	while(n--)
	{delay_1us();}
}

void delay_1ms(void)
{
    uint16 i;
    for(i=1;i<((uint16)(xtal*143-2));i++);
}

void delay_ms(uint16 tn)
{
    while(tn--)
	{delay_1ms();}
}

void PutDataToBuffer(uint8 s[], float fdata, uint8 num)
{
 	 uint8 i;
	 
	 memset(s,' ',num);
	 strcpy(PCArray,ftoa(fdata,&status));
	 num = min(num,strlen(PCArray));
	 if(status!=0) 
	 {
        memcpy(s,"0.000",min(num,5));
	 }
	 else
	 {
    	 memcpy(s,PCArray,num);
		 if(s[num-1] == '.'){s[num-1] = ' ';}
	 } 
}

void PutHexToBuffer(uint8 s[],uint32 data)
{
    uint8 i;
	uint8 *p = (uint8*)(&data);
	uint8 ch[3];

	for(i=0;i<4;i++)
	{
    	if(*p>0x0F)
		{
    		itoa(ch,*p++,16);
		}
		else
		{
    		ch[0] = '0';
    		itoa(ch+1,*p++,16);
		}
		memcpy(&s[(3-i)*2],ch,2);
	}
}

void PutDecToBuffer(uint8 s[],uint16 data,uint8 StrWidth)
{
	uint8 datawidth;
	memset(s,'0',StrWidth);
	itoa(PCArray,data,10);
	datawidth = min(StrWidth,strlen(PCArray));
	memcpy(&s[StrWidth - datawidth],PCArray,datawidth);
}

void PutHexByteToBuffer(uint8 s[],uint32 data)
{
	uint8 *p = (uint8*)(&data);
	s[3] = *p++;
	s[2] = *p++;
	s[1] = *p++;
	s[0] = *p;	
}

/*void SetParaToDefault(void)
{
	EEPROM_WRITE(CurGain_A_Addr,CurGain_A);
	EEPROM_WRITE(VolGain_A_Addr,VolGain_A);
	EEPROM_WRITE(PowerGain_A_Addr,PowerGain_A);

	EEPROM_WRITE(CurGain_B_Addr,CurGain_B);
	EEPROM_WRITE(VolGain_B_Addr,VolGain_B);
	EEPROM_WRITE(PowerGain_B_Addr,PowerGain_B);

	EEPROM_WRITE(CurGain_C_Addr,CurGain_C);
	EEPROM_WRITE(VolGain_C_Addr,VolGain_C);
	EEPROM_WRITE(PowerGain_C_Addr,PowerGain_C);
	
	EEPROM_WRITE(CurOffset_A_Addr,CurOffset_A);
	EEPROM_WRITE(VolOffset_A_Addr,VolOffset_A);
	EEPROM_WRITE(PowerOffset_A_Addr,PowerOffset_A);

	EEPROM_WRITE(CurOffset_B_Addr,CurOffset_B);
	EEPROM_WRITE(VolOffset_B_Addr,VolOffset_B);
	EEPROM_WRITE(PowerOffset_B_Addr,PowerOffset_B);

	EEPROM_WRITE(CurOffset_C_Addr,CurOffset_C);
	EEPROM_WRITE(VolOffset_C_Addr,VolOffset_C);
	EEPROM_WRITE(PowerOffset_C_Addr,PowerOffset_C);
	
	EEPROM_WRITE(TorqueGain_Addr,TorqueGain);
	EEPROM_WRITE(TorqueOffset_Addr,TorqueOffset);
	EEPROM_WRITE(RotateGain_Addr,RotateGain);
	EEPROM_WRITE(RotateOffset_Addr,RotateOffset);
	EEPROM_WRITE(Flow1Gain_Addr,Flow1Gain);
	EEPROM_WRITE(Flow1Offset_Addr,Flow1Offset);
	EEPROM_WRITE(Flow2Gain_Addr,Flow2Gain);
	EEPROM_WRITE(Flow2Offset_Addr,Flow2Offset);
	
	EEPROM_WRITE(Uart_DelayTime_Addr,Uart_DelayTime);
	EEPROM_WRITE(BGLIGHTDELAYTIME_Addr,BGLIGHTDELAYTIME);
}*/
