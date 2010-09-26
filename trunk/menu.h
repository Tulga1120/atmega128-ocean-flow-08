#ifndef __MENU_H
#define __MENU_H
#include "config.h"
//----------------------------------------------------------------------
#define  lcd_width						0x10
#define  lcd_height						0x04
#define  menu_up						-1
#define  menu_down						1
#define  menu_refresh					0
//----------------------------------------------------------------------
//�궨��
#define	 LCD_ClearGDRAM					0
#define	 LCD_Row1						1
#define	 LCD_Row2						2
#define	 LCD_Row3						3
#define	 LCD_Row4						4
//----------------------------------------------------------------------
//#define  _T(x)						((fabs(x)>0.02)?(x):fabs(x))
#define  _T(x)							((x>=0)?(x):0)
#define  SPACE16						"\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20"
#define  JUDGESTRNULL(s)				((s=="")?(SPACE16):(s))
#define  LOADMENU(x,y)					LoadMenu(x,1,y)
#define  Clear_ScreenTAB_Row(num)		memset(ScreenTAB.ScreenTable[num],' ',lcd_width);
#define  MenuStrTab						uint8 str0[],\
                                		uint8 str1[],\
                                		uint8 str2[],\
                                		uint8 str3[]
//=====================================================================
//----------------------------------------------------------------------
//�˵�����
//----------------------------------------------------------------------
//��ӭ��Ļ,��Ӧ�˵�״̬����:00-00-XX-XX
#define  Menu_WelcomeSCR_num	    0x04
#define  Menu_WelcomeSCR	 	    "    ��ӭʹ��    ",\
        		 				    "����������װ��",\
        						    "                ",\
        						    "  �й������ѧ  "
//----------------------------------------------------------------------
//���������(1),��Ӧ�˵�״̬����:00-01-XX-XX
#define  Menu_VolCur_num		    0x04
#define  Menu_VolCur		 	    "  �����ѹ����  ",\
		 						    "Va:     Ia:     ",\
        		 				    "Vb:     Ib:     ",\
        						    "Vc:     Ic:     "
//----------------------------------------------------------------------
//���������(2),��Ӧ�˵�״̬����:00-02-XX-XX
#define  Menu_P_PF_num			    0x04
#define  Menu_P_PF		 		    "���ʺ͹�������  ",\
		 						    "Pa:     PFa:    ",\
        		 				    "Pb:     PFb:    ",\
        						    "Pc:     PFc:    "
//----------------------------------------------------------------------
//���ټƲ���,��Ӧ�˵�״̬����:00-03-XX-XX
#define  Menu_FlowSpeed_num		    0x04
#define  Menu_FlowSpeed	 	  	    "  ���ټƲ���    ",\
        		 				    "Vf1:            ",\
        						    "Vf2:            ",\
        						    "Vf3:            "
//----------------------------------------------------------------------
//Ť�غ�ת��,��Ӧ�˵�״̬����:00-04-XX-XX
#define  Menu_Torque_n_num		    0x04
#define  Menu_Torque_n	 	  	    "  Ť�غ�ת��    ",\
        		 				    "T :             ",\
        						    "n :             ",\
        						    "PT:             "
//----------------------------------------------------------------------
//�ۺϲ���,��Ӧ�˵�״̬����:00-05-XX-XX
#define  Menu_integration_num	    0x04
#define  Menu_integration	 	    "    �ۺϲ���    ",\
        		 				    "P :     Vf:     ",\
        						    "PF:     Cp:     ",\
        						    "PT:             "
//----------------------------------------------------------------------
//ʱ�����ص�ѹ,��Ӧ�˵�״̬����:00-06-XX-XX
#define  Menu_Time_Bat_num		    0x04
#define  Menu_Time_Bat	 	  	    "  ʱ�����ص�ѹ",\
        		 				    "Date:    -  -   ",\
        						    "Time:   :  :    ",\
								    "Vbat:     V     "
//----------------------------------------------------------------------
//���˵�,��Ӧ�˵�״̬����:00-07-XX-XX
#define  MainMenu_num			    0x04
#define  MainMenu	 	  		    "1.��ʾ          ",\
		 						    "2.����          ",\
								    "3.���ܲ���      ",\
								    "4.��������      "
//----------------------------------------------------------------------
//��ʾ�˵�1,��Ӧ�˵�״̬����:01-00-XX-XX
#define  ShowMenuList1_num		    0x04
#define  ShowMenuList1 	  		    "1.�ۺϲ���      ",\
		 						    "2.�����ѹ����  ",\
								    "3.���ʺ͹�������",\
								    "4.���ټƲ���    "
//----------------------------------------------------------------------
//��ʾ�˵�2,��Ӧ�˵�״̬����:01-01-XX-XX
#define  ShowMenuList2_num		    0x02
#define  ShowMenuList2 	  		    "5.Ť�غ�ת��    ",\
		 						    "6.ʱ�����ص�ѹ",\
								    "                ",\
								    "                "
//----------------------------------------------------------------------
//���ò˵�,��Ӧ�˵�״̬����:02-00-XX-XX
#define  FunctionSet_num		    0x03
#define  FunctionSet			    "1.�رձ���      ",\
		 						    "2.�������ݴ洢  ",\
								    "3.�����������  ",\
								    "                "						
#define  FunctionSet_Row1		    "2.��ͣ���ݴ洢  "	
#define  FunctionSet_Row2		    "3.��������      "						
//----------------------------------------------------------------------
//�����˵�,��Ӧ�˵�״̬����:03-00-XX-XX
#define  FunctionOp_num			    0x04
#define  FunctionOp				    "1.��ʾ���ô洢  ",\
		 						    "2.�ϴ�����      ",\
								    "3.����ȫ������  ",\
								    "4.����洢��ַ  "			
//----------------------------------------------------------------------
//�������ò˵�1,��Ӧ�˵�״̬����:04-01-XX-XX
#define  ParameterConfig1_num	    0x04
#define  ParameterConfig1		    "1.��������    ",\
		 						    "2.���ټƲ���    ",\
								    "3.Ť��ת�ٲ���  ",\
								    "4.����ʱ���趨  "	
//----------------------------------------------------------------------
//�������ò˵�2,��Ӧ�˵�״̬����:04-02-XX-XX
#define  ParameterConfig2_num	    0x03
#define  ParameterConfig2		    "5.����ʱ���趨  ",\
		 						    "6.������ʱ�趨  ",\
								    "7.��е�����趨  ",\
								    "                "	
//----------------------------------------------------------------------
//�����������ò˵�1,��Ӧ�˵�״̬����:04-00-01-XX
#define  ParaCfg_PowerPara1_num	    0x04
#define  ParaCfg_PowerPara1		    "1.��ѹ����      ",\
		 						    "2.��������      ",\
								    "3.��������      ",\
								    "4.��ѹƫ��      "	
//----------------------------------------------------------------------
//�����������ò˵�2,��Ӧ�˵�״̬����:04-00-02-XX
#define  ParaCfg_PowerPara2_num	    0x02
#define  ParaCfg_PowerPara2		    "5.����ƫ��      ",\
		 						    "6.����ƫ��      ",\
								    "                ",\
								    "                "	
//----------------------------------------------------------------------
//���ټƲ������ò˵�,��Ӧ�˵�״̬����:04-00-03-XX
#define  ParaCfg_Flow_num		    0x04
#define  ParaCfg_Flow			    "1.����01����    ",\
		 						    "2.����01ƫ��    ",\
								    "3.����02����    ",\
								    "4.����02ƫ��    "	
//----------------------------------------------------------------------
//Ť��ת�ٴ������������ò˵�,��Ӧ�˵�״̬����:04-00-04-XX
#define  ParaCfg_TorRot_num	0x04
#define  ParaCfg_TorRot			    "1.Ť������      ",\
		 						    "2.Ť��ƫ��      ",\
								    "3.ת������      ",\
								    "4.ת��ƫ��      "	
//----------------------------------------------------------------------
//ʱ�������趨�˵�,��Ӧ�˵�״̬����:04-00-05-XX
#define  ParaCfg_DateTime_num	    0x02
#define  ParaCfg_DateTime		    "1.�����趨      ",\
		 						    "2.ʱ���趨      ",\
								    "                ",\
								    "                "	
//----------------------------------------------------------------------
//��е����1�趨�˵�,��Ӧ�˵�״̬����:04-00-06-XX
#define  ParaCfg_Mechanical1_num	0x04
#define  ParaCfg_Mechanical1		"1.����01����ϵ��",\
		 							"2.����02����ϵ��",\
									"3.ת�ٱ���ϵ��  ",\
									"4.Ť�ر���ϵ��  "								
//----------------------------------------------------------------------
//��е����2�趨�˵�,��Ӧ�˵�״̬����:04-00-07-XX
#define  ParaCfg_Mechanical2_num	0x04
#define  ParaCfg_Mechanical2		"5.���ٱ���ϵ��  ",\
		 							"6.ˮ�ֻ�ֱ��    ",\
									"7.ˮ�ֻ�����    ",\
									"8.Ť����ʾ����  "	
//----------------------------------------------------------------------
//�����˵�,��Ӧ�˵�״̬����:04-00-00-01
#define  ParaCfg_PwPara_VolG_num	0x03
#define  ParaCfg_PwPara_VolG		"1.��ѹ����A     ",\
		 							"2.��ѹ����B     ",\
									"3.��ѹ����C     ",\
									"                "	
//----------------------------------------------------------------------
//�����˵�,��Ӧ�˵�״̬����:04-00-00-02
#define  ParaCfg_PwPara_CurG_num	0x03
#define  ParaCfg_PwPara_CurG		"1.��������A     ",\
		 							"2.��������B     ",\
									"3.��������C     ",\
									"                "	
//----------------------------------------------------------------------
//�����˵�,��Ӧ�˵�״̬����:04-00-00-03
#define  ParaCfg_PwPara_PwG_num		0x03
#define  ParaCfg_PwPara_PwG			"1.��������A     ",\
		 							"2.��������B     ",\
									"3.��������C     ",\
									"                "	
//----------------------------------------------------------------------
//�����˵�,��Ӧ�˵�״̬����:04-00-00-04
#define  ParaCfg_PwPara_VolO_num	0x03
#define  ParaCfg_PwPara_VolO		"1.��ѹƫ��A     ",\
		 							"2.��ѹƫ��B     ",\
									"3.��ѹƫ��C     ",\
									"                "	
//----------------------------------------------------------------------
//�����˵�,��Ӧ�˵�״̬����:04-00-00-05
#define  ParaCfg_PwPara_CurO_num	0x03
#define  ParaCfg_PwPara_CurO		"1.����ƫ��A     ",\
		 							"2.����ƫ��B     ",\
									"3.����ƫ��C     ",\
									"                "	
//----------------------------------------------------------------------
//�����˵�,��Ӧ�˵�״̬����:04-00-00-06
#define  ParaCfg_PwPara_PwO_num		0x03
#define  ParaCfg_PwPara_PwO			"1.����ƫ��A     ",\
		 							"2.����ƫ��B     ",\
									"3.����ƫ��C     ",\
									"                "	
//----------------------------------------------------------------------
//�����趨�˵�"normal"
#define  KeySetMenu_num				0x01
#define  KeySetMenu					"                ",\
									"��ǰֵ:         ",\
									"�޸�Ϊ:         ",\
									"                "	
//----------------------------------------------------------------------
//�����趨�˵�"datetime"
#define  KeySetTimeMenu_num			0x01
#define  KeySetTimeMenu				"                ",\
									"��ǰ:           ",\
									"�趨:           ",\
									"                "		
//----------------------------------------------------------------------					
#define  UartDelayTimeMenuStr		"�޸��ӳ�ʱ��(ms)"
#define  BgLightTimeMenuStr			"�޸ı���ʱ��(s) "
//----------------------------------------------------------------------
#define  Flow1_ProportionStr		"����01����ϵ��  "
#define  Flow2_ProportionStr		"����02����ϵ��  "
#define  Rotate_ProportionStr		"ת�ٱ���ϵ��    "
#define  Torque_ProportionStr		"Ť�ر���ϵ��    "
#define  Wheel_AStr					"���ٱ���ϵ��    "
#define  Wheel_DStr					"ˮ�ֻ�ֱ��      "
#define  Wheel_LStr				    "ˮ�ֻ�����      "
#define  Torque_ThresholdStr		"Ť����ʾ����    "
//----------------------------------------------------------------------
#define  ParaCfg_Flow1GStr			"����01����      "
#define  ParaCfg_Flow2GStr			"����02����      "
#define  ParaCfg_Flow1OStr			"����01ƫ��      "
#define  ParaCfg_Flow2OStr			"����02ƫ��      "
#define  ParaCfg_TorGStr			"Ť������        "
#define  ParaCfg_RotGStr			"ת������        "
#define  ParaCfg_TorOStr			"Ť��ƫ��        "
#define  ParaCfg_RotOStr			"ת��ƫ��        "
#define  ParaCfg_DateStr			"�����趨        "
#define  ParaCfg_TimeStr			"ʱ���趨        "
#define  ParaCfg_VolGAStr			"��ѹ����A       "
#define  ParaCfg_VolGBStr			"��ѹ����B       "
#define  ParaCfg_VolGCStr			"��ѹ����C       "
#define  ParaCfg_CurGAStr			"��������A       "
#define  ParaCfg_CurGBStr			"��������B       "
#define  ParaCfg_CurGCStr			"��������C       "
#define  ParaCfg_PwGAStr			"��������A       "
#define  ParaCfg_PwGBStr			"��������B       "
#define  ParaCfg_PwGCStr			"��������C       "
#define  ParaCfg_VolOAStr			"��ѹƫ��A       "
#define  ParaCfg_VolOBStr			"��ѹƫ��B       "
#define  ParaCfg_VolOCStr			"��ѹƫ��C       "
#define  ParaCfg_CurOAStr			"����ƫ��A       "
#define  ParaCfg_CurOBStr			"����ƫ��B       "
#define  ParaCfg_CurOCStr			"����ƫ��C       "
#define  ParaCfg_PwOAStr			"����ƫ��A       "
#define  ParaCfg_PwOBStr			"����ƫ��B       "
#define  ParaCfg_PwOCStr			"����ƫ��C       "
//----------------------------------------------------------------------
//����ȷ�϶Ի���,��Ӧ�˵�״̬����:09-00-XX-XX
#define  EraseEnsure_num		    0x01
#define  EraseEnsure			    "  ���ݲ�����  ",\
								    "  ���ɻָ�, ��  ",\
								    "  ȷ��Ҫ������? ",\
								    "                "	
//----------------------------------------------------------------------
//�洢�ռ�ʹ��״̬,��Ӧ�˵�״̬����:09-01-XX-XX
#define  MemoryState_num		    0x01
#define  MemoryState			    "��ǰ��ַ����ֵ��",\
								    "                ",\
								    "ռ�ܴ洢�ռ�ȣ�",\
								    "                "	
//----------------------------------------------------------------------
//���ݴ���˵�,��Ӧ�˵�״̬����:09-05-XX-XX
#define  UploadPageState_num	    0x04
#define  UploadPageState		    "��ǰҳΪ:       ",\
								    "��ʼҳΪ:       ",\
								    "����ҳΪ:       ",\
								    "�����ַ�:       "
								
#define  ReuploadPageStr            "Reupld"
#define  UploadPageStr              "Upload"
#define  SetPageNumStr              "SetPg "	
#define  NoOperationStr             " NULL "		
//----------------------------------------------------------------------
//��ַ����˵�,��Ӧ�˵�״̬����:09-06-XX-XX
#define  SaveAddress_num	        0x02
#define  SaveAddress		        "��ַ�ѱ���,     ",\
								    "�����ַΪ:     ",\
								    "                ",\
								    "                "					
//----------------------------------------------------------------------
//��ת���洢״̬�Ի���,��Ӧ�˵�״̬����:09-07-XX-XX
#define  JumpToMemoryState_num	    0x01
#define  JumpToMemoryState		    "  �洢�Ѿ�����, ",\
		 						    "  Ҫ��ת���洢  ",\
								    "  ״̬�Ի�����? ",\
								    "                "
								
//----------------------------------------------------------------------
//Strings��״̬����
#define  DataIsUploading		    "  ���������ϴ�  "
#define  DataIsErasing			    "  �������ڲ���  "
#define  DataEraseIsLocked		    "  �����ѱ�����  "
//----------------------------------------------------------------------
struct SCRTAB
{
    uint8 ScreenTable[lcd_height][lcd_width]; //�������
	uint8 CurNum;      //��ǰ��¼
	uint8 TotalNum;    //�ܼ�¼����
};
struct MenuStateStruct
{
    uint8 MG1;		  //menu grade 1
    uint8 MG2;		  //menu grade 2
    uint8 MG3;		  //menu grade 3
    uint8 MG4;		  //menu grade 4
};
//----------------------------------------------------------------------

void ShowMenuBuffer(void);
void ScrollMenu(uint8 dirs);
void LoadMenu(MenuStrTab, uint8 curnum, uint8 totalnum);
void LoadMenuRowStr(uint8 MenuRowStr[],uint8 RowNum);
void ClearScreenTab(uint8 s);
void Set_MenuState(uint8 x1, uint8 x2, uint8 x3,uint8 x4);
void Set_OldMenuState(uint8 x1, uint8 x2, uint8 x3, uint8 x4);
void ShowMenu(void);  
void lcd_ReverseVideo(int8 row);
void SaveCurMenuState(void); 
void LoadOldMenuState(void);
uint8 CompareMenuOld(void);
uint8 CompareMenuState(uint8 s1,	 uint8 s2,uint8 s3,	uint8 s4, MenuGrade MenuG);
uint8 CompareOldMenuState(uint8 s1, uint8 s2, uint8 s3, uint8 s4, MenuGrade MenuG);
//----------------------------------------------------------------------
#endif
