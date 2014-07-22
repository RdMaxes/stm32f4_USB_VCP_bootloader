/*
//@brief :Practice IAP via USB_VCP interface.
//		  	Data send out from USB Out endpoint is collected in VCP_DataRx(), a while loop
//		  is waiting until the data is read. Meanwhile, in this practice the Timer3 is configured
//        with period interrupt, which has higher ISR priority then USB ISR. Data are read out in
//		  Timer3 interrupt function and also the status flag is clear for next data package.
//@Author:RdMaxes
//@Data  :2014/07/22
//@Note  :
*/

//Includes 
#include <stm32f4xx.h>
#include "led.h"
#include "usart2.h"
#include "myprintf.h"
#include "stmflash.h"
#include "ymodem.h"

#include "usbd_cdc_core.h"
#include "usbd_usr.h"
#include "usbd_desc.h"
#include "usbd_cdc_vcp.h"

__ALIGN_BEGIN USB_OTG_CORE_HANDLE  USB_OTG_dev __ALIGN_END;
typedef  void (*pFunction)(void);

//Global Variables
uint8_t FileName[FILE_NAME_LENGTH];//array to store filename of download *.bin
uint8_t buf_1k[1024] ={0};
pFunction Jump_To_Application;
uint32_t JumpAddress;

//Delay for a while
//time: delay time
static void delay(int32_t time)
{
	while(time--);
}

uint8_t test_buf[64] = "String Show test stm32f4xx \r\n";
int main(void)
{	
	char c = 0;
	LED_Init();
	Usart2_Init(9600);
	Myprintf_Init(0x00,myputc);
	USBD_Init(&USB_OTG_dev,USB_OTG_FS_CORE_ID,&USR_desc,&USBD_CDC_cb,&USR_cb);
	//Unlock the Flash Program Erase controller
	STM_FLASH_Init();

	do
	{
		USB_VCP_TxString("Please type 'Q' to start IAP\r\n");
		USB_VCP_RxChar(&c);//check if there's data input
	}while(c!='Q')

	while(1) 
	{	
		LED_loop();
	}

	return 0;
}

//////////////////////////Interrupr Functions for USB//////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
#ifdef USE_USB_OTG_FS
void OTG_FS_WKUP_IRQHandler(void)
{
  if(USB_OTG_dev.cfg.low_power)
  {
    *(uint32_t *)(0xE000ED10) &= 0xFFFFFFF9 ;
    SystemInit();
    USB_OTG_UngateClock(&USB_OTG_dev);
  }
  EXTI_ClearITPendingBit(EXTI_Line18);
}
#endif

/**
  * @brief  This function handles OTG_HS Handler.
  * @param  None
  * @retval None
  */
#ifdef USE_USB_OTG_HS
void OTG_HS_IRQHandler(void)
#else
void OTG_FS_IRQHandler(void)
#endif
{
  USBD_OTG_ISR_Handler (&USB_OTG_dev);
}
//////////////////////////End Interrupr Functions for USB//////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////