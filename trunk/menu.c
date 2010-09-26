#include "config.h"

//-----------------------------------------------------
//-----------------------------------------------------
//用于调试
extern float  ACTIVEPREG_Debug[3];
extern float  RMSVOLREG_Debug[3];
extern float  RMSCURREG_Debug[3];
//-----------------------------------------------------
//-------------------------------------------------
extern float  TorqueGain;
extern uint16 TorqueOffset;
extern float  RotateGain;
extern float  RotateOffset;
extern float  Flow1Gain;
extern float  Flow1Offset;
extern float  Flow2Gain;
extern float  Flow2Offset;
//-------------------------------------------------
extern float  Flow1_Proportion;
extern float  Flow2_Proportion;
extern float  Rotate_Proportion;
extern float  Torque_Proportion;
extern float  Wheel_A;
extern float  Wheel_D;
extern float  Wheel_L;
extern uint16 Torque_Threshold;
//-------------------------------------------------
extern SampleType SampleModeFlagAid;
extern uint16 ADCREG;
//-------------------------------------------------
extern uint16 BGLIGHTDELAYTIME;
extern struct IntegrationStructure IntegrationStruct;
//-------------------------------------------------
extern float  Flow1Time;
extern float  Flow2Time;
extern float  RotateTime;
//-------------------------------------------------
extern int16  TorqueCount;
//-----------------------------------------------------
extern uint16 AddrCounterPointToEeprom;
extern uint8  MemoryEraseEnable;
extern uint8  TorqueArray_UseState;
extern uint8  CS5463UPDF;
extern uint8  BatVolUPDF;
extern uint8  MeasureUPDF;
extern uint8  TimeUPDF;
extern const  uint8 AddrTable[4];
extern uint8  PCArray[MAXDATAWIDTH];
extern uint8  DataSaveEnableFlag;
extern uint32 MemoryAddressCounter;
extern uint16 Uart_DelayTime;
extern uint8  SampleDenModeNum;
//-------------------------------------------------
extern uint8  CurrentTimeBuf[4];
extern uint8  TimeBufForDen[3];
extern uint8  TimeBufForSpa[3];
//-------------------------------------------------
extern float  ACTIVEPREG[3];
extern float  POWERFACTORREG[3];
extern float  RMSVOLREG[3];
extern float  RMSCURREG[3];
//-------------------------------------------------
extern uint16 TorqueCountArray1[10];
extern uint16 TorqueCountArray2[10];
//-------------------------------------------------
extern SampleType SampleModeFlag;
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
//-----------------------------------------------------
extern struct KEYPRERESPONSESTRUCT KeyResponseStruct;
//-----------------------------------------------------
uint8 MenuStateUPDF = FALSE;
uint8 ScreenUPDF    = FALSE;
struct SCRTAB ScreenTAB; 
struct MenuStateStruct MenuState = {0x00,0x00,0x00,0x00};
struct MenuStateStruct OldMenuState = {0x00,0x00,0x00,0x00};
//-----------------------------------------------------

void LoadMenu(MenuStrTab, uint8 curnum, uint8 totalnum)
{
    memcpy(ScreenTAB.ScreenTable[0],str0,16);
    memcpy(ScreenTAB.ScreenTable[1],str1,16);
    memcpy(ScreenTAB.ScreenTable[2],str2,16);
    memcpy(ScreenTAB.ScreenTable[3],str3,16);
    
    ScreenTAB.CurNum=curnum;
    ScreenTAB.TotalNum=totalnum;	
}

void ScrollMenu(uint8 dirs)
{
 	 if(dirs)
	 {
    	 ScreenTAB.CurNum = (ScreenTAB.CurNum%ScreenTAB.TotalNum)+1;
	 }
	 else
	 {
    	 ScreenTAB.CurNum = ((ScreenTAB.CurNum-2+ScreenTAB.TotalNum)%ScreenTAB.TotalNum)+1;
	 }
}

void ClearScreenTab(uint8 s)
{
 	 memset(ScreenTAB.ScreenTable,s,lcd_width*lcd_height);
}

void LoadMenuRowStr(uint8 MenuRowStr[],uint8 RowNum)
{
    memcpy(ScreenTAB.ScreenTable[RowNum],MenuRowStr,lcd_width);
}

void Set_MenuState(uint8 x1,uint8 x2,uint8 x3,uint8 x4)
{
    MenuState.MG1 = x1;
    MenuState.MG2 = x2;
    MenuState.MG3 = x3;
    MenuState.MG4 = x4;
}

void Set_OldMenuState(uint8 x1, uint8 x2, uint8 x3, uint8 x4)
{
    OldMenuState.MG1 = x1;
    OldMenuState.MG2 = x2;
	OldMenuState.MG3 = x3;
	OldMenuState.MG4 = x4;
}

void lcd_ReverseVideo(uint8 row)
{
 	switch(row)
	{
        case LCD_ClearGDRAM: 
            con_disp(0x00,0x00,0x00,0x00); 
            break;
        
        case LCD_Row1:       
            con_disp(0xFF,0x00,0x00,0x00); 
            break;
        
        case LCD_Row2:       
            con_disp(0x00,0xFF,0x00,0x00); 
            break;
        
        case LCD_Row3:       
            con_disp(0x00,0x00,0xFF,0x00);
            break;
        
        case LCD_Row4:       
            con_disp(0x00,0x00,0x00,0xFF); 
            break;
        
        default:
            break;
	}
}

void ShowMenuBuffer(void)
{
    uint8 i;
    
	lcd_wrCmd(fund_function);
    lcd_wrCmd(extend_function);
	lcd_wrCmd(fund_function);
	
	for(i=1;i<=lcd_height;i++)
	{
	    lcd_WriteCharArray(_ADDR(i,1),ScreenTAB.ScreenTable[i-1],lcd_width);
	}
	
	lcd_wrCmd(lcd_ShowState | Show_ON);
	lcd_wrCmd(extend_function);
}

void SaveCurMenuState(void)
{
	memcpy(&OldMenuState.MG1,&MenuState.MG1,4);
}

void LoadOldMenuState(void)
{
	memcpy(&MenuState.MG1,&OldMenuState.MG1,4);
}

uint8 CompareMenuOld(void)
{  
	if(!memcmp(&OldMenuState.MG1,&MenuState.MG1,4))
	{	
	    return TRUE;	
	}
	else
	{	
	    return FALSE;	
	}
}

uint8 CompareMenuState(uint8 s1,	uint8 s2, uint8 s3, uint8 s4,MenuGrade MenuG)
{
	uint8 ch[4];
	ch[0] = s1;	ch[1] = s2;	ch[2] = s3;	ch[3] = s4;
	if(memcmp(&MenuState.MG1,ch,MenuG)) 
    {	
        return FALSE;   
    }
	else 
    {	
        return TRUE;    
    }
}

uint8 CompareOldMenuState(uint8 s1, uint8 s2, uint8 s3,uint8 s4,MenuGrade MenuG)
{
	uint8 ch[4];
	ch[0] = s1;	ch[1] = s2;	ch[2] = s3;	ch[3] = s4;
	
	if(memcmp(&OldMenuState.MG1,ch,MenuG)) 
    {	
        return FALSE;   
    }
	else 
    {	
        return TRUE;    
    }
}
void ShowMenu(void)
{
	uint16 TempIntValue;
	float TempFValue;
	
	switch(MenuState.MG1)
	{
	 	//------------------------
		//菜单状态为{0,x,x,x}
		case 0:
    		switch(MenuState.MG2)
        	{
        	 	//------------------------
    			//菜单状态为{0,0,x,x}
    			case 0:
    				if(!CompareMenuOld())
           			{
       					LOADMENU(Menu_WelcomeSCR,Menu_WelcomeSCR_num);//欢迎屏幕
       					SaveCurMenuState();
    				}
                   	ShowMenuBuffer();
					ScreenUPDF = FALSE;
        			break;
        		//------------------------
    			//菜单状态为{0,1,x,x}
    			case 1:
    				if(!CompareMenuOld())
           			{
       					LOADMENU(Menu_VolCur,Menu_VolCur_num);//三相电压,电流显示菜单
    					SaveCurMenuState();
    				}
                    //------------------------------------------------------------
                    //把电压放入显示数组
                    PutDataToBuffer(&ScreenTAB.ScreenTable[1][3], _T(RMSVOLREG[0]), 4);
                    PutDataToBuffer(&ScreenTAB.ScreenTable[2][3], _T(RMSVOLREG[1]), 4);
                    PutDataToBuffer(&ScreenTAB.ScreenTable[3][3], _T(RMSVOLREG[2]), 4);
                    //------------------------------------------------------------
                    //把电流放入显示数组
                    PutDataToBuffer(&ScreenTAB.ScreenTable[1][11], _T(RMSCURREG[0]), 4);
                    PutDataToBuffer(&ScreenTAB.ScreenTable[2][11], _T(RMSCURREG[1]), 4);
                    PutDataToBuffer(&ScreenTAB.ScreenTable[3][11], _T(RMSCURREG[2]), 4);
                    //------------------------------------------------------------
                   	ScreenUPDF = FALSE;
					ShowMenuBuffer();
        			break;
        		//------------------------
    			//菜单状态为{0,2,x,x}
    			case 2:
    				if(!CompareMenuOld())
           			{
       					LOADMENU(Menu_P_PF,Menu_P_PF_num);//三相功率显示菜单
       					SaveCurMenuState();
    					CS5463UPDF |= TRUE;
    					return;
    				}
    				if(ScreenUPDF)  
    				{
    					//------------------------------------------------------------
						//把功率放入显示数组
						PutDataToBuffer(&ScreenTAB.ScreenTable[1][3], _T(ACTIVEPREG[0]), 4);
        				PutDataToBuffer(&ScreenTAB.ScreenTable[2][3], _T(ACTIVEPREG[1]), 4);
        				PutDataToBuffer(&ScreenTAB.ScreenTable[3][3], _T(ACTIVEPREG[2]), 4);
        				//------------------------------------------------------------
						//把功率因数放入显示数组
						PutDataToBuffer(&ScreenTAB.ScreenTable[1][12], POWERFACTORREG[0], 4);
        				PutDataToBuffer(&ScreenTAB.ScreenTable[2][12], POWERFACTORREG[1], 4);
        				PutDataToBuffer(&ScreenTAB.ScreenTable[3][12], POWERFACTORREG[2], 4);
						
       					ScreenUPDF = FALSE;
    				}
                   	ShowMenuBuffer();
					break;
        		//------------------------
    			//菜单状态为{0,3,x,x}
    			case 3:
    				if(!CompareMenuOld())
           			{
       					LOADMENU(Menu_FlowSpeed,Menu_FlowSpeed_num);//流速显示菜单
       					SaveCurMenuState();
    					MeasureUPDF = TRUE;
    					return;
    				}
    				if(ScreenUPDF)
    				{
						//---------------------------------------------------
						//把流速1,流速2放入到显示数组中
                        if(Flow1Time)
                        {
                            PutDataToBuffer(&ScreenTAB.ScreenTable[1][4],
                							Flow1_Proportion/(Flow1Time*(8e-6)) - Flow1Offset,
                							5);
											
                        }
                        else
                        {
    						memcpy(&ScreenTAB.ScreenTable[1][4],"0.000",5);
						}
                        //---------------------------------------------------
                        if(Flow2Time)
                        {
                            PutDataToBuffer(&ScreenTAB.ScreenTable[2][4],
                							Flow2_Proportion/(Flow2Time*(8e-6)) - Flow2Offset,
                							5);
                        }
                        else
                        {
    						memcpy(&ScreenTAB.ScreenTable[2][4],"0.000",5);
						}
						//---------------------------------------------------
    				}
    				ShowMenuBuffer();
					ScreenUPDF = FALSE;
        			break;
    			//------------------------
    			//菜单状态为{0,4,x,x}
    			case 4:
    				if(!CompareMenuOld())
           			{
       					LOADMENU(Menu_Torque_n,Menu_Torque_n_num);//转速转矩显示菜单
       					SaveCurMenuState();
    					MeasureUPDF = TRUE;
    					return;
    				}
    				if(ScreenUPDF)
    				{
						//---------------------------------------------------
						//把转速,转矩,转子功率放入到显示数组中				
						if(fabs((int16)(2*TorqueCount-TorqueOffset)) < Torque_Threshold)
						{
    						PutDataToBuffer(&ScreenTAB.ScreenTable[1][3],
                    						((int16)(2*TorqueCount-TorqueOffset))*Torque_Proportion,
                    						5);
						}
						else
						{
    						memcpy(&ScreenTAB.ScreenTable[1][3],"NULL ",5);
						}
						//---------------------------------------------------
                        if(RotateTime)
                        {
                            PutDataToBuffer(&ScreenTAB.ScreenTable[2][3],
            							Rotate_Proportion/(RotateTime*(8e-6)) - RotateOffset,
            							4);
                            if(fabs((int16)(2*TorqueCount-TorqueOffset))<Torque_Threshold)
							{
    							PutDataToBuffer(&ScreenTAB.ScreenTable[3][3],
                    						(6.28*fabs((int16)(2*TorqueCount-TorqueOffset))*Torque_Proportion/(60*Wheel_A))*(Rotate_Proportion/(RotateTime*(8e-6)) - RotateOffset),
                    						6);
							}
							else
							{
    							memcpy(&ScreenTAB.ScreenTable[3][3],"0.0    ",6);
							}
                        }
                        else
                        {
                            memcpy(&ScreenTAB.ScreenTable[2][3],"0.0 ",4);
                            memcpy(&ScreenTAB.ScreenTable[3][3],"0.0    ",6);
                        }
						//---------------------------------------------------
    				}
                    ShowMenuBuffer();
					ScreenUPDF = FALSE;
        			break;
    			//------------------------
    			//菜单状态为{0,5,x,x}
    			case 5:
    				if(!CompareMenuOld())
           			{
    					LOADMENU(Menu_integration,Menu_integration_num);//综合参数菜单
    					SaveCurMenuState();
    				}
					//-----------------------------------------------------
					//在此把综合参数放入显示数组
					PutDataToBuffer(&ScreenTAB.ScreenTable[1][3],
               						IntegrationStruct.OverallPower,4);
                    PutDataToBuffer(&ScreenTAB.ScreenTable[1][11],
               						IntegrationStruct.AverageFlowSpeed,4);		
					PutDataToBuffer(&ScreenTAB.ScreenTable[2][3],
               						IntegrationStruct.AveragePowerFactor,4);
					PutDataToBuffer(&ScreenTAB.ScreenTable[2][11],
               						IntegrationStruct.PowerGetEfficiency,4);
					PutDataToBuffer(&ScreenTAB.ScreenTable[3][3],
               						IntegrationStruct.TorquePower,6);		
					//-----------------------------------------------------								
                   	ShowMenuBuffer();
					ScreenUPDF = FALSE;
        			break;
    			//------------------------
    			//菜单状态为{0,6,x,x}
    			case 6:
    				if(!CompareMenuOld())
           			{
       					LOADMENU(Menu_Time_Bat,Menu_Time_Bat_num);//日期时间,电池电压显示菜单
       					SaveCurMenuState();
       					BatVolUPDF = TRUE;
    				}
					//--------------------------------------------------------
                    //把当前时间放入显示数组
                    PCF8563_ReadArray(SecRegAddr,PCArray,3);
                    PCArray[0] &= 0x7F;
                    BCD_Separate(PCArray[0],&(ScreenTAB.ScreenTable[2][12]));
                    BCD_Separate(PCArray[1],&(ScreenTAB.ScreenTable[2][9]));
                    BCD_Separate(PCArray[2],&(ScreenTAB.ScreenTable[2][6]));
                    //--------------------------------------------------------
                    //把当前日期放入显示数组
                    PCF8563_ReadArray(DayRegAddr,PCArray,4);
                    BCD_Separate(PCArray[0],&(ScreenTAB.ScreenTable[1][13]));
                    BCD_Separate(PCArray[2],&(ScreenTAB.ScreenTable[1][10]));
                    BCD_Separate(PCArray[3],&(ScreenTAB.ScreenTable[1][7]));
                    BCD_Separate(0x20,&(ScreenTAB.ScreenTable[1][5]));
                    //--------------------------------------------------------
                    //把电池电压放入显示数组
                    TempFValue = ((float)ADCREG)/1024.0*5.02*BatGain;
                    PutDataToBuffer(&ScreenTAB.ScreenTable[3][6],TempFValue,4);
                   	ShowMenuBuffer();
					ScreenUPDF = FALSE;
        			break;
    			//------------------------
    			//菜单状态为{0,7,x,x}
    			case 7:
    				if(!CompareMenuOld())
           			{
       					LOADMENU(MainMenu,MainMenu_num);//主菜单
       					if(CompareOldMenuState(0x02,0x00,0x00,0x00,MenuG2) ||
						   CompareOldMenuState(0x02,0x01,0x00,0x00,MenuG2))
						{
    						ScreenTAB.CurNum = 2;
						}
						else if(CompareOldMenuState(0x03,0x00,0x00,0x00,MenuG2))
						{
    						ScreenTAB.CurNum = 3;
						}
						else if(CompareOldMenuState(0x04,0x01,0x00,0x00,MenuG2) ||
        						CompareOldMenuState(0x04,0x02,0x00,0x00,MenuG2))
						{
    						ScreenTAB.CurNum = 4;
						}
						SaveCurMenuState();
    				}
                    ShowMenuBuffer();
       				lcd_ReverseVideo(ScreenTAB.CurNum);
					ScreenUPDF = FALSE;
        			break;
				//------------------------
    			//菜单状态为{0,default,x,x}
    			default:break;
        	}
    		break;
		//------------------------
		//菜单状态为{1,x,x,x}
		case 1:
    		switch(MenuState.MG2)
        	{
        	 	//------------------------
    			//菜单状态为{1,0,x,x}
    			case 0:
                    if(!CompareMenuOld())
                    {
                        PCArray[0] = ScreenTAB.CurNum;
						LOADMENU(ShowMenuList1,ShowMenuList1_num);//显示菜单1
                        if(CompareOldMenuState(0x01,0x01,0x00,0x00,MenuG2) &&
						   PCArray[0] == 0x01)
						{
    						ScreenTAB.CurNum = lcd_height;
						}
						else if(CompareOldMenuState(0x00,0x00,0x00,0x00,MenuG1) &&
        						OldMenuState.MG2 >= 1 && OldMenuState.MG2 <= 3)
						{
						 	ScreenTAB.CurNum = OldMenuState.MG2 + 1;
						}
						SaveCurMenuState();
                    }
                    ShowMenuBuffer();
                    lcd_ReverseVideo(ScreenTAB.CurNum);
                    ScreenUPDF = FALSE;
        			break;
        		//------------------------
    			//菜单状态为{1,1,x,x}
    			case 1:
					if(!CompareMenuOld())
                    {
                        PCArray[0] = ScreenTAB.CurNum;
						LOADMENU(ShowMenuList2,ShowMenuList2_num);//显示菜单2
                        if(CompareOldMenuState(0x01,0x00,0x00,0x00,MenuG2) &&
						   PCArray[0] == 0x01)
						{
    						ScreenTAB.CurNum = ShowMenuList2_num;
						}
						else if(CompareOldMenuState(0x00,0x00,0x00,0x00,MenuG1) &&
        						OldMenuState.MG2 == 6 )
						{
						 	ScreenTAB.CurNum = 2;
						}
						SaveCurMenuState();
                    }
                    ShowMenuBuffer();
                    lcd_ReverseVideo(ScreenTAB.CurNum);
                    ScreenUPDF = FALSE;
        			break;
        		//------------------------
    			//菜单状态为{1,2,x,x}
    			case 2:
    			break;
        		//------------------------
    			//菜单状态为{1,default,x,x}
    			default:break;
        	}
    		break;
		//------------------------
		//菜单状态为{2,x,x,x}
		case 2:
    		switch(MenuState.MG2)
        	{
        	 	//------------------------
    			//菜单状态为{2,0,x,x}
    			case 0:				 	  //设置菜单
					LoadMenu(FunctionSet,ScreenTAB.CurNum,FunctionSet_num);
					if(!CompareMenuOld())
                    {
						if(CompareOldMenuState(0x09,0x07,0x00,0x00,MenuG2))
						{
						 	ScreenTAB.CurNum = 2;
						}
						else
						{
    						ScreenTAB.CurNum = 1;
						}
                        SaveCurMenuState();
                    }
					if(DataSaveEnableFlag)
                    {
                        LoadMenuRowStr(FunctionSet_Row1,0x01);
                    }
					if(MemoryEraseEnable)
                    {
                        LoadMenuRowStr(FunctionSet_Row2,0x02);
                    }
                    ShowMenuBuffer();
                    lcd_ReverseVideo(ScreenTAB.CurNum);
                    ScreenUPDF = FALSE;
        			break;
        		//------------------------
    			//菜单状态为{2,1,x,x}
    			case 1:
        			break;
        		//------------------------
    			//菜单状态为{2,default,x,x}
    			default:break;
        	}
    		break;
		//------------------------
		//菜单状态为{3,x,x,x}
		case 3:
    		switch(MenuState.MG2)
        	{
        	 	//------------------------
    			//菜单状态为{3,0,x,x}
    			case 0:
					if(!CompareMenuOld())
                    {
						LOADMENU(FunctionOp,FunctionOp_num);//功能操作菜单
                        if(CompareOldMenuState(0xFF,0xFF,0xFF,0x00,MenuG3) &&
    					   OldMenuState.MG4 >= 0xFC && OldMenuState.MG4 <= 0xFE)
						{   						
							ScreenTAB.CurNum = OldMenuState.MG4 - 0xFA;
						}
						SaveCurMenuState();
                    }
					ShowMenuBuffer();
                    lcd_ReverseVideo(ScreenTAB.CurNum);
                    ScreenUPDF = FALSE;
               		break;
        		//------------------------
    			//菜单状态为{3,1,x,x}
    			case 1:
    			break;
        		//------------------------
    			//菜单状态为{3,2,x,x}
    			case 2:
    			break;
        		//------------------------
    			//菜单状态为{3,default,x,x}
    			default:break;
        	}
    		break;
		//------------------------
		//菜单状态为{4,x,x,x}
		case 4:
    		switch(MenuState.MG2)
        	{
        	 	//------------------------
    			//菜单状态为{4,0,x,x}
    			case 0:
    				switch(MenuState.MG3)
    				{
        				//------------------------
            			//菜单状态为{4,0,0,x}
						case 0:
    						switch(MenuState.MG4)
    						{
        						//------------------------
                    			//菜单状态为{4,0,0,1}
								case 1:
    								if(!CompareMenuOld())
                                    {
                						LOADMENU(ParaCfg_PwPara_VolG,ParaCfg_PwPara_VolG_num);//电压增益参数配置菜单
                                        if(CompareOldMenuState(0x04,0x04,0x00,0x00,MenuG2) &&
										   OldMenuState.MG3 >= 0x11 && OldMenuState.MG3 <= 0x12)
        								{
            								ScreenTAB.CurNum = OldMenuState.MG3-0x0F;
        								}
										SaveCurMenuState();
                                    }
                					ShowMenuBuffer();
                                    lcd_ReverseVideo(ScreenTAB.CurNum);
                                    ScreenUPDF = FALSE;
    								break;
								//------------------------
                    			//菜单状态为{4,0,0,2}
								case 2:
    								if(!CompareMenuOld())
                                    {
                						LOADMENU(ParaCfg_PwPara_CurG,ParaCfg_PwPara_CurG_num);//电流增益参数配置菜单
                                        if(CompareOldMenuState(0x04,0x04,0x00,0x00,MenuG2) &&
										   OldMenuState.MG3 >= 0x14 && OldMenuState.MG3 <= 0x15)
        								{
            								ScreenTAB.CurNum = OldMenuState.MG3-0x12;
        								}
										SaveCurMenuState();
                                    }
                					ShowMenuBuffer();
                                    lcd_ReverseVideo(ScreenTAB.CurNum);
                                    ScreenUPDF = FALSE;
    								break;
								//------------------------
                    			//菜单状态为{4,0,0,3}
								case 3:
    								if(!CompareMenuOld())
                                    {
                						LOADMENU(ParaCfg_PwPara_PwG,ParaCfg_PwPara_PwG_num);//功率增益参数配置菜单
                                        if(CompareOldMenuState(0x04,0x04,0x00,0x00,MenuG2) &&
										   OldMenuState.MG3 >= 0x17 && OldMenuState.MG3 <= 0x18)
        								{
            								ScreenTAB.CurNum = OldMenuState.MG3-0x15;
        								}
										SaveCurMenuState();
                                    }
                					ShowMenuBuffer();
                                    lcd_ReverseVideo(ScreenTAB.CurNum);
                                    ScreenUPDF = FALSE;
    								break;
								//------------------------
                    			//菜单状态为{4,0,0,4}
								case 4:
    								if(!CompareMenuOld())
                                    {
                						LOADMENU(ParaCfg_PwPara_VolO,ParaCfg_PwPara_VolO_num);//电压偏置参数配置菜单
                                        if(CompareOldMenuState(0x04,0x04,0x00,0x00,MenuG2) &&
										   OldMenuState.MG3 >= 0x1A && OldMenuState.MG3 <= 0x1B)
        								{
            								ScreenTAB.CurNum = OldMenuState.MG3-0x18;
        								}
										SaveCurMenuState();
                                    }
                					ShowMenuBuffer();
                                    lcd_ReverseVideo(ScreenTAB.CurNum);
                                    ScreenUPDF = FALSE;
    								break;
								//------------------------
                    			//菜单状态为{4,0,0,5}
								case 5:
    								if(!CompareMenuOld())
                                    {
                						LOADMENU(ParaCfg_PwPara_CurO,ParaCfg_PwPara_CurO_num);//电流偏置参数配置菜单
                                        if(CompareOldMenuState(0x04,0x04,0x00,0x00,MenuG2) &&
										   OldMenuState.MG3 >= 0x1D && OldMenuState.MG3 <= 0x1E)
        								{
            								ScreenTAB.CurNum = OldMenuState.MG3-0x1B;
        								}
										SaveCurMenuState();
                                    }
                					ShowMenuBuffer();
                                    lcd_ReverseVideo(ScreenTAB.CurNum);
                                    ScreenUPDF = FALSE;
    								break;
								//------------------------
                    			//菜单状态为{4,0,0,6}
								case 6:
    								if(!CompareMenuOld())
                                    {
                						LOADMENU(ParaCfg_PwPara_PwO,ParaCfg_PwPara_PwO_num);//功率偏置参数配置菜单
                                        if(CompareOldMenuState(0x04,0x04,0x00,0x00,MenuG2) &&
										   OldMenuState.MG3 >= 0x20 && OldMenuState.MG3 <= 0x21)
        								{
            								ScreenTAB.CurNum = OldMenuState.MG3-0x1E;
        								}
										SaveCurMenuState();
                                    }
                					ShowMenuBuffer();
                                    lcd_ReverseVideo(ScreenTAB.CurNum);
                                    ScreenUPDF = FALSE;
    								break;
								//------------------------
                    			//菜单状态为{4,0,0,default}
								default:break;
    						}
            				break;
        				//------------------------
            			//菜单状态为{4,0,1,x}
						case 1:
        					if(!CompareMenuOld())
                            {
        						PCArray[0] = ScreenTAB.CurNum;
								LOADMENU(ParaCfg_PowerPara1,ParaCfg_PowerPara1_num);//参数配置菜单
                                if(CompareOldMenuState(0x04,0x00,0x00,0x00,MenuG3) &&
								   OldMenuState.MG4 >=2 && OldMenuState.MG4 <= 4)
        						{
            						ScreenTAB.CurNum = OldMenuState.MG4;
        						}
								else if(CompareOldMenuState(0x04,0x00,0x02,0x00,MenuG3) &&
            						   PCArray[0] == 0x01)
								{
                					ScreenTAB.CurNum = ParaCfg_PowerPara1_num;
								}
								SaveCurMenuState();
                            }
        					ShowMenuBuffer();
                            lcd_ReverseVideo(ScreenTAB.CurNum);
                            ScreenUPDF = FALSE;
    						break;
        				//------------------------
            			//菜单状态为{4,0,2,x}
						case 2:
        					if(!CompareMenuOld())
                            {
        						PCArray[0] = ScreenTAB.CurNum;
								LOADMENU(ParaCfg_PowerPara2,ParaCfg_PowerPara2_num);//三相电参数配置菜单
                                if(CompareOldMenuState(0x04,0x00,0x00,0x00,MenuG3) &&
								   OldMenuState.MG4 >=5 && OldMenuState.MG4 <= 6)
        						{
            						ScreenTAB.CurNum = OldMenuState.MG4 - 4;
        						}
								else if(CompareOldMenuState(0x04,0x00,0x01,0x00,MenuG3) &&
            						   PCArray[0] == 0x01)
								{
                					ScreenTAB.CurNum = ParaCfg_PowerPara2_num;
								}
								SaveCurMenuState();
                            }
        					ShowMenuBuffer();
                            lcd_ReverseVideo(ScreenTAB.CurNum);
                            ScreenUPDF = FALSE;
    						break;
						//------------------------
            			//菜单状态为{4,0,3,x}
						case 3:
        					if(!CompareMenuOld())
                            {
        						LOADMENU(ParaCfg_Flow,ParaCfg_Flow_num);//流速计参数配置菜单
                                if(CompareOldMenuState(0x04,0x04,0x00,0x00,MenuG2) &&
    							   OldMenuState.MG3 >= 0x08 && OldMenuState.MG3 <= 0x0A)
								{
    								ScreenTAB.CurNum = OldMenuState.MG3 - 0x06;
								}
								SaveCurMenuState();
                            }
        					ShowMenuBuffer();
                            lcd_ReverseVideo(ScreenTAB.CurNum);
                            ScreenUPDF = FALSE;
    						break;
						//------------------------
            			//菜单状态为{4,0,4,x}
						case 4:
        					if(!CompareMenuOld())
                            {
        						LOADMENU(ParaCfg_TorRot,ParaCfg_TorRot_num);//扭矩转速参数配置菜单
                                if(CompareOldMenuState(0x04,0x04,0x00,0x00,MenuG2) &&
								   OldMenuState.MG3 >= 0x04 && OldMenuState.MG3 <= 0x06)
								{
    								ScreenTAB.CurNum = OldMenuState.MG3 - 0x02;
								}
								SaveCurMenuState();
                            }
        					ShowMenuBuffer();
                            lcd_ReverseVideo(ScreenTAB.CurNum);
                            ScreenUPDF = FALSE;
    						break;
						//------------------------
            			//菜单状态为{4,0,5,x}
						case 5:
        					if(!CompareMenuOld())
                            {
        						LOADMENU(ParaCfg_DateTime,ParaCfg_DateTime_num);//日期时间设定菜单
                                if(CompareOldMenuState(0x04,0x04,0x0C,0x00,MenuG3))
								{
    								ScreenTAB.CurNum = 2;
								}
								SaveCurMenuState();
                            }
        					ShowMenuBuffer();
                            lcd_ReverseVideo(ScreenTAB.CurNum);
                            ScreenUPDF = FALSE;
    						break;
						//------------------------
            			//菜单状态为{4,0,6,x}
						case 6:
        					if(!CompareMenuOld())
                            {
        						PCArray[0] = ScreenTAB.CurNum;
								LOADMENU(ParaCfg_Mechanical1,ParaCfg_Mechanical1_num);//机械参数1设定菜单
                                if(CompareOldMenuState(0x04,0x05,0x00,0x00,MenuG2) &&
    							   OldMenuState.MG3 >= 2 && OldMenuState.MG3 <= 4)
								{
    								ScreenTAB.CurNum = OldMenuState.MG3;
								}
								else if(CompareOldMenuState(0x04,0x00,0x07,0x00,MenuG3) &&
    									PCArray[0] == 0x01)
								{
    								ScreenTAB.CurNum = ParaCfg_Mechanical1_num;
								}
								SaveCurMenuState();
                            }
        					ShowMenuBuffer();
                            lcd_ReverseVideo(ScreenTAB.CurNum);
                            ScreenUPDF = FALSE;
    						break;
						//------------------------
            			//菜单状态为{4,0,7,x}
						case 7:
        					if(!CompareMenuOld())
                            {
        						PCArray[0] = ScreenTAB.CurNum;
								LOADMENU(ParaCfg_Mechanical2,ParaCfg_Mechanical2_num);//机械参数2设定菜单
                                if(CompareOldMenuState(0x04,0x05,0x00,0x00,MenuG2) &&
    							   OldMenuState.MG3 >= 6 && OldMenuState.MG3 <= 8)
								{
    								ScreenTAB.CurNum = OldMenuState.MG3 - 0x04;
								}
								else if(CompareOldMenuState(0x04,0x00,0x06,0x00,MenuG3) &&
    									PCArray[0] == 0x01)
								{
    								ScreenTAB.CurNum = ParaCfg_Mechanical2_num;
								}
								SaveCurMenuState();
                            }
        					ShowMenuBuffer();
                            lcd_ReverseVideo(ScreenTAB.CurNum);
                            ScreenUPDF = FALSE;
    						break;
						//------------------------
            			//菜单状态为{4,0,default,x}
						default:break;
    				}
    				break;
				//------------------------
    			//菜单状态为{4,1,x,x}
    			case 1:
					if(!CompareMenuOld())
                    {
						PCArray[0] = ScreenTAB.CurNum;
						LOADMENU(ParameterConfig1,ParameterConfig1_num);//参数配置菜单
						if(CompareOldMenuState(0x04,0x00,0x00,0x00,MenuG2) &&
    					   OldMenuState.MG3 >=3 && OldMenuState.MG3 <= 5)
						{
    						ScreenTAB.CurNum = OldMenuState.MG3 - 1;
						}
						else if(CompareOldMenuState(0x04,0x02,0x00,0x00,MenuG2) &&
                                PCArray[0] == 0x01)
                        {
                            ScreenTAB.CurNum = ParameterConfig1_num;
                        }
						SaveCurMenuState();
                    }
					ShowMenuBuffer();
                    lcd_ReverseVideo(ScreenTAB.CurNum);
                    ScreenUPDF = FALSE;
               		break;
				//------------------------
    			//菜单状态为{4,2,x,x}
				case 2:
					if(!CompareMenuOld())
                    {
						PCArray[0] = ScreenTAB.CurNum;
						LOADMENU(ParameterConfig2,ParameterConfig2_num);//参数配置菜单
						if(CompareOldMenuState(0x04,0x01,0x00,0x00,MenuG2) &&
                           PCArray[0] == 0x01)
                        {
                            ScreenTAB.CurNum = ParameterConfig2_num;
                        }
						else if(CompareOldMenuState(0x04,0x04,0x01,0x00,MenuG3))
						{
    						ScreenTAB.CurNum = 0x02;
						}
						else if(CompareOldMenuState(0x04,0x00,0x06,0x00,MenuG3) ||
        						CompareOldMenuState(0x04,0x00,0x07,0x00,MenuG3))
						{
    						ScreenTAB.CurNum = 0x03;
						}
						SaveCurMenuState();
                    }
					ShowMenuBuffer();
                    lcd_ReverseVideo(ScreenTAB.CurNum);
                    ScreenUPDF = FALSE;
               		break;
				//------------------------
    			//菜单状态为{4,4,x,x}
				case 4:
    				switch(MenuState.MG3)
					{
    					//------------------------
            			//菜单状态为{4,4,1,x}
						case 1:
    						#define UartDelayTime_MaxStrLen	4
        					if(!CompareMenuOld())
                            {
        						LoadKeySetMenuStr("normal",UartDelayTimeMenuStr,&Uart_DelayTime,'i',UartDelayTime_MaxStrLen);//时间延迟字符串
								SaveCurMenuState();
                            }
							KeyBoardSetFunction(&Uart_DelayTime,0x04,0x02,0x00,0x00,Uart_DelayTime_Addr);
							#undef UartDelayTime_MaxStrLen
							ScreenUPDF = FALSE;
							break;
                        //------------------------
            			//菜单状态为{4,4,2,x}
						case 2:
    						#define BgLightTime_MaxStrLen	4
        					if(!CompareMenuOld())
                            {
        						LoadKeySetMenuStr("normal",BgLightTimeMenuStr,&BGLIGHTDELAYTIME,'i',BgLightTime_MaxStrLen);//背光时间字符串
								SaveCurMenuState();
                            }
        					KeyBoardSetFunction(&BGLIGHTDELAYTIME,0x04,0x02,0x00,0x00,BGLIGHTDELAYTIME_Addr);
        					ScreenUPDF = FALSE;
							#undef BgLightTime_MaxStrLen
    						break;
                   		//------------------------
                		//菜单状态为{4,4,3,x}
    					case 3:
    						#define TorqueGain_MaxStrLen	5
        					if(!CompareMenuOld())
                            {
        						LoadKeySetMenuStr("normal",ParaCfg_TorGStr,&TorqueGain,'f',TorqueGain_MaxStrLen);//扭矩增益字符串
								SaveCurMenuState();
                            }
        					KeyBoardSetFunction(&TorqueGain,0x04,0x00,0x04,0x00,TorqueGain_Addr);
        					ScreenUPDF = FALSE;
							#undef TorqueGain_MaxStrLen
    						break;
						//------------------------
                		//菜单状态为{4,4,4,x}
						case 4:
    						#define TorqueOffset_MaxStrLen	5
        					if(!CompareMenuOld())
                            {
        						LoadKeySetMenuStr("normal",ParaCfg_TorOStr,&TorqueOffset,'i',TorqueOffset_MaxStrLen);//扭矩偏置字符串
								SaveCurMenuState();
                            }
        					KeyBoardSetFunction(&TorqueOffset,0x04,0x00,0x04,0x00,TorqueOffset_Addr);
        					ScreenUPDF = FALSE;
							#undef TorqueOffset_MaxStrLen
    						break;
						//------------------------
                		//菜单状态为{4,4,5,x}
    					case 5:
    						#define RotateGain_MaxStrLen	5
        					if(!CompareMenuOld())
                            {
        						LoadKeySetMenuStr("normal",ParaCfg_RotGStr,&RotateGain,'f',RotateGain_MaxStrLen);//转速增益字符串
								SaveCurMenuState();
                            }
        					KeyBoardSetFunction(&RotateGain,0x04,0x00,0x04,0x00,RotateGain_Addr);
        					ScreenUPDF = FALSE;
							#undef RotateGain_MaxStrLen
    						break;
						//------------------------
                		//菜单状态为{4,4,6,x}
						case 6:
    						#define RotateOffset_MaxStrLen	5
        					if(!CompareMenuOld())
                            {
        						LoadKeySetMenuStr("normal",ParaCfg_RotOStr,&RotateOffset,'f',RotateOffset_MaxStrLen);//转速偏置字符串
								SaveCurMenuState();
                            }
        					KeyBoardSetFunction(&RotateOffset,0x04,0x00,0x04,0x00,RotateOffset_Addr);
        					ScreenUPDF = FALSE;
							#undef RotateOffset_MaxStrLen
    						break;
						//------------------------
                		//菜单状态为{4,4,7,x}
    					case 7:
    						#define Flow1Gain_MaxStrLen	  5
        					if(!CompareMenuOld())
                            {
        						LoadKeySetMenuStr("normal",ParaCfg_Flow1GStr,&Flow1Gain,'f',Flow1Gain_MaxStrLen);//流速1增益字符串
								SaveCurMenuState();
                            }
        					KeyBoardSetFunction(&Flow1Gain,0x04,0x00,0x03,0x00,Flow1Gain_Addr);
        					ScreenUPDF = FALSE;
							#undef Flow1Gain_MaxStrLen
    						break;
						//------------------------
                		//菜单状态为{4,4,8,x}
						case 8:
    						#define Flow1Offset_MaxStrLen	5
        					if(!CompareMenuOld())
                            {
        						LoadKeySetMenuStr("normal",ParaCfg_Flow1OStr,&Flow1Offset,'f',Flow1Offset_MaxStrLen);//流速1偏置字符串
								SaveCurMenuState();
                            }
        					KeyBoardSetFunction(&Flow1Offset,0x04,0x00,0x03,0x00,Flow1Offset_Addr);
        					ScreenUPDF = FALSE;
							#undef Flow1Offset_MaxStrLen
    						break;
						//------------------------
                		//菜单状态为{4,4,9,x}
    					case 9:
    						#define Flow2Gain_MaxStrLen	  5
        					if(!CompareMenuOld())
                            {
        						LoadKeySetMenuStr("normal",ParaCfg_Flow2GStr,&Flow2Gain,'f',Flow2Gain_MaxStrLen);//流速2增益字符串
								SaveCurMenuState();
                            }
        					KeyBoardSetFunction(&Flow2Gain,0x04,0x00,0x03,0x00,Flow2Gain_Addr);
        					ScreenUPDF = FALSE;
							#undef Flow2Gain_MaxStrLen
    						break;
						//------------------------
                		//菜单状态为{4,4,0x0A,x}
						case 0x0A:
    						#define Flow2Offset_MaxStrLen	5
        					if(!CompareMenuOld())
                            {
        						LoadKeySetMenuStr("normal",ParaCfg_Flow2OStr,&Flow2Offset,'f',Flow2Offset_MaxStrLen);//流速2偏置字符串
								SaveCurMenuState();
                            }
        					KeyBoardSetFunction(&Flow2Offset,0x04,0x00,0x03,0x00,Flow2Offset_Addr);
        					ScreenUPDF = FALSE;
							#undef Flow2Offset_MaxStrLen
    						break;
						//------------------------
                		//菜单状态为{4,4,0x0B,x}
						case 0x0B://日期设定
    						#define Date_MaxStrLen	6
							if(!CompareMenuOld())
                            {
    							LoadKeySetMenuStr("date",ParaCfg_DateStr,NULL,'l',Date_MaxStrLen);
								SaveCurMenuState();
							}
							KeyBoardSetDateTime("date",0x04,0x00,0x05,0x00);
							#undef Date_MaxStrLen
							ShowMenuBuffer();
							ScreenUPDF = FALSE;
							break;
						//------------------------
                		//菜单状态为{4,4,0x0C,x}
						case 0x0C://时间设定
    						#define Time_MaxStrLen	6
							if(!CompareMenuOld())
                            {
    							LoadKeySetMenuStr("time",ParaCfg_TimeStr,NULL,'l',Time_MaxStrLen);
								SaveCurMenuState();
							}
							else if(TimeUPDF)
							{
                                ReadCurTimeToBuffer(PCArray,0x03);
                                BCD_Separate(PCArray[2],&ScreenTAB.ScreenTable[1][6]);
                                BCD_Separate(PCArray[1],&ScreenTAB.ScreenTable[1][9]);
                                BCD_Separate(PCArray[0],&ScreenTAB.ScreenTable[1][12]);
							}
							KeyBoardSetDateTime("time",0x04,0x00,0x05,0x00);
							#undef Time_MaxStrLen
							ShowMenuBuffer();
							ScreenUPDF = FALSE;
							break;
						//------------------------
                		//菜单状态为{4,4,0x10,x}
						case 0x10:
    						#define VolGain_A_MaxStrLen	5
        					if(!CompareMenuOld())
                            {
        						LoadKeySetMenuStr("normal",ParaCfg_VolGAStr,&VolGain_A,'f',VolGain_A_MaxStrLen);//A相电压增益字符串
								SaveCurMenuState();
                            }
        					KeyBoardSetFunction(&VolGain_A,0x04,0x00,0x00,0x01,VolGain_A_Addr);
        					ScreenUPDF = FALSE;
							#undef VolGain_A_MaxStrLen
    						break;
						//------------------------
                		//菜单状态为{4,4,0x11,x}
						case 0x11:
    						#define VolGain_B_MaxStrLen	5
        					if(!CompareMenuOld())
                            {
        						LoadKeySetMenuStr("normal",ParaCfg_VolGBStr,&VolGain_B,'f',VolGain_B_MaxStrLen);//B相电压增益字符串
								SaveCurMenuState();
                            }
        					KeyBoardSetFunction(&VolGain_B,0x04,0x00,0x00,0x01,VolGain_B_Addr);
							ScreenUPDF = FALSE;
							#undef VolGain_B_MaxStrLen
    						break;
						//------------------------
                		//菜单状态为{4,4,0x12,x}
						case 0x12:
    						#define VolGain_C_MaxStrLen	5
        					if(!CompareMenuOld())
                            {
        						LoadKeySetMenuStr("normal",ParaCfg_VolGCStr,&VolGain_C,'f',VolGain_C_MaxStrLen);//C相电压增益字符串
								SaveCurMenuState();
                            }
        					KeyBoardSetFunction(&VolGain_C,0x04,0x00,0x00,0x01,VolGain_C_Addr);
        					ScreenUPDF = FALSE;
							#undef VolGain_C_MaxStrLen
    						break;
						//------------------------
                		//菜单状态为{4,4,0x13,x}
						case 0x13:
    						#define CurGain_A_MaxStrLen	5
        					if(!CompareMenuOld())
                            {
        						LoadKeySetMenuStr("normal",ParaCfg_CurGAStr,&CurGain_A,'f',CurGain_A_MaxStrLen);//A相电流增益字符串
								SaveCurMenuState();
                            }
        					KeyBoardSetFunction(&CurGain_A,0x04,0x00,0x00,0x02,CurGain_A_Addr);
        					ScreenUPDF = FALSE;
							#undef CurGain_A_MaxStrLen
    						break;
						//------------------------
                		//菜单状态为{4,4,0x14,x}
						case 0x14:
    						#define CurGain_B_MaxStrLen	5
        					if(!CompareMenuOld())
                            {
        						LoadKeySetMenuStr("normal",ParaCfg_CurGBStr,&CurGain_B,'f',CurGain_B_MaxStrLen);//B相电流增益字符串
								SaveCurMenuState();
                            }
        					KeyBoardSetFunction(&CurGain_B,0x04,0x00,0x00,0x02,CurGain_B_Addr);
        					ScreenUPDF = FALSE;
							#undef CurGain_B_MaxStrLen
    						break;
						//------------------------
                		//菜单状态为{4,4,0x15,x}
						case 0x15:
    						#define CurGain_C_MaxStrLen	5
        					if(!CompareMenuOld())
                            {
        						LoadKeySetMenuStr("normal",ParaCfg_CurGCStr,&CurGain_C,'f',CurGain_C_MaxStrLen);//C相电流增益字符串
								SaveCurMenuState();
                            }
        					KeyBoardSetFunction(&CurGain_C,0x04,0x00,0x00,0x02,CurGain_C_Addr);
        					ScreenUPDF = FALSE;
							#undef CurGain_C_MaxStrLen
    						break;
						//------------------------
                		//菜单状态为{4,4,0x16,x}
						case 0x16:
    						#define PowerGain_A_MaxStrLen	6
        					if(!CompareMenuOld())
                            {
        						LoadKeySetMenuStr("normal",ParaCfg_PwGAStr,&PowerGain_A,'l',PowerGain_A_MaxStrLen);//A相功率增益字符串
								SaveCurMenuState();
                            }
        					KeyBoardSetFunction(&PowerGain_A,0x04,0x00,0x00,0x03,PowerGain_A_Addr);
        					ScreenUPDF = FALSE;
							#undef PowerGain_A_MaxStrLen
    						break;
						//------------------------
                		//菜单状态为{4,4,0x17,x}
						case 0x17:
    						#define PowerGain_B_MaxStrLen	6
        					if(!CompareMenuOld())
                            {
        						LoadKeySetMenuStr("normal",ParaCfg_PwGBStr,&PowerGain_B,'l',PowerGain_B_MaxStrLen);//B相功率增益字符串
								SaveCurMenuState();
                            }
        					KeyBoardSetFunction(&PowerGain_B,0x04,0x00,0x00,0x03,PowerGain_B_Addr);
        					ScreenUPDF = FALSE;
							#undef PowerGain_B_MaxStrLen
    						break;
						//------------------------
                		//菜单状态为{4,4,0x18,x}
						case 0x18:
    						#define PowerGain_C_MaxStrLen	6
        					if(!CompareMenuOld())
                            {
        						LoadKeySetMenuStr("normal",ParaCfg_PwGCStr,&PowerGain_C,'l',PowerGain_C_MaxStrLen);//C相功率增益字符串
								SaveCurMenuState();
                            }
        					KeyBoardSetFunction(&PowerGain_C,0x04,0x00,0x00,0x03,PowerGain_C_Addr);
        					ScreenUPDF = FALSE;
							#undef PowerGain_C_MaxStrLen
    						break;
						//------------------------
                		//菜单状态为{4,4,0x19,x}
						case 0x19:
    						#define VolOffset_A_MaxStrLen	7
        					if(!CompareMenuOld())
                            {
        						LoadKeySetMenuStr("normal",ParaCfg_VolOAStr,&VolOffset_A,'f',VolOffset_A_MaxStrLen);//A相电压偏置字符串
								SaveCurMenuState();
                            }
        					KeyBoardSetFunction(&VolOffset_A,0x04,0x00,0x00,0x04,VolOffset_A_Addr);
        					ScreenUPDF = FALSE;
							#undef VolOffset_A_MaxStrLen
    						break;
						//------------------------
                		//菜单状态为{4,4,0x1A,x}
						case 0x1A:
    						#define VolOffset_B_MaxStrLen	7
        					if(!CompareMenuOld())
                            {
        						LoadKeySetMenuStr("normal",ParaCfg_VolOBStr,&VolOffset_B,'f',VolOffset_B_MaxStrLen);//B相电压偏置字符串
								SaveCurMenuState();
                            }
        					KeyBoardSetFunction(&VolOffset_B,0x04,0x00,0x00,0x04,VolOffset_B_Addr);
        					ScreenUPDF = FALSE;
							#undef VolOffset_B_MaxStrLen
    						break;
						//------------------------
                		//菜单状态为{4,4,0x1B,x}
						case 0x1B:
    						#define VolOffset_C_MaxStrLen	7
        					if(!CompareMenuOld())
                            {
        						LoadKeySetMenuStr("normal",ParaCfg_VolOCStr,&VolOffset_C,'f',VolOffset_C_MaxStrLen);//C相电压偏置字符串
								SaveCurMenuState();
                            }
        					KeyBoardSetFunction(&VolOffset_C,0x04,0x00,0x00,0x04,VolOffset_C_Addr);
        					ScreenUPDF = FALSE;
							#undef VolOffset_C_MaxStrLen
    						break;
						//------------------------
                		//菜单状态为{4,4,0x1C,x}
						case 0x1C:
    						#define CurOffset_A_MaxStrLen	7
        					if(!CompareMenuOld())
                            {
        						LoadKeySetMenuStr("normal",ParaCfg_CurOAStr,&CurOffset_A,'f',CurOffset_A_MaxStrLen);//A相电流偏置字符串
								SaveCurMenuState();
                            }
        					KeyBoardSetFunction(&CurOffset_A,0x04,0x00,0x00,0x05,CurOffset_A_Addr);
        					ScreenUPDF = FALSE;
							#undef CurOffset_A_MaxStrLen
    						break;
						//------------------------
                		//菜单状态为{4,4,0x1D,x}
						case 0x1D:
    						#define CurOffset_B_MaxStrLen	7
        					if(!CompareMenuOld())
                            {
        						LoadKeySetMenuStr("normal",ParaCfg_CurOBStr,&CurOffset_B,'f',CurOffset_B_MaxStrLen);//B相电流偏置字符串
								SaveCurMenuState();
                            }
        					KeyBoardSetFunction(&CurOffset_B,0x04,0x00,0x00,0x05,CurOffset_B_Addr);
        					ScreenUPDF = FALSE;
							#undef CurOffset_B_MaxStrLen
    						break;
						//------------------------
                		//菜单状态为{4,4,0x1E,x}
						case 0x1E:
    						#define CurOffset_C_MaxStrLen	7
        					if(!CompareMenuOld())
                            {
        						LoadKeySetMenuStr("normal",ParaCfg_CurOCStr,&CurOffset_C,'f',CurOffset_C_MaxStrLen);//C相电流偏置字符串
								SaveCurMenuState();
                            }
        					KeyBoardSetFunction(&CurOffset_C,0x04,0x00,0x00,0x05,CurOffset_C_Addr);
        					ScreenUPDF = FALSE;
							#undef CurOffset_C_MaxStrLen
    						break;
							//------------------------
                		//菜单状态为{4,4,0x1F,x}
						case 0x1F:
    						#define PowerOffset_A_MaxStrLen	7
        					if(!CompareMenuOld())
                            {
        						LoadKeySetMenuStr("normal",ParaCfg_PwOAStr,&PowerOffset_A,'f',PowerOffset_A_MaxStrLen);//A相功率偏置字符串
								SaveCurMenuState();
                            }
        					KeyBoardSetFunction(&PowerOffset_A,0x04,0x00,0x00,0x06,PowerOffset_A_Addr);
        					ScreenUPDF = FALSE;
							#undef PowerOffset_A_MaxStrLen
    						break;
						//------------------------
                		//菜单状态为{4,4,0x20,x}
						case 0x20:
    						#define PowerOffset_B_MaxStrLen	7
        					if(!CompareMenuOld())
                            {
        						LoadKeySetMenuStr("normal",ParaCfg_PwOBStr,&PowerOffset_B,'f',PowerOffset_B_MaxStrLen);//B相功率偏置字符串
								SaveCurMenuState();
                            }
        					KeyBoardSetFunction(&PowerOffset_B,0x04,0x00,0x00,0x06,PowerOffset_B_Addr);
        					ScreenUPDF = FALSE;
							#undef PowerOffset_B_MaxStrLen
    						break;
						//------------------------
                		//菜单状态为{4,4,0x21,x}
						case 0x21:
    						#define PowerOffset_C_MaxStrLen	7
        					if(!CompareMenuOld())
                            {
        						LoadKeySetMenuStr("normal",ParaCfg_PwOCStr,&PowerOffset_C,'f',PowerOffset_C_MaxStrLen);//C相功率偏置字符串
								SaveCurMenuState();
                            }
        					KeyBoardSetFunction(&PowerOffset_C,0x04,0x00,0x00,0x06,PowerOffset_C_Addr);
        					ScreenUPDF = FALSE;
							#undef PowerOffset_C_MaxStrLen
    						break;
						//------------------------
                		//菜单状态为{4,4,default,x}
    					default:
						break;
    				}
					break;
				//------------------------
    			//菜单状态为{4,5,x,x}
    			case 5:
    				switch(MenuState.MG3)
					{
    					//------------------------
                		//菜单状态为{4,5,1,x}
						case 1:
        					#define Flow1_Proportion_MaxStrLen	5
        					if(!CompareMenuOld())
                            {
        						LoadKeySetMenuStr("normal",Flow1_ProportionStr,&Flow1_Proportion,'f',Flow1_Proportion_MaxStrLen);
								SaveCurMenuState();
                            }
        					KeyBoardSetFunction(&Flow1_Proportion,0x04,0x00,0x06,0x00,Flow1_Proportion_Addr);
        					ScreenUPDF = FALSE;
							#undef Flow1_Proportion_MaxStrLen
							break;
						//------------------------
                		//菜单状态为{4,5,2,x}
						case 2:
    						#define Flow2_Proportion_MaxStrLen	5
        					if(!CompareMenuOld())
                            {
        						LoadKeySetMenuStr("normal",Flow2_ProportionStr,&Flow2_Proportion,'f',Flow2_Proportion_MaxStrLen);
								SaveCurMenuState();
                            }
        					KeyBoardSetFunction(&Flow2_Proportion,0x04,0x00,0x06,0x00,Flow2_Proportion_Addr);
        					ScreenUPDF = FALSE;
							#undef Flow2_Proportion_MaxStrLen
							break;
						//------------------------
                		//菜单状态为{4,5,3,x}
						case 3:
        					#define Rotate_Proportion_MaxStrLen	5
        					if(!CompareMenuOld())
                            {
        						LoadKeySetMenuStr("normal",Rotate_ProportionStr,&Rotate_Proportion,'f',Rotate_Proportion_MaxStrLen);
								SaveCurMenuState();
                            }
        					KeyBoardSetFunction(&Rotate_Proportion,0x04,0x00,0x06,0x00,Rotate_Proportion_Addr);
        					ScreenUPDF = FALSE;
							#undef Rotate_Proportion_MaxStrLen
							break;
						//------------------------
                		//菜单状态为{4,5,4,x}
						case 4:
    						#define Torque_Proportion_MaxStrLen	5
        					if(!CompareMenuOld())
                            {
        						LoadKeySetMenuStr("normal",Torque_ProportionStr,&Torque_Proportion,'f',Torque_Proportion_MaxStrLen);
								SaveCurMenuState();
                            }
        					KeyBoardSetFunction(&Torque_Proportion,0x04,0x00,0x06,0x00,Torque_Proportion_Addr);
        					ScreenUPDF = FALSE;
							#undef Torque_Proportion_MaxStrLen
							break;
						//------------------------
                		//菜单状态为{4,5,5,x}
						case 5:
        					#define Wheel_A_MaxStrLen	5
        					if(!CompareMenuOld())
                            {
        						LoadKeySetMenuStr("normal",Wheel_AStr,&Wheel_A,'f',Wheel_A_MaxStrLen);
								SaveCurMenuState();
                            }
        					KeyBoardSetFunction(&Wheel_A,0x04,0x00,0x07,0x00,Wheel_A_Addr);
        					ScreenUPDF = FALSE;
							#undef Wheel_A_MaxStrLen
							break;
						//------------------------
                		//菜单状态为{4,5,6,x}
						case 6:
    						#define Wheel_D_MaxStrLen	5
        					if(!CompareMenuOld())
                            {
        						LoadKeySetMenuStr("normal",Wheel_DStr,&Wheel_D,'f',Wheel_D_MaxStrLen);
								SaveCurMenuState();
                            }
        					KeyBoardSetFunction(&Wheel_D,0x04,0x00,0x07,0x00,Wheel_D_Addr);
        					ScreenUPDF = FALSE;
							#undef Wheel_D_MaxStrLen
							break;
						//------------------------
                		//菜单状态为{4,5,7,x}
						case 7:
        					#define Wheel_L_MaxStrLen	5
        					if(!CompareMenuOld())
                            {
        						LoadKeySetMenuStr("normal",Wheel_LStr,&Wheel_L,'f',Wheel_L_MaxStrLen);
								SaveCurMenuState();
                            }
        					KeyBoardSetFunction(&Wheel_L,0x04,0x00,0x07,0x00,Wheel_L_Addr);
        					ScreenUPDF = FALSE;
							#undef Wheel_L_MaxStrLen
							break;
						//------------------------
                		//菜单状态为{4,5,8,x}
						case 8:
    						#define Torque_Threshold_MaxStrLen	5
        					if(!CompareMenuOld())
                            {
        						LoadKeySetMenuStr("normal",Torque_ThresholdStr,&Torque_Threshold,'i',Torque_Threshold_MaxStrLen);
								SaveCurMenuState();
                            }
        					KeyBoardSetFunction(&Torque_Threshold,0x04,0x00,0x07,0x00,Torque_Threshold_Addr);
        					ScreenUPDF = FALSE;
							#undef Torque_Threshold_MaxStrLen
							break;
						//------------------------
                		//菜单状态为{4,5,default,x}
						default:break;
					}
    				break;
				//------------------------
    			//菜单状态为{4,default,x,x}
    			default:break;
        	}
    		break;
		//------------------------
		//菜单状态为{9,x,x,x}
		case 9:
    		switch(MenuState.MG2)
        	{
        	 	//------------------------
    			//菜单状态为{9,0,x,x}
    			case 0:
    				if(!CompareMenuOld())
                    {
						LOADMENU(EraseEnsure,EraseEnsure_num);//擦除确认对话框
                        SaveCurMenuState();
                    }
					ShowMenuBuffer();
                    ScreenUPDF = FALSE;
               		break;
        		//------------------------
    			//菜单状态为{9,1,x,x}
				case 1:
					LOADMENU(MemoryState,MemoryState_num);
					PutHexToBuffer(ScreenTAB.ScreenTable[1],
								   MemoryAddressCounter);
					ScreenTAB.ScreenTable[1][1] = 'x';	   
					PutDataToBuffer(ScreenTAB.ScreenTable[3],((float)MemoryAddressCounter)*100/MAX_ADDRESS,	5);
					ScreenTAB.ScreenTable[3][5] = '%';			
					ShowMenuBuffer();
                    ScreenUPDF = FALSE;
               		break;
				//------------------------
				//菜单状态为{9,4,x,x}
				case 4:
    				ClearScreenTab(' ');
					LoadMenuRowStr(DataEraseIsLocked,0x01);//数据擦除功能被锁定
					ShowMenuBuffer();
					lcd_ReverseVideo(0x00);
    				ScreenUPDF = FALSE;
    				break;
				//------------------------
				//菜单状态为{9,6,x,x}
				case 6:
    				if(!CompareMenuOld())
                    {
						LOADMENU(SaveAddress,SaveAddress_num);//地址保存菜单
                        SaveCurMenuState();
                    }

					EEPROM_READ(AddrCounterPointToEeprom,TempIntValue);
					PutHexToBuffer(ScreenTAB.ScreenTable[2],TempIntValue);
					ScreenTAB.ScreenTable[2][1] = 'x';
					ShowMenuBuffer();
    				ScreenUPDF = FALSE;
    				break;
				//------------------------
				//菜单状态为{9,7,x,x}
				case 7:
					if(!CompareMenuOld())
					{
    					LOADMENU(JumpToMemoryState,JumpToMemoryState_num);//跳转到存储状态对话框
    					SaveCurMenuState();
					}
					ShowMenuBuffer();
    				ScreenUPDF = FALSE;
					break;
				//------------------------
    			//菜单状态为{9,default,x,x}
    			default:
    				ScreenUPDF = FALSE;
    				break;
        	}
    		break;
		//------------------------
		//菜单状态为{0x0A,x,x,x}
		case 0x0A:
    		switch(MenuState.MG2)
			{
    			case 1:
                    ClearScreenTab(' ');
					SaveCurMenuState();
					//------------------------
					PutDataToBuffer(ScreenTAB.ScreenTable[0],RMSVOLREG_Debug[0],16);
                    PutDataToBuffer(ScreenTAB.ScreenTable[1],RMSVOLREG_Debug[1],16);
                    PutDataToBuffer(ScreenTAB.ScreenTable[2],RMSVOLREG_Debug[2],16);
					//------------------------
					ShowMenuBuffer();
					ScreenUPDF = FALSE;
					break;
				case 2:
                    ClearScreenTab(' ');
					SaveCurMenuState();
					//------------------------
					PutDataToBuffer(ScreenTAB.ScreenTable[0], RMSCURREG_Debug[0],16);
                    PutDataToBuffer(ScreenTAB.ScreenTable[1], RMSCURREG_Debug[1],16);
                    PutDataToBuffer(ScreenTAB.ScreenTable[2], RMSCURREG_Debug[2],16);
					//------------------------
					ShowMenuBuffer();
					ScreenUPDF = FALSE;
					break;
				case 3:
                    ClearScreenTab(' ');
					SaveCurMenuState();
					//------------------------
					PutDataToBuffer(ScreenTAB.ScreenTable[0], -1.0*ACTIVEPREG_Debug[0],16);
                    PutDataToBuffer(ScreenTAB.ScreenTable[1], -1.0*ACTIVEPREG_Debug[1],16);
                    PutDataToBuffer(ScreenTAB.ScreenTable[2], -1.0*ACTIVEPREG_Debug[2],16);
					//------------------------
					ShowMenuBuffer();
					ScreenUPDF = FALSE;
					break;
				case 4:
                    ClearScreenTab(' ');
					SaveCurMenuState();
					//------------------------
					PutDataToBuffer(ScreenTAB.ScreenTable[0], RMSVOLREG[0],16);
                    PutDataToBuffer(ScreenTAB.ScreenTable[1], RMSVOLREG[1],16);
                    PutDataToBuffer(ScreenTAB.ScreenTable[2], RMSVOLREG[2],16);
					//------------------------
					ShowMenuBuffer();
					ScreenUPDF = FALSE;
					break;
				case 5:
                    ClearScreenTab(' ');
					SaveCurMenuState();
					//------------------------
					PutDataToBuffer(ScreenTAB.ScreenTable[0], RMSCURREG[0],16);
                    PutDataToBuffer(ScreenTAB.ScreenTable[1], RMSCURREG[1],16);
                    PutDataToBuffer(ScreenTAB.ScreenTable[2], RMSCURREG[2],16);
					//------------------------
					ShowMenuBuffer();
					ScreenUPDF = FALSE;
					break;
				case 6:
                    ClearScreenTab(' ');
					SaveCurMenuState();
					//------------------------
					PutDataToBuffer(ScreenTAB.ScreenTable[0], ACTIVEPREG[0],16);
                    PutDataToBuffer(ScreenTAB.ScreenTable[1], ACTIVEPREG[1],16);
                    PutDataToBuffer(ScreenTAB.ScreenTable[2], ACTIVEPREG[2],16);
					//------------------------
					ShowMenuBuffer();
					ScreenUPDF = FALSE;
					break;
				default:break;
			}
			break;
		//------------------------
		//菜单状态为{default,x,x,x}
		default:
    		ScreenUPDF = FALSE;
    		break;
	}
}