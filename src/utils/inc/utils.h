#ifndef UTILS_H
#define UTILS_H

////////////////////////////////////////////////////////////////////////////////
/// ENUMS 
////////////////////////////////////////////////////////////////////////////////

enum e_key
{
	LEFT,
	RIGHT,
	KEY_SIZE,
};

////////////////////////////////////////////////////////////////////////////////
/// STRUCTURES 
////////////////////////////////////////////////////////////////////////////////

struct s_keychain
{
	int key[KEY_SIZE];
	size_t junk[KEY_SIZE][KEY_SIZE];
};

////////////////////////////////////////////////////////////////////////////////
/// DECLARATIONS 
////////////////////////////////////////////////////////////////////////////////

void rc4(const unsigned char *key, const size_t key_length, char *data, const size_t data_length);

__attribute__((hot)) void update_keychain_left(struct s_keychain *keychain, const char *caller, const size_t size);
__attribute__((hot)) void update_keychain_right(struct s_keychain *keychain, const char *caller, const size_t size);
__attribute__((hot)) void decrypt_left(const struct s_keychain *keychain, char *callee, const size_t size);
__attribute__((hot)) void decrypt_right(const struct s_keychain *keychain, char *callee, const size_t size);

#endif
