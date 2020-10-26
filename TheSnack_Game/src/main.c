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
#include "TFT_private.h"


#define Null (void *) 0
u8 count=0;
u8 stack[40][2];
u8 Move_H=1;
u8 Score = 0;
struct Node{
	u8 x;
	u8 y;
	struct Node * next ;
};

 struct Node * Head =Null;
 struct Node * Apple=Null;

 void Add_Apple(){
	 u8 Circle[]={ 0,28, 62, 127, 127, 127, 62, 28};

	 HTFT_voidDrawCircle ( Circle, Apple->x, Apple->y, 0xF104);

 }
void Screen_Update(){


			 HTFT_voidDrawRect (  10,  110, 10, 120, 0x2EA5);

}
void Lose(){
	 HTFT_voidDrawRect (  0,127, 0,160, 0);
	 MSTK_voidStopInterval();
	 HTFT_voidDrawText ("U LOST", 110,60 , 2,0xfe0);

}
void Add_Node(){
	struct Node * node;// = (struct Node*)malloc(sizeof(struct Node)) ;
	node = Head;
	while(node->next != Null){
		node=node->next;
	}
	struct Node *New =Null;
	New = (struct Node*)malloc(sizeof(struct Node)) ;
	New->next=0;New->x=128;
	node->next = New;

}
void Update_Snake(){
	u8 flag =0;
	u8 count =0;
	struct Node * node;
	node = Head;
//	node2 = node ;
	while(node->next != Null){

		if(flag){
		stack[count][0]=node->x;
		stack[count][1] = node->y;
		node->x=stack[count-1][0];
		node->y=stack[count-1][1];
		count++;
		node=node->next;
		}
		else{
		flag =1;
		stack[count][0] = node->x;
		stack[count][1] = node->y;

		count++;
		node=node->next;
	}}

}

void Print_Snake(){

	struct Node * node;
	node = Head;
	HTFT_voidDrawRect (  Head->x,  Head->x +10, Head->y, Head->y+10, 0x099D);
	while(node->next != Null){
		node=node->next;
		HTFT_voidDrawRect (  node->x, node->x + 10, node->y, node->y+10, 0x099D);
		if(Head->x==node->x && Head->y==node->y){Lose();}
	}

}

void Update_Score(){
	Score++;
	u8 score[20];
	sprintf(score,"SCORE : %i",Score);
	HTFT_voidDrawRect (  60, 120,144,160, 0x01C0);
	 HTFT_voidDrawText (score,  120, 145 ,1,0Xffff);

}
void Start(){
				Update_Snake();
				Move_Update();
				if (Move_H){
					Head->x +=10;
					if(Head->x >100){
					Head->x =10;
					}
				}else{
					Head->y +=10;
					if(Head->y > 110){
						Head->y=10;
					}
				}
				Print_Snake();
				if(Head->x==Apple->x && Head->y==Apple->y){
				Update_Score();
				Add_Node();
				Apple->y =Head->x;
				Apple->x+=30;
				if(Apple->x > 110){Apple ->x=30;}
				}


}

void Move_Update(){
Screen_Update();

Add_Apple();

}

void Controller(){
	Move_H = (Move_H+1)%2;
	for(int i=0;i<100000;i++){asm("NOP");}
}
void main(void)
{
	/* RCC Initialization */
	RCC_voidInitSysClock();
	RCC_voidEnableClock(RCC_APB2,2);   /* GPIOA */
	RCC_voidEnableClock(RCC_APB2,12);  /* SPI1  */

	/* Pin Initializations */
	MGPIO_VidSetPinDirection(GPIOA,3,0b1000);
	MGPIO_VidSetPinValue(GPIOA,3,1);//input pull up
	MGPIO_VidSetPinDirection(GPIOA,1,0b0010);  /* A0   */
	MGPIO_VidSetPinDirection(GPIOA,2,0b0010);  /* RST  */
	MGPIO_VidSetPinDirection(GPIOA,5,0b1001);  /* CLK  */
	MGPIO_VidSetPinDirection(GPIOA,7,0b1001);  /* MOSI */
	MNVIC_voidEnableInterrupt(6+3);
	MEXTI_voidSetCallBack(3 , Controller);
	MEXTI_voidEnableEXTI(3);
	MEXTI_voidSetSignalLatch(3 , FALLING);
	/* Init STK */
	MSTK_voidInit();

	/* SPI Init */
	MSPI1_voidInit();

	/* TFT Init */
	HTFT_voidInitialize();

	/* Display Image */

	 HTFT_voidFillColor ( 0x01C0);
	 HTFT_voidDrawRect (  0,  127, 0, 140, 0x0280);
	 HTFT_voidDrawText ("SCORE : 0",  120, 145 ,1,0Xffff);

	 MSTK_voidSetIntervalPeriodic(3000000,Start);
    Head = (struct Node*)malloc(sizeof(struct Node));
    Head->next =Null;
    Apple = (struct Node*)malloc(sizeof(struct Node));
    Apple->next=Null;


   Head->x=10;

Head->y= 20;
Apple->x=10;
Apple->y=20;
Add_Node();
	while(1){



		//Update_Snake();


	}

}

