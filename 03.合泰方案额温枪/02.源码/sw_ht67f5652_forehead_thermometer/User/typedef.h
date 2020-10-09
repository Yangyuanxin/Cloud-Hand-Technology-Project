#ifndef _TYPEDEF_H__
#define _TYPEDEF_H__
/************************Type Defination*****************/
	
	typedef unsigned char		BOOL;
	typedef unsigned char		U8;
	typedef unsigned char		u8;
	typedef unsigned int		U16;
	typedef unsigned long		U32;
	typedef char				S8;
	typedef int					S16;
	typedef long				S32;							

	#define FALSE				0
	#define TRUE				1
	#define NULL				0
	
	typedef unsigned char __1_byte;
	typedef struct {
	    unsigned char b0:1;
	    unsigned char b1:1;
	    unsigned char b2:1;
	    unsigned char b3:1;
	    unsigned char b4:1;
	    unsigned char b5:1;
	    unsigned char b6:1;
	    unsigned char b7:1;
	} __8_bits;
	typedef union {
	    __8_bits bits;
	    __1_byte U8;
	    __1_byte S8;	    
	} __byte_type;
	

//	typedef long	__S16_bit;
	typedef struct {
	    unsigned char b0:1;
	    unsigned char b1:1;
	    unsigned char b2:1;
	    unsigned char b3:1;
	    unsigned char b4:1;
	    unsigned char b5:1;
	    unsigned char b6:1;
	    unsigned char b7:1;
	    unsigned char b8:1;
	    unsigned char b9:1;
	    unsigned char b10:1;
	    unsigned char b11:1;
	    unsigned char b12:1;
	    unsigned char b13:1;
	    unsigned char b14:1;
	    unsigned char b15:1;	    
	    
	} __16_bits;
	typedef union {
		struct
		{
			unsigned char byte0;
			unsigned char byte1;		
		}byte;	
	    __16_bits bits;
		int		 		 S16;
	    unsigned int	 U16;	    
	} __16_type;



	typedef long	__S32_bit;
	typedef struct {
	    unsigned char b0:1;
	    unsigned char b1:1;
	    unsigned char b2:1;
	    unsigned char b3:1;
	    unsigned char b4:1;
	    unsigned char b5:1;
	    unsigned char b6:1;
	    unsigned char b7:1;
	    unsigned char b8:1;
	    unsigned char b9:1;
	    unsigned char b10:1;
	    unsigned char b11:1;
	    unsigned char b12:1;
	    unsigned char b13:1;
	    unsigned char b14:1;
	    unsigned char b15:1;	    
	    unsigned char b16:1;
	    unsigned char b17:1;
	    unsigned char b18:1;
	    unsigned char b19:1;
	    unsigned char b20:1;
	    unsigned char b21:1;
	    unsigned char b22:1;
	    unsigned char b23:1;	    
	    unsigned char b24:1;
	    unsigned char b25:1;
	    unsigned char b26:1;
	    unsigned char b27:1;
	    unsigned char b28:1;
	    unsigned char b29:1;
	    unsigned char b30:1;
	    unsigned char b31:1;	    
	    
	} __32_bits;
	typedef union {
		struct
		{
			unsigned char byte0;
			unsigned char byte1;
			unsigned char byte2;
			unsigned char byte3;
			
		}byte;	
	    __32_bits 		bits;
	    unsigned long 	U32;
	    long			S32;	    
	} __32_type;

#endif

