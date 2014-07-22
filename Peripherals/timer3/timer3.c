#include <stm32f4xx.h>
#include "timer3.h"

//timer3 configuration pwm output at channel 1
//APB1 bus clock is running at 84 MHz 
//@ uint16_t period: pwm period(us)
//@ uint16_t dutycycle: pwm dutycycle(us)
void Timer3_Interrupt_Init(uint16_t period, uint16_t prescaler)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

    //enable timer3 clock
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    //configure timer3 NVIC
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    //timer configuration
    TIM_TimeBaseStructure.TIM_Period = period - 1;
    TIM_TimeBaseStructure.TIM_Prescaler = prescaler - 1;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
    //enable timer3 interrupt
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
    //enable timer3
    TIM_Cmd(TIM3, ENABLE);  
}
