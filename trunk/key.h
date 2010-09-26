#ifndef __KEY_H__
#define __KEY_H__

#ifndef __KEYPRERESPONSESTRUCT__
#define __KEYPRERESPONSESTRUCT__

typedef union
{
    int16 ival;
    int32 lval;
    float fval;
}KEYSETVALUE;

#define KeySetFlag_NotSet		     0x00
#define KeySetFlag_EndOfSet	 		 0x01
#define KeySetFlag_Setting			 0x02
#define KeySetFlag_NotChanged		 0x03
#define KeySetFlag_Return			 0x04
//#define KeySetFlag_FirstIntoSet	 0x05
//#define KeySetFlag_NotFirstIntoSet 0x06
#define KeySetFlag_signedType		 0x08
#define KeySetFlag_NotFirstPoint	 0x10
#define KeySetFlag_ValueSaved		 0x20
#define KeySetFlag_IntType		 	 0x00
#define KeySetFlag_FloatType		 0x40
#define KeySetFlag_LongType		 	 0x80

#define MAXKEYSETSTRLEN				 8

#define ClearKeySetString()			 memset(KeyResponseStruct.KeySetString,0x00,MAXKEYSETSTRLEN); 
#define LowKeySetFlagByte()		 	 (KeyResponseStruct.KeySetFlag & 0x07)
#define ClearKeySetFlag()			 KeyResponseStruct.KeySetFlag &= 0xF8
#define SetKeySetFlag(flg)			 KeyResponseStruct.KeySetFlag = (KeyResponseStruct.KeySetFlag & 0xF8) | (flg)
#define ResetKeySetFlag(flg)		 KeyResponseStruct.KeySetFlag = (flg)

#define ClearKeySetFlagH(flg)		 KeyResponseStruct.KeySetFlag &= ~(flg)
#define SetKeySetFlagH(flg)			 KeyResponseStruct.KeySetFlag |= (flg)

#define KeySetStringIsEmpty()		 !KeyResponseStruct.KeySetString[0]
#define KeySetFlagIs(flg)			 (KeyResponseStruct.KeySetFlag & (flg)) 
#define KeySetFlagIsValueSaved()	 KeySetFlagIs(KeySetFlag_ValueSaved)
#define KeySetFlagIsSignedType()	 KeySetFlagIs(KeySetFlag_signedType)
#define KeySetFlagIsFirstPoint()	 !KeySetFlagIs(KeySetFlag_NotFirstPoint)

#define KeySetFlagIsType(typ)		 ((KeyResponseStruct.KeySetFlag & 0xC0) == typ)
#define KeySetFlagSetType(typ)		 KeyResponseStruct.KeySetFlag = ((KeyResponseStruct.KeySetFlag & 0x3F) | typ)
#define KeySetFlagIsIntType()		 KeySetFlagIsType(KeySetFlag_IntType)
#define KeySetFlagIsFloatType()		 KeySetFlagIsType(KeySetFlag_FloatType)
#define KeySetFlagIsLongType()		 KeySetFlagIsType(KeySetFlag_LongType)
struct KEYPRERESPONSESTRUCT
{
    uint8 KeySetFlag;
	uint8 KeySetStrLen;
	uint8 KeySetString[MAXKEYSETSTRLEN];
	KEYSETVALUE KeySetValue;
};
#endif
//----------------------------------------------------------------------
#define		k_num0 	  	0x00
#define		k_num1	 	0x01
#define		k_num2		0x02
#define		k_num3		0x03
#define		k_num4 		0x04
#define		k_num5 		0x05
#define		k_num6 		0x06
#define		k_num7 		0x07
#define		k_num8 		0x08
#define		k_num9 		0x09
#define		k_dot 		0x0A
#define		k_sign 		0x0B
#define		k_menu 		0x0C
#define		k_modify 	0x0D
#define		k_return 	0x0E
#define		k_enter 	0x0F
#define		k_up 		0x10
#define		k_left 		0x11
#define		k_right 	0x12
#define		k_down 		0x13
#define		no_key 		0x14

//----------------------------------------------------------------------
#define		KEY_PORT1	PORTB
#define		KEY_DDR1	DDRB
#define		KEY_PIN1	PINB
#define		KEY_PORT2	PORTG
#define		KEY_DDR2	DDRG
#define		KEY_PIN2	PING
//----------------------------------------------------------------------
uint8 ReadKey(void);
void  KeyBoardSetFunction(void *VarName, uint8 M1, uint8 M2, uint8 M3, uint8 M4,	uint16 EepromAddr);
bool  Key_PreResponse(void);
void  Key_Response(void);
void  LoadKeySetMenuStr(int8 menustr[], int8 str[], void* varname, int8 vartype, int8 MaxLen);
void  KeyBoardSetDateTime(int8 TypeStr[],uint8 M1, uint8 M2, uint8 M3, uint8 M4);
//----------------------------------------------------------------------
#endif