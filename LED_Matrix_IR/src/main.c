/*
 * main.c
 *
 *  Created on: Sep 2, 2020
 *      Author: Ahmed
 */
#include "STD_TYPES.h"
#include "BIT_MATH.h"

#include "RCC_interface.h"
#include "DIO_interface.h"
#include "EXTI0_interface.h"
#include "NVIC_interface.h"
#include "STK_interface.h"
#include "HLEDMAT_interface.h"


void delay (u32 t){
for(int j=0;j<t;j++){
asm("NOP");}

}

volatile u8   u8StartFlag       = 0;
volatile u32  u32FrameData[50]  = {0};
volatile u8   u8EdgeCounter     = 0;
volatile u8   u8Data            = 0;
volatile u8		speed	=0;
#define  AFIO  *((volatile u32* ) 0x40010010)

volatile u8 mode = 0;
void voidPlay(void)
{

	switch (u8Data)
	{
	case 64:MGPIO_VidSetPinValue(GPIOA,9,1); speed +=3;break;
	case 67 :MGPIO_VidSetPinValue(GPIOA,10,1); speed-=3;;break;
	case 70:MGPIO_VidSetPinValue(GPIOA,11,1);break;
	case 69:MGPIO_VidSetPinValue(GPIOA,12,1);TOG_BIT(mode,0);  break;
	}
}

void voidTakeAction(void)
{
	u8 i;
	u8Data = 0;

	if ( (u32FrameData[0] >= 10000) && (u32FrameData[0] <= 14000) )
	{
		for (i=0;i<8;i++)
		{
			if (  (u32FrameData[17+i] >= 2000) && (u32FrameData[17+i] <=2300) )
			{

				SET_BIT(u8Data,i);
			}

			else
			{
				CLR_BIT(u8Data,i);
			}
		}

		voidPlay();
	}

	else
	{
		/* Invalid Frame */
	}

	u8StartFlag     = 0;
	u32FrameData[0] = 0;
	u8EdgeCounter   = 0;
}

void voidGetFrame(void)
{

	if (u8StartFlag == 0)
	{


		/* Start Timer */
		MSTK_voidSetIntervalSingle(1000000,voidTakeAction);
		u8StartFlag = 1;
	}

	else
	{	//MGPIO_VidSetPinValue( GPIOA, 3,1 );

		u32FrameData[u8EdgeCounter] = MSTK_u32GetElapsedTime();
		MSTK_voidSetIntervalSingle(1000000,voidTakeAction);
		u8EdgeCounter++;
	}
}

void stop(void){
	while (mode%2==0){

	}

}
void main(void)
{
	speed =9;
	/* RCC Initialize */
	RCC_voidInitSysClock();
	RCC_voidEnableClock(RCC_APB2,2); /* GPIOA Enable Clock */
	RCC_voidEnableClock(RCC_APB2,3); /* GPIOB Enable Clock */
	RCC_voidEnableClock(RCC_APB2,0); /* GPIOA Enable Clock */

	/* IO Pins Initialization */
	MGPIO_VidSetPinDirection(GPIOA,8,0b0100); /* A8 Input Floating       */
	MGPIO_VidSetPinDirection(GPIOA,0,0b0010); /* A0 Output			       */
	MGPIO_VidSetPinDirection(GPIOA,1,0b0010); /* A1 Output PP "RED"       */
	MGPIO_VidSetPinDirection(GPIOA,2,0b0010); /* A1 Output PP "GREEN"     */
	MGPIO_VidSetPinDirection(GPIOA,3,0b0010); /* A1 Output PP "BLUE"      */
	MGPIO_VidSetPinDirection(GPIOA,4,0b0010); /* A1 Output PP "BLUE"      */
	MGPIO_VidSetPinDirection(GPIOA,5,0b0010); /* A1 Output PP "BLUE"      */
	MGPIO_VidSetPinDirection(GPIOA,6,0b0010); /* A1 Output PP "BLUE"      */
	MGPIO_VidSetPinDirection(GPIOA,7,0b0010); /* A1 Output PP "BLUE"      */
	MGPIO_VidSetPinDirection(GPIOA,9,0b0010); /* A1 Output PP "BLUE"      */
	MGPIO_VidSetPinDirection(GPIOA,11,0b0010); /* A1 Output PP "BLUE"      */
	MGPIO_VidSetPinDirection(GPIOA,12,0b0010); /* A1 Output PP "BLUE"      */
	MGPIO_VidSetPinDirection(GPIOA,10,0b0010); /* A1 Output PP "BLUE"      */

	MGPIO_VidSetPinDirection(GPIOB , PIN0 , OUTPUT_SPEED_10MHZ_PP);
	MGPIO_VidSetPinDirection(GPIOB , PIN1 , OUTPUT_SPEED_10MHZ_PP);
	MGPIO_VidSetPinDirection(GPIOB , PIN5 , OUTPUT_SPEED_10MHZ_PP);
	MGPIO_VidSetPinDirection(GPIOB , PIN6 , OUTPUT_SPEED_10MHZ_PP);
	MGPIO_VidSetPinDirection(GPIOB , PIN7 , OUTPUT_SPEED_10MHZ_PP);
	MGPIO_VidSetPinDirection(GPIOB , PIN8 , OUTPUT_SPEED_10MHZ_PP);
	MGPIO_VidSetPinDirection(GPIOB , PIN9 , OUTPUT_SPEED_10MHZ_PP);
	MGPIO_VidSetPinDirection(GPIOB , PIN10 , OUTPUT_SPEED_10MHZ_PP);

	/* EXTI Initializtion */
	MEXTI_voidSetCallBack(voidGetFrame);
	MEXTI_voidInit(); /* A8 EXTI8 Enabled / Falling Edge */

	/* Enable EXTI0 from NVIC */
	MNVIC_voidInit();
	MNVIC_voidEnableInterrupt(23); /* EXTI3 */
	 MNVIC_voidSetPriority(23, 0);

	/* SysTick Init */


	MSTK_voidInit(); /* Enable SysTick AHB/8 = 1MHZ */
	int FRAME[]= { 126, 73, 73, 54, 0, 126, 17, 17, 126, 0, 78, 73, 73, 49, 0, 78, 73, 73, 49, 0, 127, 73, 73, 0, 127, 1, 127, 1, 127,0};
	while(1)
	{
		HLEDMAT_voidDisplayShiftFrame(FRAME,speed,25);
	}
}


