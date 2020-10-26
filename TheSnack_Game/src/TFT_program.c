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
	MGPIO_VidSetPinValue(TFT_RST_PIN,HIGH);
	MSTK_voidSetBusyWait(100);
	MGPIO_VidSetPinValue(TFT_RST_PIN,LOW);
	MSTK_voidSetBusyWait(1);
	MGPIO_VidSetPinValue(TFT_RST_PIN,HIGH);
	MSTK_voidSetBusyWait(100);
	MGPIO_VidSetPinValue(TFT_RST_PIN,LOW);
	MSTK_voidSetBusyWait(100);
	MGPIO_VidSetPinValue(TFT_RST_PIN,HIGH);
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
void HTFT_voidDrawCircle (u8* Copy_u8Char, u8 x1, u8 y1, u16 Copy_u16Color)
{

for(int i =0 ; i<(8);i++){
	for(int j =0;j<(10);j++){
		if (Copy_u8Char[i] >>(j) &1){
		 HTFT_voidDrawRect (  x1+(j),  x1+(j)+1,  y1+(i),  y1+(i)+1, Copy_u16Color);
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


void HTFT_voidDrawText (u8* Copy_u8Text, u8 x1, u8 y1, u8 Copy_u8Size,u16 Copy_u16Color){
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
	MGPIO_VidSetPinValue(TFT_A0_PIN,LOW);
	
	/* Send Command over SPI */
	MSPI1_voidSendReceiveSynch(Copy_u8Command,&Local_u8Temp);
}

static void voidWriteData   (u8 Copy_u8Data)
{
	u8 Local_u8Temp;
	
	/* Set A0 Pin to High */
	MGPIO_VidSetPinValue(TFT_A0_PIN,HIGH);
	
	/* Send data over SPI */
	MSPI1_voidSendReceiveSynch(Copy_u8Data,&Local_u8Temp);
}






