#include "config.h"

//----------------------------------------------------------------------
//----------------------------------------------------------------------
uint8  MemoryStatusREG;//声明一个字节型变量,用于保存M25P32状态寄存器的值
bool   DataSaveEnableFlag = FALSE;
uint32 MemoryAddressCounter;
uint32 AddrCounterLastSaved;
uint32 OldMemoryAddressCounter;
uint16 AddrCounterPointToEeprom = StartAddrCounterInEeprom;
bool   MemoryEraseEnable = FALSE;
//----------------------------------------------------------------------
//----------------------------------------------------------------------
void Memory_PortInit(void)
{
    SET_REGBIT(MEMORY_DDR,MEMORY_CS  | MEMORY_IN | MEMORY_SCK | MEMORY_HOLD);   
    SET_REGBIT(MEMORY_PORT,MEMORY_CS | MEMORY_IN |MEMORY_HOLD);
						  
    CLR_REGBIT(MEMORY_DDR1,MEMORY_OUT);
    SET_REGBIT(MEMORY_PORT1,MEMORY_OUT);
	
	SET_REGBIT(MEMORY_DDR2,MEMORY_W);
	SET_REGBIT(MEMORY_PORT2,MEMORY_W);
}

void Memory_Write(uint8 cmd,uint8 data[],uint8 num)
{
    uint8 i,j;
    
    CLR_REGBIT(MEMORY_PORT,MEMORY_SCK);
	CLR_REGBIT(MEMORY_PORT,MEMORY_CS);
	for(i=0;i<8;i++)
    {
         if(cmd & 0x80)
         {
             SET_REGBIT(MEMORY_PORT,MEMORY_IN);
         }
         else
         {
             CLR_REGBIT(MEMORY_PORT,MEMORY_IN);
         }
		 SET_REGBIT(MEMORY_PORT,MEMORY_SCK);
		 cmd <<= 1;
		 CLR_REGBIT(MEMORY_PORT,MEMORY_SCK);
    }
	
	for(i=0;i<num;i++)
	{
	 	 for(j=0;j<8;j++)
		 {
    		 if(data[i] & 0x80)
             {
                 SET_REGBIT(MEMORY_PORT,MEMORY_IN);
             }
             else
             {
                 CLR_REGBIT(MEMORY_PORT,MEMORY_IN);
             }
    		 SET_REGBIT(MEMORY_PORT,MEMORY_SCK);
			 data[i] <<= 1;
    		 CLR_REGBIT(MEMORY_PORT,MEMORY_SCK);
		 }
	}
	SET_REGBIT(MEMORY_PORT,MEMORY_CS);
}

void Memory_WriteEx(uint8 cmd, uint32 addr, uint8 data[], uint8 num)
{
    uint8 i,j;

	CLR_REGBIT(MEMORY_PORT,MEMORY_SCK);
	CLR_REGBIT(MEMORY_PORT,MEMORY_CS);
	for(i=0;i<8;i++)
    {
         if(cmd & 0x80)
         {
             SET_REGBIT(MEMORY_PORT,MEMORY_IN);
         }
         else
         {
             CLR_REGBIT(MEMORY_PORT,MEMORY_IN);
         }
		 SET_REGBIT(MEMORY_PORT,MEMORY_SCK);
		 cmd <<= 1;
		 CLR_REGBIT(MEMORY_PORT,MEMORY_SCK);
    }
	
    for(i=0;i<24;i++)
    {
        if(addr & 0x800000)
        {
            SET_REGBIT(MEMORY_PORT,MEMORY_IN);
        }
        else
        {
            CLR_REGBIT(MEMORY_PORT,MEMORY_IN);
        }
        SET_REGBIT(MEMORY_PORT,MEMORY_SCK);
		addr <<= 1;
        CLR_REGBIT(MEMORY_PORT,MEMORY_SCK);
    }
	
	for(i=0;i<num;i++)
	{
	 	 for(j=0;j<8;j++)
		 {
    		 if(data[i] & 0x80)
             {
                 SET_REGBIT(MEMORY_PORT,MEMORY_IN);
             }
             else
             {
                 CLR_REGBIT(MEMORY_PORT,MEMORY_IN);
             }
    		 SET_REGBIT(MEMORY_PORT,MEMORY_SCK);
			 (data[i]) <<= 1;
    		 CLR_REGBIT(MEMORY_PORT,MEMORY_SCK);
		 }
	}
	SET_REGBIT(MEMORY_PORT,MEMORY_CS);
}

void Memory_Read(uint8 cmd,uint8 data[],uint8 num)
{
    uint8 i,j;
    
    CLR_REGBIT(MEMORY_PORT,MEMORY_SCK);
	CLR_REGBIT(MEMORY_PORT,MEMORY_CS);
	for(i=0;i<8;i++)
    {
         if(cmd & 0x80)
         {
             SET_REGBIT(MEMORY_PORT,MEMORY_IN);
         }
         else
         {
             CLR_REGBIT(MEMORY_PORT,MEMORY_IN);
         }
		 SET_REGBIT(MEMORY_PORT,MEMORY_SCK);
		 cmd <<= 1;
		 CLR_REGBIT(MEMORY_PORT,MEMORY_SCK);
    }
	
	for(i=0;i<num;i++)
	{
	 	 data[i] = 0x00;
		 for(j=0;j<0x08;j++)
		 {
    		 SET_REGBIT(MEMORY_PORT,MEMORY_SCK);
        	 data[i] <<= 1;
    		 data[i] |= READ_REGBIT(MEMORY_PIN1,MEMORY_OUTBIT);
        	 CLR_REGBIT(MEMORY_PORT,MEMORY_SCK);
		 }
	}
	SET_REGBIT(MEMORY_PORT,MEMORY_CS);
}

void Memory_ReadEx(uint8 cmd, uint32 addr, uint8 data[], uint8 num)
{
    uint8 i,j;
    
    CLR_REGBIT(MEMORY_PORT,MEMORY_SCK);
	CLR_REGBIT(MEMORY_PORT,MEMORY_CS);
	for(i=0;i<8;i++)
    {
         if(cmd & 0x80)
         {
             SET_REGBIT(MEMORY_PORT,MEMORY_IN);
         }
         else
         {
             CLR_REGBIT(MEMORY_PORT,MEMORY_IN);
         }
		 SET_REGBIT(MEMORY_PORT,MEMORY_SCK);
		 cmd <<= 1;
		 CLR_REGBIT(MEMORY_PORT,MEMORY_SCK);
    }
	
	for(i=0;i<24;i++)
    {
        if(addr & 0x800000)
        {
            SET_REGBIT(MEMORY_PORT,MEMORY_IN);
        }
        else
        {
            CLR_REGBIT(MEMORY_PORT,MEMORY_IN);
        }
        SET_REGBIT(MEMORY_PORT,MEMORY_SCK);
		addr <<= 1;
        CLR_REGBIT(MEMORY_PORT,MEMORY_SCK);
    }
	
	for(i=0;i<num;i++)
	{
	 	 data[i] = 0x00;
		 for(j=0;j<0x08;j++)
		 {
    		 SET_REGBIT(MEMORY_PORT,MEMORY_SCK);
        	 data[i] <<= 1;
    		 data[i] |= READ_REGBIT(MEMORY_PIN1,MEMORY_OUTBIT);
        	 CLR_REGBIT(MEMORY_PORT,MEMORY_SCK);
		 }
	}
	SET_REGBIT(MEMORY_PORT,MEMORY_CS);
}

void ClearAddrCounterInEeprom(void)
{
 	uint16 num;
	uint32 Value = 0x0000;
	
	for(num = StartAddrCounterInEeprom;
		num < EndAddrCounterInEeprom;
		num += 4)
	{
    	EEPROM_WRITE(num,Value);
	}
}
/*void PowerPareInit(void)
{
    float num;
	signed long int num1;
	
	//------------------------------------------
	//电流增益
	num = 19;
	EEPROM_WRITE(CurGain_A_Addr,num);
	num = 82;
	EEPROM_WRITE(CurGain_B_Addr,num);
	num = 19;
	EEPROM_WRITE(CurGain_C_Addr,num);
	//------------------------------------------
	//电压增益
	num = 484;
	EEPROM_WRITE(VolGain_A_Addr,num);
	num = 489;
	EEPROM_WRITE(VolGain_B_Addr,num);
	num = 484;
	EEPROM_WRITE(VolGain_C_Addr,num);
	//------------------------------------------
	//功率增益
	num1 = 7472;
	EEPROM_WRITE(PowerGain_A_Addr,num);
	num1 = 33872;
	EEPROM_WRITE(PowerGain_B_Addr,num);
	num1 = 7672;
	EEPROM_WRITE(PowerGain_C_Addr,num);
}*/

void Init_MemoryAddressCounter(void)
{
 	uint32 TempMemoryAddr;
	uint16 addr;
	
	EEPROM_READ(StartAddrCounterInEeprom,MemoryAddressCounter);
    for(addr = StartAddrCounterInEeprom + 4;
	    addr < EndAddrCounterInEeprom;
		addr += 4)
	{
    	EEPROM_READ(addr, TempMemoryAddr);
		if(MemoryAddressCounter < TempMemoryAddr)
		{
    		MemoryAddressCounter = TempMemoryAddr;
			AddrCounterPointToEeprom = addr;
		}
	}
	
	AddrCounterLastSaved = MemoryAddressCounter;//用于判断地址保存条件
}

void Save_MemoryAddressCounter(void)
{
    AddrCounterPointToEeprom += 4;
	if(AddrCounterPointToEeprom >= EndAddrCounterInEeprom)
	{
    	AddrCounterPointToEeprom = StartAddrCounterInEeprom;
	}
	EEPROM_WRITE(AddrCounterPointToEeprom,MemoryAddressCounter);
	AddrCounterLastSaved = MemoryAddressCounter;
}
