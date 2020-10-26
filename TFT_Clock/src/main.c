/*
 * main.c
 *
 *  Created on: Sep 21, 2020
 *      Author: Ahmed
 */
#include <stdio.h>
#include "STD_TYPES.h"
#include "BIT_MATH.h"

#include "DIO_interface.h"
#include "STK_interface.h"
#include "SPI_interface.h"
#include "RCC_interface.h"

#include "TFT_interface.h"
#include "TFT_private.h"

#include "image.h"

u8 global_u8Minutes;
u8 global_u8Hours;
u8 global_u8Seconds;
u8 global_u8Days;
u8 global_u8Months;

u8 Clock[50];u8 Day[50];
const u8 * global_Months[12] = {"JAN" ,"FEB" , "MAR" , "APR" , "MAY" , "JUN" , "JUL","AUG","SEP","OCT","NOV","DEC" };
const u8 * global_Days[7] = { "SAT" , "SUN" , "MON" , "TUS" , "WED" , "THUR","FRI"  };
void Day_Update(){
	 HTFT_voidDrawRect (  0,  127, 10, 90, 0);
	 sprintf (Day,"%.2i              2020",global_u8Days);
	 HTFT_voidDrawText (global_Months[global_u8Months], 92,56 ,2,0xF81F);
	 HTFT_voidDrawText (Day, 120,56 ,2,0xF81F);

	 HTFT_voidDrawText (global_Days[(global_u8Days-1)%7], 87,16 ,3,0x07FF);



}
void Clock_Update(){
	u8 Clear =60;
	global_u8Seconds++;
	if (global_u8Seconds==60)
	{
		global_u8Seconds=0;
		global_u8Minutes++;
		if(global_u8Minutes==60)
		{

			global_u8Minutes=0;
			global_u8Hours++;
			if(global_u8Hours==25)
			{
				global_u8Hours=0;
				global_u8Days++;
				if(global_u8Days==31){global_u8Months++; global_u8Days=1;}
				Day_Update();

			}
		}
	}
	if (global_u8Hours<12){ sprintf ( Clock,"%.2i:%.2i:%.2i AM",	 global_u8Hours%13 , global_u8Minutes , global_u8Seconds);}
	else{sprintf ( Clock,"%.2i:%.2i:%.2i PM",	 global_u8Hours%13 , global_u8Minutes , global_u8Seconds);}
	 HTFT_voidDrawRect (  0,  127, 90, 110, 0);
	 HTFT_voidDrawText (Clock, 127,90 ,2,0xffff);
};
void main(void)
{
	/* RCC Initialization */
	RCC_voidInitSysClock();
	RCC_voidEnableClock(RCC_APB2,2);   /* GPIOA */
	RCC_voidEnableClock(RCC_APB2,12);  /* SPI1  */

	/* Pin Initializations */
	MGPIO_VidSetPinDirection(GPIOA,1,0b0010);  /* A0   */
	MGPIO_VidSetPinDirection(GPIOA,2,0b0010);  /* RST  */
	MGPIO_VidSetPinDirection(GPIOA,5,0b1001);  /* CLK  */
	MGPIO_VidSetPinDirection(GPIOA,7,0b1001);  /* MOSI */

	/* Init STK */
	MSTK_voidInit();

	/* SPI Init */
	MSPI1_voidInit();

	/* TFT Init */
	HTFT_voidInitialize();

	/* Display Image */
	 HTFT_voidFillColor ( 0x0);
	 HTFT_voidDrawText ("Digital", 105,140 ,2,0xF800);
	 HTFT_voidDrawText ("Clock", 95,120 ,2,0xF800);


	  global_u8Minutes=59;
	  global_u8Hours=24;
	  global_u8Seconds=50;
	  global_u8Days=30;
	  global_u8Months=10;

	 sprintf ( Clock,"%.2i:%.2i:%.2i AM",	 global_u8Hours%13 , global_u8Minutes , global_u8Seconds);
	 HTFT_voidDrawText (Clock, 124,90 ,2,0xffff);
	 Day_Update();

	 int i =0;
	 MSTK_voidSetIntervalPeriodic( 93000000, Clock_Update );
	while(1){
/*
	 HTFT_voidDrawChar ('0'+i, 90,120-i*16 ,2,14);
	 i++;
	 if(i==6){i=0;HTFT_voidFillColor ( 0x0);}
	*/}

}

