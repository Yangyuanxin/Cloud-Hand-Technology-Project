
#ifndef _CALCULATE_H_
#define _CALCULATE_H_




#define	LU8C_TCF	0 //-45

void	fun_Vtp_Calculate(void);
void	fun_RNTC_Calculate(void);	
void	fun_Vsen_Calculate(void);
void	fun_Tobj_Calculate(void);
void	fun_CORRECTION_Calculate(U16 lu16v_table_guide1,U16 lu16v_table_guide2);
void	fun_Temp_SurfaceToBody(void);
void	fun_TCF_Calculate();
void	fun_get_Vobj_25(S32 lu16v_vobj_25);


extern	volatile	__16_type gu16v_correction_K1;
extern	volatile	__16_type gu16v_correction_K2;
extern	volatile	__16_type gu16v_tcf;

#endif

