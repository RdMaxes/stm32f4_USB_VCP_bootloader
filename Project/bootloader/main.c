/*
//@brief :
//@Author:RdMaxes
//@Data  :
//@Note  :
*/

//Includes 
#include <stm32f4xx.h>
#include "led.h"
#include "usart2.h"
#include "myprintf.h"
#include "timer3.h"

#include "usbd_cdc_core.h"
#include "usbd_usr.h"
#include "usbd_desc.h"
#include "usbd_cdc_vcp.h"

__ALIGN_BEGIN USB_OTG_CORE_HANDLE  USB_OTG_dev __ALIGN_END;

int8_t test_buf[32];
//Delay for a while
//time: delay time
static void delay(int32_t time)
{
	while(time--);
}
extern uint32_t USB_ENTER_CNT;
int main(void)
{	
	int8_t i = 0;
	LED_Init();
	Usart2_Init(9600);
	Myprintf_Init(0x00,myputc);
	Timer3_Interrupt_Init(50,84);
	USBD_Init(&USB_OTG_dev,USB_OTG_FS_CORE_ID,&USR_desc,&USBD_CDC_cb,&USR_cb);
	
	while(1) 
	{	
		LED_loop();
	}

	return 0;
}
//////////////////////////Interrupr Functions for Timer3///////////////////////////
///////////////////////////////////////////////////////////////////////////////////
void TIM3_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)
    {
    	if((USB_RX_STATUS>>7)==USB_RX_DRDY)	//check if data is ready
    	{
			LED_loop();
			USB_RX_STATUS = 0x00; //clear status
		}
		TIM_ClearITPendingBit(TIM3,TIM_FLAG_Update);	
    }
}//////////////////////////End Interrupr Functions for Timer3//////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

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