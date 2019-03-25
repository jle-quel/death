#include <war.h>

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

size_t _strlen(const char *str)
{
	size_t ret = 0;

	if (str == NULL)
		return 0;

	while (str[ret])
		ret++;

	return ret;
}

void _bzero(char *str, const size_t size)
{
	if (str == NULL)
		return ;

	for (register size_t index = 0; index < size; index++)
		str[index] = 0;
}

size_t _get_random_integer(const size_t range)
{
	char buf[4];

	_bzero(buf, 4);

	if (_getrandom(buf, 4, 1) != 4)
		return range / 2;

	int seed = *(int *)buf;
	int ret = seed % range;

	return ret;
}

void _memcpy(void *dst, void const *src, const size_t size)
{
	if (dst == NULL)
		return ;
	if (src == NULL)
		return ;

	char *dst_tmp = dst;
	char const *src_tmp = src;

	for (size_t index = 0; index < size; index++)
		dst_tmp[index] = src_tmp[index];
}
