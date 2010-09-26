#include "main.h"

//-----------------------------------------------------
//���ڵ���
float ACTIVEPREG_Debug[3];
float RMSVOLREG_Debug[3];
float RMSCURREG_Debug[3];

//-----------------------------------------------------
//������
void main(void)
{ 	 
	init_devices();
	
	LOADMENU(Menu_WelcomeSCR,Menu_WelcomeSCR_num);
	ShowMenuBuffer();

	ReadCurTimeToBuffer(CurrentTimeBuf,0x04);
	memcpy(TimeBufForDen,CurrentTimeBuf,3);
	TimeBufForDen[2] |= 0x80;//�趨һ�������ܵ����ʱ��
	memcpy(TimeBufForSpa,CurrentTimeBuf,3);
	TimeBufForSpa[2] |= 0x80;//�趨һ�������ܵ����ʱ��
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
		//������Ӧ
		if(Key_PreResponse())
		{
    		Key_Response();
		}
  
		//-------------------------------------------------------------
        //���µ繦�ʲ�������ʾ
        if(CS5463UPDF)
        {
            //-----------------------------------------
            //���CS5463���ݶ�ȡ���
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
                //���ڵ���
                if(CompareMenuState(0x0A,0x00,0x00,0x00,MenuG1) &&
                   MenuState.MG2 >= 1 && MenuState.MG2 <= 6)
                {
                    MenuStateUPDF = TRUE;
                }
				//-----------------------------------------
				
				//----------------------------------------------------------------------------------------
				//A��
                RMSCURREG[0] = CurGain_A*(ConvertUnsignedVal(CS5463_ReadReg(RMSCur_Reg,CS5463_CSA)) - CurOffset_A);
				//-----------------------------------------
				//A���������
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
				//A���ѹ����
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
				//B��
                RMSCURREG[1] = CurGain_B*(ConvertUnsignedVal(CS5463_ReadReg(RMSCur_Reg,CS5463_CSB)) - CurOffset_B);
				//-----------------------------------------
				//B���������
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
				//B���ѹ����
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
				//C��
                RMSCURREG[2] = CurGain_C*(ConvertUnsignedVal(CS5463_ReadReg(RMSCur_Reg,CS5463_CSC)) - CurOffset_C);
				//-----------------------------------------
				//C���������
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
				//C���ѹ����
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
				//�����ܹ���
				IntegrationStruct.OverallPower = ACTIVEPREG[0] + 
											     ACTIVEPREG[1] + 
												 ACTIVEPREG[2];
				IntegrationStruct.AveragePowerFactor = (POWERFACTORREG[0] + 
													    POWERFACTORREG[1] + 
													    POWERFACTORREG[2])/3;
				IntegrationUPDF = TRUE;
				//------------------------------------------------------
                //�������ݼӵ���������
                AddPowerParaToSaveAidStruct();
                CS5463UPDF &= ~UPDATETRUE;//
            }
			
			//-----------------------------------------
			//���ø�����ʾ
			if(CompareMenuState(0x00,0x01,0x00,0x00,MenuG2) ||
			   CompareMenuState(0x00,0x02,0x00,0x00,MenuG2))
            {
				ScreenUPDF = TRUE;
            }
			//-----------------------------------------
			//�����־
			CS5463UPDF = FALSE;
		}
		
		//-------------------------------------------------------------
		//���µ�ص�ѹ��ʱ�����ʾ
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
		//��������,ת��ת�ٵȵ���ʾ
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
		//�����ۺϲ�������ʾ
		if(IntegrationUPDF)
		{
			IntegrationUPDF = FALSE;
    		if(CompareMenuState(0x00,0x05,0x00,0x00,MenuG2))//�����ۺϲ�������ʾ
			{   
				ScreenUPDF = TRUE;
			}
		}
		
		//-------------------------------------------------------------
		//�ϴ�����
		if(UARTUPDF)
		{
			CmdStringResponse();
			memset(CmdString,0x00,CmdStrLength);
        	UARTUPDF = FALSE;
		}
		
		//-------------------------------------------------------------
		//�Բ���״̬��Ӧ
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
    					//��ȡ��ǰʱ��,��ǰʱ��,��10����,Ȼ�󱣴�
    					ReadCurTimeToBuffer(TimeBufForDen,3);
                        TimeAddMins(TimeBufForDen,10);
					}
					SampleDenModeNum ++;
					CS5463_WriteCmd(start_SConvert,CS5463_CSABC);					
                    SaveDenPackageHeadToFlash();
					ClearStruct(SaveAidStruct);
					Start_DenTorRotSample();//�����ܼ���ת��ת�ز���;
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
				//����ת������
                if(TorqueRotateSampleFlag & SAVETRUE)
                {
					//--------------------------------------
					//�ڴ˸�TorqueCountArray�е�ÿ�����ݼ�һ������,
					//��ÿ�����ݶ�����TorqueCountGainAll,�������TorqueCount
					//����һ���ڽ��յ����������
					ModifyTorqueCountArray();//
					//--------------------------------------
					//��ƽ��ֵ,ΪŤ����ʾ��������;��׼��
					TorqueCount = (signed int)(AVGTORCNT()/2);
					//--------------------------------------
					SaveTorqueCountArrayToFlash();
                    TorqueRotateSampleFlag &= ~SAVETRUE;
				}
				//-----------------------------------------
                //Ϊת��ת����ʾ��׼��
                if(TorqueRotateSampleFlag & SHOWTRUE)
                {
                 	MeasureUPDF = TRUE;
					TorqueRotateSampleFlag &= ~SHOWTRUE;
                }
				//--------------------------------------------
                //����ת������,��SaveStruct�ṹ����,����ת��һ�²�
                if(TorqueRotateSampleFlag & QUITTRUE)
                {
					if(RotateNumCount > 1)
					{
    					//�ܼ���ģʽ�¼���ת��ʱ��,ת��ʱ��ȡ�������Ӧ��ʵ�ʲ⵽��Ƶ��
						RotateTime = RotateGain*(RotateTimeCountEnd - RotateTimeCountStart0)/(RotateNumCount - 1);
					}
					else
					{
					 	RotateTime = 0.0;
					}
					MoveDenDataToSaveStruct(ROTATEFLAG);
					TorqueRotateSampleFlag &= ~QUITTRUE;
					//-----------------------------------------
					//�ܼ���ģʽ�¼���Ť��
					if(RotateTime && (fabs((int)(2*TorqueCount-TorqueOffset))<Torque_Threshold))
					{
    					IntegrationStruct.TorquePower = 
    						6.28*Torque_Proportion*fabs((int)(2*TorqueCount-TorqueOffset))*Rotate_Proportion/(RotateTime*(8e-6)*60*Wheel_A);
					}
					else
					{
    					IntegrationStruct.TorquePower = 0.0;
					}
					IntegrationUPDF = TRUE;//���ñ���
					//-------------------------------------------
					SampleModeFlag = DenFlowSampleStart;
                }
                //--------------------------------------------
                break;
            case DenFlowSampleStart:
				//-------------------------------------------
				//�����ܼ����ٲ���
				PrepareForMeasureFlow();
                //-----------------------------------------
				SampleModeFlag = DenFlowSampling;
    			break;
            case DenFlowSampling:
				//--------------------------------------------
                //Ϊ������ʾ��׼��
    			if(FlowSampleFlag & SHOWTRUE)
    			{
					MeasureUPDF = TRUE;
					FlowSampleFlag &= ~SHOWTRUE;
    			}
				//--------------------------------------------
				//������������,����ת��һ�²�
				if(FlowSampleFlag & QUITTRUE)
                {
					MoveDenDataToSaveStruct(FLOWFLAG | POWERFLAG);
					SaveDenPackageToFlash();
					//-----------------------------------------
					//��ձ���ṹ���븨������ṹ��
    				ClearStruct(SaveAidStruct);
                    ClearStruct(SaveStruct);
					//-----------------------------------------
					FlowSampleFlag &= ~QUITTRUE;
					//-----------------------------------------
					//����ƽ������
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
					//�������ϵ��
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
                    IntegrationUPDF = TRUE;//���ñ���
					//-----------------------------------------
					SampleModeFlag = DenTorRotSampleStart;
                }
    			break;
            case SpaTorRotSampleStart:
    			//-------------------------------------------
				if(FirstIntoSpaTorRotSample)//��һ�ν���SpaTorRotSampleStart״̬
				{
    				FirstIntoSpaTorRotSample = FALSE;
					CS5463_WriteCmd(start_SConvert,CS5463_CSABC);//����CS5463����
					ClearStruct(SaveAidStruct);
					ReadCurTimeToBuffer(TimeBufForSpa,3);
					TimeAddSecs(TimeBufForSpa,20);		   //���浱ǰʱ��,��ǰʱ���20����
					//----------------------------------------------------------
				}
				Start_SpaTorRotSample();//����ϡ����ת��ת�ز���
				//-----------------------------------------
				SampleModeFlag = SpaTorRotSampling;//��ת��SpaTorRotSampling
    			break;
            case SpaTorRotSampling:
                //--------------------------------------------
                //Ϊת����ʾ��洢��׼��
				if(TorqueRotateSampleFlag & SAVETRUE)
                {
				 	//�������ϡ���Ͳ���ʱ1���ڵ�Ť���������
					TorqueCount = (signed int)(TorqueGain*SUMTORCNT());//TorqueCountΪ�������ڵ��������
					//--------------------------------------------
					SaveAidStruct.TorqueCountSUM += TorqueCount;
                    SaveAidStruct.TorqueNum ++;
					MeasureUPDF = TRUE;
					TorqueRotateSampleFlag &= ~SAVETRUE;
                }
                //-------------------------------------------
				//Ϊת��ת����ʾ��׼��
				if(TorqueRotateSampleFlag & SHOWTRUE)
                {
					MeasureUPDF = TRUE;
					TorqueRotateSampleFlag &= ~SHOWTRUE;
                }
                //--------------------------------------------
				//����ת������,��ת
				if(TorqueRotateSampleFlag & QUITTRUE)//�����ʱ�ѵ��ı�־Ϊ��,(��ʱ�ѵ�)
                {
    				TorqueRotateSampleFlag &= ~QUITTRUE;
					//-------------------------------------------
					//�Ѳ���ֵ��ӵ��ɲ���ֵ
					SaveAidStruct.RotateTimeCountSUM += 
        				(unsigned long int)(RotateGain*(RotateTimeCountEnd - RotateTimeCountStart0));
					SaveAidStruct.RotateNum += RotateNumCount - 1;
					//-------------------------------------------
					//����Ť��
					if(RotateTime && TorqueCount)
					{
    					IntegrationStruct.TorquePower = 6.28*Torque_Proportion*fabs((int)(2*TorqueCount-TorqueOffset))*Rotate_Proportion/(RotateTime*(8e-6)*60*Wheel_A);
					}
					else
					{
    					IntegrationStruct.TorquePower = 0.0;
					}
					//-------------------------------------------
					//�������ϵ��
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
					IntegrationUPDF = TRUE;//���ñ���
					//-------------------------------------------
					SampleModeFlag = SpaFlowSampleStart;//ת�����ٲ���
                }
                break;
            case SpaFlowSampleStart:
                PrepareForMeasureFlow();//����ϡ�������ٲ���
                SampleModeFlag = SpaFlowSampling;
                break;
            case SpaFlowSampling:
                //--------------------------------------------
				//Ϊ������ʾ��׼��
				if(FlowSampleFlag & SHOWTRUE)
                {
					MeasureUPDF = TRUE;
					FlowSampleFlag &= ~SHOWTRUE;
                }
				//--------------------------------------------
                //������������,��ת
				if(FlowSampleFlag & QUITTRUE)//�����ʱ�ѵ��ı�־Ϊ��,(��ʱ�ѵ�)
                {
					FlowSampleFlag &= ~QUITTRUE;
					//-------------------------------------------
					//��������,�������ݼӵ���������
					SaveAidStruct.Flow1TimeCountSUM += 
						(Flow1TimeCountEnd - Flow1TimeCountStart0);
					SaveAidStruct.Flow1Num += Flow1NumCount - 1;
					SaveAidStruct.Flow2TimeCountSUM += 
						(Flow2TimeCountEnd - Flow2TimeCountStart0);
					SaveAidStruct.Flow2Num += Flow2NumCount - 1;
					//-----------------------------------------
					//����ƽ������
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
					IntegrationUPDF = TRUE;//���ñ���
					//-----------------------------------------
					SampleModeFlag = SpaTorRotSampleStart;//ת��ת�ٲ���
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
    			//����
    			MoveSpaDataToSaveStruct();
    			//-------------------------------------
    			//���浽�ⲿFlash
    			SaveSpaPackageToFlash();
                //-------------------------------------
                //��ձ���ṹ���븨������ṹ��
				ClearStruct(SaveAidStruct);
                ClearStruct(SaveStruct);
				//-------------------------------------
				TimeAlarm &= ~ALARMED_20SECS;
			}
			//-------------------------------------
    		//��ʼ��һ���ܼ�ģʽ�µĲ���
			//-----------------------------------------
			SampleModeFlag = DenTorRotSampleStart;
    		SampleDenModeNum = 0x00;
			TimeBufForSpa[2] |= 0x80;//�趨һ�������ܵ����ʱ��
			//-------------------------------------
			TimeAlarm &= ~ALARMED_10MINS;
		}
		else if(TimeAlarm & ALARMED_20SECS)
		{
            //-------------------------------------
            //����
            MoveSpaDataToSaveStruct();
			//-------------------------------------
            //���浽�ⲿFlash
			SaveSpaPackageToFlash();
			//-------------------------------------
            //��ձ���ṹ���븨������ṹ��
            ClearStruct(SaveAidStruct);
            ClearStruct(SaveStruct);
            //-------------------------------------
			//��ʼ��һ��ϡ��ģʽ�µĲ���
			//-----------------------------------------
			SampleModeFlag = SpaTorRotSampleStart;
			FirstIntoSpaTorRotSample = TRUE;
			//-------------------------------------
            TimeAlarm &= ~ALARMED_20SECS;
			//-------------------------------------
		}
		
		//-------------------------------------------------------------
		//�Ƚ�ʱ��
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
        //�ж϶�ʱ�Ƿ��ѵ�
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
        //�ж϶�ʱ�Ƿ��ѵ�
		if(TimeUPDF)
		{
			if(CompareTime(TimeBufForDen,CurrentTimeBuf,3))//����붨ʱʱ�����
            {
                TimeAlarm |= ALARMED_10MINS;//��ʱʱ�䵽��־��λ
            }
            if(CompareTime(TimeBufForSpa,CurrentTimeBuf,3))//����붨ʱʱ�����
            {
                TimeAlarm |= ALARMED_20SECS;//��ʱʱ�䵽��־��λ
            }
		}
		
		//---------------------------------------------------------
 		//��ʱ������Ҵ���(0x04,0x04,0x0C,0x00)ʱ,ˢ��,��������
		if( TimeUPDF && CompareMenuState(0x04,0x04,0x0C,0x00,MenuG3))
        {
            ScreenUPDF = TRUE;//������ʾʱ��
        }
		
		//-------------------------------------------------------------
		//�������ô洢��ַ��������ֵ
		if((MemoryAddressCounter != OldMemoryAddressCounter) && TimeUPDF &&
			CompareMenuState(0x09,0x01,0x00,0x00,MenuG2))
        {
           ScreenUPDF = TRUE;
           OldMemoryAddressCounter = MemoryAddressCounter;
        }
		
		//-------------------------------------------------------------
		//�˵���ʾ
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
    TCCR1B = 0x07; //�ⲿʱ��Դ
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
    TCCR2 = 0x07; //�ⲿʱ��Դ
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
    TCCR3B = 0x03; //start Timer 64��Ƶ
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

