#ifndef __MEMORY_H
#define __MEMORY_H

//--------------------------------------------------------------
//--------------------------------------------------------------
#define  M_WREN   				            0x06
#define  M_WRDI   				            0x04
#define  M_RDID   				            0x9F
#define  M_RDSR   				            0x05
#define  M_WRSR   				            0x01
#define  M_READ   				            0x03
#define  M_PP   				            0x02
#define  M_SE   				            0xD8
#define  M_BE   				            0xC7
#define  M_DP   				            0xB9
#define  M_RES   				            0xAB
//--------------------------------------------------------------
#define  M_SR_SRWD 				            BIT_NUM7
#define  M_SR_WEL 				            BIT_NUM1
#define  M_SR_WIP 				            BIT_NUM0
//--------------------------------------------------------------
#define  DummyBytes				            0x00L
//--------------------------------------------------------------
#define	 MEMORY_PORT			            PORTA
#define	 MEMORY_DDR				            DDRA
#define	 MEMORY_PIN				            PINA
#define	 MEMORY_CS				            BIT_NUM0
#define	 MEMORY_IN				            BIT_NUM2
#define	 MEMORY_SCK				            BIT_NUM3
#define	 MEMORY_HOLD			            BIT_NUM5
//--------------------------------------------------------------
#define	 MEMORY_PORT1			            PORTC
#define	 MEMORY_DDR1			            DDRC
#define	 MEMORY_PIN1			            PINC
#define	 MEMORY_OUT				            BIT_NUM7
#define	 MEMORY_OUTBIT			            0x07
//--------------------------------------------------------------
#define	 MEMORY_PORT2			            PORTF
#define	 MEMORY_DDR2			            DDRF
#define	 MEMORY_PIN2			            PINF
#define	 MEMORY_W				            BIT_NUM5
//--------------------------------------------------------------
#define	 StartAddrCounterInEeprom           0x100
#define	 EndAddrCounterInEeprom   	        0x140
//--------------------------------------------------------------
#define  Memory_WriteEn()			        Memory_Write(M_WREN,NULL,0)
#define  Memory_WriteDisEn()		        Memory_Write(M_WRDI,NULL,0)
#define  Memory_WriteBE()			        Memory_Write(M_BE,NULL,0)
#define  Memory_WriteSE(addr)				Memory_Write(M_SE,addr,3)
#define  Memory_WriteSR(data)				Memory_Write(M_WRSR,data,1)
#define  Memory_ReadID(data,num)			Memory_Read(M_RDID,data,num)
#define  Memory_ReadSR(data,num)			Memory_Read(M_RDSR,data,num)
#define  Memory_WriteData(addr,data,num)	Memory_WriteEx(M_PP,addr,data,num)
#define  Memory_ReadData(addr,data,num)		Memory_ReadEx(M_READ,addr,data,num)
#define  Memory_ReadRES(data)				Memory_ReadEx(M_RES,DummyBytes,data,1)
#define  Memory_WaitWriteInProg()			do{Memory_ReadSR(&MemoryStatusREG,1);}while(MemoryStatusREG & M_SR_WIP)

//--------------------------------------------------------------
void Memory_PortInit(void);
void Memory_Write(uint8 cmd,uint8 data[],uint8 num);
void Memory_Read(uint8 cmd,uint8 data[],uint8 num);
void Memory_WriteEx(uint8 cmd, uint32 addr, uint8 data[], uint8 num);
void Memory_ReadEx(uint8 cmd, uint32 addr, uint8 data[], uint8 num);
void Init_MemoryAddressCounter(void);
void Save_MemoryAddressCounter(void);
void ClearAddrCounterInEeprom(void);
/*void PowerPareInit(void);*/
//--------------------------------------------------------------
#endif