#ifndef __LCD_H
#define __LCD_H

//----------------------------------------------------------------------
//基本指令集
#define  lcd_Clear		   	   		0x01
#define  lcd_Areset	       	   		0x02
#define  point_set	       	   		0x04

#define  Cursor_right               0x02
#define  ShowMove_en                0x01
#define  lcd_ShowState              0x08
#define  Show_ON        	        0x04
#define  Cursor_ON		            0x02
#define  Cursor_Pos_ON              0x01

#define  fund_function     			0x30

#define  extend_function        	0x34
#define  Picture_ON		            0x02
#define	 set_CGRAM_address	 		0x40
#define	 set_DDRAM_address	 		0x80
#define	 write_to_RAM				0x200

//----------------------------------------------------------------------
//函数宏定义
#define  lcd_wrByte(x)			    lcd_write(write_to_RAM | (x))
#define  lcd_wrAddr(x)			    if((x)>=0x80 && (x)<=0x9F) lcd_write((x))
#define  lcd_wrCmd(x)			    lcd_write((x))
#define  lcd_bglight_enable()	    SET_REGBIT(LCD_PORT,lcd_bglight)
#define  lcd_bglight_disable()	    CLR_REGBIT(LCD_PORT,lcd_bglight)
#define  _ADDR(x,y)				    (AddrTable[(x)-1]+(y)-1)

//----------------------------------------------------------------------
//端口宏定义
#define	 LCD_PORT				    PORTE
#define	 LCD_DDR				    DDRE
#define	 LCD_PIN				    PINE
#define	 lcd_cs					    BIT_NUM0
#define	 lcd_std				    BIT_NUM1
#define	 lcd_sclk				    BIT_NUM2
#define	 lcd_bglight			    BIT_NUM3
#define	 lcd_reset				    BIT_NUM6
#define	 lcd_stdBIT				    0x01
//----------------------------------------------------------------------
void lcd_PortInit(void);
void lcd_init(void);
void lcd_write(uint32 data);
void reset_lcd(void);
void lcd_WriteCharArray(uint8 addr, uint8 chr[], uint8 num);
void lcd_wrSpace(uint8 addr,uint8 num);
void con_disp(uint8 data1, uint8 data2, uint8 data3, uint8 data4);
//----------------------------------------------------------------------
#endif