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

u8 Score=255;
u8 XxX=1;
u8 Gold=0;
struct EggCol{
	u8 x;
	u8 y;
	u8 GoldBomb;     //0-> Normal Egg , 1->Golden Egg , 2->Bomb
	struct EggCol * Next;
};
struct EggCol * EggList = Null;
struct EggCol  Bowl ;


void AddEgg(u8 Egg_x,u8 Egg_y){

	struct EggCol * Egg = Null;
	Egg = (struct EggCol*)malloc(sizeof(struct EggCol)) ;
	Egg->x=Egg_x;
	Egg->y=Egg_y;
	Egg->GoldBomb=0;
	Egg->Next=EggList;
	EggList=Egg;

}

void PrintBowl(){
	u8 Bowl_x =Bowl.x ;
	for (int i=0;i<16;i++)
	{
		if(i==Bowl_x)
		{
			HTFT_voidDrawRect (  Bowl_x*8, Bowl_x*8+31, 0,2,0xffff);
			HTFT_voidDrawRect (  Bowl_x*8, Bowl_x*8+2, 0,10,0xffff);
			HTFT_voidDrawRect (  Bowl_x*8+29, Bowl_x*8+31, 0,10,0xffff);
			HTFT_voidDrawRect (  Bowl_x*8+3, Bowl_x*8+28, 3,10,0);
			i+=3;
			continue;
		}
		HTFT_voidDrawRect (  i*8, i*8+7, 0,10,0);


	}


}

void UpdateEgg(){
	if (EggList==Null){return;}
	struct EggCol * LastEgg, *PreLastEgg=Null;
	EggList->y--;

	LastEgg=EggList;
	while(LastEgg->Next != Null){
		PreLastEgg=LastEgg;
		LastEgg=LastEgg->Next;
		LastEgg->y--;
	}
	if (LastEgg->y==0)
	{
		if(PreLastEgg ==Null)
		{
			free(LastEgg);
			EggList=Null;
			return;
		}
		free(LastEgg);
		PreLastEgg->Next=Null;
	}
}

void PrintEggs(){
	u8 Bowl_x =Bowl.x ;
	struct EggCol * Egg;
		Egg=EggList;
		while(Egg != Null){
			if(Egg->GoldBomb==1)
			{
				HTFT_voidDrawEgg ( Egg->x*4, Egg->y*4, 0xFEA0,0);
			}
			else if(Egg->GoldBomb==2)
			{
				HTFT_voidDrawEgg ( Egg->x*4, Egg->y*4, 0xC000,1);
			}
			else{HTFT_voidDrawEgg ( Egg->x*4, Egg->y*4, 0xffff,0);}
			if(Egg->y<2 && ( Egg->x/2 >= Bowl_x && Egg->x/2 <= Bowl_x+3) )
			{
				if(Egg->GoldBomb==1){Score+=9;}
				else if(Egg->GoldBomb==2){HTFT_voidDrawText ("A7A",  50, 100 ,1,0xffff);}
				UpdateScore();
			}
			Egg = Egg->Next;
		}
}

void ClearEgg(){
	u8 Bowl_x =Bowl.x ;
	struct EggCol * Egg;
		Egg=EggList;
		while(Egg != Null){
			HTFT_voidDrawEgg ( Egg->x*4, Egg->y*4, 0,0);
			if(Egg->y < 3 ){
				PrintBowl();
			}
			Egg = Egg->Next;
		}
}

volatile u16 Dots [20] ={};
volatile u8 Lives =3;
void Start(){
Bowl.y=0;
Bowl.x=7;
HTFT_voidFillColor ( 0);
HTFT_voidDrawRect (  0,  63, 0, 160, 0);
HTFT_voidDrawRect (  64,  127, 0, 160, 0);
MSTK_voidSetBusyWait(500000);
PrintBowl();

}
void Update(){
	ClearEgg();
	UpdateEgg();
	PrintEggs();
	XxX=(XxX+2)%29;
	Gold++;
}
void Add(){
	AddEgg(XxX,35);
	if(Gold >210){
		EggList->GoldBomb=2;
	}
	if(Gold >235){
		EggList->GoldBomb=1;
	}
}
void UpdateScore(){
	u8 Speed;
	HTFT_voidDrawRect (0,  127, 149 ,159,0);
	Score++;
	s8 SCORE[20];
	sprintf(SCORE,"SCORE %i",Score);
	Speed = Score/10; // increase speed and difficulty every 10 points
	if(Speed){
	TIM4_voidSetIntervalPeriodic  ( 700000-Speed*32000, Update );
	TIM3_voidSetIntervalPeriodic  ( 11000000-Speed*505050, Add );
	}
	HTFT_voidDrawText (SCORE,  124, 150 ,1,0xffff);
}

/*
void RESET(){
	global_u8Ball.x=7;
	global_u8Ball.y=3;
	Score=0;
	Score2=0;
	HTFT_voidFillColor ( 0);

	//TIM4_voidSetIntervalPeriodic  ( 900000, Start );

}
*/

void PLAY(){
u8 Bowl_x=Bowl.x;
u8 Key;
u8 RepeatCode=HIR_voidCheckRC();
Key= HIR_voidReturnKey();
switch (Key){

case 12:			//move rightwards
	if(RepeatCode)
	{
		Bowl.x=0;
	}
	else if( Bowl_x > 0 )
	{
		Bowl.x--;
		PrintBowl();
	}

	break;

case 13:			//move leftwards

	if(RepeatCode)
		{
			Bowl.x=12;
		}
	else if(Bowl_x <12  )
	{

		Bowl.x++;
		PrintBowl();
	}

	break;

case 25: //RESET();
		 //Turn_Num=0;
		 break;	// power Button

}

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


	Start();
	UpdateScore();
	TIM4_voidSetIntervalPeriodic  ( 700000, Update );
	TIM3_voidSetIntervalPeriodic  ( 11000000, Add );

	HIR_voidInit();
	HIR_voidSetCallBack(PLAY);

	while(1){



	}

}


