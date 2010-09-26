#include "config.h"

//-----------------------------------------------------
//用于调试
extern bool   CS5463DebugEnable;
//-----------------------------------------------------
//辅助调试变量
extern SampleType SampleModeFlagAid;
extern uint16 BGLIGHTDELAYTIME;
//-----------------------------------------------------
//背光自动关闭延时时间
extern uint16 BgLightDelayTime;
//-----------------------------------------------------
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
extern bool   TorqueArray_UseState;
extern uint16 AddrCounterPointToEeprom;
extern uint8  CmdString[CmdStrLength];
extern bool   MemoryEraseEnable;
extern uint16 TorqueCountArray1[10];
extern uint16 TorqueCountArray2[10];
extern bool   CS5463UPDF;
extern bool   ScreenUPDF;
extern bool   MeasureUPDF;
extern AlarmType TimeAlarm;
extern bool   HotKeyState;
extern bool   MenuStateUPDF;
extern bool   DataSaveEnableFlag;
extern SampleType SampleModeFlag;
extern uint8  SampleDenModeNum;
extern uint8  TimeBufForSpa[3];
extern uint8  TimeBufForDen[3];

extern float  RMSCURREG[3];
extern float  RMSVOLREG[3];
extern float  ACTIVEPREG[3];
extern float  POWERFACTORREG[3];

extern uint8  PCArray[MAXDATAWIDTH];
extern struct SCRTAB ScreenTAB; 
extern struct MenuStateStruct MenuState;
extern struct MenuStateStruct OldMenuState;
extern uint32 MemoryAddressCounter;
extern uint8  MemoryStatusREG;
extern uint16 Uart_DelayTime;
extern uint32 AddrCounterLastSaved;
//-----------------------------------------------------
//本地变量
uint8  KeyValue = no_key;
bool   HotKeyState = TRUE;
struct KEYPRERESPONSESTRUCT KeyResponseStruct;
//-----------------------------------------------------

uint8 ReadKey(void)
{
 	static uint8 key_flag=0x01;
 	uint8 key_val1;
	uint8 i;
	
 	KEY_PORT1 = 0xF0;
 	KEY_DDR1  = 0x0F;
	KEY_PORT2 = KEY_PORT2 | BIT(1);
	DDRG  = DDRG & (~BIT(1));
	delay_1us();
	key_val1=(PINB>>4) | (KEY_PIN2 & BIT(1))<<0x03;
	
	if(0x1F!=key_val1) 	 //如果有键按下
	{
	 	if(0x01==key_flag)		//如果是第一次按下
		{
		 	//----------------------------------------
			//对key_flay的第0位清零
			key_flag &= ~BIT(0);
			//----------------------------------------
			//对键盘扫描
			for(i=0;i<4;i++)
			{
			 	KEY_PORT1 = 0x0F & (~BIT(i));
				delay_1us();
				key_val1 = (PINB>>4) | ((KEY_PIN2 & BIT(1))<<0x03);
				if( 0x1F!=key_val1 )
				{ 
				    key_val1 |= i<<6;
				 	break;
				}
			}
            
            switch(key_val1)
            {
            	case 0x1E:	return k_modify;
                case 0x1D:	return k_menu;
                case 0x1B:	return k_num9;
                case 0x17:	return k_num8;
                case 0x0F:	return k_num7;
                
                case (0x40 | 0x1E):	return k_right;
                case (0x40 | 0x1D):	return k_up;	
                case (0x40 | 0x1B):	return k_num6;	
                case (0x40 | 0x17):	return k_num5;	
                case (0x40 | 0x0F):	return k_num4;	
                
                case (0x80 | 0x1E):	return k_down;	
                case (0x80 | 0x1D):	return k_left;	
                case (0x80 | 0x1B):	return k_num3;	
                case (0x80 | 0x17):	return k_num2;	
                case (0x80 | 0x0F):	return k_num1;	
                
                case (0xC0 | 0x1E):	return k_enter;	
                case (0xC0 | 0x1D):	return k_return;	
                case (0xC0 | 0x1B):	return k_sign;	
                case (0xC0 | 0x17):	return k_dot;	
                case (0xC0 | 0x0F):	return k_num0;	
                
                default:	return no_key;	
            }				
		}
		else 	//如果不是第一次按下
		{
		 	return no_key;
		}
	}
	else 	//如果无键按下
	{
	 	key_flag |= BIT(0);
		return no_key;
	}
}

void KeyBoardSetFunction(void *VarName, uint8 M1, uint8 M2, uint8 M3, uint8 M4, uint16 EepromAddr)
{
    switch(LowKeySetFlagByte())
    {
        case KeySetFlag_Setting:
            //显示字符
            if(KeySetStringIsEmpty())
            {
                memset(&ScreenTAB.ScreenTable[2][7],' ',KeyResponseStruct.KeySetStrLen);
            }
            memcpy(&ScreenTAB.ScreenTable[2][7],
                    KeyResponseStruct.KeySetString,
                    strlen(KeyResponseStruct.KeySetString));
            break;
        case KeySetFlag_EndOfSet:
            //保存数据
            if(KeySetFlagIsValueSaved())
            {
                if(KeySetFlagIsFloatType())
				{
    				memcpy(VarName,&KeyResponseStruct.KeySetValue.fval,4);
					if(EepromAddr)
					{
    					EEPROM_WRITE(EepromAddr,KeyResponseStruct.KeySetValue.fval);
					}
				}
				else if(KeySetFlagIsLongType())
				{
    				memcpy(VarName,&KeyResponseStruct.KeySetValue.lval,4);
					if(EepromAddr)
					{
    					EEPROM_WRITE(EepromAddr,KeyResponseStruct.KeySetValue.lval);
					}
				}
				else if(KeySetFlagIsIntType())
				{
    				memcpy(VarName,&KeyResponseStruct.KeySetValue.ival,2);
					if(EepromAddr)
					{
    					EEPROM_WRITE(EepromAddr,KeyResponseStruct.KeySetValue.ival);
					}
				}
				ClearKeySetFlagH(KeySetFlag_ValueSaved);
            }
            Set_OldMenuState(0xFF,0xFF,0xFF,0xFF);
            MenuStateUPDF = TRUE;
            break;
        case KeySetFlag_NotChanged:
            //原数据不变,清空字符串
            memset(&ScreenTAB.ScreenTable[2][7],' ',KeyResponseStruct.KeySetStrLen);
            break;
        case KeySetFlag_Return:
            //原数据不变,退出
            Set_MenuState(M1,M2,M3,M4);//返回到上一级菜单
            ClearKeySetFlag();
            MenuStateUPDF = TRUE;
            break;
        default:
            ClearKeySetFlag();
            break;
    }
    ShowMenuBuffer();
}

void KeyBoardSetDateTime(int8 TypeStr[], uint8 M1, uint8 M2, uint8 M3, uint8 M4)
{
    switch(LowKeySetFlagByte())
    {
        case KeySetFlag_Setting:
            //显示字符
            memset(PCArray,' ',MAXDATAWIDTH);
			memcpy(PCArray,KeyResponseStruct.KeySetString,
        		   strlen(KeyResponseStruct.KeySetString));
            memcpy(&ScreenTAB.ScreenTable[2][6],PCArray,2);	
            memcpy(&ScreenTAB.ScreenTable[2][9],&PCArray[2],2);	
			memcpy(&ScreenTAB.ScreenTable[2][12],&PCArray[4],2);	
            if(!strcmp(TypeStr,"date"))
			{
    			ScreenTAB.ScreenTable[2][8] = '-';
                ScreenTAB.ScreenTable[2][11] = '-';
			}
			else if(!strcmp(TypeStr,"time"))
			{
    			ScreenTAB.ScreenTable[2][8] = ':';
                ScreenTAB.ScreenTable[2][11] = ':';
			}
			break;
        case KeySetFlag_EndOfSet:
            //保存数据
            if(KeySetFlagIsValueSaved())
            {
			 	if(strlen(KeyResponseStruct.KeySetString) == 0x06)
				{
    				if(!strcmp(TypeStr,"date"))
					{
    					SetDate((KeyResponseStruct.KeySetString[0]-0x30)*0x10 +
            					(KeyResponseStruct.KeySetString[1]-0x30),
    							(KeyResponseStruct.KeySetString[2]-0x30)*0x10 +
            					(KeyResponseStruct.KeySetString[3]-0x30),
    							(KeyResponseStruct.KeySetString[4]-0x30)*0x10 +
            					(KeyResponseStruct.KeySetString[5]-0x30));
					}
					else if(!strcmp(TypeStr,"time"))
					{
           				SetTime((KeyResponseStruct.KeySetString[0]-0x30)*0x10 +
            					(KeyResponseStruct.KeySetString[1]-0x30),
    							(KeyResponseStruct.KeySetString[2]-0x30)*0x10 +
            					(KeyResponseStruct.KeySetString[3]-0x30),
    							(KeyResponseStruct.KeySetString[4]-0x30)*0x10 +
            					(KeyResponseStruct.KeySetString[5]-0x30));
					}
				}
				ClearKeySetFlagH(KeySetFlag_ValueSaved);
            }
            Set_OldMenuState(0xFF,0xFF,0xFF,0xFF);
            MenuStateUPDF = TRUE;
            break;
        case KeySetFlag_NotChanged:
            //原数据不变,清空字符串
            memset(&ScreenTAB.ScreenTable[2][6],' ',8);	
			if(!strcmp(TypeStr,"date"))
			{
    			ScreenTAB.ScreenTable[2][8] = '-';
                ScreenTAB.ScreenTable[2][11] = '-';
			}
			else if(!strcmp(TypeStr,"time"))
			{
    			ScreenTAB.ScreenTable[2][8] = ':';
                ScreenTAB.ScreenTable[2][11] = ':';
			}
            break;
        case KeySetFlag_Return:
            //原数据不变,退出
            Set_MenuState(M1,M2,M3,M4);//返回到上一级菜单
            ClearKeySetFlag();
            MenuStateUPDF = TRUE;
            break;
        default:
            ClearKeySetFlag();
            break;
    }
    ShowMenuBuffer();
}

void LoadKeySetMenuStr(int8 menustr[], int8 str[], void* varname, int8 vartype, int8 MaxLen)
{
	ResetKeySetFlag(KeySetFlag_Setting);
	KeyResponseStruct.KeySetStrLen = MaxLen;
    ClearKeySetString();
	
	if(!strcmp(menustr,"normal"))
	{
    	LOADMENU(KeySetMenu,KeySetMenu_num);//加载按键设置菜单
		LoadMenuRowStr(str,0);//
    	switch(vartype)
    	{
            case 'i':
            	itoa(PCArray,*((uint16*)varname),10);
            	break;
        	case 'f':
        		strcpy(PCArray,ftoa(*((float*)varname) + 0.000001,0x00));
    			KeySetFlagSetType(KeySetFlag_FloatType);
                if(PCArray[MaxLen-1] == '.')
                {
                    PCArray[MaxLen-1] = ' ';
                }
            	break;
        	case 'l':
            	ltoa(PCArray,*((long int*)varname),10);
    			KeySetFlagSetType(KeySetFlag_LongType);
            	break;
    		default:break;
    	}

        memcpy(&ScreenTAB.ScreenTable[1][7],PCArray,min(strlen(PCArray),MaxLen)); 
	}
	else if(!strcmp(menustr,"time"))
	{
    	LOADMENU(KeySetTimeMenu,KeySetTimeMenu_num);//加载时间日期按键设置菜单
		LoadMenuRowStr(str,0);//
        //把当前时间放入显示缓冲区
		ReadCurTimeToBuffer(PCArray,0x03);
        BCD_Separate(PCArray[2],&ScreenTAB.ScreenTable[1][6]);
        BCD_Separate(PCArray[1],&ScreenTAB.ScreenTable[1][9]);
        BCD_Separate(PCArray[0],&ScreenTAB.ScreenTable[1][12]);
		ScreenTAB.ScreenTable[1][8] = ':';
		ScreenTAB.ScreenTable[1][11] = ':';
	} 
	else if(!strcmp(menustr,"date"))
	{
    	LOADMENU(KeySetTimeMenu,KeySetTimeMenu_num);//加载时间日期按键设置菜单
		LoadMenuRowStr(str,0);//
        //把当前日期放入显示缓冲区
		ReadCurDateToBuffer(PCArray,0x04);
        BCD_Separate(PCArray[3],&ScreenTAB.ScreenTable[1][6]);
        BCD_Separate(PCArray[2],&ScreenTAB.ScreenTable[1][9]);
        BCD_Separate(PCArray[0],&ScreenTAB.ScreenTable[1][12]);
		ScreenTAB.ScreenTable[1][8] = '-';
		ScreenTAB.ScreenTable[1][11] = '-';
	} 
}

bool Key_PreResponse(void)
{
 	uint8 ValStr[2];
	uint8 KeyValue1;
	
	if(no_key == KeyValue)  
	{
    	return FALSE;
	}
	else
	{
		KeyValue1 = KeyValue;
	}
	
	if(BgLightDelayTime > BGLIGHTDELAYTIME) 
	{
    	lcd_bglight_enable();
    	BgLightDelayTime = 0x00;
		KeyValue = no_key;
    	return FALSE;
	}
	BgLightDelayTime = 0x00;
	
	if(k_menu == KeyValue1)	
	{
    	//清除标志
		ResetKeySetFlag(KeySetFlag_NotSet);
		return TRUE;
	}
	
	if(!(LowKeySetFlagByte()))
	{
    	return TRUE;
	}

	if(KeySetFlag_Setting != LowKeySetFlagByte())
	{
        if(k_return == KeyValue1)
		{
			SetKeySetFlag(KeySetFlag_Return);
    		KeyValue = no_key;
    		ScreenUPDF = TRUE;
			return FALSE;
		}	
		else 
		{
			SetKeySetFlag(KeySetFlag_Setting);
		}
	}	
	if(KeySetFlag_Setting == LowKeySetFlagByte())
	{
		if(KeyValue1 >= k_num0 && KeyValue1 <= k_num9)
		{
    		if(KeyResponseStruct.KeySetStrLen>strlen(KeyResponseStruct.KeySetString))
			{
    			ValStr[0] = KeyValue1 + 0x30;
    			ValStr[1] = 0x00;
    			strcat(KeyResponseStruct.KeySetString,ValStr);
			}
		}
		else if(k_dot == KeyValue1)
		{
		 	if(KeySetFlagIsFloatType())
			{
    			if(KeySetFlagIsFirstPoint() &&
				(KeyResponseStruct.KeySetStrLen>strlen(KeyResponseStruct.KeySetString)))
    			{
        			strcat(KeyResponseStruct.KeySetString,".");
					KeyResponseStruct.KeySetFlag |= KeySetFlag_NotFirstPoint;//..
    			}
			}
		}
		else if(k_sign == KeyValue1)
		{
    		if(KeySetFlagIsSignedType())
    		{
        		strcpy(PCArray,KeyResponseStruct.KeySetString);
				if(KeyResponseStruct.KeySetString[0] == '-')
    			{
        			strcpy(KeyResponseStruct.KeySetString,&PCArray[1]);
    			}
    			else
    			{
        			if(KeyResponseStruct.KeySetStrLen>strlen(KeyResponseStruct.KeySetString))
					{
    					KeyResponseStruct.KeySetString[0] = '-';
    					strcpy(&KeyResponseStruct.KeySetString[1],PCArray);
					}
    			}
    		}
		}
		else if(k_enter == KeyValue1)
		{
		 	//转化数据
			if(!KeySetStringIsEmpty())
			{
                if(KeySetFlagIsFloatType())
                {
                    KeyResponseStruct.KeySetValue.fval = 
                        atof(KeyResponseStruct.KeySetString);
                }
				else if(KeySetFlagIsLongType())
				{
    				KeyResponseStruct.KeySetValue.lval = 
                        atol(KeyResponseStruct.KeySetString);
				}
                else /*if(KeySetFlagIsIntType())*/
                {
                    KeyResponseStruct.KeySetValue.ival = 
                        atoi(KeyResponseStruct.KeySetString);
                }
				SetKeySetFlagH(KeySetFlag_ValueSaved);
				SetKeySetFlag(KeySetFlag_EndOfSet);
			}
			else
			{
				ClearKeySetFlagH(KeySetFlag_ValueSaved);
			}
		}
		else if(k_return == KeyValue1)
		{
    		if(KeySetStringIsEmpty())
			{
    			SetKeySetFlag(KeySetFlag_Return);
			}
			else
			{
				ClearKeySetString();
				SetKeySetFlag(KeySetFlag_NotChanged);
				ClearKeySetFlagH(KeySetFlag_NotFirstPoint);
				ClearKeySetFlagH(KeySetFlag_ValueSaved);
			}
		}
		ScreenUPDF = TRUE;
		KeyValue = no_key;
		return FALSE;
	}
	else
	{
        return TRUE;
	}
}

void Key_Response(void)
{
    if(no_key == KeyValue)
	{return;}
	
	if(CompareMenuState(0x09,0x05,0x00,0x00,MenuG2))
	{
    	LoadOldMenuState();
    	Set_OldMenuState(0xFF,0xFF,0xFF,0xFF);
		MenuStateUPDF = TRUE;
		KeyValue = no_key;
		return;
	}
	
	switch( KeyValue )
    {
	 	//-----------------------------
		//用于调试
		case k_num1:
		case k_num2:
		case k_num3:
		case k_num4:
		case k_num5:
		case k_num6:
    		if(CS5463DebugEnable)
    		{
        		Set_MenuState(0x0A,KeyValue,0x00,0x00);
                MenuStateUPDF = TRUE;
			}
			KeyValue = no_key;
    		break;
		//-----------------------------
		//当KeyValue == k_num0时
		/*case k_num0:
			KeyValue = no_key;
    		break;
		//当KeyValue == k_num1时
		case k_num1:
			KeyValue = no_key;
    		break;
		//当KeyValue == k_num2时
		case k_num2:
			KeyValue = no_key;
    		break;
		//当KeyValue == k_num3时
		case k_num3:
			KeyValue = no_key;
    		break;
		//当KeyValue == k_num4时
		case k_num4:
			KeyValue = no_key;
    		break;
		//当KeyValue == k_num5时
		case k_num5:
			KeyValue = no_key;
    		break;
		//当KeyValue == k_num6时
		case k_num6:
			KeyValue = no_key;
    		break;
		//当KeyValue == k_num7时
		case k_num7:
			KeyValue = no_key;
    		break;
		//当KeyValue == k_num8时
		case k_num8:
			KeyValue = no_key;
    		break;
		//当KeyValue == k_num9时
		case k_num9:
			KeyValue = no_key;
    		break;
		//当KeyValue == k_dot时
		case k_dot:
			KeyValue = no_key;
    		break;
		//当KeyValue == k_sign时
		case k_sign:
    		KeyValue = no_key;
    		break;*/
		//当KeyValue == k_menu时
		case k_menu:
    		Set_MenuState(0x00,0x07,0x00,0x00);
			MenuStateUPDF = TRUE;
			HotKeyState = FALSE;
			KeyValue = no_key;
    		break;
		//当KeyValue == k_modify时
		case k_modify:
    		if(HotKeyState)
			{
    			Set_MenuState(0x00,0x05,0x00,0x00);
    			MenuStateUPDF = TRUE;
			}
    		KeyValue = no_key;
    		break;
		//当KeyValue == k_return时
		case k_return:
			if(HotKeyState)
			{
    			Set_MenuState(0x00,0x00,0x00,0x00);
				MenuStateUPDF = TRUE;
			}
			else
			{
			 	if(CompareMenuState(0x00,0x00,0x00,0x00,MenuG1))
				{
    				if(0x01 == MenuState.MG2 ||
					   0x02 == MenuState.MG2 ||
					   0x03 == MenuState.MG2 ||
					   0x05 == MenuState.MG2)
					{
    					Set_MenuState(0x01,0x00,0x00,0x00);
                        MenuStateUPDF = TRUE;
					}
					else if(0x04 == MenuState.MG2 ||
					   	 	0x06 == MenuState.MG2)
					{
    					Set_MenuState(0x01,0x01,0x00,0x00);
                        MenuStateUPDF = TRUE;
					}
					else if(0x07 == MenuState.MG2)
					{
					 	Set_MenuState(0x00,0x00,0x00,0x00);
						HotKeyState = TRUE;
                        MenuStateUPDF = TRUE;
					}
				}
				else if(CompareMenuState(0x01,0x00,0x00,0x00,MenuG1) ||//显示菜单
					 	CompareMenuState(0x02,0x00,0x00,0x00,MenuG1) ||//设置菜单
						CompareMenuState(0x03,0x00,0x00,0x00,MenuG1))//功能菜单
				{
    				Set_MenuState(0x00,0x07,0x00,0x00);
                    MenuStateUPDF = TRUE;
				}
				else if(CompareMenuState(0x04,0x01,0x00,0x00,MenuG2) ||//参数配置菜单1
					    CompareMenuState(0x04,0x02,0x00,0x00,MenuG2))  //参数配置菜单2
				{
    				Set_MenuState(0x00,0x07,0x00,0x00);
                    MenuStateUPDF = TRUE;
				}
				else if(CompareMenuState(0x04,0x00,0x00,0x00,MenuG2))//参数配置菜单的各个子项
				{
    				if(MenuState.MG3)
					{
    					if(MenuState.MG3 <= 0x05)
						{
    						Set_MenuState(0x04,0x01,0x00,0x00);
						}
						else if(MenuState.MG3 >= 0x06 && MenuState.MG3 <= 0x07)
						{
    						Set_MenuState(0x04,0x02,0x00,0x00);
						}
					}
					else if(MenuState.MG4 >= 0x05 && MenuState.MG4 <= 0x06)
					{
    					Set_MenuState(0x04,0x00,0x02,0x00);
					}
					else
					{
    					Set_MenuState(0x04,0x00,0x01,0x00);
					}
                    MenuStateUPDF = TRUE;
				}
				else if(CompareMenuState(0x09,0x01,0x00,0x00,MenuG2))
				{
				 	Set_MenuState(0x03,0x00,0x00,0x00);
					Set_OldMenuState(0xFF,0xFF,0xFF,0xFF);
                    MenuStateUPDF = TRUE;
				}
				else if(CompareMenuState(0x09,0x00,0x00,0x00,MenuG1) &&
        			   (MenuState.MG2 == 0 || MenuState.MG2 == 4 || MenuState.MG2 == 6))
				{
    				if(MenuState.MG2 == 4 || MenuState.MG2 == 0)
					{
    					Set_OldMenuState(0xFF,0xFF,0xFF,0xFD);
						MemoryEraseEnable = FALSE;
					}
					else
					{
    					Set_OldMenuState(0xFF,0xFF,0xFF,0xFE);
					}
					Set_MenuState(0x03,0x00,0x00,0x00);
                    MenuStateUPDF = TRUE;
				}
				else if(CompareMenuState(0x09,0x07,0x00,0x00,MenuG2))
				{
                    Set_MenuState(0x02,0x00,0x00,0x00);
                    MenuStateUPDF = TRUE;
				}
				else if(CompareMenuState(0x04,0x05,0x00,0x00,MenuG2) &&
        				MenuState.MG3)
				{
                    if(MenuState.MG3 >= 1 && MenuState.MG3 <= 4)
					{
    					Set_MenuState(0x04,0x00,0x06,0x00);
					}
					else if(MenuState.MG3 >= 5 && MenuState.MG3 <= 8)
					{
    					Set_MenuState(0x04,0x00,0x07,0x00);
					}
                    MenuStateUPDF = TRUE;
				}
			}
    		KeyValue = no_key;
    		break;
		//当KeyValue == k_enter时
		case k_enter:
    		if(HotKeyState)
			{
    			Set_MenuState(0x00,0x06,0x00,0x00);
				MenuStateUPDF = TRUE;
			}
			else
			{
    			if(CompareMenuState(0x00,0x07,0x00,0x00,MenuG2))//主菜单
    			{
                    switch(ScreenTAB.CurNum)
					{
    					case 1:
        					Set_MenuState(0x01,0x00,0x00,0x00);//显示
                            MenuStateUPDF = TRUE;
        					break;
    					case 2:
    						Set_MenuState(0x02,0x00,0x00,0x00);//设置
                            MenuStateUPDF = TRUE;
        					break;
    					case 3:
    						Set_MenuState(0x03,0x00,0x00,0x00);//功能操作
                            MenuStateUPDF = TRUE;
        					break;
    					case 4:
    						Set_MenuState(0x04,0x01,0x00,0x00);//参数配置操作
                            MenuStateUPDF = TRUE;
        					break;
						default:break;
					}
				}
				else if(CompareMenuState(0x01,0x00,0x00,0x00,MenuG2))//ShowMenuList1
				{
				  	switch(ScreenTAB.CurNum)
					{
    					case 1:
        					Set_MenuState(0x00,0x05,0x00,0x00);
                            MenuStateUPDF = TRUE;
        					break;
    					case 2:
    						Set_MenuState(0x00,0x01,0x00,0x00);
                            MenuStateUPDF = TRUE;
        					break;
                        case 3:
                            Set_MenuState(0x00,0x02,0x00,0x00);
                            MenuStateUPDF = TRUE;
                            break;
    					case 4:
                            Set_MenuState(0x00,0x03,0x00,0x00);
                            MenuStateUPDF = TRUE;
                            break;
						default:break;
					}
				}
				else if(CompareMenuState(0x01,0x01,0x00,0x00,MenuG2))//ShowMenuList2
				{
				  	switch(ScreenTAB.CurNum)
					{
    					case 1:
        					Set_MenuState(0x00,0x04,0x00,0x00);
                            MenuStateUPDF = TRUE;
        					break;
    					case 2:
        					Set_MenuState(0x00,0x06,0x00,0x00);
                            MenuStateUPDF = TRUE;
        					break;
    					default:break;
					}
				}
				else if(CompareMenuState(0x02,0x00,0x00,0x00,MenuG2))//设置菜单
				{
				  	switch(ScreenTAB.CurNum)
					{
    					case 1://关闭背光
        					lcd_bglight_disable();
							BgLightDelayTime = BGLIGHTDELAYTIME+1;
							Set_MenuState(0x00,0x00,0x00,0x00);
							HotKeyState = TRUE;
                            MenuStateUPDF = TRUE;
        					break;
    					case 2://数据存储使能
							DataSaveEnableFlag = !DataSaveEnableFlag;
                            if(DataSaveEnableFlag)
                            {
                                //-------------------------------------
                                //开始新一轮密集模式下的测量
                                SampleModeFlag = DenTorRotSampleStart;
                                SampleDenModeNum = 0x00;
								TimeBufForSpa[2] |= 0x80;//设定一个不可能到达的时间
                                //-------------------------------------
								Set_MenuState(0x09,0x07,0x00,0x00);
                            }
                            MenuStateUPDF = TRUE;
        					break;
						case 3://擦除使能或禁止 
    						MemoryEraseEnable = !MemoryEraseEnable;
							MenuStateUPDF = TRUE;
							break;
						default:break;
					}
				}
				else if(CompareMenuState(0x03,0x00,0x00,0x00,MenuG2))//功能操作菜单
				{
				  	switch(ScreenTAB.CurNum)
					{
    					case 1://显示已用存储空间
                            Set_MenuState(0x09,0x01,0x00,0x00);
                            MenuStateUPDF = TRUE;
        					break;
						case 2://上传数据
                            lcd_wrCmd(lcd_Clear);
							ClearScreenTab(' ');
							LoadMenuRowStr(DataIsUploading,0x01);
							ShowMenuBuffer();
                            SendDataToComputer(0x00,MemoryAddressCounter);
							Set_OldMenuState(0xFF,0xFF,0xFF,0xFC);//设一个不可用的菜单状态变量
                            MenuStateUPDF = TRUE;
        					break;
						case 3:
                            if(MemoryEraseEnable)
							{
    							Set_MenuState(0x09,0x00,0x00,0x00);//擦除确认对话框
							}
							else
							{
    							Set_MenuState(0x09,0x04,0x00,0x00);//擦除被锁定
							}
							MenuStateUPDF = TRUE;
    						break;
    					case 4://地址保存菜单
    						Save_MemoryAddressCounter();
							Set_MenuState(0x09,0x06,0x00,0x00);
							MenuStateUPDF = TRUE;
    						break;
						default:break;
					}
				}
				else if(CompareMenuState(0x04,0x01,0x00,0x00,MenuG2))//参数配置菜单
				{
    				switch(ScreenTAB.CurNum)
					{
    					case 1:
        					Set_MenuState(0x04,0x00,0x01,0x00);//三相电参数配置菜单1
                            MenuStateUPDF = TRUE;
        					break;
    					case 2:
        					Set_MenuState(0x04,0x00,0x03,0x00);//流速计参数配置菜单
                            MenuStateUPDF = TRUE;
        					break;
    					case 3:
        					Set_MenuState(0x04,0x00,0x04,0x00);//扭矩转速传感器参数配置菜单
                            MenuStateUPDF = TRUE;
        					break;
    					case 4:
        					Set_MenuState(0x04,0x00,0x05,0x00);//日期间期设定菜单
                            MenuStateUPDF = TRUE;
        					break;
    					default:break;
					}
				}
				else if(CompareMenuState(0x04,0x02,0x00,0x00,MenuG2))//参数配置菜单
				{
    				switch(ScreenTAB.CurNum)
					{
                        case 1:
                            Set_MenuState(0x04,0x04,0x02,0x00);//背光时间设定菜单
                            MenuStateUPDF = TRUE;
							break;
                        case 2:
                            Set_MenuState(0x04,0x04,0x01,0x00);//传输延迟时间设定菜单
                            MenuStateUPDF = TRUE;
                            break;
                        case 3:
                            Set_MenuState(0x04,0x00,0x06,0x00);//机械参数设定菜单
                            MenuStateUPDF = TRUE;
                            break;
						default:break;
					}
				}
				else if(CompareMenuState(0x04,0x00,0x01,0x00,MenuG3))//三相电参数配置菜单1
				{
    				switch(ScreenTAB.CurNum)
    				{
        				case 1:
        					Set_MenuState(0x04,0x00,0x00,0x01);//电压增益菜单
    						MenuStateUPDF = TRUE;
							break;
    					case 2:
        					Set_MenuState(0x04,0x00,0x00,0x02);//电流增益菜单
    						MenuStateUPDF = TRUE;
							break;
						case 3:
        					Set_MenuState(0x04,0x00,0x00,0x03);//功率增益菜单
    						MenuStateUPDF = TRUE;
							break;
						case 4:
        					Set_MenuState(0x04,0x00,0x00,0x04);//电压偏置菜单
    						MenuStateUPDF = TRUE;
							break;
						default:break;
    				}
				}
				else if(CompareMenuState(0x04,0x00,0x02,0x00,MenuG3))//三相电参数配置菜单2
				{
    				switch(ScreenTAB.CurNum)
    				{
        				case 1:
        					Set_MenuState(0x04,0x00,0x00,0x05);//电流偏置菜单
    						MenuStateUPDF = TRUE;
							break;
    					case 2:
    						Set_MenuState(0x04,0x00,0x00,0x06);//功率偏置菜单
    						MenuStateUPDF = TRUE;
							break;
						default:break;
					}
				}
				else if(CompareMenuState(0x04,0x00,0x03,0x00,MenuG3))//流速计参数配置菜单
				{
    				switch(ScreenTAB.CurNum)
    				{
        				case 1:
							Set_MenuState(0x04,0x04,0x07,0x00);//流速1增益菜单
    						MenuStateUPDF = TRUE;
							break;
    					case 2:
							Set_MenuState(0x04,0x04,0x08,0x00);//流速1偏置菜单
    						MenuStateUPDF = TRUE;
							break;
						case 3:
							Set_MenuState(0x04,0x04,0x09,0x00);//流速2增益菜单
    						MenuStateUPDF = TRUE;
							break;
    					case 4:
							Set_MenuState(0x04,0x04,0x0A,0x00);//流速2偏置菜单
    						MenuStateUPDF = TRUE;
							break;
						default:break;
					}
				}
				else if(CompareMenuState(0x04,0x00,0x04,0x00,MenuG3))//扭矩转速传感器参数配置菜单
				{
    				switch(ScreenTAB.CurNum)
    				{
        				case 1:
							Set_MenuState(0x04,0x04,0x03,0x00);//扭矩增益菜单
    						MenuStateUPDF = TRUE;
							break;
    					case 2:
							Set_MenuState(0x04,0x04,0x04,0x00);//扭矩偏置菜单
    						MenuStateUPDF = TRUE;
							break;
						case 3:
							Set_MenuState(0x04,0x04,0x05,0x00);//转速增益菜单
    						MenuStateUPDF = TRUE;
							break;
    					case 4:
							Set_MenuState(0x04,0x04,0x06,0x00);//转速偏置菜单
    						MenuStateUPDF = TRUE;
							break;
						default:break;
					}
				}
				else if(CompareMenuState(0x04,0x00,0x05,0x00,MenuG3))//时间日期设定菜单
				{
    				switch(ScreenTAB.CurNum)
					{
    					case 1:
        					Set_MenuState(0x04,0x04,0x0B,0x00);//日期设定菜单
    						MenuStateUPDF = TRUE;
    						break;
    					case 2:
        					Set_MenuState(0x04,0x04,0x0C,0x00);//时间设定菜单
    						MenuStateUPDF = TRUE;
    						break;
    					default:break;
					}
				}
				else if(CompareMenuState(0x04,0x00,0x00,0x00,MenuG2) &&
				MenuState.MG3 >=6 && MenuState.MG3 <= 7)//机械参数设定菜单
				{
    				Set_MenuState(0x04,0x05,(MenuState.MG3-0x06)*0x04+ScreenTAB.CurNum,0x00);
					MenuStateUPDF = TRUE;
				}
				else if(CompareMenuState(0x04,0x00,0x00,0x00,MenuG3) &&
					    MenuState.MG4 >0 && MenuState.MG4 <= 0x06)
				{
    				Set_MenuState(0x04,0x04,(0x10+(MenuState.MG4-1)*3+ScreenTAB.CurNum-1),0x00);
    				MenuStateUPDF = TRUE;
				}
				else if(CompareMenuState(0x09,0x00,0x00,0x00,MenuG2))//擦除确认对话框
				{
				 	//------------------------------------------------
					//显示等待菜单
					lcd_wrCmd(lcd_Clear);
            		ClearScreenTab(' ');
            		LoadMenuRowStr(DataIsErasing,0x01);
					ShowMenuBuffer();
					//------------------------------------------------
					//写状态寄存器
                    Memory_WaitWriteInProg();
                    MemoryStatusREG = 0x00;
                    Memory_WriteEn();
                    Memory_WriteSR(&MemoryStatusREG);                   
					delay_ms(5);
					//------------------------------------------------
					//写擦除指命
					Memory_WriteEn();
					Memory_WaitWriteInProg();
                    Memory_WriteBE();
					Memory_WaitWriteInProg();
					MemoryAddressCounter = 0x00;
					AddrCounterLastSaved = 0x00;
					AddrCounterPointToEeprom = StartAddrCounterInEeprom;
					ClearAddrCounterInEeprom();//
					MemoryEraseEnable = FALSE;
					//------------------------------------------------
					//恢复菜单
            		Set_OldMenuState(0xFF,0xFF,0xFF,0xFD);//设一个不可用的菜单状态变量
					Set_MenuState(0x03,0x00,0x00,0x00);
                    MenuStateUPDF = TRUE;
					//------------------------------------------------
				}
				else if(CompareMenuState(0x09,0x04,0x00,0x00,MenuG2))//擦除已被锁定对话框
				{
                    Set_MenuState(0x03,0x00,0x00,0x00);
					Set_OldMenuState(0xFF,0xFF,0xFF,0xFF);//设一个不可用的菜单状态变量
                    MenuStateUPDF = TRUE;
				}
				else if(CompareMenuState(0x09,0x07,0x00,0x00,MenuG2))//"跳转到存储状态"对话框
				{
				 	Set_MenuState(0x09,0x01,0x00,0x00);
                    MenuStateUPDF = TRUE;
				}
			}
    		KeyValue = no_key;
    		break;
		//当KeyValue == k_up时
		case k_up:
    		if(HotKeyState)
			{
    			Set_MenuState(0x00,0x01,0x00,0x00);
    			MenuStateUPDF = TRUE;
			}
			else
			{
    			if(CompareMenuState(0x00,0x07,0x00,0x00,MenuG2))//主菜单
    			{
    				ScrollMenu(FALSE);
    				ScreenUPDF = TRUE;
				}
				else if(CompareMenuState(0x01,0x00,0x00,0x00,MenuG2))//显示菜单1				   		)
				{
                    if(0x01 == ScreenTAB.CurNum)
                    {
                        Set_MenuState(0x01,0x01,0x00,0x00);
						MenuStateUPDF = TRUE;
                    }
                    else
                    {
                        ScrollMenu(FALSE);
						ScreenUPDF = TRUE;
					} 
				}				 
				else if(CompareMenuState(0x01,0x01,0x00,0x00,MenuG2))//显示菜单2		
				{
					if(0x01 == ScreenTAB.CurNum)
					{
    					Set_MenuState(0x01,0x00,0x00,0x00);
						MenuStateUPDF = TRUE;
					}
					else
					{
    					ScrollMenu(FALSE);
						ScreenUPDF = TRUE;
					}
					
				}
                else if(CompareMenuState(0x04,0x00,0x01,0x00,MenuG3))//三相电参数配置菜单1			   		)
				{
                    if(0x01 == ScreenTAB.CurNum)
                    {
                        Set_MenuState(0x04,0x00,0x02,0x00);
						MenuStateUPDF = TRUE;
                    }
                    else
                    {
                        ScrollMenu(FALSE);
						ScreenUPDF = TRUE;
					} 
				}				 
				else if(CompareMenuState(0x04,0x00,0x02,0x00,MenuG3))//三相电参数配置菜单2	
				{
					if(0x01 == ScreenTAB.CurNum)
					{
    					Set_MenuState(0x04,0x00,0x01,0x00);
						MenuStateUPDF = TRUE;
					}
					else
					{
    					ScrollMenu(FALSE);
						ScreenUPDF = TRUE;
					}
					
				}
                else if(CompareMenuState(0x04,0x01,0x00,0x00,MenuG2))//参数配置菜单1			   		)
				{
                    if(0x01 == ScreenTAB.CurNum)
                    {
                        Set_MenuState(0x04,0x02,0x00,0x00);
						MenuStateUPDF = TRUE;
                    }
                    else
                    {
                        ScrollMenu(FALSE);
						ScreenUPDF = TRUE;
					} 
				}				 
				else if(CompareMenuState(0x04,0x02,0x00,0x00,MenuG2))//参数配置菜单2	
				{
					if(0x01 == ScreenTAB.CurNum)
					{
    					Set_MenuState(0x04,0x01,0x00,0x00);
						MenuStateUPDF = TRUE;
					}
					else
					{
    					ScrollMenu(FALSE);
						ScreenUPDF = TRUE;
					}
					
				}
				else if(CompareMenuState(0x04,0x00,0x06,0x00,MenuG3))//参数配置菜单1			   		)
				{
                    if(0x01 == ScreenTAB.CurNum)
                    {
                        Set_MenuState(0x04,0x00,0x07,0x00);
						MenuStateUPDF = TRUE;
                    }
                    else
                    {
                        ScrollMenu(FALSE);
						ScreenUPDF = TRUE;
					} 
				}				 
				else if(CompareMenuState(0x04,0x00,0x07,0x00,MenuG3))//参数配置菜单2	
				{
					if(0x01 == ScreenTAB.CurNum)
					{
    					Set_MenuState(0x04,0x00,0x06,0x00);
						MenuStateUPDF = TRUE;
					}
					else
					{
    					ScrollMenu(FALSE);
						ScreenUPDF = TRUE;
					}
					
				}
				else if(CompareMenuState(0x02,0x00,0x00,0x00,MenuG2) ||//设置菜单
					 	CompareMenuState(0x03,0x00,0x00,0x00,MenuG2) ||//功能操作菜单
						CompareMenuState(0x02,0x01,0x00,0x00,MenuG2) ||//调试菜单
						CompareMenuState(0x04,0x01,0x00,0x00,MenuG2) ||//参数配置菜单
						(CompareMenuState(0x04,0x00,0x00,0x00,MenuG2) /*&& MenuState.MG3*/)//参数配置菜单的各个子项
						)
				{
				 	ScrollMenu(FALSE);
					ScreenUPDF = TRUE;
				}
			}
    		KeyValue = no_key;
    		break;
		//当KeyValue == k_down时
		case k_down:
    		if(HotKeyState){Set_MenuState(0x00,0x02,0x00,0x00);MenuStateUPDF = TRUE;}
			else
			{
    			if(CompareMenuState(0x00,0x07,0x00,0x00,MenuG2))//主菜单
    			{
    				ScrollMenu(TRUE);
    				ScreenUPDF = TRUE;
				}
				else if(CompareMenuState(0x01,0x00,0x00,0x00,MenuG2))//显示菜单1				   		)
				{
                    if(ShowMenuList1_num == ScreenTAB.CurNum)
					{
                        Set_MenuState(0x01,0x01,0x00,0x00);
                        MenuStateUPDF = TRUE;
                    }
					else
					{
    					ScrollMenu(TRUE);
    					ScreenUPDF = TRUE;
					}
				}
				else if(CompareMenuState(0x01,0x01,0x00,0x00,MenuG2))//显示菜单2
				{
					if(ShowMenuList2_num == ScreenTAB.CurNum)
					{
    					Set_MenuState(0x01,0x00,0x00,0x00);
                        MenuStateUPDF = TRUE;
					}
					else
					{
    					ScrollMenu(TRUE);
    					ScreenUPDF = TRUE;
					}
				}
				else if(CompareMenuState(0x04,0x00,0x01,0x00,MenuG3))//三相电参数配置菜单1			   		)
				{
                    if(ParaCfg_PowerPara1_num == ScreenTAB.CurNum)
					{
                        Set_MenuState(0x04,0x00,0x02,0x00);
                        MenuStateUPDF = TRUE;
                    }
					else
					{
    					ScrollMenu(TRUE);
    					ScreenUPDF = TRUE;
					}
				}
				else if(CompareMenuState(0x04,0x00,0x02,0x00,MenuG3))//三相电参数配置菜单2
				{
					if(ParaCfg_PowerPara2_num == ScreenTAB.CurNum)
					{
    					Set_MenuState(0x04,0x00,0x01,0x00);
                        MenuStateUPDF = TRUE;
					}
					else
					{
    					ScrollMenu(TRUE);
    					ScreenUPDF = TRUE;
					}
				}
				else if(CompareMenuState(0x04,0x01,0x00,0x00,MenuG2))//三相电参数配置菜单1			   		)
				{
                    if(ParameterConfig1_num == ScreenTAB.CurNum)
					{
                        Set_MenuState(0x04,0x02,0x00,0x00);
                        MenuStateUPDF = TRUE;
                    }
					else
					{
    					ScrollMenu(TRUE);
    					ScreenUPDF = TRUE;
					}
				}
				else if(CompareMenuState(0x04,0x02,0x00,0x00,MenuG2))//三相电参数配置菜单2
				{
					if(ParameterConfig2_num == ScreenTAB.CurNum)
					{
    					Set_MenuState(0x04,0x01,0x00,0x00);
                        MenuStateUPDF = TRUE;
					}
					else
					{
    					ScrollMenu(TRUE);
    					ScreenUPDF = TRUE;
					}
				}
				else if(CompareMenuState(0x04,0x00,0x06,0x00,MenuG3))//机械参数配置菜单1			   		)
				{
                    if(ParaCfg_Mechanical1_num == ScreenTAB.CurNum)
					{
                        Set_MenuState(0x04,0x00,0x07,0x00);
                        MenuStateUPDF = TRUE;
                    }
					else
					{
    					ScrollMenu(TRUE);
    					ScreenUPDF = TRUE;
					}
				}
				else if(CompareMenuState(0x04,0x00,0x07,0x00,MenuG3))//机械参数配置菜单2
				{
					if(ParaCfg_Mechanical2_num == ScreenTAB.CurNum)
					{
    					Set_MenuState(0x04,0x00,0x06,0x00);
                        MenuStateUPDF = TRUE;
					}
					else
					{
    					ScrollMenu(TRUE);
    					ScreenUPDF = TRUE;
					}
				}
				else if(CompareMenuState(0x02,0x00,0x00,0x00,MenuG2) ||//设置菜单
					    CompareMenuState(0x03,0x00,0x00,0x00,MenuG2) ||//功能操作菜单
						CompareMenuState(0x02,0x01,0x00,0x00,MenuG2) ||//调试菜单
						CompareMenuState(0x04,0x01,0x00,0x00,MenuG2) ||//参数配置菜单
						(CompareMenuState(0x04,0x00,0x00,0x00,MenuG2) /*&& MenuState.MG3*/)   //参数配置菜单的各个子项
						)
				{
				 	ScrollMenu(TRUE);
					ScreenUPDF = TRUE;
				}
			}
    		KeyValue = no_key;
    		break;
		//当KeyValue == k_left时
		case k_left:
    		if(HotKeyState)
			{
    			Set_MenuState(0x00,0x03,0x00,0x00);
    			MenuStateUPDF = TRUE;
			}
			KeyValue = no_key;
    		break;
		//当KeyValue == k_right时
		case k_right:
    		if(HotKeyState)
			{
    			Set_MenuState(0x00,0x04,0x00,0x00);
    			MenuStateUPDF = TRUE;
			}
			KeyValue = no_key;
    		break;
		//当KeyValue == default时
		default:KeyValue = no_key;break;
    }
}