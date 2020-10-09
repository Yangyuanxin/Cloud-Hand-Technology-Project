;;--------------------------------------------------------------;;
;;    This file is part of the Holtek C Compiler V3 package     ;;
;;    For the initialization of static linkage variables        ;;
;;    Copyright (C) 2017 Holtek Semiconductor Inc.              ;;
;;    Version: 1.07 (Above IDE3000 V7.94)                       ;;
;;    Date:    2017/03/06                                        ;;
;;--------------------------------------------------------------;;

acc equ [05h]
tblp equ [07h]
tblh equ [08h]
;;tbhp equ [09h] 
r0 equ [00h]
mp0 equ [01h]
r1 equ [02h]
mp1l equ [03h]
mp1h equ [04h]
z equ [0ah].2
c equ [0ah].0

ifndef tbhp
tbhp equ [09h]
endif
 
extern startup_value_1:near

@start .section 'code'
begin_startup_value:
  mov a,low (offset startup_value_1) 
  mov tblp,a
  mov a,high (offset startup_value_1) 
  mov tbhp,a
next_table:
  ;CLR WDT
  inc tblp
  sz z
  inc tbhp
ifdef USE_TABRDC
  tabrdc mp0
else
  tabrd mp0
endif
  sz mp0
  jmp read_data
  jmp startupend1
read_data:
  inc tblp
  sz z
  inc tbhp
ifdef USE_TABRDC
  tabrdc mp1l
else
  tabrd mp1l
endif
  mov a,tblh
  mov mp1h,a
next_data:
  inc tblp
  sz z
  inc tbhp
ifdef USE_TABRDC
  tabrdc acc
else
  tabrd acc
endif
  mov r1,a
  sdz mp0
  jmp $+2
  jmp next_table
  inc mp1l
  mov a,tblh
  mov r1,a
  inc mp1l
  sdz mp0
  jmp next_data
  jmp next_table

;end_startup_value:

startupend1:
ifndef Disable_Bit_Initial
	MOV A,high  bitdatasec1_start
	MOV mp1h,a
	MOV A,offset bitdatasec1_end
	mov mp1l,A
	dec mp1l
	clr z
	sub a,offset bitdatasec1_start
	sz z
	jmp startupend2
L0005:
	set r1
	dec mp1l
	sdz  acc
	jmp L0005

startupend2:
	MOV A,high  bitdatasec0_start
	MOV mp1h,a
	MOV A,offset bitdatasec0_end
	mov mp1l,A
	dec mp1l
	clr z
	sub a,offset bitdatasec0_start
	sz  z
	jmp startupend3
L0006:
	clr r1
	dec mp1l
	sdz  acc
	jmp L0006
startupend3:
endif		
   
@ROMDATA_BASE .SECTION com_l 'CODE'  
startup_value:
;;linker range the initial value table here

@BITDATASEC1 .SECTION com_l 'DATA'  
bitdatasec1_start:

@BITDATASEC1 .SECTION com_e 'DATA'  
bitdatasec1_end:

@BITDATASEC .SECTION com_l 'DATA'  
bitdatasec0_start:

@BITDATASEC .SECTION com_e 'DATA'  
bitdatasec0_end: