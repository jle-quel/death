#include <war.h>

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

void update_keychain_left(struct s_keychain *keychain, const char *caller, const size_t size)
{
	size_t key = 0;
	size_t junk[2] = {23, 32};

	for (register size_t index = 0; index < size; index++)
	{
		key = caller[index];
		junk[index % 2] = index ^ key;
		key += caller[index] ^ (junk[index % 2] + 1);
		junk[index % 2] |= key;
		key += junk[index % 2];
	}

	keychain->key[LEFT] = key;
	keychain->junk[LEFT][0] = junk[0];
	keychain->junk[LEFT][1] = junk[1];
}

void update_keychain_right(struct s_keychain *keychain, const char *caller, const size_t size)
{
	size_t key = 0;
	size_t junk[2] = {23, 32};

	for (register size_t index = 0; index < size; index++)
	{
		key += junk[index % 2];
		junk[index % 2] = index ^ key;
		key = caller[index];
		junk[index % 2] |= key;
		key += caller[index] ^ (junk[index % 2] + 1);
	}

	keychain->key[RIGHT] = key;
	keychain->junk[RIGHT][0] = junk[0];
	keychain->junk[RIGHT][1] = junk[1];
}

void decrypt_left(const struct s_keychain *keychain, char *callee, const size_t size)
{
	for (register size_t index = 0; index < size; index++)
	{
		callee[index] ^= keychain->key[LEFT];
	}
}

void decrypt_right(const struct s_keychain *keychain, char *callee, const size_t size)
{
	for (register size_t index = 0; index < size; index++)
	{
		callee[index] ^= keychain->key[RIGHT];
	}
}
