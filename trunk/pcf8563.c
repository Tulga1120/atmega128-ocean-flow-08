#include "config.h"

extern uint8 PCArray[MAXDATAWIDTH];
//---------------------------------------------------------
AlarmType TimeAlarm = NOTALARM;
bool   TimeUPDF = FALSE;
uint8  error_code;
uint8  PCF8563OldSec = 0xFF;
uint8  CurrentTimeBuf[4];
uint8  TimeBufForDen[3] = {0xFF,0x00,0x00};//设定一个不可能到达的时间
uint8  TimeBufForSpa[3] = {0xFF,0x00,0x00};//设定一个不可能到达的时间
uint16 BGLIGHTDELAYTIME = 600;
//---------------------------------------------------------
//---------------------------------------------------------
void TWI_Init(void)
{
 DDRD&=0xfc; PORTD|=0x03;
 TWCR= 0X00; //disable twi
 TWBR= 0x20; //set bit rate 100KHz=8M/(16+64)
 TWSR= 0x00; //set prescale
 TWAR= 0x00; //set slave address
// TWCR= 0x44; //enable twi
}

void TWI_Start(void)//Start
{
	 TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	 //delay_us(50);//while (!(TWCR & (1<<TWINT)));
	 while (!(TWCR & (1<<TWINT)));
	 //if ((TWSR & 0xF8) != TWI_START)  {error_code=1;ERROR();}              
}

void TWI_SendSlaveAddr_W(uint8 SlaveAddr) //Send Slave Address Write
{
	 TWDR = SlaveAddr & 0xFE;
	 TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
	 //delay_us(50);//while (!(TWCR & (1<<TWINT))); 
	 while (!(TWCR & (1<<TWINT))); 
	 //if ((TWSR & 0xF8)!=MT_SLA_ACK) {error_code=3;ERROR();}          
}

void TWI_SendSlaveAddr_R(uint8 SlaveAddr) //Send Slave Address Read
{
	 TWDR = SlaveAddr | 0x01;
	 TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
	 //delay_us(50);//while (!(TWCR & (1<<TWINT)));
	 while (!(TWCR & (1<<TWINT)));
	 //if ((TWSR & 0xF8)!=MR_SLA_ACK) {error_code=3;ERROR();}          
}

void TWI_SendData_ACK(uint8 data) //Send Data
{
	 TWDR = data;
	 TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
	 //delay_us(50);//while (!(TWCR & (1<<TWINT)));
	 while (!(TWCR & (1<<TWINT)));
	 //if ((TWSR & 0xF8)!=MT_DATA_ACK) {error_code=3;ERROR();}          
}

void TWI_SendData_NACK(uint8 data) //Send Data
{
	 TWDR = data;
	 TWCR = (1<<TWINT)|(1<<TWEN);
	 //delay_us(50);//while (!(TWCR & (1<<TWINT))); 
	 while (!(TWCR & (1<<TWINT))); 
	 //if ((TWSR & 0xF8)!=MT_DATA_NACK) {error_code=3;ERROR();}          
}

void TWI_Stop(void)//Stop
{
 	 TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);                          
}

void TWI_Restart(void)//Restart
{
	 TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	 //delay_us(50);//while (!(TWCR & (1<<TWINT))); 
	 while (!(TWCR & (1<<TWINT))); 
	 //if ((TWSR & 0xF8) != TWI_RESTART)  {error_code=1;ERROR();}              
}

uint8 TWI_ReceiveData_ACK(void) //
{
	 TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
	 //delay_us(50);//while (!(TWCR & (1<<TWINT))); 
	 while (!(TWCR & (1<<TWINT))); 
	 //if ((TWSR & 0xF8)!=MR_DATA_ACK) {return 0x00;}  
	 return TWDR;        
}

uint8 TWI_ReceiveData_NACK(void) //
{
	 TWCR = (1<<TWINT)|(1<<TWEN);
	 //delay_us(50);//while (!(TWCR & (1<<TWINT))); 
	 while (!(TWCR & (1<<TWINT))); 
	 //if ((TWSR & 0xF8)!=MR_DATA_NACK) {return 0x00;}  
	 return TWDR;        
}

void TWI_WriteByte(uint8 SlaveAddr,uint8 reg,uint8 data)
{
 	 TWI_Start();
	 TWI_SendSlaveAddr_W(SlaveAddr);
	 TWI_SendData_ACK(reg);
	 TWI_SendData_ACK(data);
	 TWI_Stop();
}

void TWI_WriteArray(uint8 SlaveAddr,
	 				uint8 RegAddr,
					uint8 data[],
					uint8 num)
{
 	 uint8 i;
	 TWI_Start();
	 TWI_SendSlaveAddr_W(SlaveAddr);
	 TWI_SendData_ACK(RegAddr);
	 for (i=0;i<num;i++)
	 {TWI_SendData_ACK(data[i]);}
	 TWI_Stop();
}

uint8 TWI_ReadByte(uint8 SlaveAddr,uint8 reg)
{
 	 uint8 REGR_RSUT;
 	 TWI_Start();
	 TWI_SendSlaveAddr_W(SlaveAddr);
	 TWI_SendData_ACK(reg);
	 TWI_Restart();
	 TWI_SendSlaveAddr_R(SlaveAddr);
	 REGR_RSUT = TWI_ReceiveData_NACK();
	 TWI_Stop();   
	 return REGR_RSUT;
}

void TWI_ReadArray(uint8 SlaveAddr,
	 			   uint8 reg,
				   uint8 data[],
				   uint8 num)
{
 	 uint8 REGR_RSUT;
	 uint8 i;
 	 TWI_Start();
	 TWI_SendSlaveAddr_W(SlaveAddr);
	 TWI_SendData_ACK(reg);
	 TWI_Restart();
	 TWI_SendSlaveAddr_R(SlaveAddr);
	 for(i=0;i<num-1;i++)
	 {data[i] = TWI_ReceiveData_ACK();}
	 data[num-1] = TWI_ReceiveData_NACK();
	 TWI_Stop();   
}

void ERROR(void)
{
 	 switch (error_code)
	 {
	  		case 1: break;
			case 2: break;
			case 3: break;
	 }
}

uint8 BCD_DEC(uint8 BCDCode)
{
	 return (BCDCode>>4)*10 + (BCDCode & 0x0F);
}

uint8 DEC_BCD(uint8 DECCode)
{
	 return ((DECCode/10)<<4) | (DECCode%10);
}

void TimeAddOneMin(uint8 Timech[])
{
 	 uint8 TMin;
	 uint8 THour;
	 
	 TMin = BCD_DEC(Timech[0]);
	 THour = BCD_DEC(Timech[1]);
	 
	 TMin++;
	 if(TMin >= 60)
	 {
        TMin = 0;
        THour++;
        if(THour >= 24)
        {THour = 0;}
	 }
	 Timech[0] = DEC_BCD(TMin);
	 Timech[1] = DEC_BCD(THour);
}

void TimeAddMins(uint8 Timech[],uint8 mins)
{
 	 uint8 TMin;
	 uint8 THour;
	 
	 TMin = BCD_DEC(Timech[1]);
	 THour = BCD_DEC(Timech[2]);
	 
	 TMin += mins;
	 if(TMin >= 60)
	 {
        TMin %= 60;
        THour++;
        if(THour >= 24)
        {THour = 0;}
	 }
	 
	 Timech[1] = DEC_BCD(TMin);
	 Timech[2] = DEC_BCD(THour);
}

void TimeAddSecs(uint8 Timech[],uint8 secs)
{
 	 uint8 TSec;
	 uint8 TMin;
	 uint8 THour;
	 
	 TSec = BCD_DEC(Timech[0]);
	 TMin = BCD_DEC(Timech[1]);
	 THour = BCD_DEC(Timech[2]);
	 
	 TSec += secs;
	 if(TSec >= 60)
	 {
    	 TSec %= 60;
		 TMin ++;
    	 if(TMin >= 60)
    	 {
            TMin %= 60;
            THour++;
            if(THour >= 24)
            {THour = 0;}
    	 }
	 }
	 
	 Timech[0] = DEC_BCD(TSec);
	 Timech[1] = DEC_BCD(TMin);
	 Timech[2] = DEC_BCD(THour);
}

void BCD_Separate(uint8 BCDCode,uint8 ch[])
{
	ch[0] = ((BCDCode >> 4)&0x07) + 0x30;
	ch[1] = (BCDCode & 0x0F) + 0x30;
}

void ReadCurTimeToBuffer(uint8 TimeBuf[],uint8 num)
{
 	 PCF8563_ReadArray(SecRegAddr,TimeBuf,num);
	 while(num--)
	 {TimeBuf[num] &= 0x7F;}
}

void ReadCurDateToBuffer(uint8 DateBuf[],uint8 num)
{
 	 PCF8563_ReadArray(DayRegAddr,DateBuf,num);
	 while(num--)
	 {DateBuf[num] &= 0x7F;}
}

bool CompareTime(uint8 TimeBuf1[],
	 			 uint8 TimeBuf2[],
				 uint8 num)

{
 	 if(!memcmp(TimeBuf1,TimeBuf2,num))
	 {
    	 return TRUE;
	 }
	 else
	 {
    	 return FALSE;
	 }
}

void SetTime(uint8 Hour,uint8 Min,uint8 Sec)
{
	PCArray[0] = Sec;
	PCArray[1] = Min;
	PCArray[2] = Hour;
	PCF8563_WriteArray(SecRegAddr,PCArray,3);
}

void SetDate(uint8 Year,uint8 Month,uint8 Day)
{
	PCArray[0] = Day;
	PCArray[1] = 0x00;
	PCArray[2] = Month;
	PCArray[3] = Year;
	PCF8563_WriteArray(DayRegAddr,PCArray,4);
}

void SetAlarm(uint8 Hour,uint8 Min)
{
 	PCArray[0] = Min;
	PCArray[1] = Hour;
	PCArray[2] = 0x80;
	PCArray[3] = 0x80;
	PCF8563_WriteByte(CtrlStatus_Reg2,BIT(PCF_AIE));
	PCF8563_WriteArray(MinAlarmAddr,PCArray,4);
}