#include "config.h"

//------------------------------------------------------
extern uint8 PCArray[MAXDATAWIDTH];
extern struct SCRTAB ScreenTAB; 
//------------------------------------------------------
uint8  CmdStringArray[CmdStrRow][CmdStrCol];
uint8  CmdString[CmdStrLength];
uint8  RXStrBuffer[RXArrayLength];
uint8  rxi = 0x00;
bool   UARTUPDF = FALSE;
uint16 Uart_DelayTime = 10;
uint16 CurrentPageNum = 0x00;
uint16 StartPageNum = 0x00;
uint16 EndPageNum = 0x00;
//------------------------------------------------------
//------------------------------------------------------

void TX_Char(uint8 c)
{
    while(!(UCSR1A&(1<<UDRE1)));
    UDR1 = c;
}

void TX_ARRAY(uint8 ch[],uint16 num)
{
    uint8 i;
    for(i=0;i<num;i++)
    {
        while(!(UCSR1A&(1<<UDRE1)));
        UDR1 = (uint8)ch[i];
        while(!(UCSR1A&(1<<TXC1)));
        UCSR1A |= 1<<TXC1;
    }
}

uint8 RX_Char(void)
{
    while(!(UCSR1A)&(1<<RXC1));
    return UDR1;
}

// UART1 initialize
// desired baud rate:9600
// actual baud rate:9615 (0.2%)
// char size: 8 bit
// parity: Disabled
void uart1_init(void)
{
    UCSR1B = 0x00; //disable while setting baud rate
    UCSR1A = 0x00;
    UCSR1C = 0x06; //8位数据位
    UBRR1L = 0x33; //set baud rate lo//波特率9600HZ
    UBRR1H = 0x00; //set baud rate hi
    UCSR1B = 0x98; //接收使能,发送使能
}

#pragma interrupt_handler uart1_rx_isr:iv_USART1_RXC
void uart1_rx_isr(void)
{
    uint8 UDR1TempChar;
	
	if(!UARTUPDF)
	{
    	if( rxi < (CmdStrLength-1))
        {
            UDR1TempChar = UDR1;
            if(UDR1TempChar != '!')
            {
                CmdString[rxi++] = UDR1TempChar;
            }
            else
            {
                UARTUPDF = TRUE;
                rxi = 0x00;
            }
        }
		else
		{
    		rxi = 0x00;
			memset(CmdString,0x00,CmdStrLength);
		}
	}
}

void SendDataToComputer(uint32 StartAddrCounter, uint32 EndAddrCounter)
{
    uint32 AddrCounter;
    for(AddrCounter = StartAddrCounter;
        AddrCounter < EndAddrCounter;
        AddrCounter += RXArrayLength)
    {
        if((AddrCounter + RXArrayLength) >= EndAddrCounter)
		{
    		Memory_ReadData(AddrCounter,RXStrBuffer,EndAddrCounter-AddrCounter);
        	TX_ARRAY(RXStrBuffer,EndAddrCounter-AddrCounter);
    		return;
		}
		Memory_ReadData(AddrCounter,RXStrBuffer,RXArrayLength);
    	TX_ARRAY(RXStrBuffer,RXArrayLength);
		delay_ms(Uart_DelayTime);
    }
}

void SendPageToComputeruter(uint16 startPagenum,
	 				uint16 endPagenum)
{
    uint32 StartPageAddr;
    uint32 EndPageAddr;
    uint32 PageCounter;
    
    StartPageAddr = ((uint32)(0x3FFF & startPagenum))<<8;
    EndPageAddr = (((uint32)(0x3FFF & endPagenum))<<8) | 0xFF;

    for(PageCounter = StartPageAddr;
        PageCounter < EndPageAddr;
        PageCounter += RXArrayLength)
    {
		Memory_ReadData(PageCounter,RXStrBuffer,RXArrayLength);
    	TX_ARRAY(RXStrBuffer,RXArrayLength);
		delay_ms(Uart_DelayTime);
    }
}

//编写一个函数，把一个字符串中"按一个或多个空格分开的子串"
//放入到一个二维字符数组中,子串个数不会超过4个,子串不会超过8
//并把子串个数作为返回值返回
uint8 SeparateString(uint8 ch[])
{
	uint8 *p = 0x00;
	uint8 *q = 0x00;
	uint8 i = 0x00;
	
	memset(CmdStringArray,0x00,CmdStrRow*CmdStrCol);
	
	p = ch;
	while(*p == ' ')	{p ++;}
	if(*p == 0x00)   {return i;}
	q = strchr(p,' ');
	
	while(q)
	{
	 	memcpy(CmdStringArray[i],p,min(q-p,CmdStrCol-1));
		p = q + 1;
		while(*p == ' ')   {p ++;}
		if(*p == 0x00)   {return ++i;}
    	q = strchr(p,' ');
		i ++;
		if(i > CmdStrRow - 1) { return i;}
	}
	
	q = strchr(p,0x00);
	memcpy(CmdStringArray[i],p,min(q-p,CmdStrCol-1));
	
	return ++i;
}

void CmdStringResponse(void)
{
	#define  StrWidth  6
	uint8 num;
	int16 temp;
	
	num = SeparateString(CmdString);
	memset(PCArray,0x00,MAXDATAWIDTH);
	
	if(!CompareMenuState(0x09,0x05,0x00,0x00,MenuG2))
	{
    	SaveCurMenuState();
		Set_MenuState(0x09,0x05,0x00,0x00);
	}

	ClearScreenTab(' ');
	LOADMENU(UploadPageState,UploadPageState_num);
	ShowMenuBuffer();

	switch(num)
	{
    	case 1:
    		if(!strcmp(CmdStringArray[0],reuploadpageStr))//重新传送
    		{
				memcpy(PCArray,ReuploadPageStr,StrWidth);///
				SendPageToComputeruter(StartPageNum,EndPageNum);
    			CurrentPageNum = EndPageNum;
			}
    		break;
    		
		case 2:
    		if(!strcmp(CmdStringArray[0],uploadpageStr))// 传送数据
    		{
				if(!strcmp(CmdStringArray[1],allStr))//传送所有数据
				{
    				SendPageToComputeruter(0x00,0x3FFF);
    				CurrentPageNum = 0x00;
					memcpy(PCArray,UploadPageStr,StrWidth);///
				}
			
			}
			else if(!strcmp(CmdStringArray[0],setpagenumStr))//设置当前页
    		{
    			memcpy(PCArray,SetPageNumStr,StrWidth);
				temp = atoi(CmdStringArray[1]);
				if(temp<0 || temp > 0x3FFF)	{temp = 0x00;}
				CurrentPageNum = temp;
			}
    		break;
    		
		case 3:
			if(!strcmp(CmdStringArray[0],uploadpageStr))//传送数据
			{
    			memcpy(PCArray,UploadPageStr,StrWidth);
				if(!strcmp(CmdStringArray[1],nextStr))//传送下n页
				{
				 	StartPageNum = CurrentPageNum;
					
					temp = atoi(CmdStringArray[2]) - 1;
					if(temp<0 || temp > 0x3FFF)	{temp = 0x00;}
					EndPageNum = StartPageNum + min(temp,0x3FFF - StartPageNum);
					
					SendPageToComputeruter(StartPageNum,EndPageNum);
					CurrentPageNum = EndPageNum;
				}
				else if(!strcmp(CmdStringArray[1],priorStr))//传送前n页
				{
				 	EndPageNum = CurrentPageNum;
					temp = atoi(CmdStringArray[2]);
					if(temp<0 || temp > 0x3FFF)	{temp = 0x00;}
					StartPageNum = EndPageNum - min(temp,EndPageNum);
					SendPageToComputeruter(StartPageNum,EndPageNum);
					CurrentPageNum = StartPageNum;
				}
				else			   	 			  			//传送第n页到第m页
				{
    				temp = atoi(CmdStringArray[1]);
					if(temp<0 || temp > 0x3FFF)	{temp = 0x00;}
					StartPageNum = temp;
					
					temp = atoi(CmdStringArray[2]);
					if(temp<0 || temp > 0x3FFF)	{temp = 0x00;}
    				EndPageNum = temp;
					
    				SendPageToComputeruter(StartPageNum,EndPageNum);
					CurrentPageNum = EndPageNum;
				}
			}
			break;
			
    	default:break;
	}
	
	ClearScreenTab(' ');
	LOADMENU(UploadPageState,UploadPageState_num);
	if(!PCArray[0]) {memcpy(PCArray,NoOperationStr,StrWidth);}
	memcpy(&ScreenTAB.ScreenTable[3][9],PCArray,StrWidth);//PutDecToBuffer()中用到了PCArray[]
    PutDecToBuffer(&ScreenTAB.ScreenTable[0][9],CurrentPageNum,5);
    PutDecToBuffer(&ScreenTAB.ScreenTable[1][9],StartPageNum,5);
    PutDecToBuffer(&ScreenTAB.ScreenTable[2][9],EndPageNum,5);
    
	ShowMenuBuffer();
	#undef  StrWidth
}