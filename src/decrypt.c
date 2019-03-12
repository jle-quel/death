#include <war.h>

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

void decrypt(const char *caller, char *callee, const size_t caller_size, const size_t callee_size)
{
	unsigned int key = 0;
	size_t junk[] = {13, 23, -23, -13};
	
	for (size_t index = 0; index < caller_size; index++)
	{
		key += caller[index];
		junk[index % 4] = index ^ key;
		junk[index % 4] ^= junk[index % 4] + 1;
		key += caller[index] ^ junk[index % 4];
	}

	for (register size_t index = 0; index < callee_size; index++)
	{
		junk[index % 4] <<= 2;
		callee[index] ^= (key / junk[index % 4]);
		junk[index % 4] >>= 1;
	}
}
