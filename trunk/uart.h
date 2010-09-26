#ifndef __UART_H
#define __UART_H

//------------------------------------------------------
#define  RXArrayLength    0x10
#define  reuploadpageStr  "reuploadpage"
#define  uploadpageStr    "uploadpage"
#define  setpagenumStr    "setpagenum"
#define  allStr  		  "all"
#define  nextStr  		  "next"
#define  priorStr  		  "prior"
//------------------------------------------------------
uint8 RX_Char(void);
void  TX_Char(uint8 c);
void  TX_ARRAY(uint8 ch[],uint16 num);
void  uart1_init(void);
void  uart1_rx_isr(void);
void  SendDataToComputer(uint32 StrartAddrCounter, uint32 EndAddrCounter);
uint8 SeparateString(uint8 ch[]);
void  SendPageToComputer(uint16 StartPageNum, uint16 EndPageNum);
void  CmdStringResponse(void);
//------------------------------------------------------
#endif