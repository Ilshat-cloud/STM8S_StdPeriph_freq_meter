

/* Includes ------------------------------------------------------------------*/
#include "stm8s_conf.h"
#include "stm8s.h"


/* Private functions ---------------------------------------------------------*/
#define I2C_ADR 0x70 //i2c slave adress
#define I2C_SPEED 400000 //speed i2c
#define ClkFreqMHz 16
#define DeadTime   0xFF  //deadtime for transistor close
#define igniteWait  3     // frequenfy*wait delay

void Delay(uint32_t t) {
  if (t==0){t=1;}
    while(--t)
    {
    if (GPIO_ReadInputPin(GPIOD,GPIO_PIN_3))
    {
      enableInterrupts();
    }
    }
}


uint16_t i;
uint16_t ICValue1 =0, ICValue2 =0,freq;

void SendLed (uint16_t num);

void main(void)
{
  CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV1);      //16MH
  CLK_SYSCLKConfig(CLK_PRESCALER_HSIDIV1);      //16MH
  CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER2, ENABLE);
  

  //--------------GPIO---------------
  
  GPIO_Init(GPIOD,GPIO_PIN_2|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6,GPIO_MODE_OUT_PP_HIGH_SLOW );
  GPIO_Init(GPIOA,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,GPIO_MODE_OUT_PP_HIGH_SLOW);
  GPIO_Init(GPIOC,GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_6|GPIO_PIN_7,GPIO_MODE_OUT_PP_HIGH_SLOW);
  GPIO_Init(GPIOD,GPIO_PIN_3,GPIO_MODE_IN_PU_IT );
  EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOD, EXTI_SENSITIVITY_FALL_ONLY);
  enableInterrupts();
  //--------------------------------
 
   // ------------ TIM2 -------------------
  TIM2_DeInit();
  TIM2_TimeBaseInit(TIM2_PRESCALER_4096 ,50000); 
  TIM2_Cmd(ENABLE);
    // -------------------------------------
  

  
  uint8_t mod=0,gotov=0;
  uint16_t buf[10];
  uint32_t b;
  /* Infinite loop */
  while (1)
  {
    //--------------LED-------------------
   //i++; 
   //i=(i>9999)?0:i;
    Delay(20);
    mod++;
    if ((freq!=buf[3])){
      mod=0;
    for(i = 0;i< 3;i++)
    {
      buf[i]=buf[i+1];
    } 
    buf[3]=freq;
    b=0;
    for(i = 0;i<4;i++)
    {
      b= buf[i]+b;
    } 
    b=(b==0)?937500:b;
    i=937500/b;
    }
    GPIO_WriteHigh(GPIOC, GPIO_PIN_4);
    i=(i>10000)?9999:i;
   if (i>999)
   {
      SendLed((i/1000)%10);
      GPIO_WriteLow(GPIOC, GPIO_PIN_7);
      Delay(40);
      GPIO_WriteHigh(GPIOC, GPIO_PIN_7);
   }
   if (i>99)
   {
      SendLed((i/100)%10);
      GPIO_WriteLow(GPIOC, GPIO_PIN_6);
      Delay(40);
      GPIO_WriteHigh(GPIOC, GPIO_PIN_6);
   }
   if (i>9)
   {
      
      SendLed((i/10)%10);
      GPIO_WriteLow(GPIOC, GPIO_PIN_3);
      Delay(40);
      GPIO_WriteHigh(GPIOC, GPIO_PIN_3);
   }
    SendLed(i%10);
    GPIO_WriteLow(GPIOC, GPIO_PIN_4);
       
   //-----------------------------
    
    

  }
  
}


INTERRUPT_HANDLER(EXTI_PORTD_IRQHandler, 6)
{
   if (!GPIO_ReadInputPin(GPIOD,GPIO_PIN_3))
  {
    ICValue1=  TIM2_GetCounter();
    if (ICValue1>ICValue2){
      freq=ICValue1-ICValue2;
      ICValue2=ICValue1;
    } 
    if (ICValue1<ICValue2){
    freq=ICValue1+50000-ICValue2;
    ICValue2=ICValue1;     
    }
    disableInterrupts();
  }
}

 INTERRUPT_HANDLER(TIM2_CAP_COM_IRQHandler, 14)
 {
   if (TIM2_GetITStatus(TIM2_IT_CC2)==SET){
    TIM2_ClearITPendingBit(TIM2_IT_CC2);
    TIM2_ClearFlag(TIM2_FLAG_CC2);
   }
 }

void SendLed (uint16_t num)
{
  
  GPIO_WriteLow(GPIOA, GPIO_PIN_3|GPIO_PIN_1|GPIO_PIN_2);
  GPIO_WriteLow(GPIOD,  GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_2|GPIO_PIN_3);

  switch (num){ 
      case 0:
        GPIO_WriteHigh(GPIOA, GPIO_PIN_3);
        GPIO_WriteHigh(GPIOA, GPIO_PIN_1|GPIO_PIN_2);
        GPIO_WriteHigh(GPIOD, GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_2);
    break;
      case 1:
        GPIO_WriteHigh(GPIOA, GPIO_PIN_2);
        GPIO_WriteHigh(GPIOD, GPIO_PIN_4);
    break;
      case 2:
        GPIO_WriteHigh(GPIOA, GPIO_PIN_3);
        GPIO_WriteHigh(GPIOA, GPIO_PIN_2);
        GPIO_WriteHigh(GPIOD,  GPIO_PIN_2|GPIO_PIN_5|GPIO_PIN_6);
    break;
      case 3:
        GPIO_WriteHigh(GPIOA, GPIO_PIN_3);
        GPIO_WriteHigh(GPIOA, GPIO_PIN_2);
        GPIO_WriteHigh(GPIOD,  GPIO_PIN_4|GPIO_PIN_2|GPIO_PIN_6);
    break;
      case 4:
        GPIO_WriteHigh(GPIOA, GPIO_PIN_1|GPIO_PIN_2);
        GPIO_WriteHigh(GPIOD,  GPIO_PIN_4|GPIO_PIN_6);
    break;
      case 5:
        GPIO_WriteHigh(GPIOA, GPIO_PIN_3);
        GPIO_WriteHigh(GPIOA, GPIO_PIN_1);
        GPIO_WriteHigh(GPIOD,  GPIO_PIN_4|GPIO_PIN_6|GPIO_PIN_2);
    break;
      case 6:
        GPIO_WriteHigh(GPIOA, GPIO_PIN_3);
        GPIO_WriteHigh(GPIOA, GPIO_PIN_1);
        GPIO_WriteHigh(GPIOD,  GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_2);
    break;
      case 7:
        GPIO_WriteHigh(GPIOA, GPIO_PIN_3);
        GPIO_WriteHigh(GPIOA, GPIO_PIN_2);
        GPIO_WriteHigh(GPIOD,  GPIO_PIN_4);
    break;
      case 8:
        GPIO_WriteHigh(GPIOA, GPIO_PIN_3);
        GPIO_WriteHigh(GPIOA, GPIO_PIN_1|GPIO_PIN_2);
        GPIO_WriteHigh(GPIOD,  GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_2);
    break;
      case 9:
        GPIO_WriteHigh(GPIOA, GPIO_PIN_3);
        GPIO_WriteHigh(GPIOA, GPIO_PIN_1|GPIO_PIN_2);
        GPIO_WriteHigh(GPIOD,  GPIO_PIN_4|GPIO_PIN_6|GPIO_PIN_2);
    break;

    
  }

}





#ifdef USE_FULL_ASSERT


void assert_failed(u8* file, u32 line)
{ 
 
  while (1)
  {
  }
}
#endif


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
