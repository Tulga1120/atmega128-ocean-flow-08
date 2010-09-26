#include "config.h"

//-----------------------------------------------------
const uint8 AddrTable[4]={0x80,0x90,0x88,0x98};
uint16 BgLightDelayTime = 0x00;
//-----------------------------------------------------

void lcd_PortInit(void)
{
	SET_REGBIT(LCD_DDR, lcd_cs | lcd_std | lcd_sclk | lcd_reset | lcd_bglight);
    CLR_REGBIT(LCD_PORT,lcd_cs | lcd_std | lcd_sclk);
	SET_REGBIT(LCD_PORT,lcd_reset);
}

void lcd_init(void)
{
	lcd_wrCmd(fund_function );
    lcd_wrCmd(lcd_Clear );
    lcd_wrCmd(point_set | Cursor_right );
    lcd_wrCmd(lcd_ShowState | Show_ON );
	lcd_ReverseVideo(LCD_ClearGDRAM);
    lcd_bglight_enable();
}

void lcd_write(uint32 data1)
{
    uint8 i;
    uint32 data2=0x00F80000;

	data2 |= (data1 & 0x0200) << 8;//Note:8=17-9,0x0200==BIT(9)
    data2 |= (data1 & 0x0100) << 10;//Note:10=18-8,0x0100==BIT(8)
    data2 |= (data1 & 0xF0) << 8;
    data2 |= (data1 & 0x0F) << 4;
    
    SET_REGBIT(LCD_PORT,lcd_cs); 
    for(i=0;i<24;i++)
    {
        if(data2 & 0x00800000)
        {
            SET_REGBIT(LCD_PORT,lcd_std);
        }
        else
        {
            CLR_REGBIT(LCD_PORT,lcd_std);
        }
        SET_REGBIT(LCD_PORT,lcd_sclk);
        data2 <<= 1;
        CLR_REGBIT(LCD_PORT,lcd_sclk);
    }
    CLR_REGBIT(LCD_PORT,lcd_cs);
}

void reset_lcd(void)
{
    CLR_REGBIT(LCD_PORT,lcd_reset);
    delay_1us();
    SET_REGBIT(LCD_PORT,lcd_reset);
}

void lcd_WriteCharArray(uint8 addr,uint8 chr[],uint8 num)
{
    uint8 i;
    
    lcd_wrAddr( addr );
    for(i=0;i<num;i++)
    {
        lcd_wrByte( chr[i] );
    }
}

void lcd_wrSpace(uint8 addr,uint8 num)
{
    uint8 i;
    
    lcd_wrAddr( addr );
    for(i=0;i<num;i++)
    {
        lcd_wrByte( 0x20 );
    }
}

void con_disp (uint8 data1, uint8 data2,	uint8 data3, uint8 data4)
{
   uint8 i,j;
   for(j=0x80;j<0x90;j++)
   {
       for(i=0x80;i<0x88;i++)
       {
           lcd_wrCmd (extend_function);//扩充指令集
           lcd_wrCmd (j);
           lcd_wrCmd (i);
           lcd_wrCmd (fund_function);//基本指令集
           lcd_wrByte (data1);
           lcd_wrByte (data1);
       }
   }
   for(j=0x90;j<0xA0;j++)
   {
       for(i=0x80;i<0x88;i++)
       {
           lcd_wrCmd (extend_function);//扩充指令集
           lcd_wrCmd (j);
           lcd_wrCmd (i);
           lcd_wrCmd (fund_function);//基本指令集
           lcd_wrByte (data2);
           lcd_wrByte (data2);
       }
   }
   for(j=0x80;j<0x90;j++)
   {
       for(i=0x88;i<0x90;i++)
       {
           lcd_wrCmd (extend_function);//扩充指令集
           lcd_wrCmd (j);
           lcd_wrCmd (i);
           lcd_wrCmd (fund_function);//基本指令集
           lcd_wrByte (data3);
           lcd_wrByte (data3);
       }
   }
   for(j=0x90;j<0xA0;j++)
   {
       for(i=0x88;i<0x90;i++)
       {
           lcd_wrCmd (extend_function);//扩充指令集
           lcd_wrCmd (j);
           lcd_wrCmd (i);
           lcd_wrCmd (fund_function);//基本指令集
           lcd_wrByte (data4);
           lcd_wrByte (data4);
       }
   }
   lcd_wrCmd (extend_function | Picture_ON);
   lcd_wrCmd (fund_function);
}

