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
//宏定义
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
//菜单定义
//----------------------------------------------------------------------
//欢迎屏幕,对应菜单状态变量:00-00-XX-XX
#define  Menu_WelcomeSCR_num	    0x04
#define  Menu_WelcomeSCR	 	    "    欢迎使用    ",\
        		 				    "潮流发电测控装置",\
        						    "                ",\
        						    "  中国海洋大学  "
//----------------------------------------------------------------------
//发电机参数(1),对应菜单状态变量:00-01-XX-XX
#define  Menu_VolCur_num		    0x04
#define  Menu_VolCur		 	    "  三相电压电流  ",\
		 						    "Va:     Ia:     ",\
        		 				    "Vb:     Ib:     ",\
        						    "Vc:     Ic:     "
//----------------------------------------------------------------------
//发电机参数(2),对应菜单状态变量:00-02-XX-XX
#define  Menu_P_PF_num			    0x04
#define  Menu_P_PF		 		    "功率和功率因数  ",\
		 						    "Pa:     PFa:    ",\
        		 				    "Pb:     PFb:    ",\
        						    "Pc:     PFc:    "
//----------------------------------------------------------------------
//流速计参数,对应菜单状态变量:00-03-XX-XX
#define  Menu_FlowSpeed_num		    0x04
#define  Menu_FlowSpeed	 	  	    "  流速计参数    ",\
        		 				    "Vf1:            ",\
        						    "Vf2:            ",\
        						    "Vf3:            "
//----------------------------------------------------------------------
//扭矩和转速,对应菜单状态变量:00-04-XX-XX
#define  Menu_Torque_n_num		    0x04
#define  Menu_Torque_n	 	  	    "  扭矩和转速    ",\
        		 				    "T :             ",\
        						    "n :             ",\
        						    "PT:             "
//----------------------------------------------------------------------
//综合参数,对应菜单状态变量:00-05-XX-XX
#define  Menu_integration_num	    0x04
#define  Menu_integration	 	    "    综合参数    ",\
        		 				    "P :     Vf:     ",\
        						    "PF:     Cp:     ",\
        						    "PT:             "
//----------------------------------------------------------------------
//时间与电池电压,对应菜单状态变量:00-06-XX-XX
#define  Menu_Time_Bat_num		    0x04
#define  Menu_Time_Bat	 	  	    "  时间与电池电压",\
        		 				    "Date:    -  -   ",\
        						    "Time:   :  :    ",\
								    "Vbat:     V     "
//----------------------------------------------------------------------
//主菜单,对应菜单状态变量:00-07-XX-XX
#define  MainMenu_num			    0x04
#define  MainMenu	 	  		    "1.显示          ",\
		 						    "2.设置          ",\
								    "3.功能操作      ",\
								    "4.参数配置      "
//----------------------------------------------------------------------
//显示菜单1,对应菜单状态变量:01-00-XX-XX
#define  ShowMenuList1_num		    0x04
#define  ShowMenuList1 	  		    "1.综合参数      ",\
		 						    "2.三相电压电流  ",\
								    "3.功率和功率因数",\
								    "4.流速计参数    "
//----------------------------------------------------------------------
//显示菜单2,对应菜单状态变量:01-01-XX-XX
#define  ShowMenuList2_num		    0x02
#define  ShowMenuList2 	  		    "5.扭矩和转速    ",\
		 						    "6.时间与电池电压",\
								    "                ",\
								    "                "
//----------------------------------------------------------------------
//设置菜单,对应菜单状态变量:02-00-XX-XX
#define  FunctionSet_num		    0x03
#define  FunctionSet			    "1.关闭背光      ",\
		 						    "2.启动数据存储  ",\
								    "3.解除擦除锁定  ",\
								    "                "						
#define  FunctionSet_Row1		    "2.暂停数据存储  "	
#define  FunctionSet_Row2		    "3.锁定擦除      "						
//----------------------------------------------------------------------
//操作菜单,对应菜单状态变量:03-00-XX-XX
#define  FunctionOp_num			    0x04
#define  FunctionOp				    "1.显示已用存储  ",\
		 						    "2.上传数据      ",\
								    "3.擦除全部数据  ",\
								    "4.保存存储地址  "			
//----------------------------------------------------------------------
//参数配置菜单1,对应菜单状态变量:04-01-XX-XX
#define  ParameterConfig1_num	    0x04
#define  ParameterConfig1		    "1.三相电参数    ",\
		 						    "2.流速计参数    ",\
								    "3.扭矩转速参数  ",\
								    "4.日期时间设定  "	
//----------------------------------------------------------------------
//参数配置菜单2,对应菜单状态变量:04-02-XX-XX
#define  ParameterConfig2_num	    0x03
#define  ParameterConfig2		    "5.背光时间设定  ",\
		 						    "6.传输延时设定  ",\
								    "7.机械参数设定  ",\
								    "                "	
//----------------------------------------------------------------------
//三相电参数配置菜单1,对应菜单状态变量:04-00-01-XX
#define  ParaCfg_PowerPara1_num	    0x04
#define  ParaCfg_PowerPara1		    "1.电压增益      ",\
		 						    "2.电流增益      ",\
								    "3.功率增益      ",\
								    "4.电压偏置      "	
//----------------------------------------------------------------------
//三相电参数配置菜单2,对应菜单状态变量:04-00-02-XX
#define  ParaCfg_PowerPara2_num	    0x02
#define  ParaCfg_PowerPara2		    "5.电流偏置      ",\
		 						    "6.功率偏置      ",\
								    "                ",\
								    "                "	
//----------------------------------------------------------------------
//流速计参数配置菜单,对应菜单状态变量:04-00-03-XX
#define  ParaCfg_Flow_num		    0x04
#define  ParaCfg_Flow			    "1.流速01增益    ",\
		 						    "2.流速01偏置    ",\
								    "3.流速02增益    ",\
								    "4.流速02偏置    "	
//----------------------------------------------------------------------
//扭矩转速传感器参数配置菜单,对应菜单状态变量:04-00-04-XX
#define  ParaCfg_TorRot_num	0x04
#define  ParaCfg_TorRot			    "1.扭矩增益      ",\
		 						    "2.扭矩偏置      ",\
								    "3.转速增益      ",\
								    "4.转速偏置      "	
//----------------------------------------------------------------------
//时间日期设定菜单,对应菜单状态变量:04-00-05-XX
#define  ParaCfg_DateTime_num	    0x02
#define  ParaCfg_DateTime		    "1.日期设定      ",\
		 						    "2.时间设定      ",\
								    "                ",\
								    "                "	
//----------------------------------------------------------------------
//机械参数1设定菜单,对应菜单状态变量:04-00-06-XX
#define  ParaCfg_Mechanical1_num	0x04
#define  ParaCfg_Mechanical1		"1.流速01比例系数",\
		 							"2.流速02比例系数",\
									"3.转速比例系数  ",\
									"4.扭矩比例系数  "								
//----------------------------------------------------------------------
//机械参数2设定菜单,对应菜单状态变量:04-00-07-XX
#define  ParaCfg_Mechanical2_num	0x04
#define  ParaCfg_Mechanical2		"5.增速比例系数  ",\
		 							"6.水轮机直径    ",\
									"7.水轮机长度    ",\
									"8.扭矩显示门限  "	
//----------------------------------------------------------------------
//操作菜单,对应菜单状态变量:04-00-00-01
#define  ParaCfg_PwPara_VolG_num	0x03
#define  ParaCfg_PwPara_VolG		"1.电压增益A     ",\
		 							"2.电压增益B     ",\
									"3.电压增益C     ",\
									"                "	
//----------------------------------------------------------------------
//操作菜单,对应菜单状态变量:04-00-00-02
#define  ParaCfg_PwPara_CurG_num	0x03
#define  ParaCfg_PwPara_CurG		"1.电流增益A     ",\
		 							"2.电流增益B     ",\
									"3.电流增益C     ",\
									"                "	
//----------------------------------------------------------------------
//操作菜单,对应菜单状态变量:04-00-00-03
#define  ParaCfg_PwPara_PwG_num		0x03
#define  ParaCfg_PwPara_PwG			"1.功率增益A     ",\
		 							"2.功率增益B     ",\
									"3.功率增益C     ",\
									"                "	
//----------------------------------------------------------------------
//操作菜单,对应菜单状态变量:04-00-00-04
#define  ParaCfg_PwPara_VolO_num	0x03
#define  ParaCfg_PwPara_VolO		"1.电压偏置A     ",\
		 							"2.电压偏置B     ",\
									"3.电压偏置C     ",\
									"                "	
//----------------------------------------------------------------------
//操作菜单,对应菜单状态变量:04-00-00-05
#define  ParaCfg_PwPara_CurO_num	0x03
#define  ParaCfg_PwPara_CurO		"1.电流偏置A     ",\
		 							"2.电流偏置B     ",\
									"3.电流偏置C     ",\
									"                "	
//----------------------------------------------------------------------
//操作菜单,对应菜单状态变量:04-00-00-06
#define  ParaCfg_PwPara_PwO_num		0x03
#define  ParaCfg_PwPara_PwO			"1.功率偏置A     ",\
		 							"2.功率偏置B     ",\
									"3.功率偏置C     ",\
									"                "	
//----------------------------------------------------------------------
//按键设定菜单"normal"
#define  KeySetMenu_num				0x01
#define  KeySetMenu					"                ",\
									"当前值:         ",\
									"修改为:         ",\
									"                "	
//----------------------------------------------------------------------
//按键设定菜单"datetime"
#define  KeySetTimeMenu_num			0x01
#define  KeySetTimeMenu				"                ",\
									"当前:           ",\
									"设定:           ",\
									"                "		
//----------------------------------------------------------------------					
#define  UartDelayTimeMenuStr		"修改延迟时间(ms)"
#define  BgLightTimeMenuStr			"修改背光时间(s) "
//----------------------------------------------------------------------
#define  Flow1_ProportionStr		"流速01比例系数  "
#define  Flow2_ProportionStr		"流速02比例系数  "
#define  Rotate_ProportionStr		"转速比例系数    "
#define  Torque_ProportionStr		"扭矩比例系数    "
#define  Wheel_AStr					"增速比例系数    "
#define  Wheel_DStr					"水轮机直径      "
#define  Wheel_LStr				    "水轮机长度      "
#define  Torque_ThresholdStr		"扭矩显示门限    "
//----------------------------------------------------------------------
#define  ParaCfg_Flow1GStr			"流速01增益      "
#define  ParaCfg_Flow2GStr			"流速02增益      "
#define  ParaCfg_Flow1OStr			"流速01偏置      "
#define  ParaCfg_Flow2OStr			"流速02偏置      "
#define  ParaCfg_TorGStr			"扭矩增益        "
#define  ParaCfg_RotGStr			"转速增益        "
#define  ParaCfg_TorOStr			"扭矩偏置        "
#define  ParaCfg_RotOStr			"转速偏置        "
#define  ParaCfg_DateStr			"日期设定        "
#define  ParaCfg_TimeStr			"时间设定        "
#define  ParaCfg_VolGAStr			"电压增益A       "
#define  ParaCfg_VolGBStr			"电压增益B       "
#define  ParaCfg_VolGCStr			"电压增益C       "
#define  ParaCfg_CurGAStr			"电流增益A       "
#define  ParaCfg_CurGBStr			"电流增益B       "
#define  ParaCfg_CurGCStr			"电流增益C       "
#define  ParaCfg_PwGAStr			"功率增益A       "
#define  ParaCfg_PwGBStr			"功率增益B       "
#define  ParaCfg_PwGCStr			"功率增益C       "
#define  ParaCfg_VolOAStr			"电压偏置A       "
#define  ParaCfg_VolOBStr			"电压偏置B       "
#define  ParaCfg_VolOCStr			"电压偏置C       "
#define  ParaCfg_CurOAStr			"电流偏置A       "
#define  ParaCfg_CurOBStr			"电流偏置B       "
#define  ParaCfg_CurOCStr			"电流偏置C       "
#define  ParaCfg_PwOAStr			"功率偏置A       "
#define  ParaCfg_PwOBStr			"功率偏置B       "
#define  ParaCfg_PwOCStr			"功率偏置C       "
//----------------------------------------------------------------------
//擦除确认对话框,对应菜单状态变量:09-00-XX-XX
#define  EraseEnsure_num		    0x01
#define  EraseEnsure			    "  数据擦除后不  ",\
								    "  不可恢复, 您  ",\
								    "  确定要擦除吗? ",\
								    "                "	
//----------------------------------------------------------------------
//存储空间使用状态,对应菜单状态变量:09-01-XX-XX
#define  MemoryState_num		    0x01
#define  MemoryState			    "当前地址计数值：",\
								    "                ",\
								    "占总存储空间比：",\
								    "                "	
//----------------------------------------------------------------------
//数据传输菜单,对应菜单状态变量:09-05-XX-XX
#define  UploadPageState_num	    0x04
#define  UploadPageState		    "当前页为:       ",\
								    "起始页为:       ",\
								    "结束页为:       ",\
								    "命令字符:       "
								
#define  ReuploadPageStr            "Reupld"
#define  UploadPageStr              "Upload"
#define  SetPageNumStr              "SetPg "	
#define  NoOperationStr             " NULL "		
//----------------------------------------------------------------------
//地址保存菜单,对应菜单状态变量:09-06-XX-XX
#define  SaveAddress_num	        0x02
#define  SaveAddress		        "地址已保存,     ",\
								    "保存地址为:     ",\
								    "                ",\
								    "                "					
//----------------------------------------------------------------------
//跳转到存储状态对话框,对应菜单状态变量:09-07-XX-XX
#define  JumpToMemoryState_num	    0x01
#define  JumpToMemoryState		    "  存储已经启动, ",\
		 						    "  要跳转到存储  ",\
								    "  状态对话框吗? ",\
								    "                "
								
//----------------------------------------------------------------------
//Strings无状态变量
#define  DataIsUploading		    "  数据正在上传  "
#define  DataIsErasing			    "  数据正在擦除  "
#define  DataEraseIsLocked		    "  擦除已被锁定  "
//----------------------------------------------------------------------
struct SCRTAB
{
    uint8 ScreenTable[lcd_height][lcd_width]; //存放数据
	uint8 CurNum;      //当前记录
	uint8 TotalNum;    //总记录个数
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
