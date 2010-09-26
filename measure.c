#include "config.h"

extern uint16 Uart_DelayTime;
extern uint16 BGLIGHTDELAYTIME;

extern uint8  PCArray[MAXDATAWIDTH];
extern const  uint8 AddrTable[4];
extern bool   DataSaveEnableFlag;
extern struct SaveStructure SaveStruct;
extern struct SAVEAIDSTRUCT SaveAidStruct;
extern uint8  MemoryStatusREG;
extern uint32 MemoryAddressCounter;

extern float ACTIVEPREG[3];
extern float RMSVOLREG[3];
extern float RMSCURREG[3];
extern float POWERFACTORREG[3];

extern uint8 CurrentTimeBuf[4];
//-----------------------------------------------------
//ת����ر���
bool   TorqueRotateSampleFlag = FALSE;
uint8  TorqueOvfCount = 0x00;
uint16 TorqueCountArray1[10];
uint16 TorqueCountArray2[10];
bool   TorqueArray_UseState = FALSE;
signed int TorqueCount;
//----------------------------------------------------
//ת����ر���
float  RotateTime;
uint32 RotateTimeCountStart0 = 0x00;
uint32 RotateTimeCountStartx = 0x00;
uint32 RotateTimeCountEnd = 0x00;
uint32 RotateTimeCountEndx = 0x00;
uint8  RotateNumCount = 0x00;
uint8  RotateNumCountx0 = 0x00;
uint8  RotateNumCountx1 = 0x00;

//----------------------------------------------------
//���ټ�1��ر���
float  Flow1Time;
uint32 Flow1TimeCountStart0 = 0x00;
uint32 Flow1TimeCountStartx = 0x00;
uint32 Flow1TimeCountEnd = 0x00;
uint32 Flow1TimeCountEndx = 0x00;
uint8  Flow1NumCount = 0x00;
uint8  Flow1NumCountx0 = 0x00;
uint8  Flow1NumCountx1 = 0x00;

//----------------------------------------------------
//���ټ�2��ر���
float  Flow2Time;
uint32 Flow2TimeCountStart0 = 0x00;
uint32 Flow2TimeCountStartx = 0x00;
uint32 Flow2TimeCountEnd = 0x00;
uint32 Flow2TimeCountEndx = 0x00;
uint8  Flow2NumCount = 0x00;
uint8  Flow2NumCountx0 = 0x00;
uint8  Flow2NumCountx1 = 0x00;
bool   FlowSampleFlag = FALSE;

//----------------------------------------------------
//��ر���
uint16 ADCREG = 0x00;
bool   MeasureUPDF = FALSE;
bool   BatVolUPDF = FALSE;
bool   FirstIntoSpaTorRotSample;
SampleType SampleModeFlag = SampleStop;
uint16 T3OCRA_Count = 0x00;
uint16 T3OCRB_Count = 0x00;
uint8  SampleDenModeNum = 0x00;
struct IntegrationStructure IntegrationStruct;
//-----------------------------------------------------
//�ɵ�����
float  TorqueGain;
uint16 TorqueOffset;
float  RotateGain;
float  RotateOffset;
float  Flow1Gain;
float  Flow1Offset;
float  Flow2Gain;
float  Flow2Offset;

float  Flow1_Proportion = 0.2;
float  Flow2_Proportion = 0.2;
float  Rotate_Proportion	= 1.0;
float  Torque_Proportion	= 1.0;
float  Wheel_A = 1.0;
float  Wheel_D = 1.5;
float  Wheel_L = 1.3;
uint16 Torque_Threshold = 5000;
//-----------------------------------------------------
uint16 average(uint16 data[],uint8 num)
{
 	uint8 i;
	uint32 avg = 0x00;
	
	if(num==0){return 0;}
	for(i=0;i<num;i++)
	{
        avg += data[i];
	}
	avg /= num;
	return (uint16)avg;
}

uint16 Sum(uint16 data[],uint8 num)
{
 	uint8 i;
	uint16 SumNum = 0x00;
	
	for(i=0;i<num;i++)
	{
        SumNum += data[i];
	}
	
	return SumNum;
}

void InitMeasurePara_FromEeprom(void)
{
	EEPROM_READ(TorqueGain_Addr,TorqueGain);
	EEPROM_READ(TorqueOffset_Addr,TorqueOffset);
	EEPROM_READ(RotateGain_Addr,RotateGain);
	EEPROM_READ(RotateOffset_Addr,RotateOffset);
	EEPROM_READ(Flow1Gain_Addr,Flow1Gain);
	EEPROM_READ(Flow1Offset_Addr,Flow1Offset);
	EEPROM_READ(Flow2Gain_Addr,Flow2Gain);
	EEPROM_READ(Flow2Offset_Addr,Flow2Offset);
	
	EEPROM_READ(Uart_DelayTime_Addr,Uart_DelayTime);
	EEPROM_READ(BGLIGHTDELAYTIME_Addr,BGLIGHTDELAYTIME);
	
	EEPROM_READ(Flow1_Proportion_Addr,Flow1_Proportion);
	EEPROM_READ(Flow2_Proportion_Addr,Flow2_Proportion);
	EEPROM_READ(Rotate_Proportion_Addr,Rotate_Proportion);
	EEPROM_READ(Torque_Proportion_Addr,Torque_Proportion);
	EEPROM_READ(Wheel_A_Addr,Wheel_A);
	EEPROM_READ(Wheel_D_Addr,Wheel_D);
	EEPROM_READ(Wheel_L_Addr,Wheel_L);
	EEPROM_READ(Torque_Threshold_Addr,Torque_Threshold);
}

void Start_TorRotSample(uint8 TimeInterval)
{
	StopTimer123();//�ر�T1,T2,T3;
    TorqueOvfCount = 0x00;
    T3OCRB_Count = 0x00;//�����ڼ�¼��ţ�

	RotateNumCount = 0x00;
	RotateNumCountx0 = 0x00;
	RotateNumCountx1 = 0x00;
	RotateTimeCountStart0 = 0x00;
	RotateTimeCountStartx = 0x00;
	RotateTimeCountEnd = 0x00;
	RotateTimeCountEndx = 0x00;

	SelectFreqChannel(0x02);
	OCR3B = TimeInterval*0x186A;	   //��T3����ж�B��ʱ��
	TCNT1 = 0xFFFF;			//TCNT1��Ϊ0xFFFF
	TCNT2 = 0x00;			//TCNT2����
	TCNT3 = 0x00;
	TIMSK |= BIT(TOIE1) | BIT(TOIE2);//����T1,T2����ж�
	ETIMSK = BIT(OCIE3B);//����T3�Ƚ��ж�B,�رձȽ��ж�A
	StartTimer123();//��T1,T2,T3;
}

void Save_TorqueRotateCount(uint8 QuitTimeInterval,
	 				  	    uint8 ShowTimeInterval)
{   	
    //-----------------------------------------------------
    if(!TorqueArray_UseState)//TorqueArray_UseStateΪFALSEʱ,������TorqueCountArray1[]��
    {
        TorqueCountArray1[T3OCRB_Count%0x0A] =
            (uint16)TCNT2+0xFF* (uint16)TorqueOvfCount;
    }
    else		  //TorqueArray_UseStateΪTRUEʱ,������TorqueCountArray2[]��
    {
        TorqueCountArray2[T3OCRB_Count%0x0A] =
            (uint16)TCNT2+0xFF* (uint16)TorqueOvfCount;
    }	
    
    TCNT2 = 0x00;
    //-----------------------------------------------------
    TorqueOvfCount = 0x00;
    T3OCRB_Count ++;
    if(!(T3OCRB_Count % 0x0A))//�ƹ�500ms
    {
		StopTimer123();
		TCNT3 = 0x00;
		OCR3B = 0x186A;
		TorqueArray_UseState = !TorqueArray_UseState;//������һ·������		
		TorqueRotateSampleFlag |= SAVETRUE;//�ƹ�500ms�Ϳ��Ա���ת����,��λ�����־			
         
        if(!(T3OCRB_Count % (0x0A*ShowTimeInterval)))//��ʱ����,����ʾ
        {
    		TorqueRotateSampleFlag |= SHOWTRUE;//��λ��ʾ��־
			//--------------------------------------------------
			//Ϊת����ʾ��׼��
            RotateNumCountx0 = RotateNumCountx1;
            RotateNumCountx1 = RotateNumCount;
            RotateTimeCountStartx = RotateTimeCountEndx;
            RotateTimeCountEndx = RotateTimeCountEnd;
			
    		//if(RotateNumCountx1 - RotateNumCountx0)
			//{
    			//RotateTime = RotateGain*((float)(RotateTimeCountEndx - RotateTimeCountStartx))/(RotateNumCountx1 - RotateNumCountx0);
			//}
			if(RotateNumCount > 1)
    		{
				RotateTime = RotateGain*((float)(RotateTimeCountEnd - RotateTimeCountStart0))/(RotateNumCount - 1);
    		}
    		else
    		{
        		RotateTime = 0x00;
    		}
        }
		//------------------------------------------------------
		if(!(T3OCRB_Count % (0x0A*QuitTimeInterval)))//�ƹ�15s,��λ�˳���־
		{
			ETIMSK &= ~BIT(OCIE3B);//�ر�T3�Ƚ��ж�B
			StopTimer123();
			TorqueRotateSampleFlag |= QUITTRUE;//��λ�˳���־
		}
		else
		{
    		StartTimer123();
		}
	}
    else
	{
    	OCR3B += 0x186A;
	}	
}

void Save_RotateTimeCount(void)
{
    //T1����ж��е��ô˺���,���ڼ�¼ת�٣�
    RotateNumCount ++;
    if(RotateNumCount==0x01)
    {
    	RotateTimeCountStart0 =
    		(uint32)TCNT3+ 0xF424* (uint32)(T3OCRB_Count/0x0A);
        RotateTimeCountStartx = RotateTimeCountStart0;
        RotateNumCountx0 = 0x01;
        RotateNumCountx1 = 0x01;
    }
    else
	{
        RotateTimeCountEnd = 
    		(uint32)TCNT3+0xF424* (uint32)(T3OCRB_Count/0x0A);
			//�������һ�����嵽����ʱ��
    }
    TCNT1 = 0xFFFF;//TCNT1��Ϊ��ֵ
}

void SaveFlowCountForShow(uint8 FlowTimeInterval,
	 					  uint8 ShowInterval)
{
    //��T3�Ƚ��ж�A�е���,���ڸ����ٲ�����ʱ
	T3OCRA_Count ++;
    TCNT3=0x00; 
    if(!(T3OCRA_Count % ShowInterval))//��ʾ��ʱʱ�䵽
    {
		//------------------------------------------------
		//ΪFlow1��ʾ��׼������
        Flow1NumCountx0 = Flow1NumCountx1;
        Flow1NumCountx1 = Flow1NumCount;
        Flow1TimeCountStartx = Flow1TimeCountEndx;
        Flow1TimeCountEndx = Flow1TimeCountEnd;
        //------------------------------------------------
		//ΪFlow2��ʾ��׼������
		Flow2NumCountx0 = Flow2NumCountx1;
        Flow2NumCountx1 = Flow2NumCount;
        Flow2TimeCountStartx = Flow2TimeCountEndx;
        Flow2TimeCountEndx = Flow2TimeCountEnd;

		FlowSampleFlag |= SHOWTRUE;
		//------------------------------------------------
		if(!(T3OCRA_Count%FlowTimeInterval))//�˳���ʱʱ�䵽
    	{ 
            ETIMSK &= ~BIT(OCIE3A);//�ر�T3�Ƚ��ж�A
			StopTimer123();//�ر�T1,T2,T3
			FlowSampleFlag |= QUITTRUE;
			//-----------------------------------------------------------
			//��������1�Ĵ洢����ʾ����
    		if(Flow1NumCount > 1)
    		{
        		Flow1Time = Flow1Gain*((float)(Flow1TimeCountEnd - Flow1TimeCountStart0))/(Flow1NumCount - 1);
    		}
    		else
    		{
        		Flow1Time = 0x00;
    		}
			//-----------------------------------------------------------
			//��������2�Ĵ洢����ʾ����
    		if(Flow2NumCount > 1)
    		{
        		Flow2Time = Flow2Gain*((float)(Flow2TimeCountEnd - Flow2TimeCountStart0))/(Flow2NumCount - 1);
    		}
    		else
    		{
        		Flow2Time = 0x00;
    		}
			//-----------------------------------------------------------
    	}
    }
}

void Save_Flow1TimeCount(void)
{
    //T2����ж���ִ��,���ڱ�������
    Flow1NumCount ++;
    if(Flow1NumCount==0x01)
    {
    	Flow1TimeCountStart0 =
    		(uint32)TCNT3 + 0xF424*(uint32)T3OCRA_Count;
        Flow1TimeCountStartx = Flow1TimeCountStart0;
        Flow1NumCountx0 = 0x01;
        Flow1NumCountx1 = 0x01;
    }
    else
	{
        Flow1TimeCountEnd = 
    		(uint32)TCNT3 + 0xF424*(uint32)T3OCRA_Count;
			//�������һ�����嵽����ʱ��
    }
    TCNT2 = 0xFF;//TCNT2��Ϊ��ֵ
}

void Save_Flow2TimeCount(void)
{
    //T1����ж���ִ��,���ڱ�������
    Flow2NumCount ++;
    if(Flow2NumCount==0x01)
    {
    	Flow2TimeCountStart0 =
    		(uint32)TCNT3 + 0xF424*(uint32)T3OCRA_Count;
        Flow2TimeCountStartx = Flow2TimeCountStart0;
        Flow2NumCountx0 = 0x01;
        Flow2NumCountx1 = 0x01;
    }
    else
	{
        Flow2TimeCountEnd = 
    		(uint32)TCNT3+0xF424* (uint32)T3OCRA_Count;
			//�������һ�����嵽����ʱ��
    }
    TCNT1 = 0xFFFF;//TCNT1��Ϊ��ֵ
}

void PrepareForMeasureFlow(void)
{
    //Ϊ��������׼��
	StopTimer123();//�رռ�����T1,T2,T3;
	TCNT1 = 0xFFFF;
    TCNT2 = 0xFF;
	TCNT3 = 0x00;
    T3OCRA_Count = 0x00;//
    ETIMSK &= ~(BIT(OCIE3B));//�ر�T3�Ƚ��ж�B
    ETIMSK |= BIT(OCIE3A);//����T3�Ƚ��ж�A
	TIMSK |= BIT(TOIE1) | BIT(TOIE2);//����T1,T2����ж�
    SelectFreqChannel(0x00);//ͨ���л�����00ͨ��;
	//���ټ�1������ʼ��
    Flow1NumCount = 0x00;
	Flow1NumCountx0 = 0x00;
	Flow1NumCountx1 = 0x00;
	Flow1TimeCountStart0 = 0x00;
	Flow1TimeCountStartx = 0x00;
	Flow1TimeCountEnd = 0x00;
	Flow1TimeCountEndx = 0x00;
	//���ټ�2������ʼ��
    Flow2NumCount = 0x00;
	Flow2NumCountx0 = 0x00;
	Flow2NumCountx1 = 0x00;
	Flow2TimeCountStart0 = 0x00;
	Flow2TimeCountStartx = 0x00;
	Flow2TimeCountEnd = 0x00;
	Flow2TimeCountEndx = 0x00;
	
	FlowSampleFlag = FALSE;	//������ټƲ�����־
	StartTimer123();//�򿪼�����T1,T2,T3;
}

void SaveTorqueCountArrayToFlash(void)
{
 	uint8 lowaddr = MemoryAddressCounter%0x100;
	uint8 *pt;
	
	if(!DataSaveEnableFlag){return;}
	if(MemoryAddressCounter > MAX_ADDRESS - 20) {return;}
	
	if(TorqueArray_UseState)
	{pt =(uint8*)TorqueCountArray1;}
	else 
	{pt = (uint8*)TorqueCountArray2;}
	
	if((0x100-lowaddr)>=20)
	{
        Memory_WaitWriteInProg();
		Memory_WriteEn();
        Memory_WriteData(MemoryAddressCounter,pt,20);
        MemoryAddressCounter += 20;
	}
	else
	{
        Memory_WaitWriteInProg();
		Memory_WriteEn();
        Memory_WriteData(MemoryAddressCounter,pt,0x100-lowaddr);
        MemoryAddressCounter += 0x100-lowaddr;
        pt += 0x100-lowaddr;
        Memory_WaitWriteInProg();
		Memory_WriteEn();
        Memory_WriteData(MemoryAddressCounter,pt,20-0x100+lowaddr);
        MemoryAddressCounter += 20-0x100+lowaddr;
	}
}

void SaveDenPackageHeadToFlash(void)
{
    uint8 ch[6] = {'\x55','\x55'};
	
	ReadCurTimeToBuffer(CurrentTimeBuf,0x04);
	ch[2] = CurrentTimeBuf[3];
	ch[3] = CurrentTimeBuf[2];
	ch[4] = CurrentTimeBuf[1];
	ch[5] = CurrentTimeBuf[0];

    SaveDataArrayToFlash(ch,0x06);
}

void SaveDenPackageToFlash(void)
{
    uint8 lowaddr = MemoryAddressCounter%0x100;
	uint8 *pt;
	
	if(!DataSaveEnableFlag){return;}
	if(MemoryAddressCounter > MAX_ADDRESS - DenSaveStructLen) {return;}
	
	SaveStruct.tail = 0xF5F5;
	
    pt = (uint8*)(&SaveStruct);
	pt += SaveStructDiffLen;
	
	if((0x100-lowaddr)>=DenSaveStructLen)
	{
        Memory_WaitWriteInProg();
		Memory_WriteEn();
        Memory_WriteData(MemoryAddressCounter,pt,DenSaveStructLen);
        MemoryAddressCounter += DenSaveStructLen;
	}
	else
	{
        Memory_WaitWriteInProg();
		Memory_WriteEn();
        Memory_WriteData(MemoryAddressCounter,pt,0x100-lowaddr);
        MemoryAddressCounter += 0x100-lowaddr;
        pt += 0x100-lowaddr;
        Memory_WaitWriteInProg();
		Memory_WriteEn();
        Memory_WriteData(MemoryAddressCounter,pt,DenSaveStructLen-0x100+lowaddr);
        MemoryAddressCounter += DenSaveStructLen-0x100+lowaddr;
	}
}

void SaveSpaPackageToFlash(void)
{
 	uint8 lowaddr = MemoryAddressCounter%0x100;
	uint8 *pt;
	
	if(!DataSaveEnableFlag){return;}
	if(MemoryAddressCounter > MAX_ADDRESS - SpaSaveStructLen) {return;}
	
	SaveStruct.head = 0xAAAA;
	SaveStruct.RotateNumCount = 0xFAFA;
	
	ReadCurTimeToBuffer(CurrentTimeBuf,0x04);
	SaveStruct.TimeBuffer[0] = CurrentTimeBuf[1];
	SaveStruct.TimeBuffer[1] = CurrentTimeBuf[0];
	
    pt = (uint8*)(&SaveStruct);
	
	if((0x100-lowaddr)>=SpaSaveStructLen)
	{
        Memory_WaitWriteInProg();
		Memory_WriteEn();
        Memory_WriteData(MemoryAddressCounter,pt,SpaSaveStructLen);
        MemoryAddressCounter += SpaSaveStructLen;
	}
	else
	{
        Memory_WaitWriteInProg();
		Memory_WriteEn();
        Memory_WriteData(MemoryAddressCounter,pt,0x100-lowaddr);
        MemoryAddressCounter += 0x100-lowaddr;
        pt += 0x100-lowaddr;
        Memory_WaitWriteInProg();
		Memory_WriteEn();
        Memory_WriteData(MemoryAddressCounter,pt,SpaSaveStructLen-0x100+lowaddr);
        MemoryAddressCounter += SpaSaveStructLen-0x100+lowaddr;
	}
}

void SaveDataArrayToFlash(uint8 pt[],uint8 num)
{
 	uint8 lowaddr = MemoryAddressCounter%0x100;

	if(!DataSaveEnableFlag){return;}
	if(MemoryAddressCounter > MAX_ADDRESS - num) {return;}
	
	if((0x100-lowaddr)>=num)
	{
        Memory_WaitWriteInProg();
		Memory_WriteEn();
        Memory_WriteData(MemoryAddressCounter,pt,num);
        MemoryAddressCounter += num;
	}
	else
	{
        Memory_WaitWriteInProg();
		Memory_WriteEn();
        Memory_WriteData(MemoryAddressCounter,pt,0x100-lowaddr);
        MemoryAddressCounter += 0x100-lowaddr;
        pt += 0x100-lowaddr;
		Memory_WaitWriteInProg();
        Memory_WriteEn();
        Memory_WriteData(MemoryAddressCounter,pt,num-0x100+lowaddr);
        MemoryAddressCounter += num-0x100+lowaddr;
	}
}

void MoveSpaDataToSaveStruct(void)
{
    if(SaveAidStruct.PowerNum)
    {
        SaveStruct.VolCurStruct.Vol[0] = (uint16)((SaveAidStruct.Vol[0]/SaveAidStruct.PowerNum)*100);
        SaveStruct.VolCurStruct.Vol[1] = (uint16)((SaveAidStruct.Vol[1]/SaveAidStruct.PowerNum)*100);
        SaveStruct.VolCurStruct.Vol[2] = (uint16)((SaveAidStruct.Vol[2]/SaveAidStruct.PowerNum)*100);
        SaveStruct.VolCurStruct.Cur[0] = (uint16)((SaveAidStruct.Cur[0]/SaveAidStruct.PowerNum)*100);
        SaveStruct.VolCurStruct.Cur[1] = (uint16)((SaveAidStruct.Cur[1]/SaveAidStruct.PowerNum)*100);
        SaveStruct.VolCurStruct.Cur[2] = (uint16)((SaveAidStruct.Cur[2]/SaveAidStruct.PowerNum)*100);
        
        SaveStruct.ACTIVEPREGSUM = 
            (uint16)((SaveAidStruct.Power/SaveAidStruct.PowerNum)*10);
        SaveStruct.POWERFACTORREGAVG = 
            (uint16)((SaveAidStruct.PowerFactor/SaveAidStruct.PowerNum/3)*1000);
    }
	
	if(SaveAidStruct.RotateNum && SaveAidStruct.RotateTimeCountSUM)
    {
        SaveStruct.RotateTimeCountAVG = 
			(uint16)(((SaveAidStruct.RotateNum*Rotate_Proportion)/(SaveAidStruct.RotateTimeCountSUM*(8e-6)) - RotateOffset)*100);//���Rotate_Proportion != 1.0,��Ҫ����Rotate_Proportion
    }
	
    if(SaveAidStruct.Flow1Num && SaveAidStruct.Flow1TimeCountSUM)
    {
        SaveStruct.Flow1TimeCountAVG =
			(uint16)(((SaveAidStruct.Flow1Num*Flow1_Proportion)/(SaveAidStruct.Flow1TimeCountSUM*(8e-6)) - Flow1Offset)*100);
    }
	
    if(SaveAidStruct.Flow2Num && SaveAidStruct.Flow2TimeCountSUM)
    {
        SaveStruct.Flow2TimeCountAVG =
			(uint16)(((SaveAidStruct.Flow2Num*Flow2_Proportion)/(SaveAidStruct.Flow2TimeCountSUM*(8e-6)) - Flow2Offset)*100);
    }
	
    if(SaveAidStruct.TorqueNum)
    {
        SaveStruct.TorqueCountAVG =
            (uint16)(2*SaveAidStruct.TorqueCountSUM/SaveAidStruct.TorqueNum);
			
    }
	
    SaveStruct.CP = (uint16)((IntegrationStruct.PowerGetEfficiency)*1000);
}

void MoveDenDataToSaveStruct(uint8 flag)
{
	if(flag & FLOWFLAG)
	{
    	if(Flow1NumCount && Flow1Time)
        {
            SaveStruct.Flow1TimeCountAVG =
				(uint16)((Flow1_Proportion/(Flow1Time*(8e-6)) - Flow1Offset)*100.0);
        }
        if(Flow2NumCount && Flow2Time)
        {
            SaveStruct.Flow2TimeCountAVG =
				(uint16)((Flow2_Proportion/(Flow2Time*(8e-6)) - Flow2Offset)*100.0);
        }
	}
	
	if(flag & ROTATEFLAG)
	{
    	if(RotateTime)
        {
            SaveStruct.RotateTimeCountAVG =
				(uint16)((Rotate_Proportion/(RotateTime*(8e-6)) - RotateOffset)*100.0);
			SaveStruct.RotateNumCount = RotateNumCount;
        }
	}
	
	if(flag & POWERFLAG)
	{
    	if(SaveAidStruct.PowerNum)
        {
            SaveStruct.ACTIVEPREGSUM = 
                (uint16)((SaveAidStruct.Power/SaveAidStruct.PowerNum)*10);
            SaveStruct.POWERFACTORREGAVG = 
                (uint16)((SaveAidStruct.PowerFactor/SaveAidStruct.PowerNum/3)*1000);
    			
    		SaveStruct.RMSVOLREGAVG = (uint16)((SaveAidStruct.Vol[0] +
                   								    SaveAidStruct.Vol[1] +
                   								    SaveAidStruct.Vol[2]) /
                   								   (3*SaveAidStruct.PowerNum)*100);
    		SaveStruct.RMSCURREGAVG = (uint16)((SaveAidStruct.Cur[0] +
                   								    SaveAidStruct.Cur[1] +
                   								    SaveAidStruct.Cur[2]) /
                   								   (3*SaveAidStruct.PowerNum)*100);
        }
	}

}

void AddPowerParaToSaveAidStruct(void)
{
    SaveAidStruct.Power += ACTIVEPREG[0] + ACTIVEPREG[1] + ACTIVEPREG[2]; 
    SaveAidStruct.PowerFactor += POWERFACTORREG[0] + POWERFACTORREG[1] + POWERFACTORREG[2]; 
    SaveAidStruct.Vol[0] += RMSVOLREG[0];
    SaveAidStruct.Vol[1] += RMSVOLREG[1];
    SaveAidStruct.Vol[2] += RMSVOLREG[2];
    SaveAidStruct.Cur[0] += RMSCURREG[0];
    SaveAidStruct.Cur[1] += RMSCURREG[1];
    SaveAidStruct.Cur[2] += RMSCURREG[2];
    SaveAidStruct.PowerNum ++;
}

void ModifyTorqueCountArray(void)
{
 	uint8 i;
	
	if(TorqueArray_UseState)
    {	
		for(i=0;i<10;i++)
		{
            if (i==0)
            {
                TorqueCountArray1[i] = (uint16)(TorqueCountGainAll * TorqueCountArray1[i]);
            }
            else
            {
                TorqueCountArray1[i] = (uint16)(TorqueCountGainAll * TorqueCountArray1[i]);
            }
		}
	}
    else 
    {
	 	for(i=0;i<10;i++)
		{
            if (i==0)
            {
                TorqueCountArray2[i] = (uint16)(TorqueCountGainAll * TorqueCountArray2[i]);
            }
            else
            {
                TorqueCountArray2[i] = (uint16)(TorqueCountGainAll * TorqueCountArray2[i]);
            }
		}
	}
}

float emendation(float BaseValue, float MaxVal,	 float Diff, float CurValue)
{
    return CurValue + (CurValue - BaseValue)*Diff/(MaxVal-BaseValue);
}
