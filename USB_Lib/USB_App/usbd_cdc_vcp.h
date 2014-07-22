/**
  ******************************************************************************
  * @file    usbd_cdc_vcp.h
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    22-July-2011
  * @brief   Header for usbd_cdc_vcp.c file.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USBD_CDC_VCP_H
#define __USBD_CDC_VCP_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_conf.h"

#include "usbd_cdc_core.h"
#include "usbd_conf.h"
#include <stdint.h>

  
#define USB_RX_BUF_SIZE 1024 //bytes

//Exported Variables
extern uint8_t USB_RxBuffer[USB_RX_BUF_SIZE];
extern uint32_t usb_rx_data_pos;
extern uint32_t usb_read_data_pos;
//Exported Functions
uint16_t VCP_DataTx(uint8_t* Buf, uint32_t Len);
uint16_t VCP_DataRx(uint8_t* Buf, uint32_t Len);
void USB_VCP_TxString(uint8_t* str);
void USB_VCP_TxChar(char pchar);
uint8_t USB_VCP_RxChar(uint8_t *pchar);
/* Exported typef ------------------------------------------------------------*/
/* The following structures groups all needed parameters to be configured for the 
   ComPort. These parameters can modified on the fly by the host through CDC class
   command class requests. */
typedef struct
{
  uint32_t bitrate;
  uint8_t  format;
  uint8_t  paritytype;
  uint8_t  datatype;
} LINE_CODING;

#endif /* __USBD_CDC_VCP_H */

