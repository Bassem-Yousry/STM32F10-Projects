/***********************************************************/
/* Author    : Ahmed Assaf                                 */
/* Date      : 21 Sep 2020                                 */
/* Version   : V01                                         */
/***********************************************************/
#include "STD_TYPES.h"
#include "BIT_MATH.h"

#include "DIO_interface.h"
#include "SPI_interface.h"
#include "STK_interface.h"

#include "TFT_interface.h"
#include "TFT_private.h"
#include "TFT_config.h"

void HTFT_voidInitialize   (void)
{
	/* Reset Pulse */
	DIO_voidSetPinValue(TFT_RST_PIN,HIGH);
	MSTK_voidSetBusyWait(100);
	DIO_voidSetPinValue(TFT_RST_PIN,LOW);
	MSTK_voidSetBusyWait(1);
	DIO_voidSetPinValue(TFT_RST_PIN,HIGH);
	MSTK_voidSetBusyWait(100);
	DIO_voidSetPinValue(TFT_RST_PIN,LOW);
	MSTK_voidSetBusyWait(100);
	DIO_voidSetPinValue(TFT_RST_PIN,HIGH);
	MSTK_voidSetBusyWait(120000);
	
	/* Sleep Out Command */
	voidWriteCommand(0x11);
	
	/* Wait 150 ms */
	MSTK_voidSetBusyWait(150000);
	
	/* Color Mode Command */
	voidWriteCommand(0x3A);
	voidWriteData   (0x05);
	
	/* Display On Command */
	voidWriteCommand(0x29);

}


void HTFT_voidDisplayImage (const u16* Copy_Image)
{
	u16 counter;
	u8 Data;

	/* Set X Address */
	voidWriteCommand(0x2A);
	voidWriteData(0);
	voidWriteData(0);
	voidWriteData(0);
	voidWriteData(127);
	
	/* Set Y Address */
	voidWriteCommand(0x2B);
	voidWriteData(0);
	voidWriteData(0);
	voidWriteData(0);
	voidWriteData(159);

	/* RAM Write */
	voidWriteCommand(0x2C);

	for(counter = 0; counter< 20480;counter++)
	{
		Data = Copy_Image[counter] >> 8;

		/* Write the high byte */
		voidWriteData(Data);
		/* Write the low byte */
		Data = Copy_Image[counter] & 0x00ff;
		voidWriteData(Data);
	}


}

void HTFT_voidFillColor ( u16 Copy_u16Color)
{
	u16 counter;
	u8 Data;

	/* Set X Address */
	voidWriteCommand(0x2A);
	voidWriteData(0);
	voidWriteData(0);
	voidWriteData(0);
	voidWriteData(127);

	/* Set Y Address */
	voidWriteCommand(0x2B);
	voidWriteData(0);
	voidWriteData(0);
	voidWriteData(0);
	voidWriteData(159);

	/* RAM Write */
	voidWriteCommand(0x2C);

	for(counter = 0; counter< 20480;counter++)
	{
		Data = Copy_u16Color >> 8;

		/* Write the high byte */
		voidWriteData(Data);
		/* Write the low byte */
		Data = Copy_u16Color & 0x00ff;
		voidWriteData(Data);
	}


}
void HTFT_voidDrawFrameRect ( u8 x1, u8 x2, u8 y1, u8 y2, u16 Copy_u16Color,u16 Copy_u16FColor)
{
	u8 Height = y2-y1+1;

	HTFT_voidDrawRect(x1,x2,y1,y1,Copy_u16FColor);
	Height--;
	y1++;
	while(Height>1){
		HTFT_voidDrawRect(x1,x1,y1,y1,Copy_u16FColor);
		HTFT_voidDrawRect(x1+1,x2-1,y1,y1,Copy_u16Color);
		HTFT_voidDrawRect(x2,x2,y1,y1,Copy_u16FColor);
		Height--;
		y1++;
		}
	HTFT_voidDrawRect(x1,x2,y1,y1,Copy_u16FColor);
}

void HTFT_voidDrawRect ( u8 x1, u8 x2, u8 y1, u8 y2, u16 Copy_u16Color)
{
	u16 counter , size;
	u8 Data;
	size = (x2-x1+1)*(y2-y1+1);
	/* Set X Address */
	voidWriteCommand(0x2A);
	voidWriteData(0);
	voidWriteData(x1);
	voidWriteData(0);
	voidWriteData(x2);

	/* Set Y Address */
	voidWriteCommand(0x2B);
	voidWriteData(0);
	voidWriteData(y1);
	voidWriteData(0);
	voidWriteData(y2);

	/* RAM Write */
	voidWriteCommand(0x2C);

	for(counter = 0; counter< size;counter++)
	{
		Data = Copy_u16Color >> 8;

		/* Write the high byte */
		voidWriteData(Data);
		/* Write the low byte */
		Data = Copy_u16Color & 0x00ff;
		voidWriteData(Data);
	}


}
void HTFT_voidDrawCircle ( u8 x1, u8 y1, u16 Copy_u16Color)
{
u8 Copy_u8Char[8] = {0, 28, 62, 127, 127, 127, 62, 28};
for(int i =0 ; i<(8);i++){
	for(int j =0;j<8;j++){
		if (GET_BIT(Copy_u8Char[i],j) ){
		 HTFT_voidDrawRect (  x1+j,  x1+j+1,  y1+(i),  y1+(i)+1, Copy_u16Color);
		}
	}
}

}

void HTFT_voidDrawPac ( u8 x1, u8 y1, u16 Copy_u16Color,u8 Rotation,u8 Mouth)
{
	//0 -> Mouth Closed     , else Opened
u8 Pac [8] = {28, 62, 112, 96, 112, 62, 28,0};
if (!Mouth){				//close the Mouth
	for(int m=2;m<=4;m++){
		Pac[m]=127;
	}
}
int y,x ;
for( int v =0 ; v<(8);v++){
	for(int h =0;h<8;h++){
		if(Rotation==0){x = 7-v; y=h;}
		else if(Rotation==1){x=7-h; y=v;}
		else if(Rotation==2){x = h; y=v;}
		else{x = v; y=h;}

		if (GET_BIT(Pac[y],x) ){
		 HTFT_voidDrawRect (  x1+h,  x1+h,  y1+(v),  y1+(v), Copy_u16Color);
		}
		else{
			 HTFT_voidDrawRect (  x1+h,  x1+h,  y1+(v),  y1+(v), 0);

		}
	}
}
}

void HTFT_voidDrawGhost( u8 x1, u8 y1, u16 Copy_u16Color)
{
u8 Ghost [7] = {73, 107, 127, 127, 107, 62, 28, 0};
for(int i =0 ; i<(7);i++){
	for(int j =0;j<7;j++){
		if (GET_BIT(Ghost[i],j) ){
		 HTFT_voidDrawRect (  x1+j,  x1+j,  y1+(i),  y1+(i), Copy_u16Color);
		}
	}
}
}
void HTFT_voidDrawHeart ( u8 x1, u8 y1, u16 Copy_u16Color)
{
u16 Copy_u8Char[11] = {192, 480, 2040, 4092, 4092, 8190, 16383, 16383, 16383, 7998, 3612};
for(int i =0 ; i<(11);i++){
	for(int j =0;j<16;j++){
		if (GET_BIT(Copy_u8Char[i],j) ){
		 HTFT_voidDrawRect (  x1+j,  x1+j,  y1+(i),  y1+(i), Copy_u16Color);
		}
	}
}
}

void HTFT_voidDrawChar (u8 Copy_u8Char, u8 x1, u8 y1, u8 Copy_u8Size,u16 Copy_u16Color)
{
	if(Copy_u8Char==32){return;}
	 Copy_u8Char -=48;

for(int i =0 ; i<(8* Copy_u8Size);i++){
	for(int j =0;j<(5*Copy_u8Size);j++){
		if (Char_Array[Copy_u8Char][i/Copy_u8Size] >>(j/Copy_u8Size) &1){
		 HTFT_voidDrawRect (  x1+(j),  x1+(j)+1,  y1+(i),  y1+(i), Copy_u16Color);
		}
	}
}

}


void HTFT_voidDrawText (s8* Copy_u8Text, u8 x1, u8 y1, u8 Copy_u8Size,u16 Copy_u16Color){
	for(; *Copy_u8Text; Copy_u8Text++ ){
		if(*Copy_u8Text==32){x1-=3;}
		else{x1-=6*Copy_u8Size;
		HTFT_voidDrawChar ( *Copy_u8Text,   x1,  y1,  Copy_u8Size, Copy_u16Color);
		}
	}

}

void voidWriteCommand(u8 Copy_u8Command)
{
	u8 Local_u8Temp;
	
	/* Set A0 Pin to Low */
	DIO_voidSetPinValue(TFT_A0_PIN,LOW);
	
	/* Send Command over SPI */
	MSPI1_voidSendReceiveSynch(Copy_u8Command,&Local_u8Temp);
}

static void voidWriteData   (u8 Copy_u8Data)
{
	u8 Local_u8Temp;
	
	/* Set A0 Pin to High */
	DIO_voidSetPinValue(TFT_A0_PIN,HIGH);
	
	/* Send data over SPI */
	MSPI1_voidSendReceiveSynch(Copy_u8Data,&Local_u8Temp);
}





