#include <stdio.h>
#include "STD_TYPES.h"
#include "BIT_MATH.h"

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
volatile u8 Winner=0;
volatile u8 Turn_Num=0;

volatile u8 Area [3][3]={
		{0}
};
void Screen_Update(){
u8 character_num;
u8 character ;
u16 color;
	for(int y =0;y<3;y++){
		for(int x =0 ; x<3;x++){
			character_num=Area[y][x];
			if( !character_num )    //empty block
			{
				HTFT_voidDrawFrameRect (  x*42,  x*42+42, y*53, y*53+53, 0);
			}
			else{
				character = 'O';color=0x47E0;
				if(character_num==1){character='X';color=0xF800;}
				HTFT_voidDrawFrameRect (  x*42,  x*42+42, y*53, y*53+53, 0);
				HTFT_voidDrawChar (character, x*42+8,  y*53+8, 5,color);
			}
		}
	}


}

void EndGame(u8 Line){
	Screen_Update();
	if (Line <=2){ HTFT_voidDrawRect (  6,  122,  Line*53+24, Line*53+27, 0xffff);}		//draw Horizontal line
	else if (Line<=5){																	//draw Vertical line
		Line %=3;
		HTFT_voidDrawRect (  Line*42+19,  Line*42+22,  6, 154, 0xffff);}
	else if (Line==6){																	//draw \ line
		for(int i=18;i<120;i++){ HTFT_voidDrawRect (  i,  i+2,  i+12, i+14, 0xffff);}
	}
	else if (Line==7){																	//draw / line
		for(int i=120;i>18;i--){
			HTFT_voidDrawRect (  136-i,  136-i+2,  i+12, i+14, 0xffff);
		}
	}

}

u8 CheckWinner(){
	u8 element=Area[0][0];
	u8 WIN =1;
	u8 line=0;
for(int y=0;y<3;y++){ //check row by row
	element=Area[y][0];
	WIN=1;
	for(int x =1;x<3;x++){
		if(element!=Area[y][x] || element==0){WIN=0;break;}
	}
	if (WIN){Winner=element;return line;}
	line++;
}


for(int x=0;x<3;x++){ //check column by column
	element=Area[0][x];
	WIN=1;
	for(int y =1;y<3;y++){
		if(element!=Area[y][x]  || element==0){WIN=0;break;}
	}
	if (WIN){Winner=element;return line;}
	line++;
}

WIN=1;
element=Area[0][0];
for(int i=1;i<3;i++){    				 /*check '\' line */

	if(element!=Area[i][i]|| element==0){WIN=0;break;}
}
if (WIN){Winner=element;return line;}
line++;

WIN=1;
element=Area[0][2];
for(int i=1;i<3;i++){ //check /
	if(element!=Area[i][2-i]|| element==0){WIN=0;break;}
}
if (WIN){Winner=element;return line;}
else{return 10;} // no winner
}


void Start(){
		u8 w =CheckWinner();
		if (w!=10){
			EndGame(w);
			MSTK_voidSetBusyWait(2500000);
			HTFT_voidFillColor ( 0xD562);
			if(Winner==1){HTFT_voidDrawChar ( 'X', 52, 80, 6,0xF800);}
			else{HTFT_voidDrawChar ( 'O', 52, 80, 6,0x0441);}
			HTFT_voidDrawText ("WINNER|", 124,40, 3,0xffff);
			return;
		}

		Screen_Update();
		Turn_Num++;
		if(Turn_Num==10){
		HTFT_voidFillColor ( 0x6808);
		HTFT_voidDrawText ("DRAW|", 105,55, 3,0xffff);
		return;
		}


}
void RESET(){
	for(int i =0;i<3;i++)
	{
		for(int j =0 ; j<3;j++){
		Area[i][j]=0;
	}
	}

}
void PLAY(){
u8 Key;
Key= HIR_voidReturnKey();
switch (Key){

case 12:	if (Area[2][2]==0){				// Key  1
		SET_BIT(Area [2][2],Shape_Num-1);
		Shape_Num=2-(Shape_Num+1)%2;
		Start();
		}
	break;

case 24:	if (Area[2][1]==0){				// Key  2
		SET_BIT(Area [2][1],Shape_Num-1);
		Shape_Num=2-(Shape_Num+1)%2;
		Start();
		}
	break;

case 94:	if (Area[2][0]==0){				// Key  3
		SET_BIT(Area [2][0],Shape_Num-1);
		Shape_Num=2-(Shape_Num+1)%2;
		Start();
		}
	break;

case 8:	if (Area[1][2]==0){				// Key  4
		SET_BIT(Area [1][2],Shape_Num-1);
		Shape_Num=2-(Shape_Num+1)%2;
		Start();

		}
	break;

case 28:	if (Area[1][1]==0){				// Key  5
		SET_BIT(Area [1][1],Shape_Num-1);
		Shape_Num=2-(Shape_Num+1)%2;
		Start();

		}
	break;

case 90:	if (Area[1][0]==0){				// Key  6
		SET_BIT(Area [1][0],Shape_Num-1);
		Shape_Num=2-(Shape_Num+1)%2;
		Start();

		}
	break;

case 66:	if (Area[0][2]==0){				// Key  7
		SET_BIT(Area [0][2],Shape_Num-1);
		Shape_Num=2-(Shape_Num+1)%2;
		Start();

		}
	break;

case 82:	if (Area[0][1]==0){				// Key  8
		SET_BIT(Area [0][1],Shape_Num-1);
		Shape_Num=2-(Shape_Num+1)%2;
		Start();

		}
	break;

case 74:	if (Area[0][0]==0){				// Key  9
		SET_BIT(Area [0][0],Shape_Num-1);
		Shape_Num=2-(Shape_Num+1)%2;
		Start();

		}
	break;

case 69: RESET();
		 Turn_Num=0;
		 Start();
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


	// HTFT_voidDrawRect (  0,  127, 0, 140, 0x0280);
	 Start();
	 HIR_voidInit();
	 HIR_voidSetCallBack(PLAY);


	while(1){



	}

}


