#ifndef STM32F0xx_IT_H_
#define STM32F0xx_IT_H_

#ifdef __cplusplus
extern "C" {
#endif


void NMI_Handler(void);
void HardFault_Handler(void);
void SVC_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);


#ifdef __cplusplus
}
#endif

#endif /* STM32F0xx_IT_H_ */
