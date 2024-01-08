#include "stm32f10x.h"
#include "infrared.h"

volatile int irR = 0, irRU = 0, irC = 0, irLU = 0, irL = 0;

void init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);

	TIM_TimeBaseStructure.TIM_Period = (1000 - 1);
	TIM_TimeBaseStructure.TIM_Prescaler = (72 - 1);
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = DISABLE;
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);
	TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
	TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	TIM_Cmd(TIM6, ENABLE);
}


void ir_on(int num) {
	num -= 1;
	if (num & 0x1) {
		GPIO_WriteBit(GPIOC, GPIO_Pin_0, Bit_SET);
	}
	else {
		GPIO_WriteBit(GPIOC, GPIO_Pin_0, Bit_RESET);
	}
	if (num & 0x2) {
		GPIO_WriteBit(GPIOC, GPIO_Pin_1, Bit_SET);
	}
	else {
		GPIO_WriteBit(GPIOC, GPIO_Pin_1, Bit_RESET);
	}
	if (num & 0x4) {
		GPIO_WriteBit(GPIOC, GPIO_Pin_2, Bit_SET);
	}
	else {
		GPIO_WriteBit(GPIOC, GPIO_Pin_2, Bit_RESET);
	}
}

int get_ir(int num) {
	if (num == 1) {
		return irR;
	}
	else if (num == 2) {
		return irRU;
	}
	else if (num == 3) {
		return irC;
	}
	else if (num == 4) {
		return irLU;
	}
	else if (num == 5) {
		return irL;
	}
	else {
		return 0;
	}
}



void TIM6_IRQHandler(void) {
	if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET) {
		static uint8_t Flag = 0;
		static uint8_t n = 1;
		if (!Flag) {
			ir_on(n);
		}
		else {
			if (n == 5) {
				if (GPIO_ReadInputDataBit(IR1_GPIO_Port, IR1_Pin)) {
					irR = 0;
				}
				else {
					irR = 1;
				}
			}
			else if (n == 4) {
				if (GPIO_ReadInputDataBit(IR2_GPIO_Port, IR2_Pin)) {
					irRU = 0;
				}
				else {
					irRU = 1;
				}
			}
			else if (n == 3) {
				if (GPIO_ReadInputDataBit(IR3_GPIO_Port, IR3_Pin)) {
					irC = 0;
				}
				else {
					irC = 1;
				}
			}
			else if (n == 2) {
				if (GPIO_ReadInputDataBit(IR4_GPIO_Port, IR4_Pin)) {
					irLU = 0;
				}
				else {
					irLU = 1;
				}
			}
			else if (n == 1) {
				if (GPIO_ReadInputDataBit(IR5_GPIO_Port, IR5_Pin)) {
					irL = 0;
				}
				else {
					irL = 1;
				}
			}
		}
		if (Flag) {
			n++;
		}
		if (n > 5) {
			n = 0;
		}
		Flag = !Flag;
		TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
	}
}

