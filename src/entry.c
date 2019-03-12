#include <war.h>

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

void __entry(void)
{
	decrypt((char *)&__entry, (char *)&foo, &foo - &__entry, &bar - &foo);
	foo();
}

void foo(void)
{
	decrypt((char *)&foo, (char *)&bar, &bar - &foo, &__exit - &bar);
	bar();
}

void bar(void)
{
	printf("Here\n");
}

void __exit(void)
{
}
