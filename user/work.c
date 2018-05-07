#include <core/eos.h>
#define STACK_SIZE 8096

int32u_t stack1[STACK_SIZE]; // stack for task1
int32u_t stack2[STACK_SIZE]; // stack for task2
eos_tcb_t tcb1; // tcb for task1
eos_tcb_t tcb2; // tcb for task2

/* task1 function - print number 1 to 20 repeatedly */
void print_number() {
	int i = 1;
	while(++i) {
		printf("%d", i);
		eos_schedule(); // 태스크 1 수행 중단, 태스크 2 수행 재개
		if (i == 4) { i = 0; }
	}
}

/* task2 function - print alphabet a to z repeatedly */
void print_alphabet() {
	int i = 97;
	while(++i) {
		printf("%c", i);
		eos_schedule(); // 태스크 2 수행 중단, 태스크 1 수행 재개
		if (i == 100) { i = 96; }
	}
}

void eos_user_main() {
	eos_create_task(&tcb1, stack1, STACK_SIZE, print_number, NULL, 0); // 태스크 1 생성
	eos_create_task(&tcb2, stack2, STACK_SIZE, print_alphabet, NULL, 0); // 태스크 2 생성
}

