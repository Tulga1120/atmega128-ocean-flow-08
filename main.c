#include "main.h"

//-----------------------------------------------------
//用于调试
float ACTIVEPREG_Debug[3];
float RMSVOLREG_Debug[3];
float RMSCURREG_Debug[3];

//-----------------------------------------------------
//主函数
void main(void)
{ 	 
	init_devices();
	
	LOADMENU(Menu_WelcomeSCR,Menu_WelcomeSCR_num);
	ShowMenuBuffer();

	ReadCurTimeToBuffer(CurrentTimeBuf,0x04);
	memcpy(TimeBufForDen,CurrentTimeBuf,3);
	TimeBufForDen[2] |= 0x80;//设定一个不可能到达的时间
	memcpy(TimeBufForSpa,CurrentTimeBuf,3);
	TimeBufForSpa[2] |= 0x80;//设定一个不可能到达的时间
	ClearStruct(SaveAidStruct);
	ClearStruct(SaveStruct);
	memset(RXStrBuffer,0x00,RXArrayLength);
	
	InitPowerPara_FromEeprom();
	InitMeasurePara_FromEeprom();
	
	SampleModeFlag = SpaTorRotSampleStart;
	FirstIntoSpaTorRotSample = TRUE;
	//-------------------------------------------------------------
	while(1)
    {		
        //-------------------------------------------------------------
		//按键响应
		if(Key_PreResponse())
		{
    		Key_Response();
		}
  
		//-------------------------------------------------------------
        //更新电功率参数的显示
        if(CS5463UPDF)
        {
            //-----------------------------------------
            //如果CS5463数据读取完成
            if(UPDATETRUE)
            {
                if(CS5463DebugEnable)
				{
    				RMSVOLREG_Debug[0] = ConvertUnsignedVal(CS5463_ReadReg(RMSVol_Reg,CS5463_CSA));
    				RMSVOLREG_Debug[1] = ConvertUnsignedVal(CS5463_ReadReg(RMSVol_Reg,CS5463_CSB));
    				RMSVOLREG_Debug[2] = ConvertUnsignedVal(CS5463_ReadReg(RMSVol_Reg,CS5463_CSC));
    				
    				RMSCURREG_Debug[0] = ConvertUnsignedVal(CS5463_ReadReg(RMSCur_Reg,CS5463_CSA));
    				RMSCURREG_Debug[1] = ConvertUnsignedVal(CS5463_ReadReg(RMSCur_Reg,CS5463_CSB));
    				RMSCURREG_Debug[2] = ConvertUnsignedVal(CS5463_ReadReg(RMSCur_Reg,CS5463_CSC));
    				
    				ACTIVEPREG_Debug[0] = ConvertSignedVal(CS5463_ReadReg(ActiveP_Reg,CS5463_CSA));
    				ACTIVEPREG_Debug[1] = ConvertSignedVal(CS5463_ReadReg(ActiveP_Reg,CS5463_CSB));
    				ACTIVEPREG_Debug[2] = ConvertSignedVal(CS5463_ReadReg(ActiveP_Reg,CS5463_CSC));
				}
                //-----------------------------------------
                //用于调试
                if(CompareMenuState(0x0A,0x00,0x00,0x00,MenuG1) &&
                   MenuState.MG2 >= 1 && MenuState.MG2 <= 6)
                {
                    MenuStateUPDF = TRUE;
                }
				//-----------------------------------------
				
				//----------------------------------------------------------------------------------------
				//A相
                RMSCURREG[0] = CurGain_A*(ConvertUnsignedVal(CS5463_ReadReg(RMSCur_Reg,CS5463_CSA)) - CurOffset_A);
				//-----------------------------------------
				//A相电流修正
				if(RMSCURREG[0] >= 2.16)
				{
    				RMSCURREG[0] = emendation(2.16,4.22,-0.31,RMSCURREG[0]);
				}
				else if(RMSCURREG[0] >= 0.112 && RMSCURREG[0] < 2.16)
				{
    				RMSCURREG[0] = emendation(2.16,0.34,0.17,RMSCURREG[0]);
				}
				else if(RMSCURREG[0] >= 0.0 && RMSCURREG[0] < 0.112)
				{
    				RMSCURREG[0] *= 2.68;
				}
				//-----------------------------------------
				RMSVOLREG[0] = VolGain_A*(ConvertUnsignedVal(CS5463_ReadReg(RMSVol_Reg,CS5463_CSA)) - VolOffset_A);
				//-----------------------------------------
				//A相电压修正
				if(RMSVOLREG[0] >= 80.0)
				{
    				RMSVOLREG[0] = emendation(80.0,171.6,-21.6,RMSVOLREG[0]);
				}
				else if(RMSVOLREG[0] >= 5.5 && RMSVOLREG[0] < 80.0)
				{
    				RMSVOLREG[0] = emendation(80.0,13.75,11.25,RMSVOLREG[0]);
				}
				else if(RMSVOLREG[0] < 5.5 && RMSVOLREG[0] >= 0.0)
				{
    				RMSVOLREG[0] *= 18.15/5.5;
				}
                //-----------------------------------------
				ACTIVEPREG[0] = PowerGain_A*(-1.0*ConvertSignedVal(CS5463_ReadReg(ActiveP_Reg,CS5463_CSA)) - PowerOffset_A);
				POWERFACTORREG[0] = PowerFactorGain*ConvertSignedVal(CS5463_ReadReg(PowerFactor_Reg,CS5463_CSA));
                //----------------------------------------------------------------------------------------
				//B相
                RMSCURREG[1] = CurGain_B*(ConvertUnsignedVal(CS5463_ReadReg(RMSCur_Reg,CS5463_CSB)) - CurOffset_B);
				//-----------------------------------------
				//B相电流修正
				if(RMSCURREG[1] >= 2.14)
				{
    				RMSCURREG[1] = emendation(2.14,4.8,-1.14,RMSCURREG[1]);
				}
				else if(RMSCURREG[1] >= 0.191 && RMSCURREG[1] < 2.14)
				{
    				RMSCURREG[1] = emendation(2.14,0.39,0.43,RMSCURREG[1]);
				}
				else if(RMSCURREG[1] >= 0.0 && RMSCURREG[1] < 0.191)
				{
    				RMSCURREG[1] *= 0.67/0.191;
				}
				//-----------------------------------------
				RMSVOLREG[1] = VolGain_B*(ConvertUnsignedVal(CS5463_ReadReg(RMSVol_Reg,CS5463_CSB)) - VolOffset_B);
				//-----------------------------------------
				//B相电压修正
				if(RMSVOLREG[1] >= 80.0)
				{
    				RMSVOLREG[1] = emendation(80.0,157.6,-17.6,RMSVOLREG[1]);
				}
				else if(RMSVOLREG[1] >= 5.5 && RMSVOLREG[1] < 80.0)
				{
    				RMSVOLREG[1] = emendation(80.0,14.2,10.8,RMSVOLREG[1]);
				}
				else if(RMSVOLREG[1] < 5.5 && RMSVOLREG[1] >= 0.0)
				{
    				RMSVOLREG[1] *= 17.73/5.5;
				}
				//-----------------------------------------
				ACTIVEPREG[1] = PowerGain_B*(-1.0*ConvertSignedVal(CS5463_ReadReg(ActiveP_Reg,CS5463_CSB)) - PowerOffset_B);
				POWERFACTORREG[1] = PowerFactorGain*ConvertSignedVal(CS5463_ReadReg(PowerFactor_Reg,CS5463_CSB));
            	//----------------------------------------------------------------------------------------
				//C相
                RMSCURREG[2] = CurGain_C*(ConvertUnsignedVal(CS5463_ReadReg(RMSCur_Reg,CS5463_CSC)) - CurOffset_C);
				//-----------------------------------------
				//C相电流修正
				if(RMSCURREG[2] >= 2.13)
				{
    				RMSCURREG[2] = emendation(2.13,3.864,-0.224,RMSCURREG[2]);
				}
				else if(RMSCURREG[2] >= 0.15 && RMSCURREG[2] < 2.13)
				{
    				RMSCURREG[2] = emendation(2.13,0.38,0.16,RMSCURREG[2]);
				}
				else if(RMSCURREG[2] >= 0.0 && RMSCURREG[2] < 0.15)
				{
    				RMSCURREG[2] *= 2.0;
				}
				//-----------------------------------------
				RMSVOLREG[2] = VolGain_C*(ConvertUnsignedVal(CS5463_ReadReg(RMSVol_Reg,CS5463_CSC)) - VolOffset_C);
				//-----------------------------------------
				//C相电压修正
				if(RMSVOLREG[2] >= 80.0)
				{
    				RMSVOLREG[2] = emendation(80.0,170.7,-20.7,RMSVOLREG[2]);
				}
				else if(RMSVOLREG[2] >= 5.8 && RMSVOLREG[2] < 80.0)
				{
    				RMSVOLREG[2] = emendation(80.0,14.1,10.9,RMSVOLREG[2]);
				}
				else if(RMSVOLREG[2] < 5.8 && RMSVOLREG[2] >= 0.0)
				{
    				RMSVOLREG[1] *= 18.7/5.5;
				}
                //-----------------------------------------
				ACTIVEPREG[2] = PowerGain_C*(-1.0*ConvertSignedVal(CS5463_ReadReg(ActiveP_Reg,CS5463_CSC)) - PowerOffset_C);
				POWERFACTORREG[2] = PowerFactorGain*ConvertSignedVal(CS5463_ReadReg(PowerFactor_Reg,CS5463_CSC));
            
                //------------------------------------------------------
				//计算总功率
				IntegrationStruct.OverallPower = ACTIVEPREG[0] + 
											     ACTIVEPREG[1] + 
												 ACTIVEPREG[2];
				IntegrationStruct.AveragePowerFactor = (POWERFACTORREG[0] + 
													    POWERFACTORREG[1] + 
													    POWERFACTORREG[2])/3;
				IntegrationUPDF = TRUE;
				//------------------------------------------------------
                //把新数据加到旧数据中
                AddPowerParaToSaveAidStruct();
                CS5463UPDF &= ~UPDATETRUE;//
            }
			
			//-----------------------------------------
			//设置更新显示
			if(CompareMenuState(0x00,0x01,0x00,0x00,MenuG2) ||
			   CompareMenuState(0x00,0x02,0x00,0x00,MenuG2))
            {
				ScreenUPDF = TRUE;
            }
			//-----------------------------------------
			//清除标志
			CS5463UPDF = FALSE;
		}
		
		//-------------------------------------------------------------
		//更新电池电压和时间的显示
		if(BatVolUPDF || TimeUPDF)
		{
		 	BatVolUPDF = FALSE;
			TimeUPDF = FALSE;
			if(CompareMenuState(0x00,0x06,0x00,0x00,MenuG2))
			{
    			ScreenUPDF = TRUE;
			}
		}

		//-------------------------------------------------------------
		//更新流速,转矩转速等的显示
		if(MeasureUPDF)
		{
			MeasureUPDF = FALSE;
    		if(CompareMenuState(0x00,0x03,0x00,0x00,MenuG2) ||
			   CompareMenuState(0x00,0x04,0x00,0x00,MenuG2))			   
			{   
				ScreenUPDF = TRUE;
			}
		}
		
		//-------------------------------------------------------------
		//更新综合参数的显示
		if(IntegrationUPDF)
		{
			IntegrationUPDF = FALSE;
    		if(CompareMenuState(0x00,0x05,0x00,0x00,MenuG2))//更新综合参数的显示
			{   
				ScreenUPDF = TRUE;
			}
		}
		
		//-------------------------------------------------------------
		//上传数据
		if(UARTUPDF)
		{
			CmdStringResponse();
			memset(CmdString,0x00,CmdStrLength);
        	UARTUPDF = FALSE;
		}
		
		//-------------------------------------------------------------
		//对采样状态响应
		//------------------------------------------------------------
		if(!DataSaveEnableFlag &&
		   SampleModeFlag == DenTorRotSampleStart)
		{
    		SampleModeFlag = SpaTorRotSampleStart;
			FirstIntoSpaTorRotSample = TRUE;
		}
		//------------------------------------------------------------
		switch(SampleModeFlag)
		{
            case DenTorRotSampleStart:
                if(SampleDenModeNum < 3)
                {
					if(!SampleDenModeNum)
					{
    					//--------------------------------------
    					//读取当前时间,当前时间,加10分钟,然后保存
    					ReadCurTimeToBuffer(TimeBufForDen,3);
                        TimeAddMins(TimeBufForDen,10);
					}
					SampleDenModeNum ++;
					CS5463_WriteCmd(start_SConvert,CS5463_CSABC);					
                    SaveDenPackageHeadToFlash();
					ClearStruct(SaveAidStruct);
					Start_DenTorRotSample();//启动密集的转速转矩采样;
					//-----------------------------------------
					SampleModeFlag = DenTorRotSampling;
					//-----------------------------------------
                }
				else
				{
					SampleModeFlag = SpaTorRotSampleStart;
					FirstIntoSpaTorRotSample = TRUE;
				}
    			break;
            case DenTorRotSampling:
                //-----------------------------------------
				//保存转矩数据
                if(TorqueRotateSampleFlag & SAVETRUE)
                {
					//--------------------------------------
					//在此给TorqueCountArray中的每个数据加一个修正,
					//让每个数据都乘以TorqueCountGainAll,修正后的TorqueCount
					//等于一秒内接收到的脉冲个数
					ModifyTorqueCountArray();//
					//--------------------------------------
					//求平均值,为扭矩显示或其它用途作准备
					TorqueCount = (signed int)(AVGTORCNT()/2);
					//--------------------------------------
					SaveTorqueCountArrayToFlash();
                    TorqueRotateSampleFlag &= ~SAVETRUE;
				}
				//-----------------------------------------
                //为转矩转速显示作准备
                if(TorqueRotateSampleFlag & SHOWTRUE)
                {
                 	MeasureUPDF = TRUE;
					TorqueRotateSampleFlag &= ~SHOWTRUE;
                }
				//--------------------------------------------
                //保存转速数据,到SaveStruct结构体中,并跳转到一下步
                if(TorqueRotateSampleFlag & QUITTRUE)
                {
					if(RotateNumCount > 1)
					{
    					//密集型模式下计算转速时间,转速时间取倒数后对应于实际测到的频率
						RotateTime = RotateGain*(RotateTimeCountEnd - RotateTimeCountStart0)/(RotateNumCount - 1);
					}
					else
					{
					 	RotateTime = 0.0;
					}
					MoveDenDataToSaveStruct(ROTATEFLAG);
					TorqueRotateSampleFlag &= ~QUITTRUE;
					//-----------------------------------------
					//密集型模式下计算扭矩
					if(RotateTime && (fabs((int)(2*TorqueCount-TorqueOffset))<Torque_Threshold))
					{
    					IntegrationStruct.TorquePower = 
    						6.28*Torque_Proportion*fabs((int)(2*TorqueCount-TorqueOffset))*Rotate_Proportion/(RotateTime*(8e-6)*60*Wheel_A);
					}
					else
					{
    					IntegrationStruct.TorquePower = 0.0;
					}
					IntegrationUPDF = TRUE;//设置变量
					//-------------------------------------------
					SampleModeFlag = DenFlowSampleStart;
                }
                //--------------------------------------------
                break;
            case DenFlowSampleStart:
				//-------------------------------------------
				//启动密集流速采样
				PrepareForMeasureFlow();
                //-----------------------------------------
				SampleModeFlag = DenFlowSampling;
    			break;
            case DenFlowSampling:
				//--------------------------------------------
                //为流速显示作准备
    			if(FlowSampleFlag & SHOWTRUE)
    			{
					MeasureUPDF = TRUE;
					FlowSampleFlag &= ~SHOWTRUE;
    			}
				//--------------------------------------------
				//保存流速数据,并跳转到一下步
				if(FlowSampleFlag & QUITTRUE)
                {
					MoveDenDataToSaveStruct(FLOWFLAG | POWERFLAG);
					SaveDenPackageToFlash();
					//-----------------------------------------
					//清空保存结构体与辅助保存结构体
    				ClearStruct(SaveAidStruct);
                    ClearStruct(SaveStruct);
					//-----------------------------------------
					FlowSampleFlag &= ~QUITTRUE;
					//-----------------------------------------
					//计算平均流速
					if(Flow1Time && Flow2Time)
					{
    					IntegrationStruct.AverageFlowSpeed = 
        					(Flow1_Proportion/(Flow1Time*(8e-6)) + Flow2_Proportion/(Flow2Time*(8e-6)))/2;
					}
					else if(Flow1Time)
					{
					 	IntegrationStruct.AverageFlowSpeed = 
        					Flow1_Proportion/(Flow1Time*(8e-6))/2;
					}
					else if(Flow2Time)
					{
					 	IntegrationStruct.AverageFlowSpeed = 
        					Flow2_Proportion/(Flow2Time*(8e-6))/2;
					}
					else
					{
					 	IntegrationStruct.AverageFlowSpeed = 0.0;
					}
					//-----------------------------------------
					//计算获能系数
                    if(IntegrationStruct.AverageFlowSpeed && IntegrationStruct.TorquePower)
                    {
                        IntegrationStruct.PowerGetEfficiency = 
                            IntegrationStruct.TorquePower/
                            (0.5*1000*(IntegrationStruct.AverageFlowSpeed)*(IntegrationStruct.AverageFlowSpeed)*(IntegrationStruct.AverageFlowSpeed)*Wheel_D*Wheel_L);
                    }
                    else
                    {
                        IntegrationStruct.PowerGetEfficiency =0.0;
                    }
					//-----------------------------------------
                    IntegrationUPDF = TRUE;//设置变量
					//-----------------------------------------
					SampleModeFlag = DenTorRotSampleStart;
                }
    			break;
            case SpaTorRotSampleStart:
    			//-------------------------------------------
				if(FirstIntoSpaTorRotSample)//第一次进入SpaTorRotSampleStart状态
				{
    				FirstIntoSpaTorRotSample = FALSE;
					CS5463_WriteCmd(start_SConvert,CS5463_CSABC);//开启CS5463测量
					ClearStruct(SaveAidStruct);
					ReadCurTimeToBuffer(TimeBufForSpa,3);
					TimeAddSecs(TimeBufForSpa,20);		   //保存当前时间,当前时间加20秒钟
					//----------------------------------------------------------
				}
				Start_SpaTorRotSample();//开启稀疏型转速转矩测量
				//-----------------------------------------
				SampleModeFlag = SpaTorRotSampling;//跳转到SpaTorRotSampling
    			break;
            case SpaTorRotSampling:
                //--------------------------------------------
                //为转矩显示与存储作准备
				if(TorqueRotateSampleFlag & SAVETRUE)
                {
				 	//计算出在稀疏型采样时1秒内的扭矩脉冲个数
					TorqueCount = (signed int)(TorqueGain*SUMTORCNT());//TorqueCount为半秒钟内的脉冲个数
					//--------------------------------------------
					SaveAidStruct.TorqueCountSUM += TorqueCount;
                    SaveAidStruct.TorqueNum ++;
					MeasureUPDF = TRUE;
					TorqueRotateSampleFlag &= ~SAVETRUE;
                }
                //-------------------------------------------
				//为转速转矩显示作准备
				if(TorqueRotateSampleFlag & SHOWTRUE)
                {
					MeasureUPDF = TRUE;
					TorqueRotateSampleFlag &= ~SHOWTRUE;
                }
                //--------------------------------------------
				//保存转速数据,跳转
				if(TorqueRotateSampleFlag & QUITTRUE)//如果定时已到的标志为真,(定时已到)
                {
    				TorqueRotateSampleFlag &= ~QUITTRUE;
					//-------------------------------------------
					//把测量值添加到旧测量值
					SaveAidStruct.RotateTimeCountSUM += 
        				(unsigned long int)(RotateGain*(RotateTimeCountEnd - RotateTimeCountStart0));
					SaveAidStruct.RotateNum += RotateNumCount - 1;
					//-------------------------------------------
					//计算扭矩
					if(RotateTime && TorqueCount)
					{
    					IntegrationStruct.TorquePower = 6.28*Torque_Proportion*fabs((int)(2*TorqueCount-TorqueOffset))*Rotate_Proportion/(RotateTime*(8e-6)*60*Wheel_A);
					}
					else
					{
    					IntegrationStruct.TorquePower = 0.0;
					}
					//-------------------------------------------
					//计算获能系数
                    if(IntegrationStruct.AverageFlowSpeed)
                    {
                        IntegrationStruct.PowerGetEfficiency = 
                            IntegrationStruct.TorquePower/
                            (0.5*1000*(IntegrationStruct.AverageFlowSpeed)*(IntegrationStruct.AverageFlowSpeed)*(IntegrationStruct.AverageFlowSpeed)*Wheel_D*Wheel_L);
                    }
                    else
                    {
                        IntegrationStruct.PowerGetEfficiency = 0.0;
                    }
					//-------------------------------------------
					IntegrationUPDF = TRUE;//设置变量
					//-------------------------------------------
					SampleModeFlag = SpaFlowSampleStart;//转到流速测量
                }
                break;
            case SpaFlowSampleStart:
                PrepareForMeasureFlow();//开启稀疏型流速测量
                SampleModeFlag = SpaFlowSampling;
                break;
            case SpaFlowSampling:
                //--------------------------------------------
				//为流速显示作准备
				if(FlowSampleFlag & SHOWTRUE)
                {
					MeasureUPDF = TRUE;
					FlowSampleFlag &= ~SHOWTRUE;
                }
				//--------------------------------------------
                //保存流速数据,跳转
				if(FlowSampleFlag & QUITTRUE)//如果定时已到的标志为真,(定时已到)
                {
					FlowSampleFlag &= ~QUITTRUE;
					//-------------------------------------------
					//保存数据,把新数据加到旧数据上
					SaveAidStruct.Flow1TimeCountSUM += 
						(Flow1TimeCountEnd - Flow1TimeCountStart0);
					SaveAidStruct.Flow1Num += Flow1NumCount - 1;
					SaveAidStruct.Flow2TimeCountSUM += 
						(Flow2TimeCountEnd - Flow2TimeCountStart0);
					SaveAidStruct.Flow2Num += Flow2NumCount - 1;
					//-----------------------------------------
					//计算平均流速
					if(Flow1Time && Flow2Time)
					{
    					IntegrationStruct.AverageFlowSpeed = 
        					(Flow1_Proportion/(Flow1Time*(8e-6)) + Flow1_Proportion/(Flow2Time*(8e-6)))/2;
					}
					else if(Flow1Time)
					{
					 	IntegrationStruct.AverageFlowSpeed = 
        					Flow1_Proportion/(Flow1Time*(8e-6))/2;
					}
					else if(Flow2Time)
					{
					 	IntegrationStruct.AverageFlowSpeed = 
        					Flow2_Proportion/(Flow2Time*(8e-6))/2;
					}
					else
					{
					 	IntegrationStruct.AverageFlowSpeed = 0.0;
					}
					IntegrationUPDF = TRUE;//设置变量
					//-----------------------------------------
					SampleModeFlag = SpaTorRotSampleStart;//转到转速测量
                }
                break;
            case SampleStop:break;
			default:SampleModeFlag = SampleStop;break;
		}
		
		if(TimeAlarm & ALARMED_10MINS)
		{
		 	if(TimeAlarm & ALARMED_20SECS)
			{
				//-------------------------------------
    			//计算
    			MoveSpaDataToSaveStruct();
    			//-------------------------------------
    			//保存到外部Flash
    			SaveSpaPackageToFlash();
                //-------------------------------------
                //清空保存结构体与辅助保存结构体
				ClearStruct(SaveAidStruct);
                ClearStruct(SaveStruct);
				//-------------------------------------
				TimeAlarm &= ~ALARMED_20SECS;
			}
			//-------------------------------------
    		//开始新一轮密集模式下的测量
			//-----------------------------------------
			SampleModeFlag = DenTorRotSampleStart;
    		SampleDenModeNum = 0x00;
			TimeBufForSpa[2] |= 0x80;//设定一个不可能到达的时间
			//-------------------------------------
			TimeAlarm &= ~ALARMED_10MINS;
		}
		else if(TimeAlarm & ALARMED_20SECS)
		{
            //-------------------------------------
            //计算
            MoveSpaDataToSaveStruct();
			//-------------------------------------
            //保存到外部Flash
			SaveSpaPackageToFlash();
			//-------------------------------------
            //清空保存结构体与辅助保存结构体
            ClearStruct(SaveAidStruct);
            ClearStruct(SaveStruct);
            //-------------------------------------
			//开始新一轮稀疏模式下的测量
			//-----------------------------------------
			SampleModeFlag = SpaTorRotSampleStart;
			FirstIntoSpaTorRotSample = TRUE;
			//-------------------------------------
            TimeAlarm &= ~ALARMED_20SECS;
			//-------------------------------------
		}
		
		//-------------------------------------------------------------
		//比较时间
		ReadCurTimeToBuffer(CurrentTimeBuf,0x04);
		if(CurrentTimeBuf[0] != PCF8563OldSec)
		{
			BgLightDelayTime ++;
			if(BgLightDelayTime == BGLIGHTDELAYTIME)
			{
			 	lcd_bglight_disable();
			}
			else if(BgLightDelayTime > BGLIGHTDELAYTIME)
			{
			 	BgLightDelayTime = BGLIGHTDELAYTIME+1;
			}
			TimeUPDF = TRUE;
			PCF8563OldSec = CurrentTimeBuf[0];
		}
		
		//----------------------------------------------------------
        //判断定时是否已到
		if(TimeUPDF)
		{
    		EepromTimeCounter ++;
			if(((EepromTimeCounter > SaveTimeInterval) && 
			   (MemoryAddressCounter > (AddrCounterLastSaved + SaveByteNumDiff))) ||
			   (MemoryAddressCounter > (AddrCounterLastSaved + SaveByteNumDiff1)))
			{
    			Save_MemoryAddressCounter();
				EepromTimeCounter = 0x00;		
			}
		}
		//----------------------------------------------------------
        //判断定时是否已到
		if(TimeUPDF)
		{
			if(CompareTime(TimeBufForDen,CurrentTimeBuf,3))//如果与定时时间相等
            {
                TimeAlarm |= ALARMED_10MINS;//定时时间到标志置位
            }
            if(CompareTime(TimeBufForSpa,CurrentTimeBuf,3))//如果与定时时间相等
            {
                TimeAlarm |= ALARMED_20SECS;//定时时间到标志置位
            }
		}
		
		//---------------------------------------------------------
 		//当时间更新且处于(0x04,0x04,0x0C,0x00)时,刷屏,辅助调试
		if( TimeUPDF && CompareMenuState(0x04,0x04,0x0C,0x00,MenuG3))
        {
            ScreenUPDF = TRUE;//更新显示时间
        }
		
		//-------------------------------------------------------------
		//更新已用存储地址计数器的值
		if((MemoryAddressCounter != OldMemoryAddressCounter) && TimeUPDF &&
			CompareMenuState(0x09,0x01,0x00,0x00,MenuG2))
        {
           ScreenUPDF = TRUE;
           OldMemoryAddressCounter = MemoryAddressCounter;
        }
		
		//-------------------------------------------------------------
		//菜单显示
		if(MenuStateUPDF || ScreenUPDF)
		{
            MenuStateUPDF = FALSE;
            ShowMenu();
		}

    }//end of while(1)
}
//end of main()

void port_init(void)
{
    PORTA = 0x00;
    DDRA  = 0x7E;
    PORTB = 0x10;
    DDRB  = 0x1F;
    PORTC = 0x76; //m103 output only
    DDRC  = 0x7D;
    PORTD = 0x00;
    DDRD  = 0x3B;
    PORTE = 0xFA;
    DDRE  = 0x4F;
    PORTF = 0x0E;
    DDRF  = 0x0E;
    PORTG = 0x00;
    DDRG  = 0x01;
}

void timer0_init(void)
{
    TCCR0 = 0x00; //stop
    ASSR  = 0x00; //set async mode
    TCNT0 = 0x00; /*INVALID SETTING;*/ //set count
    OCR0  = 0x00; /*INVALID SETTING;*/
    TCCR0 = 0x07; //start timer
}

#pragma interrupt_handler timer0_ovf_isr:iv_TIM0_OVF
void timer0_ovf_isr(void)
{
    TCNT0 = 0x00; //reload counter value
    if( 0 == sti%3 )  {KeyValue = ReadKey();}
	if( 0 == sti%15 ) {ADCSRA |= BIT(ADSC); sti %= 15;}
    sti++;
}

void timer1_init(void)
{
    TCCR1B = 0x00; //stop
    TCNT1 = 0xFFFF;
	OCR1A = 0x00; 
    TCCR1A = 0x00;
    TCCR1B = 0x07; //外部时钟源
}

#pragma interrupt_handler timer1_ovf_isr:iv_TIM1_OVF
void timer1_ovf_isr(void)
{
 	 switch(SampleModeFlag)
	 {
        case DenTorRotSampling:
        case SpaTorRotSampling:
            Save_RotateTimeCount();
            break;
        case DenFlowSampling:
        case SpaFlowSampling:
            Save_Flow2TimeCount();
            break;
        default:break;
	 }
}

void timer2_init(void)
{
    TCCR2 = 0x00; //stop
    TCNT2 = 0xFF; //setup
    OCR2  = 0x00;
    TCCR2 = 0x07; //外部时钟源
}

#pragma interrupt_handler timer2_ovf_isr:iv_TIM2_OVF
void timer2_ovf_isr(void)
{
 	 switch(SampleModeFlag)
	 {
        case DenTorRotSampling:
        case SpaTorRotSampling:
            TorqueOvfCount ++;
            break;
        case DenFlowSampling:
        case SpaFlowSampling:
            Save_Flow1TimeCount();
            break;
        default:break;
	 }
}

void timer3_init(void)
{
    TCCR3B = 0x00; //stop
    TCNT3 = 0x00;  //setup
    OCR3A = 0xF424;
    OCR3B = 0x186A;
    OCR3C = 0x00;
    ICR3  = 0x00;
    TCCR3A = 0x00;
    TCCR3B = 0x03; //start Timer 64分频
    TCCR3C = 0x00;
}

#pragma interrupt_handler timer3_compa_isr:iv_TIM3_COMPA
void timer3_compa_isr(void)
{
 	switch(SampleModeFlag)
	{
    	case DenFlowSampling:
			Save_DenFlowCountForShow();
        	break;
    	case SpaFlowSampling:
			Save_SpaFlowCountForShow();
        	break;
    	default:break;
	}
}

#pragma interrupt_handler timer3_compb_isr:iv_TIM3_COMPB
void timer3_compb_isr(void)
{
 	switch(SampleModeFlag)
	{
    	case DenTorRotSampling:
			Save_DenTorqueRotateCount();
        	break;
    	case SpaTorRotSampling:
        	Save_SpaTorqueRotateCount();
        	break;
    	default:break;
	}
}

#pragma interrupt_handler int4_isr:iv_INT4
void int4_isr(void)
{
	if(!(CS5463_CSTMP&CS5463_CSA))
    {
        STAREG[0] = CS5463_ReadReg(Status_Reg,CS5463_CSA);
        if(STAREG[0] & BIT_NUM23)
        {
            CS5463_CSTMP |= CS5463_CSA;
		}
    }
    	
    if(!(CS5463_CSTMP&CS5463_CSB))
    {
        STAREG[1] = CS5463_ReadReg(Status_Reg,CS5463_CSB);
        if(STAREG[1] & BIT_NUM23)
        {
    		CS5463_CSTMP |= CS5463_CSB;
		}
    }
    	
    if(!(CS5463_CSTMP&CS5463_CSC))
    {
        STAREG[2] = CS5463_ReadReg(Status_Reg,CS5463_CSC);
        if(STAREG[2] & BIT_NUM23)
        {
    		CS5463_CSTMP |= CS5463_CSC;
		}
    }
	
    if((CS5463_CSTMP&CS5463_CSABC) == CS5463_CSABC)
    {
        CS5463_CSTMP = 0;
        CS5463UPDF |= UPDATETRUE;
        CS5463_WriteReg(Status_Reg,0xFFFFFF,CS5463_CSABC);
		CS5463_WriteCmd(start_SConvert,CS5463_CSABC);
    }
    else
    {   CS5463_WriteReg(Status_Reg,0xFFFFFF,CS5463_CSTMP);}
}

#pragma interrupt_handler int5_isr:iv_INT5
void int5_isr(void)
{
    //TimeAlarm = NOTALARME;
}

//ADC initialize
// Conversion time: 6uS
void adc_init(void)
{
	ADCSRA = 0x00; //disable adc
    ADMUX = BIT(REFS0); //select adc input 0 
    ACSR  = BIT(ACD);
    ADCSRA = BIT(ADEN) | BIT(ADIE) | BIT(ADPS0);
	//ADCSRA |= BIT(ADSC);
}

#pragma interrupt_handler adc_isr:iv_ADC
void adc_isr(void)
{
	 ADCREG = ADC;
	 ADCSRA |= BIT(ADIF);
	 BatVolUPDF = TRUE;
	 if(ADCREG < 0x300)
	 {
    	 Save_MemoryAddressCounter();
	 }
}

void init_devices(void)
{
    CLI(); //disable all interrupts
    XDIV  = 0x00; //xtal divider
    XMCRA = 0x00; //external memory
    //-------------------------------
    port_init();
    timer0_init();
	timer1_init();
	timer2_init();
	timer3_init();
	adc_init();
	TWI_Init();
	uart1_init();
    lcd_PortInit();
	lcd_init();
    cs5463_PortInit();
    cs5463_RegInit();
	Memory_PortInit();
	Init_MemoryAddressCounter();
    //-------------------------------
    MCUCR = 0x00;
    EICRA = 0x00; //extended ext ints
    EICRB = BIT(ISC51); //extended ext ints
    EIMSK = BIT(INT5) | BIT(INT4);
    TIMSK = BIT(TOIE0); //timer interrupt sources
	ETIMSK = 0x00;//extended timer interrupt sources
    SEI(); //re-enable interrupts
    //-------------------------------
}

