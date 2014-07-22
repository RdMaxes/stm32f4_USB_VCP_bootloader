/**
 ******************************************************************************
 * @file    usbd_cdc_vcp.c
 * @author  MCD Application Team
 * @version V1.0.0
 * @date    22-July-2011
 * @brief   Generic media access Layer.
 ******************************************************************************
 * @attention
 *
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
 ******************************************************************************
 */

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED 
#pragma     data_alignment = 4 
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */

/* Includes ------------------------------------------------------------------*/
#include "usbd_cdc_vcp.h"
#include "stm32f4xx_conf.h"


LINE_CODING g_lc;


//Global Variables @Add by RdMaxes
uint8_t USB_RX_STATUS = 0x00; //Record data received status from USB Out endpoint
                           //Default setting of USB Rx maximun length is 64 bytes
                           //bit 7::  0=data not ready, 1=data ready
                           //bit 6~0:: Received data length
uint8_t* ptrUSB_Rx_Buf = NULL; //data pointer to USB Out endpoint data buffer


/* These are external variables imported from CDC core to be used for IN 
 transfer management. */
extern uint8_t APP_Rx_Buffer[]; /* Write CDC received data in this buffer.
 These data will be sent over USB IN endpoint
 in the CDC core functions. */
extern uint32_t APP_Rx_ptr_in; /* Increment this pointer or roll it back to
 start address when writing received data
 in the buffer APP_Rx_Buffer. */

/* Private function prototypes -----------------------------------------------*/
static uint16_t VCP_Init(void);
static uint16_t VCP_DeInit(void);
static uint16_t VCP_Ctrl(uint32_t Cmd, uint8_t* Buf, uint32_t Len);

CDC_IF_Prop_TypeDef VCP_fops = {VCP_Init, VCP_DeInit, VCP_Ctrl, VCP_DataTx, VCP_DataRx };

/* Private functions ---------------------------------------------------------*/
/**
 * @brief  VCP_Init
 *         Initializes the Media on the STM32
 * @param  None
 * @retval Result of the opeartion (USBD_OK in all cases)
 */
static uint16_t VCP_Init(void)
{
//   NVIC_InitTypeDef NVIC_InitStructure;


   //RSW HACK, don't set up the port here, because we don't know what the OS wants for speed
   //settings yet...  do this in VCP_Ctrl instead..

   /* Enable USART Interrupt */
//   NVIC_InitStructure.NVIC_IRQChannel = DISCOVERY_COM_IRQn;
//   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//   NVIC_Init(&NVIC_InitStructure);

	return USBD_OK;
}



/**
 * @brief  VCP_DeInit
 *         DeInitializes the Media on the STM32
 * @param  None
 * @retval Result of the opeartion (USBD_OK in all cases)
 */
static uint16_t VCP_DeInit(void)
{
//   USART_ITConfig(DISCOVERY_COM, USART_IT_RXNE, DISABLE);
//   USART_Cmd(DISCOVERY_COM, DISABLE);
   return USBD_OK;
}




/**
 * @brief  VCP_Ctrl
 *         Manage the CDC class requests
 * @param  Cmd: Command code
 * @param  Buf: Buffer containing command data (request parameters)
 * @param  Len: Number of data to be sent (in bytes)
 * @retval Result of the opeartion (USBD_OK in all cases)
 */
static uint16_t VCP_Ctrl(uint32_t Cmd, uint8_t* Buf, uint32_t Len)
{
   LINE_CODING* plc = (LINE_CODING*)Buf;

   assert_param(Len>=sizeof(LINE_CODING));

   switch (Cmd) {
       /* Not  needed for this driver, AT modem commands */   
      case SEND_ENCAPSULATED_COMMAND:
      case GET_ENCAPSULATED_RESPONSE:
         break;

      // Not needed for this driver
      case SET_COMM_FEATURE:                  
      case GET_COMM_FEATURE:
      case CLEAR_COMM_FEATURE:
         break;
      case SET_LINE_CODING: 
            return USBD_FAIL;
         break;
      case GET_LINE_CODING:
         break;     
      case SET_CONTROL_LINE_STATE:
         break;
      case SEND_BREAK:
         break;
      default:
         break;
	}

   return USBD_OK;
}




/**
 * @brief  VCP_DataTx
 *         CDC received data to be send over USB IN endpoint are managed in
 *         this function.
 *         *Data Send to USB
 * @param  Buf: Buffer of data to be sent
 * @param  Len: Number of data to be sent (in bytes)
 * @retval Result of the opeartion: USBD_OK if all operations are OK else VCP_FAIL
 */
uint16_t VCP_DataTx(uint8_t* Buf, uint32_t Len)
{
   while (Len--) 
   {
      APP_Rx_Buffer[APP_Rx_ptr_in] = *(Buf++);
      APP_Rx_ptr_in++;
      /* To avoid buffer overflow */
      if (APP_Rx_ptr_in == APP_RX_DATA_SIZE) APP_Rx_ptr_in = 0;
   }
	return USBD_OK;
}



/**
 * @brief  VCP_DataRx
 *         Data received over USB OUT endpoint are sent over CDC interface
 *         through this function.
 *         *Data Received from USB
 *         @note
 *         This function will block any OUT packet reception on USB endpoint
 *         until exiting this function. If you exit this function before transfer
 *         is complete on CDC interface (ie. using DMA controller) it will result
 *         in receiving more data while previous ones are still not sent.
 *
 * @param  Buf: Buffer of data to be received
 * @param  Len: Number of data received (in bytes)
 * @retval Result of the opeartion: USBD_OK if all operations are OK else VCP_FAIL
 */

uint16_t VCP_DataRx(uint8_t* Buf, uint32_t Len)
{
   if (Len>1)//Len bigger then 1 means data exisit
   {
      ptrUSB_Rx_Buf = Buf; //point to Rx data buffer
      USB_RX_STATUS = (uint8_t)(0x7F&Len); //get data length
      USB_RX_STATUS |= 1<<7; //set data ready bit
      while((USB_RX_STATUS>>7)==USB_RX_DRDY);//waiting for data read by user
      
      ptrUSB_Rx_Buf = NULL;   //reset pointer
      USB_RX_STATUS = 0x00;   //reset status
   }
   return USBD_OK;
}