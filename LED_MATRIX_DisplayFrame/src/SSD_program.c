/********************************************************************************/
/* Author : Bassem																*/
/* Date   : 21 Aug 2020															*/
/* V01																			*/
/********************************************************************************/

#include "STD_TYPES.h"
#include "BIT_MATH.h"

#include "RCC_interface.h"
#include "DIO_interface.h"

#include "SSD_interface.h"
#include "SSD_private.h"
#include "SSD_config.h"

void SSD_voidDisplayNum( u8 Copy_u8Digit ){
		
		 /*PORT output push pull  10 MHz*/
	DIO_voidSetPinDirection(SEG_PORT , PIN0 , OUTPUT_SPEED_10MHZ_PP);
	DIO_voidSetPinDirection(SEG_PORT , PIN1 , OUTPUT_SPEED_10MHZ_PP);
	DIO_voidSetPinDirection(SEG_PORT , PIN2 , OUTPUT_SPEED_10MHZ_PP);
	DIO_voidSetPinDirection(SEG_PORT , PIN3 , OUTPUT_SPEED_10MHZ_PP);
	DIO_voidSetPinDirection(SEG_PORT , PIN4 , OUTPUT_SPEED_10MHZ_PP);
	DIO_voidSetPinDirection(SEG_PORT , PIN5 , OUTPUT_SPEED_10MHZ_PP);
	DIO_voidSetPinDirection(SEG_PORT , PIN6 , OUTPUT_SPEED_10MHZ_PP);
	DIO_voidSetPinDirection(SEG_PORT , PIN7 , OUTPUT_SPEED_10MHZ_PP);

if ( Copy_u8Digit <=9 ){
	u8 digit = DigitPattern [Copy_u8Digit] ;
#if 	COMMON_TYPE == COMMON_CATHODE

		DIO_voidSetPortValue (SEG_PORT , digit ) ;  // SEG_PORT AND THE DIGIT TO DISPLAY

	#elif COMMON_TYPE == COMMON_ANODE

		digit =~digit;
		DIO_voidSetPortValue (SEG_PORT , digit ) ;  // SEG_PORT AND THE DIGIT TO DISPLAY
	#else
		#error("choose COMMON_TYPE")
#endif
}


}
