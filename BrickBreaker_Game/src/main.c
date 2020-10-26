#include <stdio.h>
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

volatile u8 Shape_Num=0;
u8 Score  = 1;
u8 H_Move = 1;			//1 ->increase  x postion
u8 V_Move = 1;			//1 ->increase  y postion
struct Brick{
	u8 x;
	u8 y;
};

volatile struct Brick global_u8Ball = {7,3};
volatile struct Brick global_u8BallOld = {7,3};
volatile struct Brick global_u8Paddle = {3,2};
volatile u16 Area [17]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0xff,0xff};
volatile u8 Lives =3;

void Print_Ter(){
	u8 x = global_u8Ball.x , y = global_u8Ball.y;
	HTFT_voidDrawCircle(x*8,y*8,0xff80);

}
void UpdateLive(){
	if(Lives==0)
	{
		TIM4_voidStopInterval();
		HTFT_voidDrawRect (  0, 127, 0,159, 0x1082);
		HTFT_voidDrawText ("U LOST",  125, 60 ,4,0Xfffe);
		return;
	}
	Lives--;
	HTFT_voidDrawRect (  0, 127, 140,159, 0x1082);
	for(int i=0;i<=Lives;i++)
	{
		HTFT_voidDrawHeart(4+(16*i),145,0xf<<11);
	}
	global_u8Ball.x=global_u8Paddle.x+1;
	global_u8Ball.y=3;
}
void Screen_Update(){

	u8 x = global_u8BallOld.x , y = global_u8BallOld.y;
	HTFT_voidDrawCircle(x*8,y*8,0);
	Print_Ter();
	s8 SCORE[20];
	sprintf(SCORE,"LEVEL %i",Score);
	HTFT_voidDrawText (SCORE,  120, 145 ,1,0Xfffe);

	for(int y =14;y<17;y++){
		for(int x =0 ; x<8;x++){
			if( !GET_BIT(Area[y],x) )    //empty block
			{
				HTFT_voidDrawRect (  x*16,  x*16+16, y*8, y*8+8, 0);
			}else{
				HTFT_voidDrawFrameRect (  x*16,  x*16+16, y*8, y*8+8, 0xF000 ,0xffff);
			}
		}
	}


}

void CheckWin(){
	for (int i =14; i<17;i++)
	{
			if (Area[i] !=0){return;}
	}
	TIM4_voidStopInterval();
	HTFT_voidDrawRect (  0, 127, 0,159, 0x6808);
	HTFT_voidDrawText ("U WON",  115, 50 ,4,0Xfffe);

}

void Update_Ter(){
	global_u8BallOld=global_u8Ball;
	u8  Ball_y=global_u8Ball.y ,
		Ball_x=global_u8Ball.x,
		Paddle_x =global_u8Paddle.x,
		Paddle_y =global_u8Paddle.y;

	if (V_Move){										//moving upward
			if( Ball_y  == 16 || GET_BIT(Area[Ball_y+1],Ball_x/2) )
			{
				V_Move=0;
				global_u8Ball.y--;
				if(Ball_y!=16 && GET_BIT(Area[Ball_y+1],Ball_x/2 ) ){
					CLR_BIT( Area[Ball_y+1], Ball_x/2) ;
				}
			}else
			{
				global_u8Ball.y++;
			}

		}
		else{												//moving downward
			if( Ball_y  == 0 ) {
						V_Move=1;
						UpdateLive();
						return;
					}
			else if ( GET_BIT(Area[Ball_y-1],Ball_x/2) )
			{
				V_Move=1;
				global_u8Ball.y++;
				CLR_BIT(Area[Ball_y-1],Ball_x/2);
			}
			else if (Ball_y-1 ==Paddle_y ){
				if(Ball_x/2==Paddle_x-1){V_Move=1;H_Move=0;}
				else if (Ball_x/2==Paddle_x){V_Move=1;}
				else if (Ball_x/2==Paddle_x+1){V_Move=1;H_Move=1;}
				else {global_u8Ball.y--;}

			}
			else
			{
				global_u8Ball.y--;
			}

		}
	if (H_Move){										//moving leftward
		if( Ball_x  == 15 || GET_BIT(Area[Ball_y],(Ball_x+1)/2 ) )
		{
			H_Move=0;
			global_u8Ball.x--;
			if(GET_BIT(Area[Ball_y],(Ball_x+1)/2 ) ){
				CLR_BIT( Area[Ball_y], (Ball_x+1)/2 ) ;
			}
		}else
		{
			global_u8Ball.x++;
		}
		/*
		else if ( GET_BIT(Area[y],(x+1)/2 ) ){
			H_Move=0;
			global_u8Ball.x--;
			CLR_BIT( Area[y], (x+1)/2 ) ;
		}*/
	}
	else{												//moving rightward
		if( Ball_x  == 0 ) {
					H_Move=1;
					global_u8Ball.x++;
				}
		else if ( GET_BIT(Area[Ball_y],(Ball_x-1)/2 ) )
		{
			H_Move=1;
			global_u8Ball.x++;
			CLR_BIT(Area[Ball_y],(Ball_x-1)/2);
		}else
		{
			global_u8Ball.x--;
		}

	}

}

void Print_Paddle(){
	u8 Paddle_x =global_u8Paddle.x -1, Paddle_y =global_u8Paddle.y;
	for (int i =0;i<8;i++){
		if(i==Paddle_x ){
			HTFT_voidDrawRect (  (i)*16+5,  (i)*16+16, Paddle_y*8, Paddle_y*8+8, 4);
			HTFT_voidDrawRect (  (i+1)*16,  (i+1)*16+16, Paddle_y*8, Paddle_y*8+8, 4);
			HTFT_voidDrawRect (  (i+2)*16,  (i+2)*16+11, Paddle_y*8, Paddle_y*8+8, 4);
			i+=2;
			continue;
		}
		HTFT_voidDrawRect (  (i)*16,  (i)*16+16, Paddle_y*8, Paddle_y*8+8, 0);

	}

}

void Start(){

	Screen_Update();
	Print_Paddle();
	Update_Ter();
	if (global_u8Ball.y>=14)
	{
		CheckWin();
	}

}
void RESET(){
	global_u8Ball.x=7;
	global_u8Ball.y=3;
	global_u8Paddle.x=3;
	Lives=3;
	Area[15]=0xff;
	Area[16]=0xff;
	HTFT_voidFillColor ( 0);
	UpdateLive();
	TIM4_voidSetIntervalPeriodic  ( 900000, Start );

}
void PLAY(){
u8 Key;
Key= HIR_voidReturnKey();
switch (Key){

case 12:
	if(global_u8Paddle.x>1){global_u8Paddle.x--;}
	Print_Paddle();

	break;

case 13:
	if(global_u8Paddle.x<6){global_u8Paddle.x++;}
	Print_Paddle();

	break;

case 25: RESET();
		 //Turn_Num=0;
		 break;	// power Button

}

}



void main(void)
 {

	Shape_Num=1;
	/* RCC Initialization */
	RCC_voidInitSysClock();
	RCC_voidEnableClock(RCC_APB2,2);   /* GPIOA */
	RCC_voidEnableClock(RCC_APB2,12);  /* SPI1  */
	//RCC_voidEnableClock(RCC_APB1,0);  /* SPI1  */
	//RCC_voidEnableClock(RCC_APB1,1);  /* SPI1  */
	RCC_voidEnableClock(RCC_APB1,2);  /* SPI1  */
	//RCC_voidEnableClock(RCC_APB1,3);  /* SPI1  */


	MNVIC_voidEnableInterrupt(30);
	MNVIC_voidSetPriority(30,3<<1);
	//TIM2_voidInit();
	//TIM2_voidSetIntervalSingle  ( 5000000, tm1 );
	//TIM3_voidInit();
	//TIM3_voidSetIntervalSingle  ( 6000000, tm2 );
	TIM4_voidInit();
	TIM4_voidSetIntervalPeriodic  ( 900000, Start );
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
	// MSTK_voidSetIntervalPeriodic(5000000,Start);

	/* Display Image */
	HTFT_voidFillColor ( 0);
	HTFT_voidDrawRect (  0,  127, 32, 140, 0);
	UpdateLive();
	 Start();
	 HIR_voidInit();
	 HIR_voidSetCallBack(PLAY);

	while(1){



	}

}


