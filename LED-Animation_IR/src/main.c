#include "STD_TYPES.h"
#include "BIT_MATH.h"

#include "RCC_interface.h"
#include "DIO_interface.h"
#include "EXTI0_interface.h"
#include "NVIC_interface.h"
#include "STK_interface.h"

void delay (u32 t){
for(int j=0;j<t;j++){
asm("NOP");}

}

volatile u8   u8StartFlag       = 0;
volatile u32  u32FrameData[50]  = {0};
volatile u8   u8EdgeCounter     = 0;
volatile u8   u8Data            = 0;
#define  AFIO  *((volatile u32* ) 0x40010010)

volatile u8 mode = 0;
void voidPlay(void)
{

	switch (u8Data)
	{

		case 70:
			mode = (mode + 1)%2;


			//MGPIO_VidSetPinValue(GPIOA,7,HIGH);
			break;
		case 69: MGPIO_VidSetPinValue(GPIOA,7,LOW);  break;
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


void main(void)
{
	/* RCC Initialize */
	RCC_voidInitSysClock();
	RCC_voidEnableClock(RCC_APB2,2); /* GPIOA Enable Clock */
	RCC_voidEnableClock(RCC_APB2,3); /* GPIOB Enable Clock */
	RCC_voidEnableClock(RCC_APB2,0); /* AFO Enable Clock */

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


	/* EXTI Initializtion */
	MEXTI_voidSetCallBack(voidGetFrame);
	MEXTI_voidInit(); /* A8 EXTI8 Enabled / Falling Edge */

	/* Enable EXTI8 from NVIC */
	MNVIC_voidInit();
	MNVIC_voidEnableInterrupt(23); /* EXTI8 */

	/* SysTick Init */
AFIO = 0x00;

	MSTK_voidInit(); /* Enable SysTick AHB/8 = 1MHZ */

	while(1)
	{
		if(!mode){
		for (int i =0 ; i<8;i++){
			 DIO_voidSetPortValue (GPIOA,1<<i);
		delay (197000);

		if (mode){break;}

		}
		for (int i =7 ; i>-1;i--){
			 DIO_voidSetPortValue (GPIOA,1<<i);
		delay (197000);
		if (mode){break;}
		}
		}
		else{
			for (int y =0 ; y<8;y++){
			DIO_voidSetPortValue (GPIOA,0xff);
			delay(500000);
			if(!mode){break;}

			DIO_voidSetPortValue (GPIOA,0);
			delay(500000);
			if(!mode){break;}
			}

		}

	}

}
