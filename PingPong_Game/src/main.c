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
u8 Score  = 0;
u8 Score2 = 0;
u8 H_Move = 1;			//1 ->increase  x postion
s8 V_Move = 1;			//1 ->increase  y postion
struct Brick{
	u8 x;
	u8 y;
};

volatile struct Brick global_u8Ball = {7,3};
volatile struct Brick global_u8BallOld = {7,3};
volatile struct Brick global_u8Paddle = {0,10};
volatile struct Brick global_u8Paddle2 = {15,10};


volatile u8 Lives =3;

void Print_Ter(){
	u8 x = global_u8Ball.x , y = global_u8Ball.y;
	HTFT_voidDrawCircle(x*8,y*8,0xffff);

}

void ScoreUpdate(){
	u8 ScoreNum;
	s8 SCORE[5];
	HTFT_voidDrawRect (11,  45, 130 ,147,0);
	HTFT_voidDrawRect (75,  110, 130 ,147,0);
	ScoreNum=sprintf(SCORE,"%i",Score);
	HTFT_voidDrawText (SCORE,  35+ScoreNum*5, 130 ,2,0Xfffe);
	ScoreNum=sprintf(SCORE,"%i",Score2);
	HTFT_voidDrawText (SCORE,  100+ScoreNum*5, 130 ,2,0Xfffe);
}
void Screen_Update(){

	//HTFT_voidDrawRect (  0,  63, 0, 159, 0);
	if(global_u8Ball.x==5||global_u8Ball.x==9){
	for(int i=0;i<20;i++)
	{
		HTFT_voidDrawRect(63,63,i*8,i*8+8,0xffff*((1+i)%2));
		HTFT_voidDrawRect(63,63,i*8,i*8+8,0xffff*((1+i)%2));
	}}
	//HTFT_voidDrawRect (  64,  127, 0, 159, 0);

	u8 x = global_u8BallOld.x , y = global_u8BallOld.y;
	HTFT_voidDrawCircle(x*8,y*8,0);
	//Print_Ter();

}

void CheckWin(){
	TIM4_voidStopInterval();
	HTFT_voidDrawRect (  0, 127, 0,159, 0x6808);
	HTFT_voidDrawText ("U WON",  115, 50 ,4,0Xfffe);

}

void Update_Ter(){
	global_u8BallOld=global_u8Ball;
	u8  Ball_y=global_u8Ball.y ,
		Ball_x=global_u8Ball.x,
		Paddle_y =global_u8Paddle.y,
		Paddle2_y =global_u8Paddle2.y;
	if (H_Move){										//moving leftward
		if( Ball_x  == 15 )
		{
			Score++;
			H_Move=0;
			global_u8Ball.x--;
			ScoreUpdate();

		}
		else if ( Ball_x+1==15  )
			{
			if(Ball_y>=Paddle2_y-2 &&Ball_y<Paddle2_y ){H_Move=0;global_u8Ball.x--;	V_Move=-1;}
			else if(Ball_y==Paddle2_y){H_Move=0;global_u8Ball.x--;V_Move=0;}
			else if(Ball_y>Paddle2_y &&Ball_y<=Paddle2_y+2){H_Move=0;global_u8Ball.x--;V_Move=1;}
			else{global_u8Ball.x++;}

			}
		else
		{
			global_u8Ball.x++;
		}
	}
	else{												//moving rightward
		if( Ball_x  == 0 ) {
				Score2++;
				ScoreUpdate();
				H_Move=1;
				global_u8Ball.x++;

		}
		else if ( Ball_x-1==0)
		{
			if(Ball_y>=Paddle_y-2 &&Ball_y<Paddle_y ){H_Move=1;global_u8Ball.x++;	V_Move=-1;}
			else if(Ball_y==Paddle_y){H_Move=1;global_u8Ball.x++;V_Move=0;}
			else if(Ball_y>Paddle_y &&Ball_y<=Paddle_y+2){H_Move=1;global_u8Ball.x++;V_Move=1;}
			else{global_u8Ball.x--;}

		}else
		{
			global_u8Ball.x--;
		}

	}
	if (V_Move==1){										//moving upward
			if( Ball_y  == 19  )
			{
				V_Move=-1;
				global_u8Ball.y--;

			}
			else
			{
				global_u8Ball.y++;
			}

		}
		else if (V_Move==-1){												//moving downward
			if( Ball_y  == 0 ) {
						V_Move=1;
						global_u8Ball.y++;
			}
			else
			{
				global_u8Ball.y--;
			}

		}


}

void Print_Paddle(){
	u8  Paddle_y =global_u8Paddle.y-2, Paddle_y2 =global_u8Paddle2.y;
	for (int i =0;i<20;i++){
		if(i==Paddle_y ){
			for(int j =0 ; j<=4;j++){
			//HTFT_voidDrawRect (  5,  7, i*8+4, i*8+7, 0xffff);
			HTFT_voidDrawRect (  6,  7, (i+j)*8, (i+j)*8+7, 0xffff);
			//HTFT_voidDrawRect (  5,  7, (i+2)*8, (i+2)*8+7, 0xffff);
			//HTFT_voidDrawRect (  5,  7, (i+3)*8, (i+3)*8+7, 0xffff);
			//HTFT_voidDrawRect (  5,  7, (i+4)*8, (i+4)*8+4, 0xffff);
			}
			i+=4;
			continue;
		}
		HTFT_voidDrawRect (  6,  7, i*8, i*8+8, 0);
	}

}
void PrintPaddle2(){
	u8  Paddle_y2 =global_u8Paddle2.y-2;
	for (int i =0;i<20;i++){
		if(i==Paddle_y2 ){
			for(int j =0 ; j<=4;j++)
			{
				HTFT_voidDrawRect (  120,  121, (i+j)*8, (i+j)*8+7, 0xffff);
			}
			i+=4;
			continue;
		}
		HTFT_voidDrawRect (  120,  121, i*8, i*8+8, 0);
	}

}

void Start(){

	Screen_Update();
	Print_Paddle();
	if(global_u8Ball.x>=2&&global_u8Ball.y >=16){ScoreUpdate();}
	Print_Ter();
	Update_Ter();
}
void RESET(){
	global_u8Ball.x=7;
	global_u8Ball.y=3;
	Score=0;
	Score2=0;
	HTFT_voidFillColor ( 0);

	//TIM4_voidSetIntervalPeriodic  ( 900000, Start );

}
void PLAY(){
u8 Key;
Key= HIR_voidReturnKey();
switch (Key){

case 9:
	global_u8Paddle.y=17;
	Print_Paddle();

	break;
case 27:
	global_u8Paddle.y=2;
	Print_Paddle();

	break;


case 10:
	if(global_u8Paddle.y<18){global_u8Paddle.y++;}
	Print_Paddle();

	break;

case 11:
	if(global_u8Paddle.y>1){global_u8Paddle.y--;}
	Print_Paddle();

	break;

case 25: RESET();
		 //Turn_Num=0;
		 break;	// power Button

}

}
void UpdatePaddle2(){
	u8 Paddle2_y = global_u8Paddle2.y , Ball_y=global_u8Ball.y;

	if(Paddle2_y+2 <Ball_y && Paddle2_y<17){
		global_u8Paddle2.y++;
	}
	else if(Paddle2_y-2 >Ball_y && Paddle2_y>2){
		global_u8Paddle2.y--;
	}
	else{
		global_u8Paddle2.y+=V_Move;
	}
	PrintPaddle2();
}


void main(void)
 {

	Shape_Num=1;
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
	// MSTK_voidSetIntervalPeriodic(5000000,Start);

	/* Display Image */
 	HTFT_voidFillColor ( 0);
	HTFT_voidDrawRect (  0,  63, 0, 160, 0);
	for(int i=0;i<20;i++)
	{
		HTFT_voidDrawRect(63,63,i*8,i*8+8,0xffff);
	}
	HTFT_voidDrawRect (  64,  127, 0, 160, 0);
	ScoreUpdate();
	MSTK_voidSetBusyWait(500000);
	PrintPaddle2();

	Start();
	TIM4_voidSetIntervalPeriodic  ( 500000, Start );
	TIM3_voidSetIntervalPeriodic  ( 2000000, UpdatePaddle2 );

	HIR_voidInit();
	HIR_voidSetCallBack(PLAY);

	while(1){



	}

}


