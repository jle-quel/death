#include <war.h>

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

void update_keychain_left(struct s_keychain *keychain, const char *caller, const size_t size)
{
	int key = 0;
	size_t junk[] = {23, 32};

	for (register size_t index = 0; index < size; index++)
	{
		key += caller[index];
	}

	keychain->key[LEFT] = key;
	keychain->junk[LEFT][LEFT] = junk[LEFT];
	keychain->junk[LEFT][RIGHT] = junk[RIGHT];
}

void update_keychain_right(struct s_keychain *keychain, const char *caller, const size_t size)
{
	int key = 0;
	size_t junk[] = {23, 32};

	for (register size_t index = 0; index < size; index++)
	{
		key += caller[index];
	}

	keychain->key[RIGHT] = key;
	keychain->junk[RIGHT][LEFT] = junk[LEFT];
	keychain->junk[RIGHT][RIGHT] = junk[RIGHT];
}

void decrypt_left(const struct s_keychain *keychain, char *callee, const size_t size)
{
	for (register size_t index = 0; index < size; index++)
		callee[index] ^= keychain->key[LEFT];
}

void decrypt_right(const struct s_keychain *keychain, char *callee, const size_t size)
{
	for (register size_t index = 0; index < size; index++)
		callee[index] ^= keychain->key[RIGHT];
}
