#include "stm32f10x.h"   
#include "infrared.h"
#include "Emachinery.h"
#include "Delay.h"

extern volatile int irR, irRU, irC, irLU, irL;
int Now_Location_X = 0;
int Now_Location_Y = 0;
int Inside_X = 0;
int Inside_Y = 0;
int Inside_Step = 0;
int Stack_X[40];
int Stack_Y[40];
int Stack_number = 0;
int Queue_X[40];
int Queue_Y[40];
int queue_number = 0;
unsigned char maze[8][8];
unsigned char step[8][8];
int direction = 0;

void Stack_Push(int x, int y){
	Stack_X[Stack_number] = x;
	Stack_Y[Stack_number] = y;
	Stack_number++;
}

int Stack_Pop_X(){
	return Stack_X[Stack_number - 1];
}

int Stack_Pop_Y(){
	return Stack_Y[Stack_number - 1];
}

void Stack_Location_Pop(){
	Stack_X[Stack_number - 1] = 0;
	Stack_Y[Stack_number - 1] = 0;
	Stack_number--;
}

int Is_Empty_Stack(){
	if (Stack_number == 0) {
		return 1;
	}
	else {
		return 0;
	}
}



void queue_push(int x, int y){
	Queue_X[queue_number] = x;
	Queue_Y[queue_number] = y;
	queue_number++;
}

int Queue_Peek_X(){
	return Queue_X[0];
}

int Queue_Peek_Y(){
	return Queue_Y[0];
}

void Queue_delete_Top(){
	unsigned int i = 0;
	for (i = 0; i < queue_number; i++) {
		Queue_X[i] = Queue_X[i + 1];
		Queue_Y[i] = Queue_Y[i + 1];
	}
	queue_number--;
}

int Is_Queue_Empty(){
	if (queue_number == 0) {
		return 1;
	}
	else {
		return 0;
	}
}


int get_forward_information(){
	return direction;
}

int get_right_information(){
	return ((direction + 1) % 4);
}

int get_back_information(){
	return ((direction + 2) % 4);
}

int get_left_information(){
	return ((direction + 3) % 4);
}

int next_X(int Direc){
	int X = Now_Location_X;
	if (Direc == 0) {
		if (get_forward_information() == 0 || get_forward_information() == 2) {
			return X;
		}
		else if (get_forward_information() == 1) {
			return ++X;
		}
		else if (get_forward_information() == 3) {
			return --X;
		}
	}
	else if (Direc == 1) {
		if (get_right_information() == 0 || get_right_information() == 2) {
			return X;
		}
		else if (get_right_information() == 1) {
			return ++X;
		}
		else if (get_right_information() == 3) {
			return --X;
		}
	}
	else if (Direc == 2) {
		if (get_back_information() == 0 || get_back_information() == 2) {
			return X;
		}
		else if (get_back_information() == 1) {
			return ++X;
		}
		else if (get_back_information() == 3) {
			return --X;
		}
	}
	else if (Direc == 3) {
		if (get_left_information() == 0 || get_left_information() == 2) {
			return X;
		}
		else if (get_left_information() == 1) {
			return ++X;
		}
		else if (get_left_information() == 3) {
			return --X;
		}
	}
}

int next_Y(int Direc){
	int Y = Now_Location_Y;
	if (Direc == 0) {
		if (get_forward_information() == 1 || get_forward_information() == 3) {
			return Y;
		}
		else if (get_forward_information() == 0) {
			return ++Y;
		}
		else if (get_forward_information() == 2) {
			return --Y;
		}
	}
	else if (Direc == 1) {
		if (get_right_information() == 1 || get_right_information() == 3) {
			return Y;
		}
		else if (get_right_information() == 0) {
			return ++Y;
		}
		else if (get_right_information() == 2) {
			return --Y;
		}
	}
	else if (Direc == 2) {
		if (get_back_information() == 1 || get_back_information() == 3) {
			return Y;
		}
		else if (get_back_information() == 0) {
			return ++Y;
		}
		else if (get_back_information() == 2) {
			return --Y;
		}
	}
	else if (Direc == 3) {
		if (get_left_information() == 1 || get_left_information() == 3) {
			return Y;
		}
		else if (get_left_information() == 0) {
			return ++Y;
		}
		else if (get_left_information() == 2) {
			return --Y;
		}
	}
}


int ergodic_judge(int x, int y){
	unsigned int i = maze[x][y];
	return ((i >> 4) | (0x0)) > 0;
}

int ergodic_It(int x, int y){
	return step[x][y] != 100;
}

void set_block_information(int direction_sign){
	switch (direction_sign) {
	case 0:
		maze[Now_Location_X][Now_Location_Y] |= (1 << 0);
		break;
	case 1:
		maze[Now_Location_X][Now_Location_Y] |= (1 << 1);
		break;
	case 2:
		maze[Now_Location_X][Now_Location_Y] |= (1 << 2);
		break;
	case 3:
		maze[Now_Location_X][Now_Location_Y] |= (1 << 3);
		break;
	}
}

void set_come_information(){
	if (Now_Location_X == 0 && Now_Location_Y == 0) {
		return;
	}

	if (get_forward_information() == 0) {
		if (ergodic_judge(Now_Location_X, Now_Location_Y) == 0) {
			maze[Now_Location_X][Now_Location_Y] |= 0x40;
		}
	}
	else if (get_forward_information() == 1) {
		if (ergodic_judge(Now_Location_X, Now_Location_Y) == 0) {
			maze[Now_Location_X][Now_Location_Y] |= 0x80;
		}
	}
	else if (get_forward_information() == 2) {
		if (ergodic_judge(Now_Location_X, Now_Location_Y) == 0) {
			maze[Now_Location_X][Now_Location_Y] |= 0x10;
		}
	}
	else if (get_forward_information() == 3) {
		if (ergodic_judge(Now_Location_X, Now_Location_Y) == 0) {
			maze[Now_Location_X][Now_Location_Y] |= 0x20;
		}
	}
}

void set_short_load(int x, int y, int Direct){
	if (Direct == 0) {
		maze[x][y] &= 0x0F;
		maze[x][y] |= 0x40;
	}
	else if (Direct == 1) {
		maze[x][y] &= 0x0F;
		maze[x][y] |= 0x80;
	}
	else if (Direct == 2) {
		maze[x][y] &= 0x0F;
		maze[x][y] |= 0x10;
	}
	else if (Direct == 3) {
		maze[x][y] &= 0x0F;
		maze[x][y] |= 0x20;
	}
}
int check_load_walkable(){
	int count = 3;
	if (irR == 1 || ergodic_judge(next_X(3), next_Y(3)) == 1) {
		count--;
		if (irR == 1) {
			set_block_information(get_left_information());
		}
	}
	if (irC == 1 || ergodic_judge(next_X(0), next_Y(0)) == 1) {
		count--;
		if (irC == 1) {
			set_block_information(get_forward_information());
		}
	}
	if (irL == 1 || ergodic_judge(next_X(1), next_Y(1)) == 1) {
		count--;
		if (irL == 1) {
			set_block_information(get_right_information());
		}
	}
	return count;
}

void change_postion(){
	if (get_forward_information() == 0) {
		Now_Location_Y++;
	}
	else if (get_forward_information() == 1) {
		Now_Location_X++;
	}
	else if (get_forward_information() == 2) {
		Now_Location_Y--;
	}
	else if (get_forward_information() == 3) {
		Now_Location_X--;
	}
}

void set_toward(int Direct){
	if (Direct == 1) {
		direction += 1;
		direction %= 4;
	}
	else if (Direct == 2) {
		direction += 2;
		direction %= 4;
	}
	else if (Direct == 3) {
		direction += 3;
		direction %= 4;
	}
}

int load_walkable(int X, int Y, int Direct){
	if (Direct == 0 && (maze[X][Y] & 0x01) != 0) {
		return 1;
	}
	if (Direct == 1 && (maze[X][Y] & 0x02) != 0) {
		return 1;
	}
	if (Direct == 2 && (maze[X][Y] & 0x04) != 0) {
		return 1;
	}
	if (Direct == 3 && (maze[X][Y] & 0x08) != 0) {
		return 1;
	}
	return 0;
}

int judge_walkable_load_number(int X, int Y){
	unsigned int i = 0;
	int count = 4;
	for (i = 0; i < 4; ++i) {
		if (load_walkable(X, Y, i) == 1) {
			count--;
		}
	}
	return count;
}

int get_come_information(void){
	if (maze[Now_Location_X][Now_Location_Y] & 0x10) {
		return 0;
	}
	else if (maze[Now_Location_X][Now_Location_Y] & 0x20) {
		return 1;
	}
	else if (maze[Now_Location_X][Now_Location_Y] & 0x40) {
		return 2;
	}
	else if (maze[Now_Location_X][Now_Location_Y] & 0x80) {
		return 3;
	}
}

void back_trace(void){
	unsigned int X = Stack_Pop_X();
	unsigned int Y = Stack_Pop_Y();
	while (!(Now_Location_X == X && Now_Location_Y == Y)) {
		if (get_come_information() == get_back_information()) {
			set_toward(2);
			Turn_Back();
			change_postion();
			One_forward();
		}
		else if (get_come_information() == get_left_information()) {
			set_toward(3);
			Turn_Left();
			change_postion();
			One_forward();
		}
		else if (get_come_information() == get_right_information()) {
			set_toward(1);
			Turn_Right();
			change_postion();
			One_forward();
		}
		else if (get_come_information() == get_forward_information()) {
			change_postion();
			One_forward();
		}
	}
	if (check_load_walkable() == 0) {
		Stack_Location_Pop();
	}
}

void go_start(void){
	set_come_information();
	if (irL == 0 && ergodic_judge(next_X(1), next_Y(1)) == 0) {
		set_toward(1);
		Turn_Right();
		change_postion();
		One_forward();
	}
	else if (irC == 0 && ergodic_judge(next_X(0), next_Y(0)) == 0) {
		change_postion();
		One_forward();
	}
	else if (irR == 0 && ergodic_judge(next_X(3), next_Y(3)) == 0) {
		set_toward(3);
		Turn_Left();
		change_postion();
		One_forward();
	}
}

int main(void){
unsigned int i = 0;
unsigned int j = 0;
init();
Delay_ms(1000);
Stack_Push(Now_Location_X, Now_Location_Y);

	while (!Is_Empty_Stack()) {
		int choice = 0;
		choice = check_load_walkable();
		if (choice == 0) {
			set_come_information();
			back_trace();
		}
		else if (choice == 1) {
			go_start();
		}
		else {
			Stack_Push(Now_Location_X, Now_Location_Y);
			go_start();
		}
	}
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			step[i][j] = 100;
		}
	}
	Inside_Step = 1;
	step[Inside_X][Inside_Y] = 1;
	Stack_Push(Inside_X, Inside_Y);
	queue_push(Inside_X, Inside_Y);
	while (!Is_Queue_Empty()) {
		Inside_X = Queue_Peek_X();
		Inside_Y = Queue_Peek_Y();
		if (judge_walkable_load_number(Inside_X, Inside_Y) == 0) {
			Queue_delete_Top();
			continue;
		}
		Inside_Step = step[Inside_X][Inside_Y];
		if (load_walkable(Inside_X, Inside_Y, 0) == 0 && ergodic_It(Inside_X, Inside_Y + 1) == 0) {
			queue_push(Inside_X, Inside_Y + 1);
			step[Inside_X][Inside_Y + 1] = Inside_Step + 1;
		}
		if (load_walkable(Inside_X, Inside_Y, 1) == 0 && ergodic_It(Inside_X + 1, Inside_Y) == 0) {
			queue_push(Inside_X + 1, Inside_Y);
			step[Inside_X + 1][Inside_Y] = Inside_Step + 1;
		}
		if (load_walkable(Inside_X, Inside_Y, 2) == 0 && ergodic_It(Inside_X, Inside_Y - 1) == 0) {
			queue_push(Inside_X, Inside_Y - 1);
			step[Inside_X][Inside_Y - 1] = Inside_Step + 1;
		}
		if (load_walkable(Inside_X, Inside_Y, 3) == 0 && ergodic_It(Inside_X - 1, Inside_Y) == 0) {
			queue_push(Inside_X - 1, Inside_Y);
			step[Inside_X - 1][Inside_Y] = Inside_Step + 1;
		}
		Queue_delete_Top();
	}
	while (Inside_Step != 0) {
		if (load_walkable(Inside_X, Inside_Y, 0) == 0 && step[Inside_X][Inside_Y + 1] == Inside_Step - 1) {
			set_short_load(Inside_X, Inside_Y + 1, 0);
			Inside_Y += 1;
		}
		else if (load_walkable(Inside_X, Inside_Y, 1) == 0 && step[Inside_X + 1][Inside_Y] == Inside_Step - 1) {
			set_short_load(Inside_X + 1, Inside_Y, 1);
			Inside_X += 1;
		}
		else if (load_walkable(Inside_X, Inside_Y, 2) == 0 && step[Inside_X][Inside_Y - 1] == Inside_Step - 1) {
			set_short_load(Inside_X, Inside_Y - 1, 2);
			Inside_Y -= 1;
		}
		else if (load_walkable(Inside_X, Inside_Y, 3) == 0 && step[Inside_X - 1][Inside_Y] == Inside_Step - 1) {
			set_short_load(Inside_X - 1, Inside_Y, 3);
			Inside_X -= 1;
		}
		Inside_Step--;
	}
	set_toward(2);
	Turn_Back();
	while (!(Now_Location_X == 7 && Now_Location_Y == 7)) {
		Delay_ms(500);
		if (get_forward_information() == get_come_information()) {
			change_postion();
			One_forward();
		}
		else if (get_left_information() == get_come_information()) {
			set_toward(3);
			Turn_Left();
			change_postion();
			One_forward();
		}
		else if (get_right_information() == get_come_information()) {
			set_toward(1);
			Turn_Right();
			change_postion();
			One_forward();
		}
	}
	while (1) {}
}

