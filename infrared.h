#ifndef __INFRARED_H
#define __INFRARED_H
#include "stm32f10x.h"                  // Device header

#define IR5_Pin GPIO_Pin_4
#define IR5_GPIO_Port GPIOA
#define IR4_Pin GPIO_Pin_5
#define IR4_GPIO_Port GPIOA
#define IR3_Pin GPIO_Pin_6
#define IR3_GPIO_Port GPIOA
#define IR2_Pin GPIO_Pin_7
#define IR2_GPIO_Port GPIOA
#define IR1_Pin GPIO_Pin_4
#define IR1_GPIO_Port GPIOC

void init(void);
void ir_on(int num);
int get_ir(int num);
void TIM6_IRQHandler(void);
#endif
