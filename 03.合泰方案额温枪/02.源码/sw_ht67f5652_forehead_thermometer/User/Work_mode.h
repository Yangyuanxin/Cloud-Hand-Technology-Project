#ifndef _Work_mode_H_
#define	_Work_mode_H_


U8		get_obj_temp();
void	fun_CORRECTION_MODE_FUN();
U8		fun_get_VTP();
U8  	fun_get_NTC();
void	fun_Work_mode(void);


#define	CHECK_START_KEY	0
#define	MEMS_MODE		1
#define	CORRECTION_MODE	2
#define	NTC_CC_MODE		3


extern	volatile	__16_type	gu16v_adc_res2_buff;

extern	volatile    	U8 gu8v_key_enable;
extern	volatile    	U8 gu8v_key_old;
extern	volatile    	U8 gu8_ntc_mode;
extern	__byte_type	gu8v_dispaly_mode;
extern 	volatile U8 gu8v_display_mode_old;

#endif
