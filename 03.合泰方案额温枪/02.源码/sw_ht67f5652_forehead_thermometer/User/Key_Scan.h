#ifndef _fun_Key_Scan_H_
#define	_fun_Key_Scan_H_


/********************************************************************
Function:	function
********************************************************************/	
void	fun_Key_Scan(void);
void	Key_dealwith(void);

/********************************************************************
Function:	KEY
********************************************************************/
//¥h§Ý®É¶¡ n*10ms
#define		LUCC_KEY_DEBOUNCE	5		


/********************************************************************
Function:	GPIO
********************************************************************/
#define		GPIO_fun_Key_Scan_WU	_pawu6
#define		GPIO_fun_Key_Scan_PU	_papu6
#define		GPIO_fun_Key_Scan_IO	_pac6
#define		GPIO_KEY_SACAN			_pa6

#define		GPIO_KEY_Switch_WU		_pawu5
#define		GPIO_KEY_Switch_PU		_papu5
#define		GPIO_KEY_Switch_IO		_pac5
#define		GPIO_KEY_Switch			_pa5

#define		GPIO_KEY_ROOM_PU		_papu3
#define		GPIO_KEY_ROOM_IO		_pac3
#define		GPIO_KEY_ROOM			_pa3

#define		GPIO_KEY_Surface_PU		_pbpu0
#define		GPIO_KEY_Surface_IO		_pbc0
#define		GPIO_KEY_Surface		_pb0

#define		GPIO_KEY_BODY_PU		_pbpu2
#define		GPIO_KEY_BODY_IO		_pbc2
#define		GPIO_KEY_BODY			_pb2

#define		GPIO_KEY_BEEP_PU		_papu4
#define		GPIO_KEY_BEEP_IO		_pac4
#define		GPIO_KEY_BEEP			_pa4
/********************************************************************
Function:	volatile
********************************************************************/				

/************************fun_Key_Scan Variable****************/					
extern	volatile				__byte_type 		gu8v_KeyCurrent;
#define KeyCurrent0 			gu8v_KeyCurrent.bits.b0
#define KeyCurrent1 			gu8v_KeyCurrent.bits.b1
#define KeyCurrent2 			gu8v_KeyCurrent.bits.b2
#define KeyCurrent3 			gu8v_KeyCurrent.bits.b3
#define KeyCurrent4 			gu8v_KeyCurrent.bits.b4
#define KeyCurrent5 			gu8v_KeyCurrent.bits.b5
#define KeyCurrent6 			gu8v_KeyCurrent.bits.b6
#define KeyCurrent7 			gu8v_KeyCurrent.bits.b7


extern	volatile				__byte_type			gu8v_KeyDown;
#define gbv_KeyDown_Scan		gu8v_KeyDown.bits.b0
#define gbv_KeyDown_Switch		gu8v_KeyDown.bits.b1
#define gbv_KeyDown_Room 		gu8v_KeyDown.bits.b2
#define gbv_KeyDown_Surface		gu8v_KeyDown.bits.b3
#define gbv_KeyDown_Body		gu8v_KeyDown.bits.b4
#define gbv_KeyDown_Beep		gu8v_KeyDown.bits.b5


extern	volatile				__byte_type 	gu8v_Key_status;
#define gbv_off_sys				gu8v_Key_status.bits.b0
#define gbv_on_sys				gu8v_Key_status.bits.b1
#define gu8v_Keystatus2			gu8v_Key_status.bits.b2
#define gu8v_Keystatus3			gu8v_Key_status.bits.b3
#define gu8v_Keystatus4 		gu8v_Key_status.bits.b4
#define gu8v_Keystatus5	 		gu8v_Key_status.bits.b5
#define gu8v_Keystatus6			gu8v_Key_status.bits.b6
#define gu8v_Keystatus7 		gu8v_Key_status.bits.b7


extern	volatile				U8	gu8v_KeyOld;
extern	volatile				U8	gu8v_KeyNoChangedTime;
extern	volatile				__byte_type  gu8v_KeyPress;
#define gbv_on_KeyPress			gu8v_KeyPress.bits.b0
#define gbv_mems_KeyPress		gu8v_KeyPress.bits.b1
#define gbv_ear_KeyPress		gu8v_KeyPress.bits.b2
#define gu8v_KeyPress3			gu8v_KeyPress.bits.b3
#define gu8v_KeyPress4			gu8v_KeyPress.bits.b4
#define gu8v_KeyPress5	 		gu8v_KeyPress.bits.b5
#define gu8v_KeyPress6			gu8v_KeyPress.bits.b6
#define gu8v_KeyPress7 			gu8v_KeyPress.bits.b7


//extern	volatile			U8	gu8v_KeyUp;
extern	volatile	__byte_type	gu8v_KeyUp;

#define gbv_KeyUp_Beep 		gu8v_KeyUp.bits.b0



extern	volatile			U8	gu8v_KeyLast;
extern 	volatile			U8 	gu8v_KeyCanChange;	
extern	volatile			U8 	gu8v_Keypress_time;

#endif
