#include <stdio.h>
#include <limits.h>
 
int main(void)
{
        void *stack_base;
        void *sp;
        int stack_size = 4;
        int *int32;
        

        int32 = stack_base + stack_size/4 - 1;  // stack pointer
        *(int32++) = NULL;   // entry return address
        *(int32++) = NULL;   // eflags
        *(int32++) = NULL;   // eax

        printf("포인터 자체의 주소 : %p\n", &int32); // 포인터 자체의 주소
        printf("포인터 안에 담긴 주소 : %p\n", int32); // 포인터 자체의 내용 = 포인터 안에 담긴 주소
        printf("포인터 안에 담긴 주소의 내용 : %p\n", *int32); // 포인터 안에 담긴 주소의 내용

        return 0;
}