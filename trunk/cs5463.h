#ifndef __CS5463_H
#define __CS5463_H
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//指令
#define	 start_SConvert		0xE0
#define	 start_CConvert		0xE8
#define	 SYNC0				0xFE
#define	 SYNC1				0xFF
#define	 SYNC3				0x00FFFFFF
#define	 power_UpHalt		0xA0
#define	 software_Reset		0x80
#define	 standby_State		(0x80 | (0x01<<3))
#define	 sleep_State		(0x80 | (0x02<<3))
#define	 Reg_Write			0x40
#define	 calibration		0xC0
//----------------------------------------------------------------------
//Mask/State Register Bit
#define	 CS_DRDY		BIT_NUM23
#define	 CS_CRDY		BIT_NUM20
#define	 CS_IOR			BIT_NUM17
#define	 CS_VOR			BIT_NUM16
#define	 CS_IROR		BIT_NUM14
#define	 CS_VROR		BIT_NUM13
#define	 CS_EOR			BIT_NUM12
#define	 CS_IFAULT		BIT_NUM11
#define	 CS_VSAG		BIT_NUM10
#define	 CS_TUP			BIT_NUM7
#define	 CS_TOD			BIT_NUM6
#define	 CS_VOD			BIT_NUM3
#define	 CS_LSD			BIT_NUM2
#define	 CS_FUP			BIT_NUM1
#define	 CS_IC			BIT_NUM0
//----------------------------------------------------------------------
//Configuration Register Bit
#define	 CS_IMODE		BIT_NUM12
#define	 CS_IINV		BIT_NUM11
#define	 CS_K3			BIT_NUM3
#define	 CS_K2			BIT_NUM2
#define	 CS_K1			BIT_NUM1
#define	 CS_K0			BIT_NUM0
//----------------------------------------------------------------------
//Control Register Bit
#define	 CS_STOP		BIT_NUM8
#define	 CS_INTOD		BIT_NUM4
#define	 CS_NOCPU		BIT_NUM2
#define	 CS_NOOSC		BIT_NUM1
//----------------------------------------------------------------------
//Mode Register Bit
#define	 CS_IHPF		BIT_NUM6
#define	 CS_VHPF		BIT_NUM5
//----------------------------------------------------------------------
//Register
#define  Config_Reg				(0x00)
#define  CurDCOff_Reg			(0x01<<1)
#define  CurGain_Reg			(0x02<<1)
#define  VolDCOff_Reg			(0x03<<1)
#define  VolGain_Reg			(0x04<<1)
#define  CycleCount_Reg			(0x05<<1)
#define  PulseRateE_Reg			(0x06<<1)
#define  InsCur_Reg				(0x07<<1)
#define  InsVol_Reg				(0x08<<1)
#define  InsP_Reg				(0x09<<1)
#define  ActiveP_Reg			(0x0A<<1)
#define  RMSCur_Reg				(0x0B<<1)
#define  RMSVol_Reg				(0x0C<<1)
#define  Epsilon_Reg			(0x0D<<1)
#define  POff_Reg				(0x0E<<1)
#define  Status_Reg				(0x0F<<1)
#define  CurACOff_Reg			(0x10<<1)
#define  VolACOff_Reg			(0x11<<1)
#define  Mode_Reg				(0x12<<1)
#define  Temperature_Reg		(0x13<<1)
#define  PowerFactor_Reg		(0x19<<1)
#define  Mask_Reg				(0x1A<<1)
#define  Ctrl_Reg				(0x1C<<1)
#define  Page_Reg				(0x1F<<1)
//the register of page3
#define  VSAG_Dur_Reg			0x06
#define  ISAG_Dur_Reg			0x0A
#define  VSAG_Level_Reg			0x07
#define  ISAG_Level_Reg			0x0B
//----------------------------------------------------------------------
//页代码
#define  Page0					0x00
#define  Page1					0x01
#define  Page3					0x03
//----------------------------------------------------------------------
//校正项
#define  Cal_CurDCOff			0x09
#define  Cal_CurDCGain			0x0A
#define  Cal_CurACOff			0x0D
#define  Cal_CurACGain			0x0E
#define  Cal_VolDCOff			0x11
#define  Cal_VolDCGain			0x12
#define  Cal_VolACOff			0x15
#define  Cal_VolACGain			0x16
#define  Cal_CurVolDCOff		0x19
#define  Cal_CurVolDCGain		0x1A
#define  Cal_CurVolACOff		0x1D
#define  Cal_CurVolACGain		0x1E
//----------------------------------------------------------------------
#define  CS5463_WriteReg(Reg,dat,CS)	    cs5463_write(Reg_Write | (Reg),(dat),CS)
#define  CS5463_WriteCmd(Cmd,CS)		    cs5463_write((Cmd),SYNC3,CS)
#define  CS5463_ReadReg(Reg,CS)			    cs5463_read((Reg),CS)
#define  ConvertUnsignedVal(reg) 			RegValue2Float(reg,FALSE)
#define  ConvertSignedVal(reg) 				RegValue2Float(reg,TRUE)
//----------------------------------------------------------------------						
#define	 CS5463_PORT	PORTC
#define	 CS5463_DDR		DDRC
#define	 CS5463_PIN		PINC

#define	 CS5463_CSA		BIT_NUM6
#define	 CS5463_CSB		BIT_NUM5
#define	 CS5463_CSC		BIT_NUM4
#define	 CS5463_CSABC	0x70
#define	 CS5463_SDI		BIT_NUM3
#define	 CS5463_RESET	BIT_NUM2
#define	 CS5463_SDO		BIT_NUM1
#define	 CS5463_SCLK	BIT_NUM0
#define	 CS5463_SDOBIT	0x01

#define	 CS5463_PORT1	PORTE
#define	 CS5463_DDR1	DDRE
#define	 CS5463_PIN1	PINE
#define	 CS5463_INT		BIT_NUM4

#define  PowerFactorGain	  -1.0
//#define  CurOffset_A  	  0.0120
//#define  CurOffset_B  	  0.0088
//#define  CurOffset_C 		  0.0111

//#define  VolOffset_A 		  0.0628
//#define  VolOffset_B  	  0.0614
//#define  VolOffset_C  	  0.0567

//#define  PowerOffset_A  	  0.00075
//#define  PowerOffset_B  	  0.00054
//#define  PowerOffset_C  	  0.00063
//----------------------------------------------------------------------
void cs5463_PortInit(void);
void cs5463_RegInit(void);
void cs5463_write(uint32 Cmd, uint32 data, uint8 CS5463_CS);
uint32 cs5463_read(uint8 Cmd, uint8 CS5463_CS);
float RegValue2Float(int32 data,bool tflag);
void InitPowerPara_FromEeprom(void);
void CS5463_HarwareReset(void);
//----------------------------------------------------------------------
#endif