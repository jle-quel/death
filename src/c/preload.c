#include <war.h>

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

void preload(void)
{
/*
	void *base = (void *)__entry;
	size_t diff = (unsigned long)base % PAGE_SIZE;

	if (mprotect(base - diff, ((void *)__exit - (void *)__entry) + PAGE_SIZE, PROT_READ | PROT_WRITE | PROT_EXEC) < 0)
	{
		perror("mprotect");
		exit(errno);
	}
*/
}
