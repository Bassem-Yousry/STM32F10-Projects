#include <stdio.h>
#include <stdlib.h>
#include "STD_TYPES.h"
#include "BIT_MATH.h"

#include"GPTIM_interface.h"
#include "DIO_interface.h"
#include "STK_interface.h"
#include "SPI_interface.h"
#include "RCC_interface.h"
#include "EXTI_interface.h"
#include "NVIC_interface.h"
#include "TFT_interface.h"
#include "HIR_interface.h"


#define Null (void *) 0

u8 Score=0;
u8 Leg=0;
u8 LegSpeed=0;
u8 Jump =0;
u8 CloudTime=0;
u8 Speed=0;
struct Dinosaur{
	s8 x;
	s8 y;
	u8 Type;
};
struct Dinosaur  TRex ={3,4,0};
struct Dinosaur  Cactus ={3,80,1};
struct Dinosaur  Bird ={30,110,1};
struct Dinosaur  Cloud ={50,70,1};


void PLAY(){

u8 Key;
u8 RepeatCode=HIR_voidCheckRC();
Key= HIR_voidReturnKey();
switch (Key){

case 25:
		Jump=1;
		 break;	// power Button

}

}

void CheckObstacle(){
	s8 x,y;
	u8 Flag=0;
	 x=TRex.x ;
	 y = TRex.y ;
	 if(Bird.y<=y+9 && Bird.y+8 >=y){
	 	if( x >=Bird.x && x < Bird.x+8 )
	 	 {
			Flag=1;
	 	 }
	 }
	if(Cactus.y<=y+9 && Cactus.y+6 >=y){
		 if( x >=Cactus.x && x < Cactus.x+ (Cactus.Type*6) )
		 {
			Flag=1;
		 }
	}
	if(Flag){
			asm("cpsid i" : : : "memory");
			TIM4_voidStopInterval();
			TIM3_voidStopInterval();
			HTFT_voidDrawTRex(x*2,y*2,Leg,2,0xffff);
			HTFT_voidDrawCactus(2*Cactus.x,2*Cactus.y,Cactus.Type,0xffff);
			asm("cpsie i" : : : "memory");
	}
}

void ObstacleMove()
{

	HTFT_voidDrawBird(2*Bird.x,2*Bird.y,Bird.Type,0);
	Bird.y-=2;
	Bird.Type=(Bird.Type+1)%2;
	HTFT_voidDrawBird(2*Bird.x,2*Bird.y,Bird.Type,0xffff);

	HTFT_voidDrawCactus(2*Cactus.x,2*Cactus.y,Cactus.Type,0);
	Cactus.y-=2;
	HTFT_voidDrawCactus(2*Cactus.x,2*Cactus.y,Cactus.Type,0xffff);
	CheckObstacle();
	if(Cactus.y<=-12)
	{
		Cactus.Type=(Cactus.Type)%2+1;
		Cactus.y=80;
	}
	if(Bird.y<=-10 && Cactus.y<70)
	{
		Bird.y=Cactus.y+60;
		Speed++;
		TIM4_voidSetIntervalPeriodic  ( 450000 -Speed*8000, ObstacleMove );

	}
	if(Cloud.y<=-20)
	{
		Cloud.x+=3;
		if(Cloud.x>60){Cloud.x-=20;}
		Cloud.y=80;
	}
	if(!CloudTime){
		HTFT_voidDrawCloud(2*Cloud.x,2*Cloud.y,0);
		Cloud.y--;
		HTFT_voidDrawCloud(2*Cloud.x,2*Cloud.y,0xffff);
	}
CloudTime=(CloudTime+1)%2;

}


void Update()
{
	int x,y;
	 x=TRex.x ;
	 y = TRex.y ;

	if(Jump)
	{
		//__disable_irq();
		asm("cpsid i" : : : "memory");
		HTFT_voidDrawRect (  x*2, (x)*2 +34 , 8,40 , 0);
		asm("cpsie i" : : : "memory");
		x+=4;
		TRex.x+=4;
		if (TRex.x>=40)
		{
			Jump=0;
		}
	}
	else if(TRex.x!=3)
	{
		asm("cpsid i" : : : "memory");
		HTFT_voidDrawRect (  x*2, (x)*2 +34 , 8,40 , 0);
		asm("cpsie i" : : : "memory");
		x-=4;
		TRex.x-=4;
	}

	if(TRex.x!=3){HTFT_voidDrawTRex(TRex.x*2,TRex.y*2,Leg,2,0xffcc);}

	if(!LegSpeed)
	{
	 x =TRex.x;
	 y =TRex.y;
	Leg=(Leg+1)%2;
	HTFT_voidDrawRect (  x*2, (4+x)*2 , 0,30 , 0);
	HTFT_voidDrawTRex(x*2,y*2,Leg,2,0xffcc);
	}
	CheckObstacle();

	LegSpeed=(LegSpeed+1)%4;

}
void main(void)
 {


	/* RCC Initialization */
	RCC_voidInitSysClock();
	RCC_voidEnableClock(RCC_APB2,2);   /* GPIOA */
	RCC_voidEnableClock(RCC_APB2,12);  /* SPI1  */
	//RCC_voidEnableClock(RCC_APB1,0);  /* SPI1  */
	RCC_voidEnableClock(RCC_APB1,1);  /* SPI1  */
	RCC_voidEnableClock(RCC_APB1,2);  /* SPI1  */
	//RCC_voidEnableClock(RCC_APB1,3);  /* SPI1  */

	MNVIC_voidEnableInterrupt(29);
	MNVIC_voidSetPriority(29,4<<1);

	MNVIC_voidEnableInterrupt(30);
	MNVIC_voidSetPriority(30,3<<1);
	//TIM2_voidInit();
	//TIM2_voidSetIntervalSingle  ( 5000000, tm1 );
	TIM3_voidInit();
	//TIM3_voidSetIntervalSingle  ( 6000000, tm2 );
	TIM4_voidInit();
	/* Pin Initializations */
	DIO_voidSetPinDirection(GPIOA,3,0b1000);
	DIO_voidSetPinValue(GPIOA,3,1);//input pull up
	DIO_voidSetPinDirection(GPIOA,4,0b1000);
	DIO_voidSetPinValue(GPIOA,4,1);
	DIO_voidSetPinDirection(GPIOA,8,0b1000);
	DIO_voidSetPinValue(GPIOA,8,1);

	DIO_voidSetPinDirection(GPIOA,1,0b0010);  /* A0   */
	DIO_voidSetPinDirection(GPIOA,2,0b0010);  /* RST  */
	DIO_voidSetPinDirection(GPIOA,5,0b1001);  /* CLK  */
	DIO_voidSetPinDirection(GPIOA,7,0b1001);  /* MOSI */

	/* Init STK */
	MSTK_voidInit();

	/* SPI Init */
	MSPI1_voidInit();


	/* TFT Init */
	HTFT_voidInitialize();
	HTFT_voidFillColor ( 0);
	HTFT_voidDrawRect (  0,  63, 0, 160, 0);
	HTFT_voidDrawRect (  64,  127, 0, 160, 0);
	//Start();
	TIM3_voidSetIntervalPeriodic  ( 120000, Update );
	TIM4_voidSetIntervalPeriodic  ( 450000, ObstacleMove );
	HIR_voidInit();
	HIR_voidSetCallBack(PLAY);

	while(1){



	}

}


