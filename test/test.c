#include <stdio.h>
#include <limits.h>
 
int main(void)
{
        char *test_t;
        printf("%d\n", sizeof(test_t) * CHAR_BIT);
        printf("%d\n", sizeof(test_t) * CHAR_BIT);
        
        printf("포인터 a자체의 주소 : %p\n", &test_t); // 포인터 자체의 주소
        printf("포인터 안에 담긴 주소 : %p\n", test_t); // 포인터 자체의 내용 = 포인터 안에 담긴 주소
        printf("포인터 안에 담긴 주소의 내용 : %p\n", *test_t); // 포인터 안에 담긴 주소의 내용
        printf("============================\n");
        // test_t = 1;  /* 포인터 안에 담긴 주소의 내용을 읽을때 에러. 아무 메모리나 참조하지 못하는거같다! base/bound 검사인가! */
        test_t += 1 ; /* 오! 막 += 10000 하면 segmentation fault 나는데 1~500정도 더하는건 괜찮은가보다! */
        printf("test_t += 1;\n");
        printf("포인터 자체의 주소 : %p\n", &test_t); // 포인터 자체의 주소
        printf("포인터 안에 담긴 주소 : %p\n", test_t); // 포인터 자체의 내용 = 포인터 안에 담긴 주소
        printf("포인터 안에 담긴 주소의 내용 : %p\n", *test_t); // 포인터 안에 담긴 주소의 내용
        printf("============================\n");
        *test_t = 'a'; // 포인터 안에 담긴 주소의 내용을 바꿈 .. 
        printf("*test_t = 'a';\n");
        printf("포인터 자체의 주소 : %p\n", &test_t); // 포인터 자체의 주소
        printf("포인터 안에 담긴 주소 : %p\n", test_t); // 포인터 자체의 내용 = 포인터 안에 담긴 주소
        printf("포인터 안에 담긴 주소의 내용 : %p\n", *test_t); // 포인터 안에 담긴 주소의 내용
        printf("============================\n");
        test_t -= 1 ; /* 오! 막 += 10000 하면 segmentation fault 나는데 1~500정도 더하는건 괜찮은가보다! */
        printf("test_t -= 1;\n");
        printf("포인터 자체의 주소 : %p\n", &test_t); // 포인터 자체의 주소
        printf("포인터 안에 담긴 주소 : %p\n", test_t); // 포인터 자체의 내용 = 포인터 안에 담긴 주소
        printf("포인터 안에 담긴 주소의 내용 : %p\n", *test_t); // 포인터 안에 담긴 주소의 내용

        return 0;
}