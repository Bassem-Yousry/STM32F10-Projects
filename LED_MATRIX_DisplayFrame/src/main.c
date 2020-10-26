
#include "STD_TYPES.h"
#include "BIT_MATH.h"

#include "HLEDMAT_interface.h"


#include "STK_interface.h"
#include "DIO_interface.h"
#include "RCC_interface.h"
#include "SSD_interface.h"

int main (){

	RCC_voidInitSysClock();
	MSTK_voidInit();
	RCC_voidEnableClock (RCC_APB2,2);
	RCC_voidEnableClock (RCC_APB2,3);
	DIO_voidSetPinDirection(GPIOA , PIN0 , OUTPUT_SPEED_10MHZ_PP);




	DIO_voidSetPinDirection(GPIOA , PIN0 , OUTPUT_SPEED_10MHZ_PP);
	DIO_voidSetPinDirection(GPIOA , PIN1 , OUTPUT_SPEED_10MHZ_PP);
	DIO_voidSetPinDirection(GPIOA , PIN2 , OUTPUT_SPEED_10MHZ_PP);
	DIO_voidSetPinDirection(GPIOA , PIN3 , OUTPUT_SPEED_10MHZ_PP);
	DIO_voidSetPinDirection(GPIOA , PIN4 , OUTPUT_SPEED_10MHZ_PP);
	DIO_voidSetPinDirection(GPIOA , PIN5 , OUTPUT_SPEED_10MHZ_PP);
	DIO_voidSetPinDirection(GPIOA , PIN6 , OUTPUT_SPEED_10MHZ_PP);
	DIO_voidSetPinDirection(GPIOA , PIN7 , OUTPUT_SPEED_10MHZ_PP);
	u8 FRAME[]= { 60, 102, 194, 132, 194, 102, 60, 0, 0, 0, 0, 224, 96, 96, 32, 32, 32, 60, 32, 60, 32, 60, 0, 63, 160, 32, 56, 0,0, 0,0,0,0,0,0,0,0};
	 HLEDMAT_voidInit();
	while (1){
	HLEDMAT_voidDisplayShiftFrame(FRAME,2,37);

		/*
for (int j=0;j<4;j++){
for (int i =0 ; i<8;i++){
	 DIO_voidSetPortValue (GPIOA,1<<i);
delay (200000);


}
for (int i =7 ; i>-1;i--){
	 DIO_voidSetPortValue (GPIOA,1<<i);
delay (200000);


}
}


DIO_voidSetPortValue (GPIOA,0);

for (int j=0;j<4;j++){

for (int i =0;i<=3;i++){
	DIO_voidSetPinValue (GPIOA,3-i,HIGH);
	DIO_voidSetPinValue (GPIOA,4+i,HIGH);
	if (i !=4){delay(200000);}
}

for (int i =0;i<3;i++){
	DIO_voidSetPinValue (GPIOA,7-i,LOW);
	DIO_voidSetPinValue (GPIOA,0+i,LOW);
	delay(200000);
}

}


for (int y =0 ; y<8;y++){
DIO_voidSetPortValue (GPIOA,0xff);
delay(500000);
DIO_voidSetPortValue (GPIOA,0);
delay(500000);

}
//DIO_voidSetPinValue (GPIOA,1,HIGH);


		//void DIO_voidSetPortValue (u8 Copy_u8Port, u8 Copy_u8Value);


		// DIO_voidSetPinValue (GPIOA,1,HIGH);

*/
}
}
