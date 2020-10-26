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
u8 AlienNum=23;
u8 AlienShootTime=0;
u8 LastRow=0;
s8 XxX=1;
s8 YyY=0;
u8 AliensKilled=0;
u8 MoveArms =0;
u8 Ship_x=30;
struct EggCol{
	u8 x;
	u8 y;
	struct EggCol * Next;
};
struct EggCol * EggList ;
struct EggCol * Bullets ;
struct EggCol * AlienBullets  ;

void AddEgg(struct EggCol ** Head ,u8 Egg_x,u8 Egg_y){

	struct EggCol * Egg = Null;
	Egg = (struct EggCol*)malloc(sizeof(struct EggCol)) ;
	Egg->x=Egg_x;
	Egg->y=Egg_y;
	Egg->Next=*Head;
	*Head=Egg;

}

void DeleteNode(struct EggCol **  Head ,u8 NodeNum){
	if (*Head==Null){return;}

	u8 Count =0;
	struct EggCol * LastEgg, *PreLastEgg=Null;
	LastEgg=*Head;
	if (NodeNum==0){
		* Head = LastEgg ->Next;
		free(LastEgg);//LastEgg);
		return;
	}
	while(LastEgg->Next!=Null && Count !=NodeNum){
		PreLastEgg=LastEgg;
		LastEgg=LastEgg->Next;
		Count++;
	}
	if(Count != NodeNum){return;}
	PreLastEgg->Next=LastEgg->Next;
	free(LastEgg);
}
void UpdateScore(){
	HTFT_voidDrawRect (0,  127, 149 ,159,0);
	Score++;
	s8 SCORE[20];
	sprintf(SCORE,"SCORE %i",Score);

	HTFT_voidDrawText (SCORE,  124, 150 ,1,0xffff);
}



void PrintBullets(struct EggCol * Bullet,u16 Color){
	if(Bullet==Null){return;}
	struct EggCol * Egg;
		Egg=Bullet;
		while(Egg != Null){
			HTFT_voidDrawRect(2*Egg->x,2*Egg->x+1,2*Egg->y,2*Egg->y+4,Color);
			Egg = Egg->Next;
		}
}

void ClearEggs(struct EggCol * Head){
	if (Head==Null){return;}
	struct EggCol * Egg;
		Egg=Head;
		while(Egg != Null){
			HTFT_voidDrawRect(Egg->x*4,Egg->x*4+13,Egg->y*4,Egg->y*4+14,0);
			Egg = Egg->Next;
		}
}

void PrintEggs(struct EggCol * Head,u16 Color){
	if(Head==Null){return;}
	struct EggCol * Egg;
		Egg=Head;
		while(Egg != Null){
			HTFT_voidDrawAlien(Egg->x*4,Egg->y*4,Color,MoveArms);
			Egg = Egg->Next;
		}
}

void UpdateBullet(){
	if(Bullets==Null){return;}
	struct EggCol * Egg;
		Egg=Bullets;
		while(Egg != Null){
			Egg->y++;
			Egg = Egg->Next;
		}
}


void UpdateAlienBullet(){
	if(AlienBullets==Null){return;}
	struct EggCol * Egg;
	u8 BulletHitNum[10];
	u8 BulletNum =0;
	u8 BulletCount=0;
	Egg=AlienBullets;
	while(Egg != Null)
	{
		if(Egg->y==0)
			{
			PrintBullets(AlienBullets,0);
			BulletHitNum[BulletNum++]=BulletCount;
			}
		else if((Egg->x >= Ship_x && Egg->x < Ship_x+5  ) &&Egg->y<=9)
		{
			BulletHitNum[BulletNum++]=BulletCount;
		}
		else
		{
			Egg->y--;
		}
		BulletCount++;
		Egg = Egg->Next;
	}
	for(int i =BulletNum-1;i>=0;i-- )
	{
		DeleteNode(&AlienBullets,BulletHitNum[i]);
	}

}
//increment
void UpdateAliens(s8 Increment_x , s8 Increment_y){
	struct EggCol * Egg;
			Egg=EggList;
			while(Egg != Null){
				Egg->x+=Increment_x;
				Egg->y+=Increment_y;
				Egg = Egg->Next;
			}

}

void Start(){
for(int j =0 ; j<4;j++){
	for(int i =0 ; i<6;i++){
		AddEgg(&EggList,i*5,23+j*4);
	}
}
	PrintEggs(EggList,0xafff);
	HTFT_voidDrawShip ( 2*Ship_x, 0, 0xF800);

}



void BulletHit(){
	u8 BulletHitNum[10];
	u8 BulletNum =0;
	struct EggCol * Bullet,*Alien;
	int BulletCount=0 , AlienCount=0;
	Bullet=Bullets;
	Alien=EggList;
	while(Bullet != Null)
	{
		AlienCount=0;
		Alien=EggList;
		if(Bullet->y>=77)
		{
			PrintBullets(Bullets,0);
			BulletHitNum[BulletNum++]=BulletCount;
		}
		else{
			while(Alien != Null)
			{
				if (Alien->y<=4 )//&& Alien->x *2 ==Ship_x)
				{
					TIM3_voidStopInterval();
					TIM4_voidStopInterval();
					return;
				}
				if( (Bullet->x >= Alien->x*2 && Bullet->x <(Alien->x+3)*2  ) &&Bullet->y==Alien->y *2)
				{
					AlienNum--;
					HTFT_voidDrawRect (  Alien->x*4,  Alien->x*4+16, Alien->y*4, Alien->y*4+11, 0);
					AliensKilled++;
					BulletHitNum[BulletNum++]=BulletCount;
					DeleteNode(&EggList,AlienCount);

					break;
				}
				AlienCount++;
				Alien = Alien->Next;
			}
		}
		Bullet = Bullet->Next;
		BulletCount++;
	}

	for(int i =BulletNum-1;i>=0;i-- )
	{
		DeleteNode(&Bullets,BulletHitNum[i]);
	}

}


void AliensMove(){
	//
	YyY=0;
	u8 i =0;
	struct EggCol * Node=EggList;
	while(i<6 && Node !=Null)
	{
		if(Node->x ==0)
		{
			XxX=1;
			break;
		}
		else if(Node->x ==29)
		{
			YyY=-1;
			XxX=-1;
			break;
		}
		Node=Node->Next;
		i++;
	}

	ClearEggs(EggList);
	UpdateAliens(XxX,YyY);

	PrintEggs(EggList,0xafff);
	MoveArms=(MoveArms+1)%2;
	TIM4_voidSetIntervalSingle  ( 10000000-AliensKilled*420000, AliensMove );
}

void AlienShoot(u8 AlienNum){
	if(EggList==Null){return;}
	struct EggCol* Node=EggList;
	u8 i=0;
	while(Node->Next!=Null && i!=AlienNum){
		Node=Node->Next;
		i++;
	}
	AddEgg(&AlienBullets,Node->x*2+3,Node->y*2-3);

}

void Update(){

				/* Shoot When AlienShootTime=100*/
	AlienShootTime++;
	if(AlienShootTime==100)
	{
		AlienShootTime=0;
		AlienShoot(AlienNum-LastRow);
		LastRow++;
		if(LastRow==6){LastRow=0;}
	}
				/* Alien's Bullet Speed = SpaceShip speed / 4 */
	if(AlienShootTime%4==0)
		{
			PrintBullets(AlienBullets,0);
			UpdateAlienBullet();
			PrintBullets(AlienBullets,0xafff);
		}

				/*SpaceShip Bullet */
	PrintBullets(Bullets,0);
	UpdateBullet();
	PrintBullets(Bullets,0xD680);


	BulletHit();

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
HTFT_voidDrawShip ( 2*Ship_x, 0, 0);
u8 Key;
u8 RepeatCode=HIR_voidCheckRC();
Key= HIR_voidReturnKey();
switch (Key){

case 12:			//move rightwards
	if(RepeatCode)
	{
 	 	 Ship_x=0;
 	}
	else if( Ship_x > 1 )
	{
		Ship_x-=2;
	}

	break;

case 13:			//move leftwards

	if(RepeatCode)
		{
			Ship_x=59;
		}
	else if(Ship_x <58 )
	{

		Ship_x+=2;

	}

	break;

case 25:
		AddEgg(&Bullets,Ship_x+2,10);
		 break;	// power Button

}
HTFT_voidDrawShip ( 2*Ship_x, 0, 0xF800);

}


void main(void)
  {
	EggList = Null;
	Bullets =Null ;
	AlienBullets =Null ;

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
	Start();
	TIM3_voidSetIntervalPeriodic  ( 100000, Update );
	TIM4_voidSetIntervalSingle ( 10000000, AliensMove );
	HIR_voidInit();

	HIR_voidSetCallBack(PLAY);

	while(1){



	}

}


