#include <core/eos.h>
#include <core/eos_internal.h>

typedef struct _os_context {
  // 문맥 전환 시 저장해야 할 CPU 레지스터의 종류와 순서를 결정
  // 순서?
	/* low address */
  int32u_t edi;     // order 10
  int32u_t esi;     // order 9
  int32u_t ebp;     // order 8
  int32u_t esp;     // order 7
  int32u_t ebx;     // order 6
  int32u_t edx;     // order 5
  int32u_t ecx;     // order 4
  int32u_t eax;     // order 3
  int32u_t eflags;  // order 2
  int32u_t eip;     // order 1
	/* high address */	
} _os_context_t;

void print_context(addr_t context) {
	if(context == NULL) return;
	_os_context_t *ctx = (_os_context_t *)context;
	//PRINT("reg1  =0x%x\n", ctx->reg1);
	//PRINT("reg2  =0x%x\n", ctx->reg2);
	//PRINT("reg3  =0x%x\n", ctx->reg3);
	//...
}

addr_t _os_create_context(addr_t stack_base, size_t stack_size, void (*entry)(void *), void *arg) {
  printf("===Start create context===\n");
  int32u_t* sp = stack_base + stack_size/4 - 1;  // stack pointer
  //printf("***sp = %p\n", sp);
  *(sp--) = arg;    // arg
  *(sp--) = NULL;   // entry return address
  *(sp--) = entry;  // eip
  *(sp--) = NULL;   // eflags
  *(sp--) = NULL;   // eax
  *(sp--) = NULL;   // ecx
  *(sp--) = NULL;   // edx
  *(sp--) = NULL;   // ebx
  *(sp--) = NULL;   // esp
  *(sp--) = NULL;   // ebp
  *(sp--) = NULL;   // esi
  *(sp) = NULL;     // edi
  printf("===End create context===\n");
  return (addr_t)sp;        // return context address
}

void _os_restore_context(addr_t sp) {
  printf("===Start restore context===\n");
 __asm__ __volatile__ ("\
    mov %0, %%esp;\
    pop %%edi;\
    pop %%esi;\
    pop %%ebp;\
    pop %%esp;\
    pop %%ebx;\
    pop %%edx;\
    pop %%ecx;\
    pop %%eax;\
    pop _eflags;\
    leave;\
    ret;"
    :: "r"(sp));
 printf("===End restore context===\n");
}

addr_t _os_save_context() {
  int32u_t* eip;
  int32u_t* sp0;
  int32u_t* sp01;
  int32u_t* sp02;
  int32u_t* sp1;
  int32u_t* sp2;
  int32u_t* eax;
  int32u_t* ebp0;
  int32u_t* ebp01;
  int32u_t* ebp;
  int32u_t* ebp2;
  printf("===Start save context===\n");
  /* push register */
  __asm__ __volatile__ ("\
    push $resume_eip;\
    push _eflags;"
    :: );
  printf("save context\n");
  printf("eip : %p\n, eip");
  __asm__ __volatile__ ("\
    mov %%esp, %0;\
    mov %%ebp, %1;\
    push %%eax;\
    mov %%esp, %2;\
    push %%ecx;\
    push %%edx;\
    push %%ebx;\
    push %%esp;\
    push %%ebp;\
    push %%esi;\
    push %%edi;\
    mov %%esp, %3;"
    : "=r"(sp0), "=r"(ebp0), "=r"(sp01), "=r"(sp02) : );
  printf("@@ sp0 : %p\n, @@ ebp0 : %p\n, @@ sp01 : %p\n, @@ sp02 : %p\n", sp0, ebp0, sp01, sp02);
  printf("save context2\n");
  __asm__ __volatile__ ("\
    mov %%esp, %%eax;\
    mov %%esp, %0;\
    push 1(%%ebp);\
    push %%ebp;\
    mov %%ebp, %4;\
    mov %%esp, %%ebp;\
    mov %%esp, %1;\
    mov %%eax, %2;\
    mov %%ebp, %3;\
  resume_eip:\
    pop %5;"
    : "=r"(sp1), "=r"(sp2), "=r"(eax), "=r"(ebp), "=r"(ebp01), "=r"(eip) : );
  printf("@@ sp1 : %p\n, sp2 : %p\n, eax : %p\n, ebp01 : %p\n, ebp : %p\n, eip : %p\n", sp1, sp2, eax, ebp01, ebp, eip);
  
  __asm__ __volatile__ ("\
    mov %%ebp, %%esp;\
    pop %%ebp;\
    mov %%ebp, %0;"
    : "=r"(ebp2) : );
  printf("@@ ebp2 : %p\n", ebp2);
  printf("===End save context===\n");

  __asm__ __volatile__ ("\
    ret;"
    :: );
}
