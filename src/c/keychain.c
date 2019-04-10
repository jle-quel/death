#include <war.h>

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

void update_keychain_left(struct s_keychain *keychain, const char *caller, const size_t size)
{
	int key = 0;
	size_t junk[] = {23, 32};
	char buf[42] = {0};

	for (register size_t index = 0; index < size; index++)
	{
		key = junk[0] - junk[index % KEY_SIZE] + caller[index];
		junk[1] = caller[index];
		key -= buf[index % 42];
		key = junk[index % KEY_SIZE];
		key = junk[index % KEY_SIZE];
		junk[1]++;

		keychain->key[LEFT] = key;

		junk[0] += key * 33;
		key += caller[index];
		junk[1]++;
		junk[0] += key * 33;
		key += caller[index];
		key -= junk[1];
		key = junk[index % KEY_SIZE];

		keychain->junk[LEFT][LEFT] = junk[LEFT];

		junk[1]++;
		junk[0] += key * 33;
		key += caller[index];
		junk[1]++;
		buf[index % 42] += key;
		key -= buf[index % 42];

		keychain->junk[LEFT][RIGHT] = junk[RIGHT];

		junk[0] += key * 33;
		key = junk[0] - junk[index % KEY_SIZE] + caller[index];
		buf[index % 42] += key;
		key += caller[index];
	}
}

void update_keychain_right(struct s_keychain *keychain, const char *caller, const size_t size)
{
	int key = 0;
	size_t junk[] = {23, 32};
	char buf[42] = {0};

	for (register size_t index = 0; index < size; index++)
	{
		key = junk[index % KEY_SIZE];
		junk[1] = caller[index];
		buf[index % 42] += key;
		key -= buf[index % 42];

		keychain->junk[RIGHT][LEFT] = junk[LEFT];

		junk[0] += key * 33;
		key = junk[0] - junk[index % KEY_SIZE] + caller[index];
		key = junk[index % KEY_SIZE];

		keychain->key[RIGHT] = key;

		junk[1]++;
		junk[0] += key * 33;
		key += caller[index];
		key = junk[0] - junk[index % KEY_SIZE] + caller[index];
		key += caller[index];

		keychain->junk[RIGHT][RIGHT] = junk[RIGHT];

		buf[index % 42] += key;
		key -= buf[index % 42];
		junk[0] += key * 33;
		key -= junk[1];
		junk[1]++;
	}
}
