#include "stm32f10x.h"                  // Device header
#include "Emachinery.h"
#include "Delay.h"


int Right[] = { 0x11,0x33,0x22,0x66,0x44,0xcc,0x88,0x99 };
int Left[] = { 0x11,0x99,0x88,0xcc,0x44,0x66,0x22,0x33 };
int Back[] = { 0x11,0x39,0x28,0x6c,0x44,0xc6,0x82,0x93 };
int Forward[] = { 0x11,0x93,0x82,0xc6,0x44,0x6c,0x28,0x39 };
extern volatile int irR, irRU, irC, irLU, irL;


void run(int temp) {
	if (temp & 0x01) {
		GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_SET);
	}
	else {
		GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_RESET);
	}

	if (temp & 0x02) {
		GPIO_WriteBit(GPIOA, GPIO_Pin_1, Bit_SET);
	}
	else {
		GPIO_WriteBit(GPIOA, GPIO_Pin_1, Bit_RESET);
	}

	if (temp & 0x04) {
		GPIO_WriteBit(GPIOA, GPIO_Pin_2, Bit_SET);
	}
	else {
		GPIO_WriteBit(GPIOA, GPIO_Pin_2, Bit_RESET);
	}

	if (temp & 0x08) {
		GPIO_WriteBit(GPIOA, GPIO_Pin_3, Bit_SET);
	}
	else {
		GPIO_WriteBit(GPIOA, GPIO_Pin_3, Bit_RESET);
	}

	if (temp & 0x10) {
		GPIO_WriteBit(GPIOA, GPIO_Pin_8, Bit_SET);
	}
	else {
		GPIO_WriteBit(GPIOA, GPIO_Pin_8, Bit_RESET);
	}

	if (temp & 0x20) {
		GPIO_WriteBit(GPIOA, GPIO_Pin_9, Bit_SET);
	}
	else {
		GPIO_WriteBit(GPIOA, GPIO_Pin_9, Bit_RESET);
	}

	if (temp & 0x40) {
		GPIO_WriteBit(GPIOA, GPIO_Pin_10, Bit_SET);
	}
	else {
		GPIO_WriteBit(GPIOA, GPIO_Pin_10, Bit_RESET);
	}

	if (temp & 0x80) {
		GPIO_WriteBit(GPIOA, GPIO_Pin_11, Bit_SET);
	}
	else {
		GPIO_WriteBit(GPIOA, GPIO_Pin_11, Bit_RESET);
	}
}



void Amend(void) {
	unsigned int count = 3;
	unsigned int i = 0;
	if (irC == 1) {
		return;
	}
	if (irRU == 1) {
		Delay_ms(200);
		while (count--) {
			for (i = 0; i < 8; i++) {
				run(Right[i]);
				Delay_ms(3);
			}
		}
	}
	else if (irLU == 1) {
		Delay_ms(200);
		while (count--) {
			for (i = 0; i < 8; i++) {
				run(Left[i]);
				Delay_ms(3);
			}
		}
	}
}

void Amend_Safe(void) {
	unsigned int count = 3;
	unsigned int i = 0;
	if (irC == 1) {
		return;
	}

	if (irRU == 1 && irR == 1) {
		Delay_ms(200);
		while (count--) {
			for (i = 0; i < 8; i++) {
				run(Right[i]);
				Delay_ms(3);
			}
		}
	}
	else if (irLU == 1 && irL == 1) {
		Delay_ms(200);
		while (count--) {
			for (i = 0; i < 8; i++) {
				run(Left[i]);
				Delay_ms(3);
			}
		}
	}
}
void Go_Forward(double count_in) {
	unsigned int count;
	unsigned int i = 0;
	if (count_in == 0) {
		count_in = -1;
	}
	else {
		count_in *= 128;
	}
	count = (int)count_in;
	Amend();
	while (count--) {
		if (count % 16 == 0) {
			if (irRU == 1 && irC == 1 && irL == 1) {
				break;
			}
		}
		if (count % 16 == 0) {
			Amend_Safe();
		}
		for (i = 0; i < 8; i++) {
			run(Forward[i]);
			Delay_ms(3);
		}
	}
}

void One_forward(void) {
	Delay_ms(200);
	Go_Forward(0.82);
	Amend();
}

void Go_Back(double count_in) {
	unsigned int count;
	unsigned int i = 0;
	Delay_ms(200);
	if (count_in == 0) {
		count_in = -1;
	}
	else {
		count_in *= 128;
	}
	count = (int)count_in;
	while (count--) {
		for (i = 0; i < 8; i++) {
			run(Back[i]);
			Delay_ms(3);
		}
	}
}

void Turn_Left(void) {
	unsigned int count =41;
	unsigned int i = 0;
	Delay_ms(500);
	while (count--) {
		for (i = 0; i < 8; i++) {
			run(Left[i]);
			Delay_ms(3);
		}
	}
}

void Turn_Right(void) {
	unsigned int count = 41;
	unsigned int i = 0;
	Delay_ms(500);
	while (count--) {
		for (i = 0; i < 8; i++) {
			run(Right[i]);
			Delay_ms(3);
		}
	}
}
void Turn_Back(void) {
	unsigned int i = 0;
	unsigned int count = 84;;
	Delay_ms(500);
	while (count--) {
		for (i = 0; i < 8; i++) {
			run(Right[i]);
			Delay_ms(3);
		}
	}
}
