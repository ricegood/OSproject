#include <core/eos.h>
#include <core/eos_internal.h>

typedef struct _os_context {
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
	int32u_t* sp = (int32u_t *)(stack_base + stack_size/4 - 1);  // stack pointer
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
	return (addr_t)sp;        // return context address
}

void _os_restore_context(addr_t sp) {
	int32u_t *spValue = (int32u_t *)sp;
	__asm__ __volatile__ ("\
		movl %0, %%esp;\
		pop %%edi;\
		pop %%esi;\
		pop %%ebp;\
		addl $0x4, %%esp;\
		pop %%ebx;\
		pop %%edx;\
		pop %%ecx;\
		pop %%eax;\
		pop _eflags;\
		ret;"
		:: "m"(spValue));
}

addr_t _os_save_context() {
	int32u_t *eax;
	__asm__ __volatile__ ("\
	  mov %%eax, %0;\
		call get_eip;\
		push %%eax;\
		mov %0, %%eax;\
		push _eflags;\
		push %%eax;\
		push %%ecx;\
		push %%edx;\
		push %%ebx;\
		push %%esp;\
		push %%ebp;\
		push %%esi;\
		push %%edi;\
		mov %%esp, %%eax;\
		push 4(%%ebp);\
		push %%ebp;\
		mov %%esp, %%ebp;\
		leave;\
		ret;\
	get_eip:\
		mov %%esp, %%eax;\
		ret;"
		: "=m"(eax) : );
}
