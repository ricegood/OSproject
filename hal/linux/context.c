#include <core/eos.h>
#include <core/eos_internal.h>

typedef struct _os_context {
  // 문맥 전환 시 저장해야 할 CPU 레지스터의 종류와 순서를 결정
  // 순서?
	/* low address */
  int32u_t edi;
  int32u_t esi;
  int32u_t ebp;
  int32u_t esp;
  int32u_t ebx;
  int32u_t edx;
  int32u_t ecx;
  int32u_t eax;
  int32u_t eflags;
  int32u_t eip;
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
}

void _os_restore_context(addr_t sp) {
}

addr_t _os_save_context() {
}
