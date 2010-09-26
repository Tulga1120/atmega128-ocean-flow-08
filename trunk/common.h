#ifndef __COMMON_H
#define __COMMON_H

//----------------------------------------------------------------------

#define  CmdStrRow  	             3
#define  CmdStrCol  	            15
#define  CmdStrLength               (CmdStrCol*2)

#define  MAX_PARA_WIDTH             6
#define  PARA_NUM	 	            6
#define  MAXDATAWIDTH	            16
#define  xtal	      	            8u

#define  Addr_Len	 	            (0x40)
#define  Addr_Width	 	            (0x04)

#define  min(n1,n2)   	            ((n1<=n2)?(n1):(n2))
#define  max(n1,n2)   	            ((n1>=n2)?(n1):(n2))
#define  delay_1clk()               asm("nop")
#define  SET_REGBIT(REG,RETBIT)		REG |= (RETBIT)
#define  CLR_REGBIT(REG,REGBIT)		REG &= (~(REGBIT))
#define  READ_REGBIT(REG,REGBIT) 	(((REG) >> (REGBIT)) & BIT_NUM0)
#define  ClearStruct(st)			memset(&(st),0x00,sizeof(st))
//----------------------------------------------------------------------
//EEPROM中数据的存储地址 gain:增益 offset:偏置
//------------------------------------
#define  TorqueGain_Addr	        0x30
#define  TorqueOffset_Addr	        0x34
#define  RotateGain_Addr	        0x38
#define  RotateOffset_Addr	        0x3C
//------------------------------------
#define  Flow1Gain_Addr	  	        0x40
#define  Flow1Offset_Addr	        0x44
#define  Flow2Gain_Addr	  	        0x48
#define  Flow2Offset_Addr	        0x4C
//------------------------------------gagain
#define  VolGain_A_Addr		        0x50
#define  VolOffset_A_Addr	        0x54
#define  CurGain_A_Addr		        0x58
#define  CurOffset_A_Addr	        0x5C
#define  PowerGain_A_Addr	        0x60
#define  PowerOffset_A_Addr	        0x64
//------------------------------------
#define  VolGain_B_Addr		        0x70
#define  VolOffset_B_Addr	        0x74
#define  CurGain_B_Addr		        0x78
#define  CurOffset_B_Addr	        0x7C
#define  PowerGain_B_Addr	        0x80
#define  PowerOffset_B_Addr	        0x84
//------------------------------------
#define  VolGain_C_Addr		        0x90
#define  VolOffset_C_Addr	        0x94
#define  CurGain_C_Addr		        0x98
#define  CurOffset_C_Addr	        0x9C
#define  PowerGain_C_Addr	        0xA0
#define  PowerOffset_C_Addr	        0xA4
//------------------------------------
#define  Uart_DelayTime_Addr    	0xB0
#define  BGLIGHTDELAYTIME_Addr  	0xB4
//------------------------------------
#define  Flow1_Proportion_Addr		0xB8
#define  Flow2_Proportion_Addr		0xBC
#define  Rotate_Proportion_Addr		0xC0
#define  Torque_Proportion_Addr		0xC4
#define  Wheel_A_Addr				0xC8
#define  Wheel_D_Addr				0xCC
#define  Wheel_L_Addr				0xD0
#define  Torque_Threshold_Addr		0xD4
//----------------------------------------------------------------------
//----------------------------------------------------------------------
void delay_1us(void);
void delay_us(uint16 n);
void delay_1ms(void);
void delay_ms(uint16 data);
void PutDataToBuffer(uint8 s[], float fdata, uint8 num);
void PutHexToBuffer(uint8 s[],uint32 data);
void PutDecToBuffer(uint8 s[],uint16 data,uint8 StrWidth);//把一个十进制整数放入
	 						 	  		//到一个数组中,右对齐,左面补零
void PutHexByteToBuffer(uint8 s[],uint32 data);
/*void SetParaToDefault(void);*/
//----------------------------------------------------------------------
#endif